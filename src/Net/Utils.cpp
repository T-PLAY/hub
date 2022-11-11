
#include "Utils.hpp"

#include <cassert>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32

//#include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
//#include <WS2tcpip.h>

#    pragma comment( lib, "ws2_32.lib" )
//#pragma comment(lib, "Ws2_32.lib")

// using socklen_t = int;
// using socket_fd = SOCKET;

//#include <basetsd.h>
// using socket_fd = UINT_PTR;

#else // #ifdef WIN32
#    define INVALID_SOCKET -1
#    define closesocket close
//using socket_fd = int;
#    include <arpa/inet.h>
#    include <csignal>
#    include <sys/socket.h>
#    include <unistd.h>
#endif // #ifdef WIN32

namespace hub {
namespace net {
namespace utils {

static bool s_inited = false;

 socket_fd invalidSocket() {
    return INVALID_SOCKET;
}

static inline  void init() {
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
        //        signal( SIGINT, signalHandler );
//        signal( SIGPIPE, SIG_IGN );

#endif
        s_inited = true;
    }
}

 bool isValid( socket_fd sock ) {
    return sock != INVALID_SOCKET;
}

 void closeSocket( socket_fd& sock ) {
    assert( sock != INVALID_SOCKET );
    //    s_mtx.lock();

    //            std::cout << getHeader() << "closeSocket(" << sock << ")" << std::endl;

#ifdef DEBUG_NET
    std::cout << getHeader() << "closeSocket(" << sock << ") close socket" << std::endl;
    std::cout << getHeader() << "s_sockets = ";
    for ( const auto& socket : s_sockets ) {
        std::cout << socket << " ";
    }
    std::cout << std::endl;
#endif
    ::closesocket( sock );
    //    s_sockets.remove( sock );
    // assert(s_sockets.size() == size - 1);

    //#ifdef WIN32
    //    if ( s_sockets.empty() ) {
    //        s_inited = false;
    // TODO: find a way to cleanup WSA when program ended
    //    }
    //#endif

    sock = INVALID_SOCKET;

    //    s_mtx.unlock();
}

bool isConnected( socket_fd sock ) {
    //    assert(m_fdSock != INVALID_SOCKET);
    assert( net::utils::isValid( sock ) );

#ifdef WIN32
    // not sure how to check this in windows
    return true;
#else

    // TODO - make this better, because its probably wrong (but seems to work)

    int optval;
    socklen_t optlen = sizeof( optval );

    int res;
    //    try {
    res = getsockopt( sock, SOL_SOCKET, SO_ERROR, &optval, &optlen );
    //    }
    //    catch ( std::exception& e ) {
    //        std::cout << "[Socket] isConnected() catch exception : " << e.what() << std::endl;
    //        return false;
    //    }
    //    catch ( std::runtime_error& e ) {
    //        std::cout << "[Socket] isConnected() catch runtime error : " << e.what() << std::endl;
    //        return false;
    //    }

    if ( optval == 0 && res == 0 ) return true;
#endif

    return false;
}

/////////////////////////////// SERVER ADDRESS /////////////////////////////////////////////

class ServerAddrImpl
{
  public:
    void init( int port ) {
        //    // Server address construction
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        m_sockAddr.sin_family      = AF_INET;
        m_sockAddr.sin_addr.s_addr = INADDR_ANY;
        m_sockAddr.sin_port        = htons( port ); // Server port
    }
    //            ~ServerAddrImpl() = default;
    //            sockaddr_in & get();
    //            void setPort(int port) {
    //            }

    //          private:
    struct sockaddr_in m_sockAddr;
};

ServerAddr::ServerAddr() : m_pimpl( new ServerAddrImpl ) {}

ServerAddr::~ServerAddr() = default;
//        {
//            m_pimpl.release();
//            m_pimpl.release();
//            m_pimpl.reset();
//        }

//        ServerAddr::ServerAddr(ServerAddr &&serverAddr) noexcept = default;
//        ServerAddr &ServerAddr::operator=(ServerAddr &&serverAddr) noexcept = default;

void ServerAddr::init( int port ) {
    m_pimpl->init( port );
}

//        void ServerAddr::setPort(int port)
//        {
//            m_pimpl->setPort(port);
//        }

 socket_fd serverSocket() {
    if ( !s_inited ) init();
    return ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
}

 int bind( socket_fd sock, ServerAddr& addr ) {
    //            return addr.bind(sock);
    return ::bind(
        sock, (struct sockaddr*)&addr.m_pimpl->m_sockAddr, sizeof( struct sockaddr_in ) );
    //                return ::bind(sock, addr, namelen);
}

 int listen( socket_fd sock, int backlog ) {
    return ::listen( sock, backlog );
}

socket_fd accept( socket_fd sock, ServerAddr& addr ) {
    auto& sockAddr       = addr.m_pimpl->m_sockAddr;
    socklen_t addrlen    = sizeof( sockAddr );
    socket_fd new_socket = accept( sock, (struct sockaddr*)&sockAddr, &addrlen );
    return new_socket;
    //    socklen_t addrlen    = sizeof( mAddress );
    //    return ::accept(sock, address, addrLen);
}

// socket_fd socket()
//{
//     return ::socket( PF_INET, SOCK_STREAM, 0 );
// }

///////////////////////////////// CLIENT ADDRESS ///////////////////////////////////////////

class ClientAddrImpl
{
  public:
    //            ~ClientAddrImpl() = default;
    void init( const std::string& ipv4, int port ) {
        //    // Server address construction
        //                memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        //                m_sockAddr.sin_family      = AF_INET;
        //                m_sockAddr.sin_addr.s_addr = INADDR_ANY;
        //                m_sockAddr.sin_port        = htons( port ); // Server port
        // Server address construction
        //                struct sockaddr_in serv_addr;
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        m_sockAddr.sin_family = AF_INET;
        // m_sockAddr.sin_addr.s_addr = inet_addr(m_ipv4.c_str()); // winsock 1.0
        inet_pton( AF_INET, ipv4.c_str(), &m_sockAddr.sin_addr.s_addr ); // winsock 2.0
        m_sockAddr.sin_port = htons( port );                             // Server port
    }

