
#include "OutputStreamServer.hpp"

#include <execution>
#include <iostream>
#include <typeinfo>

#include "net/ServerSocket.hpp"

// #define DEBUG_OUTPUT_STREAM

namespace hub {
namespace output {

OutputStreamServer::OutputStreamServer( int streamPort ) :
    io::StreamServer( "", "", 0 ), m_data( std::make_unique<SharedData>() ) {

    m_data->m_streamPort = streamPort;

    startStreaming();
}

void OutputStreamServer::initStream() {
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] stream connecting" << std::endl;
#endif
    assert( !m_data->m_streamConnected );
    m_data->m_serverSocket->write( ClientType::STREAMER );

    m_data->m_serverSocket->write( m_name );

    io::StreamInterface::ServerMessage mess;
    m_data->m_serverSocket->read( mess );
    if ( mess == io::StreamInterface::ServerMessage::FOUND ) {
        m_data->m_serverSocket->close();
        throw net::system::SocketSystem::exception(
            ( std::string( "stream '" ) + m_name + "' is already attached to server" ).c_str() );
    }
    assert( mess == io::StreamInterface::ServerMessage::NOT_FOUND );

    m_data->m_serverSocket->write( m_data->m_streamPort );
    m_data->m_serverSocket->read( m_data->m_streamPort );

    m_data->m_serverSocket->write( (int)m_data->m_streamSockets.size() );

    m_data->m_serverSocket->read( mess );
    assert( mess == io::StreamInterface::ServerMessage::STREAMER_INITED );
    m_data->m_streamConnected = true;
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] stream connected" << std::endl;
#endif
}

