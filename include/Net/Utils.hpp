#pragma once

#include <iostream>
#include <memory>

#include "Macros.hpp"

namespace hub {
namespace net {
namespace utils {

using socket_fd = uint64_t;
//    using socklen_t = int;

socket_fd SRC_API invalidSocket();
//     void SRC_API init();
bool SRC_API isValid( socket_fd sock );
void SRC_API closeSocket( socket_fd& sock );
bool SRC_API isConnected( socket_fd sock );

///////////////////////////////////// SERVER ADDRESS ///////////////////////////////////////

class ServerAddrImpl;

///
/// \brief The ServerAddr class
///
class SRC_API ServerAddr
{
  public:
    explicit ServerAddr();
    ~ServerAddr();
    //          ~ServerAddr() = default;

    // movable
    //        ServerAddr(ServerAddr && serverAddr) noexcept;
    //        ServerAddr& operator=(ServerAddr && serverAddr) noexcept;

    // not copyable
    //        ServerAddr(const ServerAddr& serverAddr) = delete;
    //        ServerAddr & operator=(const ServerAddr& serverAddr) = delete;

    ///
    /// \brief init
    /// \param port
    ///
    void init( int port );
    //        void setPort(int port);
    //      private:

    ///
    /// \brief m_pimpl
    ///
    std::unique_ptr<ServerAddrImpl> m_pimpl;
};
// socket_fd accept(socket_fd sock, struct sockaddr*, socklen_t*);
///
/// \brief serverSocket
/// \return
///
socket_fd SRC_API serverSocket();

///
/// \brief bind
/// \param sock
/// \param addr
/// \return
///
int SRC_API bind( socket_fd sock, ServerAddr& addr );

///
/// \brief listen
/// \param sock
/// \param backlog
/// \return
///
int SRC_API listen( socket_fd sock, int backlog );

///
/// \brief accept
/// \param sock
/// \param addr
/// \return
///
socket_fd SRC_API accept( socket_fd sock, ServerAddr& addr );

/////////////////////////////////////// CLIENT ADDRESS /////////////////////////////////////

class ClientAddrImpl;

///
/// \brief The ClientAddr class
///
class SRC_API ClientAddr
{
  public:
    explicit ClientAddr();
    ~ClientAddr();

    // movable
    ///
    /// \brief ClientAddr
    /// \param clientAddr
    ///
    ClientAddr( ClientAddr&& clientAddr ) noexcept;
    //        ClientAddr& operator=(ClientAddr && clientAddr) noexcept;

    //        // not copyable
    //        ClientAddr(const ClientAddr& clientAddr) = delete;
    //        ClientAddr & operator=(const ClientAddr& clientAddr) = delete;

    ///
    /// \brief init
    /// \param ipv4
    /// \param port
    ///
    void init( const std::string& ipv4, int port );

    ///
    /// \brief setPort
    /// \param port
    ///
    void setPort( int port );

    ///
    /// \brief setIpv4
    /// \param ipv4
    ///
    void setIpv4( const std::string& ipv4 );

    ///
    /// \brief m_pimpl
    ///
    std::unique_ptr<ClientAddrImpl> m_pimpl;
};

///
/// \brief clientSocket
/// \return
///
socket_fd SRC_API clientSocket();

///
/// \brief connect
/// \param sock
/// \param addr
/// \return
///
int SRC_API connect( socket_fd sock, ClientAddr& addr );

///
/// \brief send
/// \param sock
/// \param buf
/// \param len
/// \param flags
/// \return
///
int SRC_API send( socket_fd sock, const char* buf, int len, int flags );

///
/// \brief recv
/// \param buf
/// \param len
/// \param flags
/// \return
///
int SRC_API recv( socket_fd, char* buf, int len, int flags );

// static std::string getHeader() {
//     const unsigned int id =
//         static_cast<int>( std::hash<std::thread::id> {}( std::this_thread::get_id() ) );
//     std::string str = "[\033[31mNet\033[0m:\033[" + std::to_string( 31 + id % 7 ) + "m" +
//                       std::to_string( id ) + "]\033[0m ";
//     return str;
// }

// static bool s_inited = false;
// static std::list<socket_fd> s_sockets;
// static std::mutex s_mtx;

// static void registerSocket( socket_fd socket ) {
//     s_mtx.lock();

//            std::cout << getHeader() << "registerSocket(" << socket << ")" << std::endl;

// #ifdef DEBUG_NET
//     std::cout << getHeader() << "registerSocket(" << socket << ")" << std::endl;
// #endif
//     s_sockets.push_back( socket );
// #ifdef DEBUG_NET
//     std::cout << getHeader() << "s_sockets = ";
//     for ( const auto& socket : s_sockets ) {
//         std::cout << socket << " ";
//     }
//     std::cout << std::endl;
// #endif

//    s_mtx.unlock();
//}

} // namespace utils
} // namespace net
} // namespace hub
