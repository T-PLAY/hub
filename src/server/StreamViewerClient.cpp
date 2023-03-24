
#include "StreamViewerClient.hpp"
#include "Server.hpp"

#include <iomanip>

namespace hub {
namespace server {

class OutputStream : public hub::io::Output
{
  public:
    explicit OutputStream( hub::net::ClientSocket&& clientSocket ) :
        m_clientSocket( std::move( clientSocket ) ) {}

  protected:
    void write( const hub::Acquisition& acq ) override;

    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;

  private:
    hub::net::ClientSocket m_clientSocket;
    //    bool m_closing = false;
    bool m_inputStreamClosed = false;

    friend class StreamViewerClient;
};

void OutputStream::write( const hub::Acquisition& acq ) {
    //    if (m_closing) {
    //        std::cout << "[OutputStream] closing connection, unable to write acquisition" <<
    //        std::endl; return;
    //    }
    hub::io::Output::write( hub::net::ClientSocket::Message::NEW_ACQ );
    hub::io::Output::write( acq );
}

void OutputStream::write( const unsigned char* data, size_t len ) {
    m_clientSocket.write( data, len );
}

void OutputStream::close() {
    //    if ( isOpen() )
    assert(m_inputStreamClosed);
    m_clientSocket.write( hub::net::ClientSocket::Message::STREAM_VIEWER_CLOSED );
//    m_clientSocket.write( hub::net::ClientSocket::Message::STREAM_VIEWER_CLOSED );
    //            output.write( hub::net::ClientSocket::Message::CLOSE );
    m_clientSocket.close();
}

bool OutputStream::isOpen() const {
    return m_clientSocket.isOpen();
}

//////////////////////////////////////////////////////////////////////

StreamViewerClient::StreamViewerClient( Server* server,
                                        int iClient,
                                        hub::net::ClientSocket&& sock ) :
    Client( server, iClient ) {

    assert( m_server != nullptr );
    const auto& streamers = m_server->getStreamers();

    sock.read( m_streamName );
    if ( streamers.find( m_streamName ) == streamers.end() ) {
        sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
        std::cout << headerMsg() << "unknown stream name : '" << m_streamName << "'" << std::endl;
        return;
    }
    else { sock.write( hub::net::ClientSocket::Message::OK ); }
    assert( streamers.find( m_streamName ) != streamers.end() );

    //    sock.read( m_syncStreamName );
    //    if ( m_syncStreamName != "" && streamers.find( m_syncStreamName ) == streamers.end() ) {
    //        sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
    //        std::cout << headerMsg() << "unknown sync stream name : '" << m_syncStreamName << "'"
    //                  << std::endl;
    //        return;
    //    }
    //    else {
    //        sock.write( hub::net::ClientSocket::Message::OK );
    //    }
    //    assert( m_syncStreamName == "" || streamers.find( m_syncStreamName ) != streamers.end() );

    //    sock.read( m_mergeSyncAcqs );

    //    m_streamer = streamers.at( m_streamName );

    //    hub::SensorSpec sensorSpec = m_streamer->getInputSensor().getSpec();
    //    if ( m_syncStreamName != "" && m_mergeSyncAcqs ) {
    //        sensorSpec += streamers.at( m_syncStreamName )->getInputSensor().getSpec();
    //    }
    const auto& sensorSpec = m_server->getSensorSpec( m_streamName );
    m_outputSensor =
        std::make_unique<hub::OutputSensor>( sensorSpec, OutputStream( std::move( sock ) ) );

    //    m_streamer->addStreamViewer( this );

    m_server->addStreamViewer( this );

    //    try {
    //        if ( m_streamer->isRecordStream() ) {
    //            std::lock_guard<std::mutex> guard( m_mtxOutputSensor );
    //            const auto& saveAcqs = m_streamer->getSaveAcqs( m_syncStreamName );
    // #if ( __cplusplus >= 201703L )
    //             for ( const auto& [start, acq] : saveAcqs ) {
    // #else
    //            for ( const auto& pair : saveAcqs ) {
    //                const auto & acq = pair.second;
    // #endif
    //                *m_outputSensor << *acq;
    //            }
    //            std::cout << headerMsg() << "sended " << saveAcqs.size() << " saved acqs" <<
    //            std::endl;
    //        }
    //        else {
    //            const auto& lastAcq = m_streamer->getLastAcq( m_syncStreamName );
    //            if ( lastAcq != nullptr ) {
    //                assert( lastAcq->getSize() == m_outputSensor->getSpec().getAcquisitionSize()
    //                ); *m_outputSensor << *lastAcq;
    //            }
    //        }
    //    }
    //    catch ( std::exception& e ) {
    //        assert( false );
    //        std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
    //                  << std::endl;
    //        if ( m_pingFailed ) { assert( false ); }
    //        else {
    //            assert( false );
    //            m_updateFailed = true;
    //            s_mtxCout.lock();
    //            m_streamer->delStreamViewer( this );
    //            s_mtxCout.unlock();
    //            std::thread( [this]() { delete this; } ).detach();
    //        }
    //        return;
    //    }

    m_thread = std::thread( [this]() {
        try {

            OutputStream& outputStream = dynamic_cast<OutputStream&>( m_outputSensor->getOutput() );
            hub::net::ClientSocket::Message message;
            // wait for lost connection
            outputStream.m_clientSocket.read( message );
            assert( message == net::ClientSocket::Message::INPUT_STREAM_CLOSED );
//            m_inputStreamClosed = true;
            outputStream.m_inputStreamClosed = true;

            //            assert( false );
            //            std::cout << headerMsg() << "receive message : " << message << std::endl;

            //            m_outputSensor->getOutput();

            //            m_lastUpdateAcq = std::chrono::high_resolution_clock::now();

            //            while ( !m_updateFailed ) {

            //                const std::chrono::time_point<std::chrono::high_resolution_clock> now
            //                =
            //                    std::chrono::high_resolution_clock::now();

            //                const int idleDuration =
            //                    std::chrono::duration_cast<std::chrono::milliseconds>( now -
            //                    m_lastUpdateAcq )
            //                        .count();

            //                constexpr int pingPeriod = 1'000; // ms
            //                if ( idleDuration > pingPeriod ) {
            //                    //                    m_mtxOutputSensor.lock();
            //                    //                    const auto& lastAcq =
            //                    m_streamer->getLastAcq(
            //                    //                    m_syncStreamName );
            //                    if ( m_updateFailed ) break;
            //                    assert( !m_updateFailed );
            //                    //                    if ( lastAcq.get() == nullptr ) {
            //                    if ( m_lastAcq.isEmpty() ) {
            //                        m_outputSensor->getOutput().write(
            //                        hub::net::ClientSocket::Message::PING ); std::cout <<
            //                        headerMsg() << "send ping " << std::endl;
            //                    }
            //                    else {
            //                        std::cout << headerMsg() << "send last acq ping " <<
            //                        std::endl; *m_outputSensor << m_lastAcq;
            //                    }
            //                    //                    m_mtxOutputSensor.unlock();
            //                    std::this_thread::sleep_for( std::chrono::milliseconds( pingPeriod
            //                    ) );
            //                }
            //                else {
            //                    std::this_thread::sleep_for(
            //                        std::chrono::milliseconds( pingPeriod - idleDuration ) );
            //                }

            //            } // while ( !m_updateFailed )
        }
        catch ( hub::net::Socket::exception& ex ) {
            //            assert( !m_updateFailed );
            //            assert( !m_pingFailed );
            //            m_pingFailed = true;

                        std::cout << headerMsg() << "catch exception : " << ex.what()
                                  << std::endl;
        }
        catch ( std::exception& ) {
            assert( false );
        }

        //        if ( m_updateFailed ) {
        //            std::cout << headerMsg() << "thread ended (update failed)" << std::endl;
        //        }
        //        else {
        //            std::cout << headerMsg() << "thread ended (lost connection)" << std::endl;
        //            //            m_streamer->delStreamViewer( this );
        //            //            m_server->delStreamViewer( this );

        //            //            m_mtxOutputSensor.unlock();
        //        }
        std::thread( [this]() { delete this; } ).detach();
    } );

    //    std::cout << headerMsg() << "new stream viewer watching '" << m_streamName << "'" <<
    //    std::endl; printStatusMessage( "new streamViewer" );
}

StreamViewerClient::~StreamViewerClient() {
    //    std::cout << headerMsg() << "delete start" << std::endl;
    //    assert(! m_ending);
    m_ending = true;

    //    assert( m_pingFailed == false );
    //    m_pingFailed = true;
    assert( m_thread.joinable() );
    m_thread.join();

    //    std::cout << headerMsg() << "delete ended" << std::endl;
    m_server->delStreamViewer( this );
    //    printStatusMessage( "del streamViewer" );
}

std::string StreamViewerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamViewer] ";
}

