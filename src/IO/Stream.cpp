#include "Stream.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

OutputStream::OutputStream( const std::string& streamName, net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {

    std::cout << "\t[OutputStream] OutputStream(string, ClientSocket&&)" << std::endl;

    Interface::write( net::ClientSocket::Type::STREAMER );

    Interface::write( streamName );

    net::ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        assert( false );
        throw net::Socket::exception(
            ( std::string( "sensor '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );
}

InputStream::InputStream( const std::string& streamName,
                          const std::string& syncStreamName,
                          net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {

    std::cout << "\t[InputStream] InputStream(string, string, ClientSocket&&)" << std::endl;

    Interface::write( net::ClientSocket::Type::STREAM_VIEWER );

    Interface::write( streamName );
    ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStream] exception sensor '" << streamName
                                         << "' is not attached to server" );
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

    std::cout << "\t[InputStream] InputStream(string, string, ClientSocket&&) end" << std::endl;
}

Acquisition InputStream::getAcquisition( int acquisitionSize ) const {
//    std::cout << "[InputStream **************************************" << std::endl;
    net::ClientSocket::Message message;
    do {
        Interface::read( message );
    } while ( message == net::ClientSocket::Message::PING );

    assert( message == net::ClientSocket::Message::NEW_ACQ );
    auto acq = Interface::getAcquisition( acquisitionSize );
    return acq;
}

} // namespace io
} // namespace hub
