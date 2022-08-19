#include "Client.hpp"

namespace hub {
namespace io {

static constexpr std::string_view type2string[static_cast<int>( Client::Type::COUNT )] = {
    "NONE",
    "STREAMER",
    "VIEWER",
    "STREAM_VIEWER",
};
std::ostream& operator<<( std::ostream& os, const Client::Type& type ) {
    os << type2string[(int)type];
    return os;
}

static constexpr std::string_view message2string[static_cast<int>( Client::Message::COUNT )] = {
    "NONE",
    "PING",
    "SYNC",
    "DATA",
    "OK",
    "CLOSE",
    "DEL_STREAMER",
    "NEW_STREAMER",
    "NOT_FOUND",
    "FOUND",
};
std::ostream& operator<<( std::ostream& os, const Client::Message& msg ) {
    os << message2string[(int)msg];
    return os;
}

Client::Client( net::ClientSocket&& clientSocket ) :
    m_socket( new net::ClientSocket( std::move( clientSocket ) ) ) {}
//    m_socket( new net::ClientSocket( clientSocket  ) ) {}

void Client::close() {}

void Client::write( const unsigned char* data, size_t len ) const {
    m_socket->write( data, len );
}

void Client::read( unsigned char* data, size_t len ) const {
    m_socket->read( data, len );
}

} // namespace io
} // namespace hub
