
/* comstatus.cpp */

#include "comstatus.h"

#include <cstring>


ComStatus::ComStatus( SerialPort * const port )
: port(port), running(1), exit(0), ready(0) {
    poll_thread = std::thread{&ComStatus::pollRX, this};
}

ComStatus::~ComStatus() {
    exit = 1;
    poll_thread.join();
}

void ComStatus::pollRX() {
    while ( !exit ) {

        memset( RECIEVE, 0, MAX_DATA_LENGTH + 1 );
        int readErr = port->readSerialPort( RECIEVE, MAX_DATA_LENGTH );
        if ( readErr == 0 ) {
            exit = 1;
            break;
        }

        size_t size = std::strlen( RECIEVE );
        if ( size ) {
            switch ( RECIEVE[ size-1 ] ) {
                case 'g':
                    ready = 1;
                    break;
                case 'B':
                    ready = 0;
                    break;
                default:
                    exit = 1;
                    break;
            }
        }

    }
    running = 0;
    ready = 0;
}