void StreamViewerClient::update( const Acquisition& acq ) {
    assert(m_outputSensor->getOutput().isOpen());
    if ( m_ending ) {
        throw net::ClientSocket::exception("stream viewer is ending");
    }
    else { *m_outputSensor << acq; }
}

void StreamViewerClient::end() {
    //    assert(! m_ending);
    //    if ( !m_ending ) {
    //        m_ending = true;

//	OutputStream& outputStream = dynamic_cast<OutputStream&>( m_outputSensor->getOutput() );
    auto& output = m_outputSensor->getOutput();
    if (output.isOpen()) {
        output.write( hub::net::ClientSocket::Message::STREAM_VIEWER_CLOSED );
    }
//    else {
//        assert(m_inputStreamClosed);
//    }
    //        if (output.isOpen())
    //            output.write( hub::net::ClientSocket::Message::CLOSE );
    //        m_outputSensor->getOutput().close();
    //    }
}

// bool StreamViewerClient::update( const hub::Acquisition& acq ) {
//     assert(! m_pingFailed);
////    if ( m_pingFailed ) return false;

//    try {
//        //        m_mtxOutputSensor.lock();
//        assert( !m_pingFailed );
//        assert( !m_updateFailed );
//        *m_outputSensor << acq;
////        m_lastUpdateAcq = std::chrono::high_resolution_clock::now();
//        //        m_mtxOutputSensor.unlock();
//    }
//    catch ( std::exception& e ) {
//        assert( !m_pingFailed );
//        assert( !m_updateFailed );
//        m_updateFailed = true;

//        std::cout << headerMsg()
//                  << "update(Acquisition) : catch outputSensor exception : " << e.what()
//                  << std::endl;
//        std::cout << headerMsg() << "update(Acquisition) failed with stream '" << m_streamName
//                  << "'" << std::endl;

//        //        m_streamer->delStreamViewer( this );
////        std::thread( [this]() { delete this; } ).detach();
//        //        m_mtxOutputSensor.unlock();

//        return false;
//    }

//    return true;
//}

// const std::string& StreamViewerClient::getSyncStreamName() const {
//     return m_syncStreamName;
// }

// const std::string& StreamViewerClient::getStreamName() const {
//     return m_streamName;
// }

// bool StreamViewerClient::shoudMergeSyncAcqs() const {
//     return m_mergeSyncAcqs;
// }

} // namespace server
} // namespace hub
