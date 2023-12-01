
#include <cstring>
#include <iomanip>

#include "ServerImpl2.hpp"
#include "StreamerClient2.hpp"

namespace hub {
namespace server {

StreamerClient2::StreamerClient2( ServerImpl2* server,
                                  int iClient,
                                  hub::io::InputOutputSocket&& sock,
                                  std::string streamName_,
                                  std::string ipv4_,
                                  int port_ ) :
    Client2( server, iClient ),
    streamName( std::move( streamName_ ) ),
    ipv4( std::move( ipv4_ ) ),
    port( port_ ),
    m_sock( std::make_unique<hub::io::InputOutputSocket>( std::move( sock ) ) ) {

    std::cout << headerMsg() << "StreamerClient2() start" << std::endl;

    std::cout << headerMsg() << "stream name = '" << streamName << "'" << std::endl;

    m_sock->read( m_nStreamViewer );

    assert( m_server != nullptr );
    m_server->addStreamer( this );

    std::atomic<bool> streamerClientInited = false;

    auto* sockPtr = m_sock.get();
    m_thread      = std::thread( [this, sockPtr, &streamerClientInited]() mutable {
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
                    m_server->newStreamViewer( this );
                    sockPtr->write( hub::io::StreamBase::ServerMessage::STREAM_VIEWER_INITED );
                    // std::cout << headerMsg() << "new stream viewer" << std::endl;
                    // m_server->printStatus();
                }
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

    // m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_INITED );
    // hub::io::StreamBase::ClientMessage clientMess;
    // while ( !streamerClientInited ) {
    //     std::cout << "[StreamerClient2] waiting for streamer client inited ..." << std::endl;
    //     std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    // };
    // m_sock->read(clientMess);
    // assert(clientMess == io::StreamBase::ClientMessage::STREAMER_CLIENT_INITED);
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
    m_thread.join();

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

// const std::string& StreamerClient2::getStreamName() const {
//     return streamName;
// }

// bool StreamerClient2::isPackedStream() const {
//     return m_isPackedStream;
// }

// const std::set<sensor::Acquisition>& StreamerClient2::getPackedAcqs() const {
//     int iTry = 0;
//     while ( m_packedAcqs.size() != m_nAcq && iTry < 10 ) {
//         std::cout << headerMsg() << "waiting for all packed acqs" << std::endl;
//         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//         ++iTry;
//     }
//     assert( iTry < 10 );
//     assert( m_packedAcqs.size() == m_nAcq );
//     return m_packedAcqs;
// }

// void StreamerClient2::addStreamViewer( StreamViewerClient* streamViewer ) {

// void StreamerClient2::addSyncStreamViewer( StreamViewerClient* syncStreamViewer ) {

// void StreamerClient2::delStreamViewer( StreamViewerClient* streamViewer ) {

// void StreamerClient2::newAcquisition( const std::string& streamerName,

// const std::string& StreamerClient2::getStreamName() const {
// }

// const std::map<std::string, std::list<StreamViewerClient*>>&
// StreamerClient2::getSyncViewers() const {
// }

// const std::shared_ptr<Acquisition>
// StreamerClient2::getLastAcq( const std::string& streamName ) const {
// }

// const std::map<long long, std::shared_ptr<Acquisition>>&
// StreamerClient2::getSaveAcqs( const std::string& streamName ) const {
// }

// void StreamerClient2::saveNewAcq( const std::string& streamName, Acquisition&& newAcq ) {

// #if ( __cplusplus >= 201703L )
// #else
// #endif

// bool StreamerClient2::isRecordStream() const {
// }

// const std::string& StreamerClient2::getParent() const {
// }

// const std::list<StreamViewerClient*>& StreamerClient2::getStreamViewers() const {
// }

// const std::chrono::time_point<std::chrono::high_resolution_clock>&
// StreamerClient2::getLastUpdateAcqDate( const std::string& streamName ) const {
// }

} // namespace server
} // namespace hub

// #if ( __cplusplus >= 201703L )
// #else
// #endif
