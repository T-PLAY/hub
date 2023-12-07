
#include "OutputStreamServer2.hpp"

#include <execution>
#include <iostream>
#include <typeinfo>

#include "net/ServerSocket.hpp"

namespace hub {
namespace output {

OutputStreamServer2::OutputStreamServer2( int streamPort ) :
    io::StreamServer2( "", "", 0 ), m_data( std::make_unique<SharedData>() ) {

    m_data->m_streamPort = streamPort;

    startStreaming();
}

OutputStreamServer2::OutputStreamServer2( const std::string& streamName,
                                          int port,
                                          const std::string& ipv4,
                                          bool retained ) :
    io::StreamServer2( streamName, ipv4, port ),
    m_data( std::make_unique<SharedData>(
        std::make_unique<hub::io::InputOutputSocket>( net::ClientSocket( ipv4, port ) ),
        retained ) ) {

    initStream();
    assert( m_data->m_serverConnected );

    auto* data             = m_data.get();
    m_data->m_serverThread = std::make_unique<std::thread>( [this, data]() {
        hub::io::StreamBase::ServerMessage mess;
        while ( !data->m_shutdown ) {
            try {
                if ( !data->m_serverSocket->isConnected() ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] try connect to server " << *data->m_serverSocket
                              << std::endl;
#endif
                    data->m_serverSocket->connect();
                }
                if ( !data->m_serverConnected ) {
                    // std::cout << "[OutputStream] stream connect" << std::endl;
                    initStream();
                }
                data->m_serverSocket->read( mess );

                if ( mess == hub::io::StreamBase::ServerMessage::SERVER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] server closed" << std::endl;
#endif
                    data->m_serverSocket->write(
                        hub::io::StreamBase::ClientMessage::CLIENT_SERVER_DOWN );
                    data->m_serverSocket->close();
                    data->m_serverConnected = false;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                    continue;
                }
                else if ( mess == hub::io::StreamBase::ServerMessage::STREAMER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] streamer closed" << std::endl;
#endif
                    data->m_serverConnected = false;
                    break;
                }
                else if ( mess == hub::io::StreamBase::ServerMessage::STREAM_VIEWER_INITED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] stream viewer inited" << std::endl;
#endif
                    assert( !data->m_streamViewerInited );
                    data->m_streamViewerInited = true;
                    continue;
                }
                else if ( mess == hub::io::StreamBase::ServerMessage::RETAINED_SHARED_TO_VIEWER ) {
                    data->m_retainedSharedToViewer = true;
                    continue;
                }
                else { assert( false ); }
            }
            catch ( net::system::SocketSystem::exception& ex ) {
                // #ifdef DEBUG_OUTPUT_STREAM
                std::cout << "[OutputStream] catch exception : " << ex.what() << std::endl;
                // #endif
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }
        }
    } );

    //    while ( !serverConnected ) {
    while ( !m_data->m_serverConnected ) {
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream] waiting for stream connected ..." << std::endl;
#endif
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    };

    startStreaming();
}

OutputStreamServer2::OutputStreamServer2( OutputStreamServer2&& outputStream ) noexcept :
    StreamServer2( outputStream.m_name, outputStream.m_ipv4, outputStream.m_port ),
    // m_serverSocket( std::move( outputStream.m_serverSocket ) ),
    m_data( std::move( outputStream.m_data ) )
// m_writingFun( std::move( outputStream.m_writingFun ) ),
// m_streamThread( std::move( outputStream.m_streamThread ) ),
// m_serverThread( std::move( outputStream.m_serverThread ) ),
// m_moved( outputStream.m_moved )
{
    outputStream.m_moved = true;
}

