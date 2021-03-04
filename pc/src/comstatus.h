
/* comstatus.h */

#ifndef __COMSTATUS_H
#define __COMSTATUS_H

#include "SerialPort/include/SerialPort.hpp"

#include <thread>

class ComStatus {
    public:
        ComStatus( SerialPort * const );
        ~ComStatus();
        bool ComIsReady() { return ready; }
        bool active() { return running; }
    private:
        void pollRX(void);
        std::thread poll_thread;
        SerialPort * const port;
        
        bool running, exit;
        bool ready;
        char RECIEVE[ MAX_DATA_LENGTH + 1 ];
};

#endif
