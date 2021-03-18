#include <iostream>
#include <string.h>

#include "SerialPort/SerialPort.hpp"
#include "Wave-Parser/wave.h"
#include "comstatus.h"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <algorithm>

#define __PACKET_SIZE ( 1UL << 9 )
#define __MAX_SIZE ( __PACKET_SIZE << 11 )

const char portName[] = "COM6";

// SerialPort object
SerialPort * stm32;
ComStatus * stm32_status;
Wave * audio;

int main( int argc, char * argv[] ) {

    if ( argc < 2 ) {
        std::cerr << "[ERROR]: Did not give file name." << std::endl;
        return 1;
    }


    /* Init audio */
    audio = new Wave( argv[1] );
    if (   audio->GetNumChannels()  != 2
        || ( (audio->GetBitsPerSample() & 0b111) != 0 )
        || audio->GetSampleRate()   != 44100 ) {
        std::cerr << "[ERROR]: Bad wave file format." << std::endl;
        return 2;
    }

    /* Init STM32 */
    stm32 = new SerialPort( portName );
    while ( !stm32->isConnected() ) {
        Sleep(100);
        std::cout << ".";
        delete stm32;
        stm32 = new SerialPort(portName);
    }
    std::cout << "Connected!\n";

    /* Init Com Status */
    stm32_status = new ComStatus( stm32 );

    /* Start sending audio samples */
    const uint32_t numSamples = audio->numSamples();
    const uint16_t numChannels = audio->GetNumChannels(); // 2
    const uint32_t buffer_size = std::min( (uint32_t)(numSamples * numChannels), (uint32_t)__MAX_SIZE );

    std::cout << buffer_size << std::endl;

    uint8_t buffer[ buffer_size ];
    uint32_t buffer_i = 0;
    for ( uint32_t sample_i = 0; (sample_i*numChannels) < buffer_size; sample_i++ ) {
        for ( uint16_t channel_i = 0; channel_i < numChannels; channel_i++ ) {
            buffer[ buffer_i ] = audio->getSample( sample_i, channel_i ) >> ( audio->GetBitsPerSample() - 8 );
            buffer_i++;
        }
    }

    buffer_i = 0;
    while ( stm32_status->active() && stm32->isConnected() ) {

        // std::string s;
        // std::cout << "Give input:\n";
        // getline( std::cin, s );

        if ( stm32_status->ComIsReady() ) {
            // std::cout << "Sending: ";
            // for ( uint8_t i_print = 0; i_print < __PACKET_SIZE; i_print++ )
            //     std::cout << (const uint16_t)*(mem + i + i_print) << " ";
            // std::cout << std::endl;

            int writeErr = stm32->writeSerialPort( (const char*)buffer + buffer_i, __PACKET_SIZE );
            if ( writeErr == 0 ) break;
            buffer_i = (buffer_i + __PACKET_SIZE) % buffer_size;
        }
    }

    std::cout << "Exiting..." << std::endl;
    delete audio;
    delete stm32_status;
    delete stm32;

    std::cout << "Done." << std::endl;

    return 0;
}
