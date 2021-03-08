#include <iostream>
#include <string.h>

#include "SerialPort/include/SerialPort.hpp"
#include "Wave-Parser/wave.h"
#include "comstatus.h"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define _PACKET_SIZE ( 1UL << 4 )

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
        || audio->GetBitsPerSample()!= 16
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
    size_t i = 0;
    const uint8_t * mem = (const uint8_t *) audio->getSampleMemory();
    uint32_t mem_size = audio->getSampleMemorySize();

    while ( stm32_status->active() && stm32->isConnected() ) {

        // std::string s;
        // std::cout << "Give input:\n";
        // getline( std::cin, s );

        if ( stm32_status->ComIsReady() ) {
            // std::cout << "Sending: ";
            // for ( uint8_t i_print = 0; i_print < _PACKET_SIZE; i_print++ )
            //     std::cout << (const uint16_t)*(mem + i + i_print) << " ";
            // std::cout << std::endl;

            int writeErr = stm32->writeSerialPort( (const char*)mem + i, _PACKET_SIZE );
            if ( writeErr == 0 ) break;
            i += _PACKET_SIZE;
            if ( i >= mem_size ) i = 0;
        }
    }

    std::cout << "Exiting..." << std::endl;
    delete audio;
    delete stm32_status;
    delete stm32;

    std::cout << "Done." << std::endl;

    return 0;
}
