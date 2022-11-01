#include "Stream.hpp"

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
            ( std::string( "sensor '" ) + streamName + "' is already attached to server" )
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

void OutputStream::close() {
    net::ClientSocket::close();
}
#endif

///////////////////////////////////////////////////////////////

InputStream::InputStream( const std::string& streamName,
                          const std::string& syncStreamName,
                          net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {

    Interface::write( net::ClientSocket::Type::STREAM_VIEWER );

    Interface::write( streamName );
    ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStream] exception sensor '" << streamName
                                         << "' is not attached to server" );
#endif
        throw ClientSocket::exception(
            ( std::string( "sensor '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( mess == ClientSocket::Message::OK );

    Interface::write( syncStreamName );
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
        throw ClientSocket::exception(
            ( std::string( "sync sensor '" ) + syncStreamName + "' is not attached to server" )
                .c_str() );
    }
    assert( mess == ClientSocket::Message::OK );
}

#ifdef WIN32
void InputStream::write( const unsigned char* data, size_t len ) const {
    net::ClientSocket::write( data, len );
}

void InputStream::read( unsigned char* data, size_t len ) const {
    net::ClientSocket::read( data, len );
}

void InputStream::close() {
    net::ClientSocket::close();
}
#endif

Acquisition InputStream::getAcquisition( int acquisitionSize ) const {
    net::ClientSocket::Message message;
    do {
        Interface::read( message );
        if ( message == net::ClientSocket::Message::PING ) {
            std::cout << "[InputStream] receive ping" << std::endl;
        }
    } while ( message == net::ClientSocket::Message::PING );

    assert( message == net::ClientSocket::Message::NEW_ACQ );
    auto acq = Interface::getAcquisition( acquisitionSize );
    return acq;
}

} // namespace io
} // namespace hub