OutputStreamServer2::~OutputStreamServer2() {

    if ( !m_moved ) {
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "~[OutputStream] starting ..." << std::endl;
#endif
        if ( m_data->m_serverThread != nullptr ) {
            assert( m_data->m_serverThread->joinable() );
            if ( m_data->m_serverSocket != nullptr && m_data->m_serverSocket->isConnected() )
                m_data->m_serverSocket->write(
                    hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED );

            m_data->m_shutdown = true;
            m_data->m_serverThread->join();
        }

        if ( m_data->m_streamThread != nullptr ) {
            assert( m_data->m_streamThread->joinable() );
            stop();
            m_data->m_streamThread->join();
        }
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "~[OutputStream] ended" << std::endl;
#endif
    }
}

//////////////////////////////////////////////////////////////////////

// void OutputStreamServer2::streamProcess() {
// }

void OutputStreamServer2::initStream() {
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] stream connecting" << std::endl;
#endif
    assert( !m_data->m_serverConnected );
    m_data->m_serverSocket->write( ClientType::STREAMER );

    m_data->m_serverSocket->write( m_name );

    hub::io::StreamBase::ServerMessage mess;
    m_data->m_serverSocket->read( mess );
    if ( mess == hub::io::StreamBase::ServerMessage::FOUND ) {
        m_data->m_serverSocket->close();
        throw net::system::SocketSystem::exception(
            ( std::string( "stream '" ) + m_name + "' is already attached to server" ).c_str() );
    }
    assert( mess == hub::io::StreamBase::ServerMessage::NOT_FOUND );

    m_data->m_serverSocket->write( m_data->m_streamPort );
    m_data->m_serverSocket->read( m_data->m_streamPort );

    m_data->m_serverSocket->write( (int)m_data->m_streamSockets.size() );
    m_data->m_serverSocket->write( m_data->m_retained );

    m_data->m_serverSocket->read( mess );
    assert( mess == hub::io::StreamBase::ServerMessage::STREAMER_INITED );
    // m_data->m_serverSocket->write( hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_INITED );

    m_data->m_serverConnected = true;
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] stream connected" << std::endl;
#endif
}

void OutputStreamServer2::startStreaming() {
    assert( m_data->m_streamThread == nullptr );
    assert( !m_data->m_isStreaming );
    assert( m_data->m_streamPort != 0 );

    auto* data             = m_data.get();
    m_data->m_streamThread = std::make_unique<std::thread>( [data]() {
        net::ServerSocket serverSocket( data->m_streamPort );
        assert( serverSocket.isConnected() );
        data->m_isStreaming = true;

        while ( !data->m_killed ) {
            auto streamSock = hub::io::InputOutputSocket( serverSocket.waitNewClient() );
            hub::io::StreamBase::ClientType clientType;
            streamSock.read( clientType );

            if ( clientType == hub::io::StreamBase::ClientType::STREAM_VIEWER ) {

                if ( data->m_serverSocket != nullptr && data->m_serverSocket->isConnected() ) {
                    data->m_serverSocket->write(
                        hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_NEW_STREAM_VIEWER );
                    data->m_serverSocket->write( (int)data->m_streamSockets.size() + 1 );

                    data->m_streamViewerInited = false;
                    while ( !data->m_streamViewerInited ) {
#ifdef DEBUG_OUTPUT_STREAM
                        std::cout << "[OutputStream] waiting for stream viewer inited ..."
                                  << std::endl;
#endif
                        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                    };
                }

                data->m_mtxClientSockets.lock();
                streamSock.write(
                    hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_STREAM_VIEWER_INITED );

                if ( !data->m_retainedData.empty() ) {
                    // std::cout << "[OutputStream] write retain data (" <<
                    // data->m_retainedData.size()
                    // << " bytes)" << std::endl;
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] load retain data " << data->m_retainedData
                              << std::endl;
#endif
                    streamSock.write( data->m_retainedData.data(), data->m_retainedData.size() );
                }

                data->m_streamSockets.push_back( std::move( streamSock ) );
                data->m_mtxClientSockets.unlock();
            }
            else if ( clientType == hub::io::StreamBase::ClientType::KILLER ) {
                if ( data->m_killed ) {}
                else { data->m_killed = true; }
            }
            else { assert( false ); }
        }

#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream] server down" << std::endl;
#endif
    } );

    while ( !m_data->m_isStreaming ) {};
}

