
#include "OutputStreamServer.hpp"

#include <execution>
#include <iostream>
#include <typeinfo>

#include "net/ServerSocket.hpp"

// #define DEBUG_OUTPUT_STREAM

namespace hub {
namespace output {

OutputStreamServer::OutputStreamServer( int streamPort ) :
    io::StreamServer( "", "", 0 ), m_serverDataThread( std::make_unique<ServerDataThread>() ) {
    m_serverDataThread->m_streamPort = streamPort;
    startServer();
}

void OutputStreamServer::streamConnect() {
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] stream connecting" << std::endl;
#endif
    assert( !m_streamConnected );
    m_clientSocket->write( ClientType::STREAMER );

    m_clientSocket->write( m_name );

    io::StreamInterface::ServerMessage mess;
    m_clientSocket->read( mess );
    if ( mess == io::StreamInterface::ServerMessage::FOUND ) {
        m_clientSocket->close();
        throw net::system::SocketSystem::exception(
            ( std::string( "stream '" ) + m_name + "' is already attached to server" ).c_str() );
    }
    assert( mess == io::StreamInterface::ServerMessage::NOT_FOUND );

    m_clientSocket->write( m_serverDataThread->m_streamPort );
    m_clientSocket->read( m_serverDataThread->m_streamPort );

    m_clientSocket->write( (int)m_serverDataThread->m_clientSockets.size() );

    m_clientSocket->read( mess );
    assert( mess == io::StreamInterface::ServerMessage::STREAMER_INITED );
    m_streamConnected = true;
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] stream connected" << std::endl;
#endif
}

OutputStreamServer::OutputStreamServer( const std::string& streamName,
                                        int port,
                                        const std::string& ipv4 ) :
    io::StreamServer( streamName, ipv4, port ),
    m_clientSocket( std::make_unique<io::InputOutputSocket>( net::ClientSocket( ipv4, port ) ) ),
    m_serverDataThread( std::make_unique<ServerDataThread>() ) {

    streamConnect();
    assert( m_streamConnected );

    m_clientThread = std::make_unique<std::thread>( [this, streamName]() {
        io::StreamInterface::ServerMessage mess;
        while ( !m_shutdown ) {
            try {
                if ( !m_clientSocket->isConnected() ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] try connect to server " << *m_clientSocket
                              << std::endl;
#endif
                    m_clientSocket->connect();
                }
                if ( !m_streamConnected ) {
                    // std::cout << "[OutputStream] stream connect" << std::endl;
                    streamConnect();
                }
                m_clientSocket->read( mess );

                if ( mess == io::StreamInterface::ServerMessage::SERVER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] server closed" << std::endl;
#endif
                    m_clientSocket->write( io::StreamInterface::ClientMessage::CLIENT_SERVER_DOWN );
                    m_clientSocket->close();
                    m_streamConnected = false;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                    continue;
                }
                else if ( mess == io::StreamInterface::ServerMessage::STREAMER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] streamer closed" << std::endl;
#endif
                    m_streamConnected = false;
                    break;
                }
                else if ( mess == io::StreamInterface::ServerMessage::STREAM_VIEWER_INITED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] stream viewer inited" << std::endl;
#endif
                    assert( !m_streamViewerInited );
                    m_streamViewerInited = true;
                    continue;
                }
                else { assert( false ); }
            }
            catch ( net::system::SocketSystem::exception& ex ) {
#ifdef DEBUG_OUTPUT_STREAM
                std::cout << "[OutputStream] catch exception : " << ex.what() << std::endl;
#endif
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }
        }
    } );

    //    while ( !serverConnected ) {
    while ( !m_streamConnected ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    };

    startServer();
}

OutputStreamServer::OutputStreamServer( OutputStreamServer&& outputStream ) :
    StreamServer( outputStream.m_name, outputStream.m_ipv4, outputStream.m_port ),
    m_clientSocket( std::move( outputStream.m_clientSocket ) ),
    m_serverDataThread( std::move( outputStream.m_serverDataThread ) ),
    m_writingFun( std::move( outputStream.m_writingFun ) ),
    m_serverThread( std::move( outputStream.m_serverThread ) ),
    m_moved( outputStream.m_moved ) {
    outputStream.m_moved = true;
}

