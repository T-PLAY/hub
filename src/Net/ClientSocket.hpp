#pragma once

#include "Socket.hpp"
#include "IO/Interface.hpp"

namespace hub {
namespace net {

class ClientSocket : public Socket, public io::Interface
{
  public:
    enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, COUNT };
    static constexpr char const* type2string[static_cast<int>( Type::COUNT )] = {
        "NONE",
        "STREAMER",
        "VIEWER",
        "STREAM_VIEWER",
    };
    friend std::ostream& operator<<( std::ostream& os, const Type& type ) {
        os << type2string[(int)type];
        return os;
    }

    SRC_API ClientSocket();
    SRC_API ClientSocket( const std::string& ipv4, int port );
    //    SRC_API ClientSocket( const std::string& sensorName,
    //                          const std::string& syncSensorName = "",
    //                          const std::string ipv4            = SERVICE_IP,
    //                          int port = SERVICE_PORT ); // client : stream viewer
    SRC_API ClientSocket( socket_fd fdSock ); // server side client (bind and listen)

    ClientSocket( const ClientSocket& sock ) = delete;
    ClientSocket( ClientSocket& sock )       = delete;
    SRC_API ClientSocket( ClientSocket&& sock ) noexcept;

    ClientSocket& operator=( const ClientSocket& sock ) = delete;
    ClientSocket&& operator=( ClientSocket&& sock )     = delete;

    SRC_API ~ClientSocket();

  public:
    void close() override;

    void write( const unsigned char* data, size_t len ) const override;
    template <class T>
    void write( const T& t ) const;

    void read( unsigned char* data, size_t len ) const override;
    template <class T>
    void read( T& t ) const;

    void waitClose() const;
    SRC_API void clear();

    void setupOutput( const std::string& sensorName ) const override;

    SRC_API void setIsServer( bool isServer );

  private:
    void connectToServer();

  private:
    std::string mIpv4;
    int mPort;
    bool mIsServer = false;
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
