
#include <cstring>
#include <iomanip>

#include "ServerImpl2.hpp"
#include "StreamerClient2.hpp"

namespace hub {
namespace server {
namespace impl2 {

// class InputStreamClient : public Input
//{
//   public:
////    explicit InputStreamClient( net::ClientSocket&& clientSocket );
//    explicit InputStreamClient( io::InputOutputSocket&& clientSocket );

//    void read( sensor::Acquisition& acq );

//    InputStreamClient( InputStreamClient&& inputStream );
//    ~InputStreamClient();

//  protected:
//    void read( unsigned char* data, size_t len ) override;
//    void close() override;
//    bool isOpen() const override;
//    bool isEnd() const override;
//    void clear() override;

//  private:
////    net::ClientSocket m_clientSocket;
//    io::InputOutputSocket m_clientSocket;
//    bool m_outputStreamClosed = false;

//    bool m_moved = false;

//    friend class StreamerClient2;
//};

////InputStreamClient::InputStreamClient( net::ClientSocket&& clientSocket ) :
// InputStreamClient::InputStreamClient( io::InputOutputSocket&& clientSocket ) :
//     m_clientSocket( std::move( clientSocket ) ) {

//    std::cout << "[InputStreamClient] InputStreamClient()" << std::endl;
//}

// InputStreamClient::InputStreamClient( InputStreamClient&& inputStream ) :
//     m_clientSocket( std::move( inputStream.m_clientSocket ) ),
//     m_outputStreamClosed( inputStream.m_outputStreamClosed ) {
//     inputStream.m_moved = true;
//     std::cout << "[InputStreamClient] InputStreamClient(&&)" << std::endl;
// }

// InputStreamClient::~InputStreamClient() {
//     std::cout << "[InputStreamClient] ~InputStreamClient()" << std::endl;
// #ifdef DEBUG
//     if ( !m_moved ) { assert( !isOpen() ); }
// #endif
// }

// void InputStreamClient::read( sensor::Acquisition& acq )
//// Acquisition InputStreamClient::getAcq()
//{
//    io::StreamBase::ClientMessage mess;
//    m_clientSocket.read( mess );
//    if ( mess == io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED ) {
////        throw net::Socket::exception( "streamer closed" );
//        throw net::system::SocketSystem::exception( "streamer closed" );
//    }
//    assert( mess == io::StreamBase::ClientMessage::STREAMER_CLIENT_NEW_ACQ );
//    Input::read( acq );
//}

// void InputStreamClient::read( unsigned char* data, size_t len ) {
//     m_clientSocket.read( data, len );
// }

// void InputStreamClient::close() {
//     std::cout << "[InputStreamClient] close()" << std::endl;
//     m_clientSocket.write( io::StreamBase::ServerMessage::STREAMER_CLOSED );
//     m_clientSocket.close();
// }

// bool InputStreamClient::isOpen() const {
//     return m_clientSocket.isOpen();
// }

// bool InputStreamClient::isEnd() const {
//     return m_clientSocket.isEnd();
// }

// void InputStreamClient::clear() {
//     // todo server
// }

//////////////////////////////////////////////////////////////////////////////

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

    auto* sockPtr = m_sock.get();
    m_thread      = std::thread( [sockPtr, this]() mutable {
        try {

            while ( true ) {
                hub::io::StreamBase::ClientMessage mess =
                    hub::io::StreamBase::ClientMessage::NONE;
                sockPtr->read( mess );
                if ( mess == hub::io::StreamBase::ClientMessage::CLIENT_SERVER_DOWN ) {
                    m_serverDown = true;
                    break;
                }
                else if ( mess ==
                          hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED ) {
                    break;
                }
                else if ( mess == hub::io::StreamBase::ClientMessage::
                                      STREAMER_CLIENT_NEW_STREAM_VIEWER ) {
                    sockPtr->read( m_nStreamViewer );
                    m_server->newStreamViewer( this );
                    sockPtr->write( hub::io::StreamBase::ServerMessage::STREAM_VIEWER_INITED );
                    // std::cout << headerMsg() << "new stream viewer" << std::endl;
                    // m_server->printStatus();
                }
                // else if ( mess == io::StreamBase::ClientMessage::STREAMER_CLIENT_START ) {
                // std::cout << "[StreamerClient2] STREAMER_CLIENT_START" << std::endl;
                // }
                else { assert( false ); }
            }
        }
        catch ( net::system::SocketSystem::exception& ex ) {
            m_server->m_mtxPrint.lock();
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }

        std::thread( [this]() { delete this; } ).detach();
    } );

    m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_INITED );
}

int StreamerClient2::getNStreamViewer() const {
    return m_nStreamViewer;
}

StreamerClient2::~StreamerClient2() {
    //    m_server->m_mtxPrint.lock();
    std::cout << headerMsg() << "delete start" << std::endl;
    //    m_server->m_mtxPrint.unlock();

    assert( m_thread.joinable() );
    m_thread.join();

    if ( m_server != nullptr ) m_server->delStreamer( this );

    //    m_server->m_mtxPrint.lock();
    //    std::cout << headerMsg() << "delete ended" << std::endl;
    m_server->m_mtxPrint.unlock();
    if ( !m_serverDown ) {
        assert( m_sock->isOpen() );
        m_sock->write( hub::io::StreamBase::ServerMessage::STREAMER_CLOSED );
    }
}

std::string StreamerClient2::headerMsg() const {
    return Client2::headerMsg() + "[Streamer] ";
}

// const sensor::InputSensor* StreamerClient2::getInputSensor() const {
//     assert( m_inputSensor != nullptr );
//     return m_inputSensor.get();
// }

// sensor::Acquisition StreamerClient2::getLastAcq() const {

////    while ( m_lastAcq.isEnd() ) {
//    while ( ! m_lastAcq.hasValue() ) {
//        std::cout << "last acq empty" << std::endl;
//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//    }
////    assert( !m_lastAcq.isEnd() );
//    assert( m_lastAcq.hasValue() );
//    auto acq = m_lastAcq.clone();

//    return acq;
//}

void StreamerClient2::end( hub::io::StreamBase::ServerMessage message ) {
    //    InputStreamClient& input = dynamic_cast<InputStreamClient&>( m_inputSensor->getInput() );
    //    assert( input.m_clientSocket.isOpen() );
    assert( m_sock->isOpen() );
    m_sock->write( message );
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

} // namespace impl2
} // namespace server
} // namespace hub

// #if ( __cplusplus >= 201703L )
// #else
// #endif
