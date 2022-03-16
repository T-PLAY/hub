
#ifndef NET_H
#define NET_H

#if defined WIN32
//#define NOMINMAX
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library
using socklen_t = int;
using socket_fd = SOCKET;

#else
#define INVALID_SOCKET -1
#define closesocket close
using socket_fd = int;
#include <arpa/inet.h>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <stdio.h>
#include <thread>

namespace Net {
static bool sInited = false;
static std::list<socket_fd> sSockets;

static void clearSocket(socket_fd& sock)
{
    std::cout << "Net::clearSocket(" << sock << ") close socket" << std::endl;
    closesocket(sock);
    size_t size = sSockets.size();
    assert(std::find(sSockets.begin(), sSockets.end(), sock) != sSockets.end());
    sSockets.remove(sock);
    assert(sSockets.size() == size - 1);

#ifdef WIN32
    if (sSockets.empty()) {
        std::cout << "Net::clearSocket(" << sock << ") WSACleanup()" << std::endl;
        WSACleanup();
        sInited = false;
        // TODO: find a way to cleanup WSA when program ended
    }
#endif

    sock = INVALID_SOCKET;
}

#ifndef WIN32
static void signalHandler(int signum)
{
    std::cout << "Net::signalHandler() Interrupt signal (" << signum << ") received." << std::endl;

    // cleanup and close up stuff here
    // terminate program
    for (const socket_fd& sock : sSockets) {
        if (sock != INVALID_SOCKET) {
            std::cout << "Net::clearSocket(" << sock << ") close socket" << std::endl;
            closesocket(sock);
        }
    }
    exit(signum);
}
#endif

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

#endif
        sInited = true;
    }
}

};

#endif // NET_H
