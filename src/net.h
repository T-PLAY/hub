
#ifndef NET_H
#define NET_H

#if defined WIN32
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library
using socklen_t = int;
using socket_fd = SOCKET;

#else
#define closesocket close
using socket_fd = int;
#include <arpa/inet.h>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cstring>
#include <iostream>
#include <list>
#include <stdio.h>
#include <thread>
#include <set>

namespace Net {
static bool sInited = false;
static std::set<socket_fd> sSockets;

static void clearSocket(socket_fd sock)
{
    std::cout << "Net::clearSocket(" << sock << ")" << std::endl;
    closesocket(sock);
#ifdef WIN32
    WSACleanup();
#endif
}

#ifndef WIN32
static void signalHandler(int signum)
{
    std::cout << "Net::signalHandler() Interrupt signal (" << signum << ") received." << std::endl;

    // cleanup and close up stuff here
    // terminate program
    for (const socket_fd & sock : sSockets) {
        if (sock != -1) {
            clearSocket(sock);
        }
    }
    exit(signum);
}
#endif

// class Net {
// public:
static void init()
{
    if (!sInited) {
        std::cout << "Net::init()" << std::endl;
#if defined WIN32
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("error at WSAStartup\n");
            exit(1);
        }
#else
        signal(SIGINT, signalHandler);
//        signal(SIGPIPE, signalHandler);
//        signal(SIGSTOP, signalHandler);
//        signal(SIGTERM, signalHandler);
//        signal(SIGKILL, signalHandler);

#endif
        sInited = true;
    }
}

};

#endif // NET_H
