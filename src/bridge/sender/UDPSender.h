#ifndef BRIDGE_UDPSENDER_H
#define BRIDGE_UDPSENDER_H
#include "Sender.h"

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

class UDPSender : public Sender {
private :
#ifdef _WIN32
    SOCKET _sock;
    SOCKADDR_IN _addr;
#elif __linux__
    int _sock;
    struct sockaddr_in _addr;
#endif

public:
    UDPSender(const std::string& ipAddress, int port);

    void send(std::string message) override;
    void send(TrackerData data) override;
    void cleanup() override;
};


#endif //BRIDGE_UDPSENDER_H