    //            sockaddr_in & get();
    //            void setPort(int port) {
    //            }
    void setPort( int port ) {
        m_sockAddr.sin_port = htons( port ); // Server port
    }
    void setIpv4( const std::string& ipv4 ) {
        inet_pton( AF_INET, ipv4.c_str(), &m_sockAddr.sin_addr.s_addr ); // winsock 2.0
    }

    //          private:
    struct sockaddr_in m_sockAddr;
};

ClientAddr::ClientAddr() : m_pimpl( new ClientAddrImpl ) {}

ClientAddr::~ClientAddr()                                  = default;
ClientAddr::ClientAddr( ClientAddr&& clientAddr ) noexcept = default;
//        ClientAddr& ClientAddr::operator=(ClientAddr&& clientAddr) noexcept = default;

//        ClientAddr::ClientAddr(const ClientAddr &clientAddr)
//            : m_pimpl(new ClientAddrImpl(*clientAddr.m_pimpl)) {}

//        ClientAddr &ClientAddr::operator=(const ClientAddr &clientAddr) {
//            if (this != &clientAddr)
//                m_pimpl.reset(new ClientAddrImpl(*clientAddr.m_pimpl));
//            return *this;
//        }

void ClientAddr::init( const std::string& ipv4, int port ) {
    m_pimpl->init( ipv4, port );
}

void ClientAddr::setPort( int port ) {
    m_pimpl->setPort( port );
}

void ClientAddr::setIpv4( const std::string& ipv4 ) {
    m_pimpl->setIpv4( ipv4 );
}

 socket_fd clientSocket() {
    if ( !s_inited ) init();
    return ::socket( PF_INET, SOCK_STREAM, 0 );
}

 int connect( socket_fd sock, ClientAddr& addr ) {
    //            return  ::connect( sock, (struct sockaddr*)&m_serverAddress, sizeof(
    //            m_serverAddress ) ) <
    return ::connect(
        sock, (struct sockaddr*)&addr.m_pimpl->m_sockAddr, sizeof( struct sockaddr_in ) );
}

 int send( socket_fd sock, const char* buf, int len, int flags ) {
    return ::send( sock, buf, len, flags );
}

 int recv( socket_fd sock, char* buf, int len, int flags ) {
    return ::recv( sock, buf, len, flags );
}

} // namespace utils
} // namespace net
} // namespace hub

//#include <cassert>
//#include <cstring>
//#include <functional>
//#include <iostream>
//#include <list>
//#include <set>
//#include <mutex>
//#include <string>
//#include <thread>

//#include <winsock2.h>

//#define UDT_API // no dllimport/export

//#include <winsock.h>
//#pragma comment(lib, "ws2_32.lib") // Winsock Library
//#include <WinSock2.h>
//#pragma comment( lib, "Ws2_32.lib" )

//#include <winsock2.h>
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif

//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//#include <Windows.h>
////#include <WS2tcpip.h>

//#include <winsock.h>
//#include <windows.h>
//#endif

//#include <iphlpapi.h>
//#include <stdio.h>
//#include <windows.h>
//#include <ws2tcpip.h>

//#include <WS2tcpip.h>

//#pragma comment(lib, "Ws2_32.lib")

// source:
// https://www.zachburlingame.com/2011/05/resolving-redefinition-errors-betwen-ws2def-h-and-winsock-h/
//#ifndef _WINDOWS_

//#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
//#ifndef _WINDOWS_
//#undef WIN32_LEAN_AND_MEAN

//#endif
//#define _WINSOCKAPI_
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#undef WIN32_LEAN_AND_MEAN
//#endif

//#include <windows.h>
//#include <ws2tcpip.h>
//#include <WS2tcpip.h>

//#pragma comment( lib, "ws2_32.lib" )
//// end source
///
//#ifndef WIN32
// static void signalHandler( int signum ) {
//#    ifdef DEBUG_NET
//    std::cout << getHeader() << "signalHandler() Interrupt signal (" << signum << ") received."
//              << std::endl;
//#    endif

//    assert( s_sockets.empty() );
//    std::cout << getHeader() << "signalHandler() Interrupt signal (" << signum << ") received."
//              << std::endl;
//    std::cout << getHeader() << "signalHandler() s_sockets size = " << s_sockets.size()
//              << std::endl;

//    // cleanup and close up stuff here
//    // terminate program
//    for ( const socket_fd& sock : s_sockets ) {
//        if ( sock != INVALID_SOCKET ) {
//            //#ifdef DEBUG_NET
//            std::cout << getHeader() << "signalHandler() close socket" << sock << std::endl;
//            //#endif
//            closesocket( sock );
//        }
//    }
//    exit( signum );
//}
//#endif

// socket_fd createSocket(int af, int type, int protocol)
//{
//     return ::socket(af, type, protocol);
// }
