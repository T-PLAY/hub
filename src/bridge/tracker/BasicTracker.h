#ifndef BRIDGE_BASICTRACKER_H
#define BRIDGE_BASICTRACKER_H
#include "Tracker.h"
#include <thread>

#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#endif

class BasicTracker : public Tracker{

#ifdef _WIN32
private:
    SOCKET _sock;
    SOCKADDR_IN _addr;

#elif __linux__
private:
    int _sock;
    struct sockaddr_in _addr;
#endif

private :
    bool _connected = false;
    TrackerData _data;
    std::thread _readThread;
    bool _listen = true;
    const float div = 39.37f;

    void readData();

public :
    bool connected() const override;
    TrackerData data() const override;

    void start() override;
    void end() override;

    BasicTracker();


};


#endif //BRIDGE_BASICTRACKER_H
