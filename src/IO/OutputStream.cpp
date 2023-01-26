
#include "IO/OutputStream.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

OutputStream::OutputStream( const std::string& streamName, net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {

    Interface::write( net::ClientSocket::Type::STREAMER );

    Interface::write( streamName );

    net::ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        throw net::Socket::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );
}

#ifdef WIN32
void OutputStream::write( const unsigned char* data, size_t len ) const {
    net::ClientSocket::write( data, len );
}

void OutputStream::read( unsigned char* data, size_t len ) const {
    net::ClientSocket::read( data, len );
}

void OutputStream::close() const {
    net::ClientSocket::close();
}

bool OutputStream::isOpen() const {
    return net::ClientSocket::isOpen();
}

bool OutputStream::isEnd() const {
    return net::ClientSocket::isEnd();
}
#endif

} // namespace io
} // namespace hub
