
#include "NetUtilsSystem.hpp"

#include <cstring>
#include <mutex>
#include <regex>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

static const std::regex s_ipv4Regex { "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" };

#ifdef WIN32

#    ifdef __MINGW64__
#        undef _WIN32_WINNT
#        define _WIN32_WINNT _WIN32_WINNT_VISTA // Windows Vista
#    endif

#    include <winsock2.h>
#    include <ws2tcpip.h>

#    pragma comment( lib, "ws2_32.lib" )

#else // Linux or Apple
#    define INVALID_SOCKET -1ul
#    define closesocket close
#    include <arpa/inet.h>
#    include <csignal>
#    include <sys/socket.h>
#    include <unistd.h>
#endif // end WIN32

namespace hub {
namespace net {
namespace system {
namespace utils {

///
/// \brief The NetManager class
///
class NetManager
{
  public:
    ~NetManager() {
#ifdef HUB_DEBUG_NET
        std::cout << "[net] all socket closed" << std::endl;
#endif
        // Todo fix
//#ifndef WIN32
#if defined( OS_LINUX ) || defined(OS_MACOS)
		assert( m_sockets.empty() );
#endif
    }
#ifdef HUB_DEBUG_NET
    void printStatus() const {
        std::cout << "[NetManager:" << std::this_thread::get_id() << "] sockets : ";
        for ( const auto& sock : m_sockets ) {
            std::cout << sock << " ";
        }
        std::cout << std::endl;
    }
#endif

    ///
    /// \brief registerSocket
    /// \param sock
    ///
    void registerSocket( const socket_fd& sock ) {
        int iTry = 0;
        m_mtxSockets.lock();
        while ( m_sockets.find( sock ) != m_sockets.end() && iTry < 10 ) {
            m_mtxSockets.unlock();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
#ifdef HUB_DEBUG_NET
            std::cout << "[NetManager] waiting for unregistered socket ..." << std::endl;
#endif
            ++iTry;
            m_mtxSockets.lock();
        }
        m_mtxSockets.unlock();

        m_mtxSockets.lock();
        if ( m_sockets.find( sock ) != m_sockets.end() ) {
#ifdef HUB_DEBUG_NET
            std::cout << "[NetManager] unable to register sock : " << sock
                      << ", because already presents" << std::endl;
            printStatus();
#endif
        }
        assert( m_sockets.find( sock ) == m_sockets.end() );
        m_sockets.insert( sock );
#ifdef HUB_DEBUG_NET
        std::cout << "[NetManager:" << std::this_thread::get_id() << "] registerSocket(" << sock
                  << ")" << std::endl;
        printStatus();
#endif
        m_mtxSockets.unlock();
    }

    ///
    /// \brief unregisterSocket
    /// \param sock
    ///
    void unregisterSocket( const socket_fd& sock ) {

        m_mtxSockets.lock();
        assert( m_sockets.find( sock ) != m_sockets.end() );
        m_sockets.erase( sock );
#ifdef HUB_DEBUG_NET
        std::cout << "[NetManager:" << std::this_thread::get_id() << "] unregisterSocket(" << sock
                  << ")" << std::endl;
        printStatus();
#endif
        m_mtxSockets.unlock();
    }

  private:
    std::set<socket_fd> m_sockets;
    std::mutex m_mtxSockets;
};
static NetManager s_netManager;

static bool s_inited = false;

socket_fd invalidSocket() {
    return INVALID_SOCKET;
}

static inline void init() {
#ifdef HUB_DEBUG_NET
    std::cout << getHeader() << "init()" << std::endl;
#endif
#if defined WIN32
    if ( !s_inited ) {
        WSADATA wsaData;
        int iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
        if ( iResult != 0 ) {
            printf( "error at WSAStartup\n" );
            exit( 1 );
        }
    }
#else

#endif
    s_inited = true;
}

bool isValid( socket_fd sock ) {
    return sock != INVALID_SOCKET;
}

void closeSocket( socket_fd& sock ) {
    assert( isValid( sock ) );

#ifdef HUB_DEBUG_NET
    std::cout << getHeader() << "closeSocket(" << sock << ") close socket" << std::endl;
    std::cout << getHeader() << "s_sockets = ";
    for ( const auto& socket : s_sockets ) {
        std::cout << socket << " ";
    }
    std::cout << std::endl;
#endif

    if ( ::closesocket( sock ) != 0 ) { perror( "closesocket" ); }

    s_netManager.unregisterSocket( sock );

#ifdef HUB_DEBUG_NET
    std::cout << "[net] closing socket: " << sock << std::endl;
#endif

    sock = INVALID_SOCKET;
}

bool isConnected( socket_fd sock ) {

    assert( utils::isValid( sock ) );

#ifdef WIN32
    return true;
#else

    // TODO - make this better, because its probably wrong (but seems to work)

    int optval;
    socklen_t optlen = sizeof( optval );

    int res;
    res = getsockopt( sock, SOL_SOCKET, SO_ERROR, &optval, &optlen ); // todo

    if ( optval == 0 && res == 0 ) return true;
#endif

    return false;
}

/////////////////////////////// SERVER ADDRESS /////////////////////////////////////////////

///
/// \brief The ServerAddrImpl class
///
class ServerAddrImpl
{
  public:

