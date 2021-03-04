#include <iostream>
#include <string.h>

#include "SerialPort/include/SerialPort.hpp"
#include "Wave-Parser/wave.h"
#include "comstatus.h"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define _PACKET_SIZE 4

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

    audio = new Wave( argv[1] );

    if (   audio->GetNumChannels()  != 2
        || audio->GetBitsPerSample()!= 16
        || audio->GetSampleRate()   != 44100 ) {
        std::cerr << "[ERROR]: Bad wave file format." << std::endl;
        return 1;
    }

    stm32 = new SerialPort( portName );
    while ( !stm32->isConnected() ) {
        Sleep(100);
        std::cout << ".";
        delete stm32;
        stm32 = new SerialPort(portName);
    }

    stm32_status = new ComStatus( stm32 );

    size_t i = 0;
    const char * mem = (const char *) audio->getSampleMemory();
    uint8_t packet_size = audio->getSampleMemorySize() / _PACKET_SIZE;
    while ( 1 ) {
        if ( !stm32_status->active() ) break;
        
        if ( stm32_status->ComIsReady() ) {
            int writeErr = stm32->writeSerialPort( mem + i, _PACKET_SIZE );
            if ( writeErr == 0 ) break;
            i = ( i + 1 ) % packet_size;
        }
    }

    delete audio;
    delete stm32_status;
    delete stm32;


    std::cout << "Done." << std::endl;

    return 0;
}
