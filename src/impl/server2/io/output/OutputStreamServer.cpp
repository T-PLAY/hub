
#include "OutputStreamServer.hpp"

#include <iostream>
#include <typeinfo>

// #include "impl/server2/io/input/InputStreamServer.hpp"
#include "net/ServerSocket.hpp"

namespace hub {
namespace output {

OutputStreamServer::OutputStreamServer( const std::string& streamName,
                                        const std::string& ipv4,
                                        int port ) :
    // OutputStreamServer::OutputStreamServer( const std::string& streamName, net::ClientSocket&&
    // clientSocket ) :
    io::StreamServer( streamName, ipv4, port ),
    //    m_serverSocket( std::move( clientSocket ) ) {
    //    m_serverSocket( ipv4, port )
    //    m_serverSocket( std::make_unique<net::ClientSocket>( ipv4, port ) )
    //    m_serverSocket( std::make_unique<io::InputOutputSocket>( net::ClientSocket(ipv4, port) ) )
    m_serverSocket( net::ClientSocket( ipv4, port ) )
//    m_writingFun(write)

{
    setRetain( false );

    //    m_writingFun = [this](const Data_t *data, Size_t size) {
    //        write(data, size);
    //    };

    //    Output::write( net::ClientSocket::Type::STREAMER );
    m_serverSocket.write( ClientType::STREAMER );

    //    Output::write( streamName );
    m_serverSocket.write( streamName );

    io::StreamInterface::ServerMessage mess;
    m_serverSocket.read( mess );
    if ( mess == io::StreamInterface::ServerMessage::FOUND ) {
        m_serverSocket.close();
        //        throw net::Socket::exception(
        throw net::system::SocketSystem::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == io::StreamInterface::ServerMessage::NOT_FOUND );

    m_serverSocket.read( m_port );

    assert( m_serverSocket.isOpen() );
    //    auto* clientSocket   = m_serverSocket.get();
    //    auto* serverClosed   = m_serverClosed.get();
    //    auto* streamerClosed = m_streamerClosed.get();
    m_thread = std::make_unique<std::thread>( [this]() {
        DEBUG_MSG( "[OutputStream] listening port " << m_port );
        net::ServerSocket serverSocket( m_port );
        assert( serverSocket.isConnected() );
        while ( !m_killed ) {
            auto clientSock = io::InputOutputSocket( serverSocket.waitNewClient() );
            std::cout << "[OutputStream] new client: " << clientSock << std::endl;
            io::StreamInterface::ClientType clientType;
            clientSock.read( clientType );

            if ( clientType == io::StreamInterface::ClientType::STREAM_VIEWER ) {
                DEBUG_MSG( "[OutputStream] accepting new stream viewer at " << clientSock );
                if ( !m_retainedData.empty() ) {
                    clientSock.write( m_retainedData.data(), m_retainedData.size() );
                }
                m_clientSockets.push_back( std::move( clientSock ) );
            }
            else if ( clientType == io::StreamInterface::ClientType::KILLER ) {
                if ( m_killed ) { std::cout << "[OutputStream] harakiri" << std::endl; }
                else {
                    std::cout << "[OutputStream] killing by peer" << std::endl;
                    m_killed = true;
                }
            }
            else { assert( false ); }
        }
    } );

    //    m_threadWriting = std::make_unique<std::thread>( [this]() {
    //        auto * clientSocket = m_serverSocket.get();
    //            std::cout << "[OutputStreamServer:" << this
    //                      << "] OutputStreamServer(string, string, int) thread started" <<
    //                      std::endl;
    //            io::StreamInterface::ServerMessage serverMsg;
    //            assert( clientSocket->isOpen() );
    //            clientSocket->read( serverMsg );
    //            if ( serverMsg == io::StreamInterface::ServerMessage::SERVER_CLOSED ) {

    //                std::cout << "[OutputStreamServer] server closed" << std::endl;
    //                *serverClosed = true;
    //            }
    //            else if ( serverMsg == io::StreamInterface::ServerMessage::STREAMER_CLOSED ) {
    //                std::cout << "[OutputStreamServer] streamer closed" << std::endl;
    //                *streamerClosed = true;
    //            }
    //            else { assert( false ); }

    //            if ( clientSocket->isOpen() )
    //                clientSocket->write(
    //                io::StreamInterface::ClientMessage::STREAMER_CLIENT_CLOSED );

    //            std::cout << "[OutputStreamServer] OutputStreamServer(string, string, int)
    //            thread ended"
    //                      << std::endl;
    //    } );

    //    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //    std::cout << "[OutputStreamServer:" << this << "] OutputStreamServer(string, string, int)
    //    ended"
    //              << std::endl;
}

OutputStreamServer::~OutputStreamServer() {
    //    std::cout << "[OutputStreamServer:" << this << "] ~OutputStreamServer()" << std::endl;

    if ( m_thread != nullptr ) {
        //        SERVER_MSG( "~Server() joining main thread" );
        assert( m_thread->joinable() );
        //        if ( m_running ) { stop(); }
        stop();
        m_thread->join();
    }
    //    assert( !m_running );

    //    if ( !m_moved ) {

    //        //        assert(m_serverSocket->isOpen());
    //        //        if (m_serverSocket->isOpen())
    //        //            m_serverSocket->close();
    //        //    assert( m_serverSocket->isOpen() );
    //        if ( OutputStreamServer::isOpen() ) {
    //            std::cout << "[OutputStreamServer:" << this
    //                      << "] ~OutputStreamServer() closing connection" << std::endl;
    //            OutputStreamServer::close();
    //        }
    //        assert( !OutputStreamServer::isOpen() );

    //        assert( m_thread->joinable() );
    //        std::cout << "[OutputStreamServer:" << this << "] ~OutputStreamServer() joining
    //        thread"
    //                  << std::endl;
    //        m_thread->join();
    //    }
    //    std::cout << "[OutputStreamServer:" << this << "] ~OutputStreamServer() ended" <<
    //    std::endl;
}

void hub::output::OutputStreamServer::write( const Data_t* data, size_t size ) {
    //    assert(m_writingFun);
    m_writingFun( data, size );
    for ( auto& clientSocket : m_clientSockets ) {
        clientSocket.write( data, size );
    }
    //    m_serverSocket.write( data, len );
}

void OutputStreamServer::setRetain( bool retain ) {
    if ( retain ) {
        m_writingFun = [this]( const Data_t* data, Size_t size ) {
            m_retainedData.insert( m_retainedData.end(), data, data + size );
            //            for ( auto& clientSocket : m_clientSockets ) {
            //                clientSocket.write( data, size );
            //            }
        };
    }
    else {
//        m_writingFun = nullptr;
        m_writingFun = []( const Data_t*, Size_t) {
//            for ( auto& clientSocket : m_clientSockets ) {
//                clientSocket.write( data, size );
//            }
        };
    }
}

void OutputStreamServer::stop() {
    m_killed        = true;
    auto clientSock = io::InputOutputSocket( hub::net::ClientSocket( "127.0.0.1", m_port ) );
    clientSock.write( io::StreamInterface::ClientType::KILLER );
    //    hub::input::InputStreamServer inputStream(
    //        io::StreamServer::s_exitSignal, "127.0.0.1", m_port );
}

// OutputStreamServer::OutputStreamServer( OutputStreamServer&& outputStream ) :
//     //    OutputStreamInterface(outputStream.m_name, outputStream.m_ipv4, outputStream.m_port),
//     StreamServer( outputStream.m_name, outputStream.m_ipv4, outputStream.m_port ),
//     m_serverSocket( std::move( outputStream.m_serverSocket ) ),
//     m_thread( std::move( outputStream.m_thread ) ),
//     m_moved( outputStream.m_moved ),
//     m_serverClosed( std::move( outputStream.m_serverClosed ) ),
//     m_streamerClosed( std::move( outputStream.m_streamerClosed ) ) {

//    std::cout << "[OutputStreamServer:" << this << "] OutputStreamServer(OutputStreamServer&&)"
//              << std::endl;
//    outputStream.m_moved = true;
//}

} // namespace output
} // namespace hub