OutputStreamServer::OutputStreamServer( const std::string& streamName,
                                        int port,
                                        const std::string& ipv4 ) :
    io::StreamServer( streamName, ipv4, port ),
    m_data( std::make_unique<SharedData>(
        std::make_unique<io::InputOutputSocket>( net::ClientSocket( ipv4, port ) ) ) ) {

    initStream();
    assert( m_data->m_streamConnected );

    auto* data             = m_data.get();
    m_data->m_serverThread = std::make_unique<std::thread>( [this, data]() {
        io::StreamInterface::ServerMessage mess;
        while ( !data->m_shutdown ) {
            try {
                if ( !data->m_serverSocket->isConnected() ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] try connect to server " << *data->m_serverSocket
                              << std::endl;
#endif
                    data->m_serverSocket->connect();
                }
                if ( !data->m_streamConnected ) {
                    // std::cout << "[OutputStream] stream connect" << std::endl;
                    initStream();
                }
                data->m_serverSocket->read( mess );

                if ( mess == io::StreamInterface::ServerMessage::SERVER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] server closed" << std::endl;
#endif
                    data->m_serverSocket->write(
                        io::StreamInterface::ClientMessage::CLIENT_SERVER_DOWN );
                    data->m_serverSocket->close();
                    data->m_streamConnected = false;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                    continue;
                }
                else if ( mess == io::StreamInterface::ServerMessage::STREAMER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] streamer closed" << std::endl;
#endif
                    data->m_streamConnected = false;
                    break;
                }
                else if ( mess == io::StreamInterface::ServerMessage::STREAM_VIEWER_INITED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] stream viewer inited" << std::endl;
#endif
                    assert( !data->m_streamViewerInited );
                    data->m_streamViewerInited = true;
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
    while ( !m_data->m_streamConnected ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    };

    startStreaming();
}

OutputStreamServer::OutputStreamServer( OutputStreamServer&& outputStream ) :
    StreamServer( outputStream.m_name, outputStream.m_ipv4, outputStream.m_port ),
    // m_serverSocket( std::move( outputStream.m_serverSocket ) ),
    m_data( std::move( outputStream.m_data ) )
// m_writingFun( std::move( outputStream.m_writingFun ) ),
// m_streamThread( std::move( outputStream.m_streamThread ) ),
// m_serverThread( std::move( outputStream.m_serverThread ) ),
// m_moved( outputStream.m_moved )
{
    outputStream.m_moved = true;
}

OutputStreamServer::~OutputStreamServer() {

    if ( !m_moved ) {
        if ( m_data->m_serverThread != nullptr ) {
            assert( m_data->m_serverThread->joinable() );
            if ( m_data->m_serverSocket != nullptr && m_data->m_serverSocket->isConnected() )
                m_data->m_serverSocket->write(
                    io::StreamInterface::ClientMessage::STREAMER_CLIENT_CLOSED );

            m_data->m_shutdown = true;
            m_data->m_serverThread->join();
        }

        if ( m_data->m_streamThread != nullptr ) {
            assert( m_data->m_streamThread->joinable() );
            stop();
            m_data->m_streamThread->join();
        }
    }
}

//////////////////////////////////////////////////////////////////////

// void OutputStreamServer::streamProcess() {
// }

void OutputStreamServer::startStreaming() {
    assert( m_data->m_streamThread == nullptr );
    assert( !m_data->m_serverStarted );
    assert( m_data->m_streamPort != 0 );

    auto* data             = m_data.get();
    m_data->m_streamThread = std::make_unique<std::thread>( [data]() {
        net::ServerSocket serverSocket( data->m_streamPort );
        assert( serverSocket.isConnected() );
        data->m_serverStarted = true;

        while ( !data->m_killed ) {
            auto streamSock = io::InputOutputSocket( serverSocket.waitNewClient() );
            io::StreamInterface::ClientType clientType;
            streamSock.read( clientType );

            if ( clientType == io::StreamInterface::ClientType::STREAM_VIEWER ) {

                if ( data->m_serverSocket != nullptr && data->m_serverSocket->isConnected() ) {
                    data->m_serverSocket->write(
                        io::StreamInterface::ClientMessage::STREAMER_CLIENT_NEW_STREAM_VIEWER );
                    data->m_serverSocket->write( (int)data->m_streamSockets.size() + 1 );

                    data->m_streamViewerInited = false;
                    while ( !data->m_streamViewerInited ) {
                        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                    };
                }

                data->m_mtxClientSockets.lock();
                streamSock.write(
                    io::StreamInterface::ClientMessage::STREAMER_CLIENT_STREAM_VIEWER_INITED );

                if ( !data->m_retainedData.empty() ) {
                    streamSock.write( data->m_retainedData.data(), data->m_retainedData.size() );
                }

                data->m_streamSockets.push_back( std::move( streamSock ) );
                data->m_mtxClientSockets.unlock();
            }
            else if ( clientType == io::StreamInterface::ClientType::KILLER ) {
                if ( data->m_killed ) {}
                else { data->m_killed = true; }
            }
            else { assert( false ); }
        }

#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream] server down" << std::endl;
#endif
    } );

    while ( !m_data->m_serverStarted ) {};
}

void hub::output::OutputStreamServer::write( const Data_t* data, size_t size ) {
    if ( m_data->m_writingFun != nullptr ) m_data->m_writingFun( data, size );
    m_data->m_mtxClientSockets.lock();
    auto& clientSockets = m_data->m_streamSockets;
#ifdef HUB_USE_TBB
    std::for_each( std::execution::par,
#else
    std::for_each( std::execution::seq,
#endif
                   clientSockets.begin(),
                   clientSockets.end(),
                   [=]( auto& clientSocket ) { clientSocket.write( data, size ); } );
    m_data->m_mtxClientSockets.unlock();
}

void OutputStreamServer::setRetain( bool retain ) {
    if ( retain ) {
        m_data->m_writingFun = [this]( const Data_t* data, Size_t size ) {
            m_data->m_retainedData.insert( m_data->m_retainedData.end(), data, data + size );
        };
    }
    else { m_data->m_writingFun = nullptr; }
}

void OutputStreamServer::stop() {
    m_data->m_killed = true;
    auto clientSock =
        io::InputOutputSocket( hub::net::ClientSocket( "127.0.0.1", m_data->m_streamPort ) );
    clientSock.write( io::StreamInterface::ClientType::KILLER );
}

} // namespace output
} // namespace hub