    ///
    /// \brief init
    /// \param port
    ///
    void init( int port ) {
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        m_sockAddr.sin_family      = AF_INET;
        m_sockAddr.sin_addr.s_addr = INADDR_ANY;
        m_sockAddr.sin_port        = htons( port ); // Server port
    }

    ///
    /// \brief m_sockAddr
    ///
    struct sockaddr_in m_sockAddr;
};

ServerAddr::ServerAddr() : m_pimpl( new ServerAddrImpl ) {}

ServerAddr::~ServerAddr() = default;

void ServerAddr::init( int port ) {
    m_pimpl->init( port );
}

socket_fd serverSocket() {
    if ( !s_inited ) init();
    int sock;
    sock = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    s_netManager.registerSocket( sock );
    int option = 1;
    (void)option;
#ifndef WIN32
    setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof( option ) );
#endif
#ifdef HUB_DEBUG_NET
    std::cout << "[net] init socket: " << sock << std::endl;
#endif
    return sock;
}

int bind( socket_fd sock, ServerAddr& addr ) {
    return ::bind( sock,
                   reinterpret_cast<struct sockaddr*>( &addr.m_pimpl->m_sockAddr ),
                   sizeof( struct sockaddr_in ) ); // todo
}

int listen( socket_fd sock, int backlog ) {
    return ::listen( sock, backlog ); // todo
}

///////////////////////////////// CLIENT ADDRESS ///////////////////////////////////////////

///
/// \brief The ClientAddrImpl class
///
class ClientAddrImpl
{
  public:
    ///
    /// \brief init
    /// \param ipv4
    /// \param port
    ///
    void init( const std::string& ipv4, int port ) {
        // Server address construction
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        m_sockAddr.sin_family = AF_INET;
        inet_pton( AF_INET, ipv4.c_str(), &m_sockAddr.sin_addr.s_addr ); // winsock 2.0
        m_sockAddr.sin_port = htons( port );                             // Server port
    }

    ///
    /// \brief setPort
    /// \param port
    ///
    void setPort( int port ) {
        m_sockAddr.sin_port = htons( port ); // Server port
    }

    ///
    /// \brief setIpv4
    /// \param ipv4
    ///
    void setIpv4( const std::string& ipv4 ) {
        inet_pton( AF_INET, ipv4.c_str(), &m_sockAddr.sin_addr.s_addr ); // winsock 2.0
    }

    ///
    /// \brief m_sockAddr
    ///
    struct sockaddr_in m_sockAddr;
};

ClientAddr::ClientAddr() : m_pimpl( new ClientAddrImpl ) {}

ClientAddr::~ClientAddr()                                  = default;
ClientAddr::ClientAddr( ClientAddr&& clientAddr ) noexcept = default;

void ClientAddr::init( const std::string& ipv4, int port ) {
    m_pimpl->init( ipv4, port );
}

void ClientAddr::setPort( int port ) {
    m_pimpl->setPort( port );
}

void ClientAddr::setIpv4( const std::string& ipv4 ) {
    m_pimpl->setIpv4( ipv4 );
}

std::string ClientAddr::getIpv4() const {
    char buff[INET_ADDRSTRLEN] { 0 };
    inet_ntop( AF_INET, &m_pimpl->m_sockAddr.sin_addr.s_addr, buff, INET_ADDRSTRLEN );
    return std::string( buff );
}

int ClientAddr::getPort() const {
    return m_pimpl->m_sockAddr.sin_port;
}

///////////////////////////////////////////////////////////////////////////

socket_fd accept( socket_fd sock, ClientAddr& addr ) {
    auto& sockAddr    = addr.m_pimpl->m_sockAddr;
    socklen_t addrlen = sizeof( sockAddr );
    socket_fd new_socket =
        accept( sock, reinterpret_cast<struct sockaddr*>( &sockAddr ), &addrlen );
#ifdef HUB_DEBUG_NET
    std::cout << "[net] init socket: " << new_socket << std::endl;
#endif
    s_netManager.registerSocket( new_socket );
    return new_socket;
}

socket_fd clientSocket() {
    if ( !s_inited ) init();
    int sock = ::socket( PF_INET, SOCK_STREAM, 0 );
    s_netManager.registerSocket( sock );
#ifdef HUB_DEBUG_NET
    std::cout << "[net] init socket: " << sock << std::endl;
#endif
    return sock;
}

int connect( socket_fd sock, ClientAddr& addr ) {
    return ::connect( sock,
                      reinterpret_cast<struct sockaddr*>( &addr.m_pimpl->m_sockAddr ),
                      sizeof( struct sockaddr_in ) ); // todo
}

int64_t send( socket_fd sock, const char* buf, size_t len, int flags ) {
    assert( isConnected( sock ) );
    return ::send( sock, buf, len, flags ); // todo
}

int64_t recv( socket_fd sock, char* buf, size_t len, int flags ) {
    assert( isConnected( sock ) );
    return ::recv( sock, buf, len, flags ); // todo
}

bool isValid( const std::string& ipv4 ) {
    return std::regex_match( ipv4, s_ipv4Regex );
}

bool isValid( int port ) {
    return ( 0 <= port && port <= 65535 );
}

} // namespace utils
} // namespace system
} // namespace net
} // namespace hub
