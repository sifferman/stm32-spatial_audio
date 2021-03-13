
/* comstatus.cpp */

#include "comstatus.h"

#include <chrono>
#include <cstring>


ComStatus::ComStatus( SerialPort * const port )
: port(port), running(1), exit(0), ready(1) {
    poll_thread = std::thread{&ComStatus::pollRX, this};
    std::cout << "polling read" << std::endl;
}

ComStatus::~ComStatus() {
    exit = 1;
    poll_thread.join();
}

void ComStatus::pollRX() {
    while ( !exit ) {

        std::chrono::milliseconds timespan(1);
        std::this_thread::sleep_for( timespan );

        memset( RECIEVE, 0, MAX_DATA_LENGTH + 1 );
        int readErr = port->readSerialPort( RECIEVE, MAX_DATA_LENGTH );
        if ( readErr == 0 ) {
            continue;
        }

        size_t size = std::strlen( RECIEVE );
        // printf("recieved: \"%s\"\n", RECIEVE);
        switch ( RECIEVE[ size-1 ] ) {
            case 'g':
            case 'E':
                if ( ready == 0 ) printf("Enabling.\n");
                ready = 1;
                break;
            case 'B':
                if ( ready == 1 ) printf("Disabling.\n");
                ready = 0;
                break;
            default:
                std::cerr << "[WARNING]: Poll recieved bad char \'" << RECIEVE[ size-1 ] << '\'' << std::endl;
                break;
        }

    }
    running = 0;
    ready = 0;
    std::cout << "Exited polling." << std::endl;
}
