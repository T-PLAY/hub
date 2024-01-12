
#include "OutputStreamServer2.hpp"

#ifdef HUB_USE_TBB
#include <execution>
#endif

#include <iostream>
#include <typeinfo>

// include "net/ServerSocket.hpp"

namespace hub {
namespace output {

static constexpr int s_pingToServerDelay = 500; // ms

OutputStreamServer2::OutputStreamServer2( const io::Header& header, int streamPort ) :
    io::StreamServer2( "", "", 0 ), m_data( std::make_unique<SharedData>() ) {

    m_data->m_header     = header;
    m_data->m_streamPort = streamPort;
    m_data->m_writingFun = [this]( const Data_t* data, Size_t size ) {};

    startStreaming();
}



OutputStreamServer2::OutputStreamServer2( const io::Header& header,
                                          const std::string& streamName,
                                          int port,
                                          const std::string& ipv4
                                          // bool retained ) :
                                          ) :
    io::StreamServer2( streamName, ipv4, port ),
    m_data( std::make_unique<SharedData>(
        std::make_unique<hub::io::InputOutputSocket>( net::ClientSocket( ipv4, port ) ) ) ) {

    m_data->m_header     = header;
    m_data->m_writingFun = [this]( const Data_t* data, Size_t size ) {};

    tryConnectToServer();
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
                if ( !data->m_serverConnected ) { tryConnectToServer(); }
#ifdef DEBUG_OUTPUT_STREAM
                std::cout << "[OutputStream] waiting for server message ..." << std::endl;
#endif
                data->m_serverSocket->read( mess );

                if ( mess == hub::io::StreamBase::ServerMessage::SERVER_CLOSED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] server closed" << std::endl;
#endif
                    data->m_serverSocket->write(
                        hub::io::StreamBase::ClientMessage::CLIENT_SERVER_DOWN );
                    data->m_serverSocket->close();
                    data->m_serverConnected = false;
                    std::this_thread::sleep_for( std::chrono::milliseconds( s_pingToServerDelay ) );
                    // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
                    // continue;
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
                    // continue;
                }
                else if ( mess == hub::io::StreamBase::ServerMessage::STREAMER_INITED ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] streamer inited" << std::endl;
#endif
                    // assert( !data->m_streamerInited );
                    data->m_streamerInited = true;
                    // continue;
                }
                // else if ( mess == hub::io::StreamBase::ServerMessage::RETAINED_SHARED_TO_VIEWER )
                // {
                //     data->m_retainedSharedToViewer = true;
                //     continue;
                // }
                else { assert( false ); }
            }
            catch ( net::system::SocketSystem::exception& ex ) {
                // #ifdef DEBUG_OUTPUT_STREAM
                std::cout << "[OutputStream] catch exception : " << ex.what() << std::endl;
                // #endif
                std::this_thread::sleep_for( std::chrono::milliseconds( s_pingToServerDelay ) );
                // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
            }
        } // while (! data->m_shutdown)
    } );

    //    while ( !serverConnected ) {
    while ( !m_data->m_serverConnected ) {
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream] waiting for server connected ..." << std::endl;
#endif
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    };

    startStreaming();

    while ( !m_data->m_streamerInited ) {
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream] waiting for streamer initing ..." << std::endl;
#endif
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    };
}

OutputStreamServer2::OutputStreamServer2( OutputStreamServer2&& outputStream ) noexcept :
    StreamServer2( std::move( outputStream.m_name ),
                   std::move( outputStream.m_ipv4 ),
                   outputStream.m_port ),
    m_data( std::move( outputStream.m_data ) ) {
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
            stopStreaming();
            m_data->m_streamThread->join();
        }
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "~[OutputStream] ended" << std::endl;
#endif
    }
}

void OutputStreamServer2::tryConnectToServer() {
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] server connecting" << std::endl;
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

    m_data->m_serverSocket->write( (int)m_data->m_streamViewerSocks.size() );

    m_data->m_serverSocket->write( m_data->m_header );

    m_data->m_serverConnected = true;
#ifdef DEBUG_OUTPUT_STREAM
    std::cout << "[OutputStream] server connected" << std::endl;
#endif
}

