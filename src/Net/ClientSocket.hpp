#pragma once

#include "IO/Interface.hpp"
#include "Socket.hpp"

namespace hub {
namespace net {

class ClientSocket : public Socket, public virtual io::Interface
{
  public:
    enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, COUNT };
    friend std::ostream& operator<<( std::ostream& os, const Type& type );

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
        COUNT
    };
    friend std::ostream& operator<<( std::ostream& os, const Message& msg );

    SRC_API ClientSocket();
    SRC_API ClientSocket( const std::string& ipv4, int port );
    //    SRC_API ClientSocket( const std::string& sensorName,
    //                          const std::string& syncSensorName = "",
    //                          const std::string ipv4            = SERVICE_IP,
    //                          int port = SERVICE_PORT ); // client : stream viewer
    SRC_API ClientSocket( socket_fd fdSock ); // server side client (bind and listen)

    ClientSocket( const ClientSocket& sock ) = delete;
    //    ClientSocket( ClientSocket& sock )       = delete;
    SRC_API ClientSocket( ClientSocket&& sock ) = default;

    ClientSocket& operator=( const ClientSocket& sock ) = delete;
    ClientSocket&& operator=( ClientSocket&& sock )     = delete;

    SRC_API ~ClientSocket();

  public:
    //    void close() override;
    template <class T>
    void write( const T& t ) const;

    template <class T>
    void read( T& t ) const;

  protected:
    void write( const unsigned char* data, size_t len ) const override;

    void read( unsigned char* data, size_t len ) const override;

    void close() override;

    //    void waitClose() const;
    SRC_API void clear();

    //    void setupOutput( const std::string& sensorName ) const override;

    //    SRC_API void setIsServer( bool isServer );

  private:
    void connectToServer();

  private:
    std::string mIpv4;
    int mPort;

    //    bool mIsServer = false;
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
