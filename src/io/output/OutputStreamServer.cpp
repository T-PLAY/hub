
#include "OutputStreamServer.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace output {

OutputStreamServer::OutputStreamServer( const std::string& streamName,
                                        const std::string& ipv4,
                                        int port ) :
    // OutputStreamServer::OutputStreamServer( const std::string& streamName, net::ClientSocket&&
    // clientSocket ) :
    io::StreamServer( streamName, ipv4, port ),
    //    m_clientSocket( std::move( clientSocket ) ) {
    //    m_clientSocket( ipv4, port )
//    m_clientSocket( std::make_unique<net::ClientSocket>( ipv4, port ) )
    m_clientSocket( std::make_unique<io::InputOutputSocket>( net::ClientSocket(ipv4, port) ) )
{

    //    Output::write( net::ClientSocket::Type::STREAMER );
    m_clientSocket->write( ClientType::STREAMER );

    //    Output::write( streamName );
    m_clientSocket->write( streamName );

    io::StreamBase::ServerMessage mess;
    m_clientSocket->read( mess );
    if ( mess == io::StreamBase::ServerMessage::FOUND ) {
        m_clientSocket->close();
//        throw net::Socket::exception(
        throw net::system::SocketSystem::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == io::StreamBase::ServerMessage::NOT_FOUND );

    assert( m_clientSocket->isOpen() );
    auto* clientSocket   = m_clientSocket.get();
    auto* serverClosed   = m_serverClosed.get();
    auto* streamerClosed = m_streamerClosed.get();
    m_thread             = std::make_unique<std::thread>( [=, this]() {
        //        auto * clientSocket = m_clientSocket.get();
        std::cout << "[OutputStreamServer:" << this
                  << "] OutputStreamServer(string, string, int) thread started" << std::endl;
        io::StreamBase::ServerMessage serverMsg;
        assert( clientSocket->isOpen() );
        clientSocket->read( serverMsg );
        if ( serverMsg == io::StreamBase::ServerMessage::SERVER_CLOSED ) {

            std::cout << "[OutputStreamServer] server closed" << std::endl;
            *serverClosed = true;
        }
        else if ( serverMsg == io::StreamBase::ServerMessage::STREAMER_CLOSED ) {
            std::cout << "[OutputStreamServer] streamer closed" << std::endl;
            *streamerClosed = true;
        }
        else { assert( false ); }

        if ( clientSocket->isOpen() )
            clientSocket->write( io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED );

        std::cout << "[OutputStreamServer] OutputStreamServer(string, string, int) thread ended"
                  << std::endl;
    } );

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    std::cout << "[OutputStreamServer:" << this << "] OutputStreamServer(string, string, int) ended"
              << std::endl;
}

OutputStreamServer::OutputStreamServer( OutputStreamServer&& outputStream ) :
    //    OutputStreamInterface(outputStream.m_name, outputStream.m_ipv4, outputStream.m_port),
    StreamServer( outputStream.m_name, outputStream.m_ipv4, outputStream.m_port ),
    m_clientSocket( std::move( outputStream.m_clientSocket ) ),
    m_thread( std::move( outputStream.m_thread ) ),
    m_moved( outputStream.m_moved ),
    m_serverClosed( std::move( outputStream.m_serverClosed ) ),
    m_streamerClosed( std::move( outputStream.m_streamerClosed ) ) {

    std::cout << "[OutputStreamServer:" << this << "] OutputStreamServer(OutputStreamServer&&)"
              << std::endl;
    outputStream.m_moved = true;
}

OutputStreamServer::~OutputStreamServer() {
    std::cout << "[OutputStreamServer:" << this << "] ~OutputStreamServer()" << std::endl;

    if ( !m_moved ) {

        //        assert(m_clientSocket->isOpen());
        //        if (m_clientSocket->isOpen())
        //            m_clientSocket->close();
        //    assert( m_clientSocket->isOpen() );
        if ( OutputStreamServer::isOpen() ) {
            std::cout << "[OutputStreamServer:" << this
                      << "] ~OutputStreamServer() closing connection" << std::endl;
            OutputStreamServer::close();
        }
        assert( !OutputStreamServer::isOpen() );

        assert( m_thread->joinable() );
        std::cout << "[OutputStreamServer:" << this << "] ~OutputStreamServer() joining thread"
                  << std::endl;
        m_thread->join();
    }
    std::cout << "[OutputStreamServer:" << this << "] ~OutputStreamServer() ended" << std::endl;
}

} // namespace output
} // namespace hub
