#ifndef STREAMER_H
#define STREAMER_H

#if defined WIN32
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library
using socklen_t = int;

#else
#define closesocket close
#include <arpa/inet.h>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include <string>

class Hub {
public:
    Hub(std::string ipv4, int port);
    ~Hub();

    template <class T>
    void sendPacket(const T & t);

private:
    std::string mIpv4;
    int mPort;
    SOCKET mSock;
};

#endif // STREAMER_H