OutputStreamServer::~OutputStreamServer() {

    if ( !m_moved ) {
        if ( m_clientThread != nullptr ) {
            assert( m_clientThread->joinable() );
            if ( m_clientSocket != nullptr && m_clientSocket->isConnected() )
                m_clientSocket->write( io::StreamInterface::ClientMessage::STREAMER_CLIENT_CLOSED );

            m_shutdown = true;
            m_clientThread->join();
        }

        if ( m_serverThread != nullptr ) {
            assert( m_serverThread->joinable() );
            stop();
            m_serverThread->join();
        }
    }
}

//////////////////////////////////////////////////////////////////////

void OutputStreamServer::serverProcess() {
    net::ServerSocket serverSocket( m_serverDataThread->m_streamPort );
    assert( serverSocket.isConnected() );
    m_serverDataThread->m_serverStarted = true;

    while ( !m_serverDataThread->m_killed ) {
        auto streamViewerSock = io::InputOutputSocket( serverSocket.waitNewClient() );
        io::StreamInterface::ClientType clientType;
        streamViewerSock.read( clientType );

        if ( clientType == io::StreamInterface::ClientType::STREAM_VIEWER ) {
            if ( !m_serverDataThread->m_retainedData.empty() ) {
                streamViewerSock.write( m_serverDataThread->m_retainedData.data(),
                                        m_serverDataThread->m_retainedData.size() );
            }

            if ( m_clientSocket->isConnected() ) {
                m_clientSocket->write(
                    io::StreamInterface::ClientMessage::STREAMER_CLIENT_NEW_STREAM_VIEWER );
                m_clientSocket->write( (int)m_serverDataThread->m_clientSockets.size() + 1 );

                m_streamViewerInited = false;
                while ( !m_streamViewerInited ) {
                    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                };
            }

            streamViewerSock.write(
                io::StreamInterface::ClientMessage::STREAMER_CLIENT_STREAM_VIEWER_INITED );

            m_serverDataThread->m_mtxClientSockets.lock();
            m_serverDataThread->m_clientSockets.push_back( std::move( streamViewerSock ) );
            m_serverDataThread->m_mtxClientSockets.unlock();
        }
        else if ( clientType == io::StreamInterface::ClientType::KILLER ) {
            if ( m_serverDataThread->m_killed ) {}
            else { m_serverDataThread->m_killed = true; }
        }
        else { assert( false ); }
    }

#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] server down" << std::endl;
#endif
}

void OutputStreamServer::startServer() {
    assert( m_serverThread == nullptr );
    assert( !m_serverDataThread->m_serverStarted );
    assert( m_serverDataThread->m_streamPort != 0 );
    m_serverThread = std::make_unique<std::thread>( [this]() { serverProcess(); } );

    while ( !m_serverDataThread->m_serverStarted ) {};
}

void hub::output::OutputStreamServer::write( const Data_t* data, size_t size ) {
    if ( m_writingFun != nullptr ) m_writingFun( data, size );
    m_serverDataThread->m_mtxClientSockets.lock();
    auto& clientSockets = m_serverDataThread->m_clientSockets;
#ifdef HUB_USE_TBB
    std::for_each( std::execution::par,
#else
    std::for_each( std::execution::seq,
#endif
                   clientSockets.begin(),
                   clientSockets.end(),
                   [=]( auto& clientSocket ) { clientSocket.write( data, size ); } );
    m_serverDataThread->m_mtxClientSockets.unlock();
}

void OutputStreamServer::setRetain( bool retain ) {
    if ( retain ) {
        m_writingFun = [this]( const Data_t* data, Size_t size ) {
            m_serverDataThread->m_retainedData.insert(
                m_serverDataThread->m_retainedData.end(), data, data + size );
        };
    }
    else { m_writingFun = nullptr; }
}

void OutputStreamServer::stop() {
    m_serverDataThread->m_killed = true;
    auto clientSock              = io::InputOutputSocket(
        hub::net::ClientSocket( "127.0.0.1", m_serverDataThread->m_streamPort ) );
    clientSock.write( io::StreamInterface::ClientType::KILLER );
}

} // namespace output
} // namespace hub
