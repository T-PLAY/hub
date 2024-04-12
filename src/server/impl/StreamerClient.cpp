
#include <cstring>
#include <iomanip>

#include "ServerImpl.hpp"
#include "StreamerClient.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#    include "sensor/SensorSpec.hpp"
#endif

namespace hub {
namespace server {

StreamerClient::StreamerClient( ServerImpl* server,
                                  int iClient,
                                  hub::io::InputOutputSocket&& sock,
                                  std::string streamName,
                                  std::string streamIpv4,
                                  int streamPort ) :
    Client( server, iClient ),
    m_streamName( std::move( streamName ) ),
    m_streamIpv4( std::move( streamIpv4 ) ),
    m_streamPort( streamPort ),
    m_sock( std::make_unique<hub::io::InputOutputSocket>( std::move( sock ) ) ) {

    std::cout << headerMsg() << "new stream '" << m_streamName << "' at " << *m_sock << std::endl;

    m_sock->read( m_nStreamViewer );

    m_sock->read( m_header );
    if ( m_header.getUserDefined().empty() ) {
        std::cout << headerMsg() << "dectected raw stream of size " << m_header.getDataSize()
                  << std::endl;
    }
#ifndef HUB_NON_BUILD_SENSOR
    else {
        hub::io::Memory memory( m_header.getUserDefined() );
        hub::sensor::SensorSpec sensorSpec;
        memory.read( sensorSpec );

        std::cout << headerMsg() << "sensor '" << sensorSpec.getSensorName() << "' connected"
                  << std::endl;
        std::cout << headerMsg() << "resolution: " << sensorSpec.getResolution() << std::endl;

        const auto& metaData = sensorSpec.getMetaData();
        for ( const auto& pair : metaData ) {
            std::cout << headerMsg() << "metaData: " << pair << std::endl;
        }
    }
#endif

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
                }
                else if ( mess ==
                          hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_DEL_STREAM_VIEWER ) {
                    sockPtr->read( m_nStreamViewer );
                    m_server->delStreamViewer( this );
                }

                else {
                    assert( false );
                }
            }
        }
        catch ( net::system::SocketSystem::exception& ex ) {
            m_mtxPrint.lock();
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
            m_mtxPrint.unlock();
        }

        std::thread( [this]() { delete this; } ).detach();
    } );

    assert( m_server != nullptr );
    m_server->addStreamer( this );
}

int StreamerClient::getNStreamViewer() const {
    return m_nStreamViewer;
}

StreamerClient::~StreamerClient() {
    m_mtxPrint.lock();
    std::cout << headerMsg() << "delete start" << std::endl;

    assert( m_thread.joinable() );
    std::cout << headerMsg() << "joining thread" << std::endl;
    m_thread.join();
    std::cout << headerMsg() << "thread joined" << std::endl;

    if ( m_server != nullptr ) m_server->delStreamer( this );

    m_mtxPrint.unlock();
    if ( !m_serverDown ) {
        if ( m_sock->isOpen() ) {
            assert( m_sock->isOpen() );
            m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_CLOSED );
        }
    }
}

std::string StreamerClient::headerMsg() const {
    return Client::clientMsg() + "[Streamer] ";
    // return Client::headerMsg() + "[Streamer] ";
}

void StreamerClient::end( hub::io::StreamBase::ServerMessage message ) {
    assert( m_sock->isOpen() );
    m_sock->write( message );
}

void StreamerClient::notifyInited() {
    assert( m_sock->isOpen() );
    m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_INITED );
}

} // namespace server
} // namespace hub