void output::OutputStreamServer2::write( const Data_t* data, Size_t size ) {
    if ( m_data->m_writingFun != nullptr ) m_data->m_writingFun( data, size );
    m_data->m_mtxClientSockets.lock();
    auto& clientSockets = m_data->m_streamSockets;

    std::list<io::InputOutputSocket*> socketToRemoves;

#ifdef OS_MACOS
    for ( auto& clientSocket : clientSockets ) {
#else
#    ifdef HUB_USE_TBB
    std::for_each(
        std::execution::par,
#    else
    std::for_each(
        std::execution::seq,
#    endif
        clientSockets.begin(),
        clientSockets.end(),
        [&]( auto& clientSocket ) {
#endif

        try {
            clientSocket.write( data, size );
        }
        catch ( std::exception& ex ) {
            clientSocket.close();
            std::cout << "[OutputStream] catch exception : " << ex.what() << std::endl;
            socketToRemoves.push_back( &clientSocket );
            // clientSockets.remove()
        }

#ifdef OS_MACOS
    }
#else
        } );
#endif

    for ( auto* socketToRemove : socketToRemoves ) {
        clientSockets.remove( *socketToRemove );
        m_data->m_serverSocket->write(
            hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_DEL_STREAM_VIEWER );
        m_data->m_serverSocket->write( (int)m_data->m_streamSockets.size() );
    }

    // auto it = clientSockets.begin();
    // while ( it != clientSockets.end() ) {
    //     auto& clientSocket = *it;
    //     try {
    //         clientSocket.write( data, size );
    //     }
    //     catch ( std::exception& ex ) {
    //         clientSocket.close();
    //         std::cout << "[OutputStream] catch exception : " << ex.what() << std::endl;
    //         it = clientSockets.erase( it );
    //         continue;
    //     }
    //     ++it;
    // }

    m_data->m_mtxClientSockets.unlock();
}

void OutputStreamServer2::setRetain( bool retain ) {
    if ( retain ) {
        m_data->m_writingFun = [this]( const Data_t* data, Size_t size ) {
            assert( size > 0 );
            m_data->m_retainedData.insert( m_data->m_retainedData.end(), data, data + size );
            // m_data->m_serverSocket->write(size);
            m_data->m_serverSocket->write( io::StreamBase::ClientMessage::NEW_RETAIN_DATA );
            m_data->m_serverSocket->write( size );
            m_data->m_serverSocket->write( data, size );
            // std::cout << "[OutputStream] save retained data (" << size << " bytes)" << std::endl;
            // std::vector<Data_t> vector(data, data + size);
#ifdef DEBUG_OUTPUT_STREAM
            std::cout << "[OutputStream] save retained data : " << m_data->m_retainedData
                      << std::endl;
#endif
        };
    }
    else {
        // assert( !m_data->m_retainedSharedToViewer );
        if ( m_data->m_retained ) {
            m_data->m_retainedSharedToViewer = false;
            m_data->m_serverSocket->write( io::StreamBase::ClientMessage::FULLY_RETAINED_DATA );
            // m_data->m_fullyRetained = true;
            m_data->m_writingFun = nullptr;
            while ( !m_data->m_retainedSharedToViewer ) {
                std::cout << "[OutputStream] waiting for shared retained data to viewer ..."
                          << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        }
    }
}

// int OutputStreamServer2::getNStreamViewer() const {
// return m_data->m_streamSockets.size();
// }

void OutputStreamServer2::stop() {
    m_data->m_killed = true;
    auto clientSock =
        hub::io::InputOutputSocket( hub::net::ClientSocket( "127.0.0.1", m_data->m_streamPort ) );
    clientSock.write( hub::io::StreamBase::ClientType::KILLER );
}

} // namespace output
} // namespace hub