void OutputStreamServer2::startStreaming() {
    assert( m_data->m_streamThread == nullptr );
    assert( !m_data->m_isStreaming );
    assert( m_data->m_streamPort != 0 );

    auto* data             = m_data.get();
    m_data->m_streamThread = std::make_unique<std::thread>( [data]() {
#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream][Stream] starting stream server" << std::endl;
#endif

        assert( data->m_streamSocket == nullptr );
        data->m_streamSocket = std::make_unique<net::ServerSocket>( data->m_streamPort );

        assert( data->m_streamSocket->isConnected() );
        data->m_isStreaming = true;

        while ( !data->m_killed ) {
#ifdef DEBUG_OUTPUT_STREAM
            std::cout << "[OutputStream][Stream] waiting for new client" << std::endl;
#endif
            auto streamViewerSock =
                hub::io::InputOutputSocket( data->m_streamSocket->waitNewClient() );
            hub::io::StreamBase::ClientType clientType;
            streamViewerSock.read( clientType );

            if ( clientType == hub::io::StreamBase::ClientType::STREAM_VIEWER ) {
#ifdef DEBUG_OUTPUT_STREAM
                std::cout << "[OutputStream][Stream] new stream viewer" << std::endl;
#endif

                if ( data->m_serverSocket != nullptr && data->m_serverSocket->isConnected() ) {
                    data->m_serverSocket->write(
                        hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_NEW_STREAM_VIEWER );
                    data->m_serverSocket->write( (int)data->m_streamViewerSocks.size() + 1 );

                    data->m_streamViewerInited = false;
                    while ( !data->m_streamViewerInited ) {
#ifdef DEBUG_OUTPUT_STREAM
                        std::cout << "[OutputStream][Stream] waiting for stream viewer inited ..."
                                  << std::endl;
#endif
                        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                    };
                }

                streamViewerSock.write( data->m_header );

                data->m_mtxClientSockets.lock();
                streamViewerSock.write(
                    hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_STREAM_VIEWER_INITED );

                if ( !data->m_retainedData.empty() ) {
#ifdef DEBUG_OUTPUT_STREAM
                    std::cout << "[OutputStream] load retain data " << data->m_retainedData
                              << std::endl;
#endif
                    streamViewerSock.write( data->m_retainedData.data(),
                                            data->m_retainedData.size() );
                }

                data->m_streamViewerSocks.push_back( std::move( streamViewerSock ) );
                data->m_mtxClientSockets.unlock();
            }
            else if ( clientType == hub::io::StreamBase::ClientType::KILLER ) {
                if ( data->m_killed ) {}
                else { data->m_killed = true; }
            }
            else { assert( false ); }
        }
        data->m_isStreaming = false;

#ifdef DEBUG_OUTPUT_STREAM
        std::cout << "[OutputStream][Stream] server down" << std::endl;
#endif
    } ); // thread

    while ( !m_data->m_isStreaming ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    };
}

void output::OutputStreamServer2::write( const Data_t* data, Size_t size ) {
    assert( m_data->m_header.getDataSize() == size );

    assert( m_data->m_writingFun != nullptr );
    m_data->m_writingFun( data, size );

    m_data->m_mtxClientSockets.lock();
    auto& clientSockets = m_data->m_streamViewerSocks;

    std::list<io::InputOutputSocket*> socketToRemoves;

// #ifdef OS_MACOS
    // for ( auto& clientSocket : clientSockets ) {
// #else
#    ifdef HUB_USE_TBB
    std::for_each(
        std::execution::par,
        // #    else
        // std::for_each(
        // std::execution::seq,
        // #    endif
        clientSockets.begin(),
        clientSockets.end(),
        [&]( auto& clientSocket ) {
#else
    for ( auto& clientSocket : clientSockets ) {
#endif

            try {
                clientSocket.write( data, size );
            }
            catch ( std::exception& ex ) {
                std::cout << "[OutputStream] catch exception : " << ex.what() << std::endl;
                socketToRemoves.push_back( &clientSocket );
            }

// #ifdef OS_MACOS
#ifdef HUB_USE_TBB
        } );
#else
            }
#endif

    for ( auto* socketToRemove : socketToRemoves ) {
        clientSockets.remove( *socketToRemove );
        // if ( m_data->m_serverSocket != nullptr && m_data->m_serverSocket->isConnected() )
        if ( m_data->m_serverSocket != nullptr && m_data->m_serverSocket->isConnected() ) {
            m_data->m_serverSocket->write(
                hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_DEL_STREAM_VIEWER );
            m_data->m_serverSocket->write( (int)m_data->m_streamViewerSocks.size() );
        }
    }

    m_data->m_mtxClientSockets.unlock();

    // message log
    if ( !m_data->m_streamViewerSocks.empty() ) {
        m_data->m_byteWrote += m_data->m_streamViewerSocks.size() * size;
        const auto now = std::chrono::high_resolution_clock::now();
        const auto period =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - m_data->m_lastClock )
                .count();
        if ( period > 1'000 ) { // 1 sec
            const auto bytePerSecond = ( 1000.0 * m_data->m_byteWrote ) / period;
            std::cout << "[OutputStream] data rate : " << PRETTY_BYTES( bytePerSecond )
                      << "/s, watched by " << m_data->m_streamViewerSocks.size() << " streamViewers"
                      << std::endl;
            m_data->m_lastClock = now;
            m_data->m_byteWrote = 0;
        }
    }
}

void OutputStreamServer2::setRetain( bool retain ) {
    if ( retain ) {
        m_data->m_writingFun = [this]( const Data_t* data, Size_t size ) {
            assert( size > 0 );
            m_data->m_retainedData.insert( m_data->m_retainedData.end(), data, data + size );
#ifdef DEBUG_OUTPUT_STREAM
            std::cout << "[OutputStream] save retained data : " << m_data->m_retainedData
                      << std::endl;
#endif
        };
    }
    else {
        m_data->m_writingFun = [this]( const Data_t* data, Size_t size ) {};
    }
}

int OutputStreamServer2::getNStreamViewer() const {

    m_data->m_mtxClientSockets.lock();
    const auto nStreamViewer = m_data->m_streamViewerSocks.size();
    m_data->m_mtxClientSockets.unlock();

    return nStreamViewer;
}

const io::Header& OutputStreamServer2::getHeader() const {
    return m_data->m_header;
}


void OutputStreamServer2::stopStreaming() {
    m_data->m_killed = true;

    assert( m_data->m_isStreaming );
    auto clientSock =
        hub::io::InputOutputSocket( hub::net::ClientSocket( "127.0.0.1", m_data->m_streamPort ) );

    if ( clientSock.isConnected() ) {
        assert( clientSock.isConnected() );
        clientSock.write( hub::io::StreamBase::ClientType::KILLER );
    }
}

} // namespace output
} // namespace hub
