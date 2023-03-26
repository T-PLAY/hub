#pragma once

//#include "io/Input.hpp"
#include "io/InputOutput.hpp"
#include "net/Socket.hpp"

namespace hub {
namespace net {

///
/// \brief s_defaultServicePort
/// is the default opened for the hub service (server).
///
static constexpr int s_defaultServicePort = 4042;

///
/// \brief s_defaultServiceIp
/// by default hub service is running localy.
///
static std::string s_defaultServiceIp = "127.0.0.1";

///
/// \brief The ClientSocket class
/// allows connection to remote server hubs.
/// This class describes the functionality needed to open a remote communication.
///
// class SRC_API ClientSocket : public Socket, public io::Input, public io::Output
class SRC_API ClientSocket : public Socket, public io::InputOutput
// class SRC_API ClientSocket : public Socket
{
  public:
    ///
    /// \brief The Type enum
    /// corresponds of the type of connection uses by the server to process
    /// things corresponding of each type of new connected client.
    ///
    enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, ASKER, COUNT };

    ///
    /// \brief operator <<
    /// \param os
    /// \param type
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Type& type );

    ///
    /// \brief The Message enum
    /// define atomic message to communicate to the server.
    ///
    enum class Message {
        NONE,
        PING,
        SYNC,
        DATA,
        OK,
        CLOSE, // connection
        DEL_STREAMER,
        NEW_STREAMER,
        NOT_FOUND,
        FOUND,
        NEW_ACQ,
        LIST_STREAMS,
        GET_SENSOR_SPEC,
        GET_ACQUISITION,
        CLOSED,
        STREAM_VIEWER_CLOSED,
        STREAMER_CLOSED,
        INPUT_STREAM_CLOSED,
        OUTPUT_STREAM_CLOSED,
        VIEWER_CLOSED,
        VIEWER_CLIENT_CLOSED,
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
    /// is used to open a remote communication
    /// \param ipv4
    /// [in] string corresponds to the server ip (ex: "127.0.0.1" for local server)
    /// \param port
    /// [in] opened service of the running server
    /// \param autoConnect
    /// [in] connect to the server
    ///
    ClientSocket( const std::string& ipv4, int port, bool autoConnect = true );

    ///
    /// \brief ClientSocket
    /// \param fdSock
    ///
    explicit ClientSocket( net::utils::socket_fd fdSock ); // server side client (bind and listen)

    ClientSocket( const ClientSocket& sock ) = delete;

    ClientSocket( ClientSocket&& sock ) = default;

    ClientSocket& operator=( const ClientSocket& sock ) = delete;
    ClientSocket&& operator=( ClientSocket&& sock ) = delete;

    ~ClientSocket();

  public:
    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( const T& t );

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t );

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

  public:
    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;

    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    void write( const unsigned char* data, size_t len ) override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( unsigned char* data, size_t len ) override;

  public:
    ///
    /// \brief close
    ///
    void close() override;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;

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

  private:
    std::string m_ipv4;
    int m_port;
    net::utils::ClientAddr m_addr;
    bool m_connected = false;
//    bool m_moved     = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class T>
void ClientSocket::write( const T& t ) {
    // todo fix
    // server side
    // Assertion failed: isOpen()
    // D:\a\_work\1\s\src\vctools\crt\github\stl\src\mutex.cpp(64): mutex destroyed while busy
//    assert( isOpen() );
    io::Output::write( t );
}

template <class T>
void ClientSocket::read( T& t ) {
//    assert( isOpen() );
//    assert( isConnected() );
    io::Input::read( t );
}

} // namespace net
} // namespace hub
