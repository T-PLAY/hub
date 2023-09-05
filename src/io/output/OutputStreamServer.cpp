
#include "OutputStreamServer.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace output {

OutputStreamServer::OutputStreamServer(const std::string &streamName, const std::string &ipv4, int port) :
//OutputStreamServer::OutputStreamServer( const std::string& streamName, net::ClientSocket&& clientSocket ) :
    io::StreamServer(streamName, ipv4, port),
//    m_clientSocket( std::move( clientSocket ) ) {
//    m_clientSocket( ipv4, port )
    m_clientSocket( std::make_unique<net::ClientSocket>(ipv4, port) )
{

    Output::write( net::ClientSocket::Type::STREAMER );

    Output::write( streamName );

    net::ClientSocket::Message mess;
    m_clientSocket->read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        m_clientSocket->close();
        throw net::Socket::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );


    assert(m_clientSocket->isOpen());
    auto * clientSocket = m_clientSocket.get();
    auto * serverClosed = m_serverClosed.get();
    auto * streamerClosed = m_streamerClosed.get();
    m_thread = std::make_unique<std::thread>( [=]() {
//        auto * clientSocket = m_clientSocket.get();
        std::cout << "[OutputStreamServer] OutputStreamServer(string, string, int) thread started" << std::endl;
        hub::net::ClientSocket::Message message;
        assert( clientSocket->isOpen() );
        clientSocket->read( message );
        if ( message == net::ClientSocket::Message::SERVER_CLOSED ) {

            std::cout << "[OutputStreamServer] server closed" << std::endl;
            *serverClosed = true;
        }
        else if ( message == net::ClientSocket::Message::STREAMER_CLOSED ) {
            std::cout << "[OutputStreamServer] streamer closed" << std::endl;
            *streamerClosed = true;
        }
        else {
            assert( false );
        }

        if ( clientSocket->isOpen() )
            clientSocket->write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
        std::cout << "[OutputStreamServer] OutputStreamServer(string, string, int) thread ended" << std::endl;
    } );
    std::cout << "[OutputStreamServer] OutputStreamServer(string, string, int) ended" << std::endl;
}


OutputStreamServer::OutputStreamServer( OutputStreamServer&& outputStream ) :
//    OutputStreamInterface(outputStream.m_name, outputStream.m_ipv4, outputStream.m_port),
    StreamServer(outputStream.m_name, outputStream.m_ipv4, outputStream.m_port),
    m_clientSocket( std::move( outputStream.m_clientSocket ) ),
    m_thread( std::move( outputStream.m_thread ) ),
    m_moved( outputStream.m_moved ) {

    std::cout << "[OutputStreamServer] OutputStreamServer(OutputStreamServer&&)" << std::endl;
    outputStream.m_moved = true;
}

OutputStreamServer::~OutputStreamServer() {

    if ( !m_moved ) {
        assert( !OutputStreamServer::isOpen() );
        assert( m_thread->joinable() );
        m_thread->join();
    }
}

} // namespace output
} // namespace hub
