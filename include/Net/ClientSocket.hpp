#pragma once

#include <regex>

#include "IO/Interface.hpp"
#include "Net/Socket.hpp"

namespace hub {
namespace net {

///
/// \brief The ClientSocket class
/// is an connected socket to the server
///
class SRC_API ClientSocket : public Socket, public virtual io::Interface
{
  public:
    ///
    /// \brief The Type enum
    ///
    enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, COUNT };

    ///
    /// \brief operator <<
    /// \param os
    /// \param type
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Type& type );

    ///
    /// \brief The Message enum
    ///
    enum class Message {
        NONE,
        PING,
        SYNC,
        DATA,
        OK,
        CLOSE,
        DEL_STREAMER,
        NEW_STREAMER,
        NOT_FOUND,
        FOUND,
        NEW_ACQ,
        COUNT
    };

    ///
    /// \brief operator <<
    /// \param os
    /// \param msg
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Message& msg );

    ClientSocket();

    ///
    /// \brief ClientSocket
    /// \param ipv4
    /// \param port
    /// \param autoConnect
    ///
    ClientSocket( const std::string& ipv4, int port, bool autoConnect = true );

    ///
    /// \brief ClientSocket
    /// \param fdSock
    ///
    ClientSocket( net::utils::socket_fd fdSock ); // server side client (bind and listen)

    ClientSocket( const ClientSocket& sock ) = delete;

    ///
    /// \param sock
    ///
    ClientSocket( ClientSocket&& sock )      = default;

    ClientSocket& operator=( const ClientSocket& sock ) = delete;
    ClientSocket&& operator=( ClientSocket&& sock ) = delete;

    ~ClientSocket();

  public:
    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( const T& t ) const;

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t ) const;

    ///
    /// \brief initServerAddress
    ///
    void initServerAddress();

    ///
    /// \brief connect
    ///
    void connect();

    ///
    /// \brief setIpv4
    /// \param newIpv4
    ///
    void setIpv4( const std::string& newIpv4 );

    ///
    /// \brief setPort
    /// \param newPort
    ///
    void setPort( int newPort );

    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;

  protected:
    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    void write( const unsigned char* data, size_t len ) const override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( unsigned char* data, size_t len ) const override;

    ///
    /// \brief close
    ///
    void close() const override;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;

    //    void clear() const;

  public:
    ///
    /// \brief getIpv4
    /// \return
    ///
    const std::string& getIpv4() const;

    ///
    /// \brief getPort
    /// \return
    ///
    const int& getPort() const;

    ///
    /////////////////////////////////// PRIVATE SECTION ///////////////////////////////////////////
    ///

  private:
    //    void connectToServer();

  private:
    std::string m_ipv4;
    int m_port;
    //    struct sockaddr_in m_serverAddress;
    net::utils::ClientAddr m_addr;
    mutable bool m_connected = false;

};

template <class T>
void ClientSocket::write( const T& t ) const {
    assert( isOpen() );
    io::Interface::write( t );
}

template <class T>
void ClientSocket::read( T& t ) const {
    assert( isOpen() );
    assert( isConnected() );
    io::Interface::read( t );
}

} // namespace net
} // namespace hub
