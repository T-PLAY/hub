#pragma once

#if defined WIN32
//#define NOMINMAX
//#include <winsock.h>
//#pragma comment(lib, "ws2_32.lib") // Winsock Library
//#include <WinSock2.h>
//#pragma comment( lib, "Ws2_32.lib" )

#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif

#    include <iphlpapi.h>
#    include <stdio.h>
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>

#    pragma comment( lib, "Ws2_32.lib" )

using socklen_t = int;
using socket_fd = SOCKET;
//#include <basetsd.h>
// using socket_fd = UINT_PTR;

#else
#    define INVALID_SOCKET -1
#    define closesocket close
using socket_fd = int;
#    include <arpa/inet.h>
#    include <csignal>
#    include <sys/socket.h>
#    include <unistd.h>
#endif

#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <stdio.h>
#include <thread>
#include <mutex>

//#define DEBUG_NET

namespace hub {
namespace net {

static std::string getHeader() {
    const unsigned int id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    std::string str = "[\033[31mNet\033[0m:\033[" + std::to_string( 31 + id % 7 ) +
                      "m" + std::to_string( id ) + "]\033[0m ";
    return str;
}

static bool s_inited = false;
static std::list<socket_fd> s_sockets;
static std::mutex s_mtx;

static void registerSocket( socket_fd socket) {
    s_mtx.lock();

#ifdef DEBUG_NET
    std::cout << getHeader() << "registerSocket(" << socket << ")" << std::endl;
#endif
    s_sockets.push_back(socket);
#ifdef DEBUG_NET
    std::cout << getHeader() << "s_sockets = ";
    for (const auto & socket : s_sockets) {
        std::cout << socket << " ";
    }
    std::cout << std::endl;
#endif

    s_mtx.unlock();
}

static void clearSocket( socket_fd& sock ) {
    s_mtx.lock();

#ifdef DEBUG_NET
    std::cout << getHeader() << "clearSocket(" << sock << ") close socket" << std::endl;
    std::cout << getHeader() << "s_sockets = ";
    for (const auto & socket : s_sockets) {
        std::cout << socket << " ";
    }
    std::cout << std::endl;
#endif
    closesocket( sock );
    //    size_t size = s_sockets.size();
//    assert( std::find( s_sockets.begin(), s_sockets.end(), sock ) != s_sockets.end() );
    s_sockets.remove( sock );
    // assert(s_sockets.size() == size - 1);

#ifdef WIN32
    if ( s_sockets.empty() ) {
        std::cout << "Net::clearSocket(" << sock << ") WSACleanup()" << std::endl;
        WSACleanup();
        s_inited = false;
        // TODO: find a way to cleanup WSA when program ended
    }
#endif

    sock = INVALID_SOCKET;

    s_mtx.unlock();
}

#ifndef WIN32
static void signalHandler( int signum ) {
#ifdef DEBUG_NET
    std::cout << getHeader() << "signalHandler() Interrupt signal (" << signum << ") received." << std::endl;
#endif

    assert(s_sockets.empty());
    std::cout << getHeader() << "signalHandler() Interrupt signal (" << signum << ") received." << std::endl;

    // cleanup and close up stuff here
    // terminate program
    for ( const socket_fd& sock : s_sockets ) {
        if ( sock != INVALID_SOCKET ) {
#ifdef DEBUG_NET
            std::cout << getHeader() << "clearSocket(" << sock << ") close socket" << std::endl;
#endif
            closesocket( sock );
        }
    }
    exit( signum );
}
#endif

static void init() {
    if ( !s_inited ) {
#ifdef DEBUG_NET
        std::cout << getHeader() << "init()" << std::endl;
#endif
#if defined WIN32
        WSADATA wsaData;
        int iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
        if ( iResult != 0 ) {
            printf( "error at WSAStartup\n" );
            exit( 1 );
        }
#else
        signal( SIGINT, signalHandler );
        signal( SIGPIPE, SIG_IGN );

#endif
        s_inited = true;
    }
}

} // namespace net
} // namespace hub
