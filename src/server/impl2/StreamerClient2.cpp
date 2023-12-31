
#include <cstring>
#include <iomanip>

#include "ServerImpl2.hpp"
#include "StreamerClient2.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#include "sensor/SensorSpec.hpp"
#endif

namespace hub {
namespace server {

StreamerClient2::StreamerClient2( ServerImpl2* server,
                                  int iClient,
                                  hub::io::InputOutputSocket&& sock,
                                  std::string streamName,
                                  std::string streamIpv4,
                                  int streamPort ) :
    Client2( server, iClient ),
    m_streamName( std::move( streamName ) ),
    m_streamIpv4( std::move( streamIpv4 ) ),
    m_streamPort( streamPort ),
    m_sock( std::make_unique<hub::io::InputOutputSocket>( std::move( sock ) ) ) {

    // std::cout << headerMsg() << "StreamerClient2() start" << std::endl;

    std::cout << headerMsg() << "new stream '" << m_streamName << "'" << std::endl;

    m_sock->read( m_nStreamViewer );

    // m_sock->read( m_retained );
    m_sock->read( m_header );
    if ( m_header.getUserDefined().empty() ) {
        std::cout << headerMsg() << "dectected raw stream of size " << m_header.getDataSize() << std::endl;
    }
#ifndef HUB_NON_BUILD_SENSOR
    else {
        hub::io::Memory memory(m_header.getUserDefined());
        hub::sensor::SensorSpec sensorSpec;
        memory.read(sensorSpec);

        std::cout << headerMsg() << "detected sensor stream '" << sensorSpec.getSensorName() << "'" << std::endl;
        std::cout << headerMsg() << "acquisitionSize: " << sensorSpec.getResolution().size() << std::endl;
        std::cout << headerMsg() << "resolution: " << sensorSpec.getResolution() << std::endl;

        const auto& metaData = sensorSpec.getMetaData();
        for ( const auto& pair : metaData ) {
            std::cout << headerMsg() << "metaData: " << pair << std::endl;
        }
    }
#endif

    // std::atomic<bool> streamerClientInited = false;

    auto* sockPtr = m_sock.get();
    m_thread      = std::thread( [this, sockPtr]() mutable {
        try {

            while ( true ) {
                hub::io::StreamBase::ClientMessage mess = hub::io::StreamBase::ClientMessage::NONE;
                sockPtr->read( mess );
                if ( mess == hub::io::StreamBase::ClientMessage::CLIENT_SERVER_DOWN ) {
                    m_serverDown = true;
                    break;
                }
                else if ( mess == hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED ) {
                    break;
                }
                else if ( mess ==
                          hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_NEW_STREAM_VIEWER ) {
                    sockPtr->read( m_nStreamViewer );
                    m_server->addStreamViewer( this );
                    sockPtr->write( hub::io::StreamBase::ServerMessage::STREAM_VIEWER_INITED );
                    // std::cout << headerMsg() << "new stream viewer" << std::endl;
                    // m_server->printStatus();
                }
                else if ( mess ==
                          hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_DEL_STREAM_VIEWER ) {
                    sockPtr->read( m_nStreamViewer );
                    m_server->delStreamViewer( this );
                    // sockPtr->write( hub::io::StreamBase::ServerMessage::STREAM_VIEWER_INITED );
                    // std::cout << headerMsg() << "new stream viewer" << std::endl;
                    // m_server->printStatus();
                }
                // else if ( mess == io::StreamBase::ClientMessage::NEW_RETAIN_DATA ) {
                //     // assert( m_retained );
                //     Size_t size;
                //     sockPtr->read( size );
                //     assert( size > 0 );
                //     // const auto previousSize = m_retainedData.size();
                //     const auto previousSize = m_header.size();
                //     // m_retainedData.resize( m_retainedData.size() + size );
                //     m_header.resize( m_header.size() + size );
                //     // sockPtr->read( m_retainedData.data() + previousSize, size );
                //     sockPtr->read( m_header.data() + previousSize, size );
                //     // std::cout << "[StreamerClient] new retain data : " << m_retainedData <<
                //     // std::endl;
                // }
                // else if ( mess == io::StreamBase::ClientMessage::FULLY_RETAINED_DATA ) {
                //     assert( m_retained );
                //     m_fullyRetained = true;

                //     while ( !m_retainedSharedToViewer ) {
                //         std::cout
                //             << "[StreamerClient] waiting for shared retained data to viewer ..."
                //             << std::endl;
                //         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                //     }
                //     sockPtr->write( hub::io::StreamBase::ServerMessage::RETAINED_SHARED_TO_VIEWER
                //     );
                //     // std::cout << "[StreamerClient] new retain data : " << m_retainedData <<
                //     // std::endl;
                // }
                // else if ( mess == io::StreamBase::ClientMessage::STREAMER_CLIENT_INITED ) {
                // // std::cout << "[StreamerClient2] STREAMER_CLIENT_START" << std::endl;
                //     streamerClientInited = true;
                // }
                else { assert( false ); }
            }
        }
        catch ( net::system::SocketSystem::exception& ex ) {
            //            m_server->m_mtxPrint.lock();
            m_mtxPrint.lock();
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
            m_mtxPrint.unlock();
        }

        std::thread( [this]() { delete this; } ).detach();
    } );

    assert( m_server != nullptr );
    m_server->addStreamer( this );
}

int StreamerClient2::getNStreamViewer() const {
    return m_nStreamViewer;
}

StreamerClient2::~StreamerClient2() {
    //    m_server->m_mtxPrint.lock();
    m_mtxPrint.lock();
    std::cout << headerMsg() << "delete start" << std::endl;
    //    m_server->m_mtxPrint.unlock();

    assert( m_thread.joinable() );
    std::cout << headerMsg() << "joining thread" << std::endl;
    m_thread.join();
    std::cout << headerMsg() << "thread joined" << std::endl;

    if ( m_server != nullptr ) m_server->delStreamer( this );

    //    m_server->m_mtxPrint.lock();
    //    std::cout << headerMsg() << "delete ended" << std::endl;
    //    m_server->m_mtxPrint.unlock();
    m_mtxPrint.unlock();
    if ( !m_serverDown ) {
        // todo check fix
        if ( m_sock->isOpen() ) {
            assert( m_sock->isOpen() );
            m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_CLOSED );
        }
    }
}

std::string StreamerClient2::headerMsg() const {
    return Client2::headerMsg() + "[Streamer] ";
}

void StreamerClient2::end( hub::io::StreamBase::ServerMessage message ) {
    //    InputStreamClient& input = dynamic_cast<InputStreamClient&>( m_inputSensor->getInput() );
    //    assert( input.m_clientSocket.isOpen() );
    assert( m_sock->isOpen() );
    m_sock->write( message );
}

void StreamerClient2::notifyInited() {
    assert( m_sock->isOpen() );
    // m_sock->write( message );
    m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_INITED );
}

} // namespace server
} // namespace hub

// #if ( __cplusplus >= 201703L )
// #else
// #endif
