#pragma once

#include "IO/Interface.hpp"
#include "Socket.hpp"

namespace hub {
namespace net {

///
/// \brief The ClientSocket class
/// is an connected socket to the server
///
class SRC_API ClientSocket : public Socket, public virtual io::Interface
{
  public:
    enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, COUNT };
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Type& type );

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
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Message& msg );

     ClientSocket();
     ClientSocket( const std::string& ipv4, int port );
     ClientSocket( socket_fd fdSock ); // server side client (bind and listen)

    ClientSocket( const ClientSocket& sock )    = delete;
     ClientSocket( ClientSocket&& sock ) = default;

    ClientSocket& operator=( const ClientSocket& sock ) = delete;
    ClientSocket&& operator=( ClientSocket&& sock )     = delete;

     ~ClientSocket();

  public:
    template <class T>
    void write( const T& t ) const;

    template <class T>
    void read( T& t ) const;

  protected:
    void write( const unsigned char* data, size_t len ) const override;

    void read( unsigned char* data, size_t len ) const override;

    void close() override;

//    Acquisition getAcquisition(int acquistionSize) const override;

     void clear();

  private:
    void connectToServer();

  private:
    const std::string m_ipv4;
    int m_port;
};

template <class T>
void ClientSocket::write( const T& t ) const {
    io::Interface::write( t );
}

template <class T>
void ClientSocket::read( T& t ) const {
    io::Interface::read( t );
}

} // namespace net
} // namespace hub
