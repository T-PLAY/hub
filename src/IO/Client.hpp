#pragma once

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

class Client : public Interface
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

    Client( net::ClientSocket&& clientSocket );
    Client( const Client& client ) = delete;

  protected:
    void close() override;
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;

  private:
    std::unique_ptr<net::ClientSocket> m_socket;
};

} // namespace io
} // namespace hub
