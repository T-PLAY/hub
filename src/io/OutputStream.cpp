
#include "io/OutputStream.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

OutputStream::OutputStream( const std::string& streamName, net::ClientSocket&& clientSocket ) :
    m_clientSocket( std::move( clientSocket ) ) {

    Output::write( net::ClientSocket::Type::STREAMER );

    Output::write( streamName );

    net::ClientSocket::Message mess;
    m_clientSocket.read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        m_clientSocket.close();
        throw net::Socket::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );

    m_thread = std::make_unique<std::thread>( [&]() {
        hub::net::ClientSocket::Message message;
        assert( m_clientSocket.isOpen() );
        m_clientSocket.read( message );
        if ( message == net::ClientSocket::Message::SERVER_CLOSED ) {

            std::cout << "[OutputStream] server closed" << std::endl;
            m_serverClosed = true;
        }
        else if ( message == net::ClientSocket::Message::STREAMER_CLOSED ) {
            std::cout << "[OutputStream] streamer closed" << std::endl;
            m_streamerClosed = true;
        }
        else {
            assert( false );
        }

        if ( m_clientSocket.isOpen() )
            m_clientSocket.write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
    } );
}

OutputStream::OutputStream( OutputStream&& outputStream ) :
    m_clientSocket( std::move( outputStream.m_clientSocket ) ),
    m_thread( std::move( outputStream.m_thread ) ),
    m_moved( outputStream.m_moved ) {
    outputStream.m_moved = true;
}

OutputStream::~OutputStream() {

    if ( !m_moved ) {
        assert( !OutputStream::isOpen() );
        assert( m_thread->joinable() );
        m_thread->join();
    }
}

} // namespace io
} // namespace hub
