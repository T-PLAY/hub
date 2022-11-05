
#include "StreamViewerClient.hpp"
#include "Server.hpp"

#include <iomanip>

class StreamerInterface : public hub::io::OutputInterface, public hub::net::ClientSocket
{
  public:
    StreamerInterface( hub::net::ClientSocket&& clientSocket ) :
        hub::net::ClientSocket( std::move( clientSocket ) ) {}

  protected:
    void write( const hub::Acquisition& acq ) const override;

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() override;
#endif
};

void StreamerInterface::write( const hub::Acquisition& acq ) const {
    hub::io::Interface::write( hub::net::ClientSocket::Message::NEW_ACQ );
    hub::io::Interface::write( acq );
}

#ifdef WIN32 // msvc warning C4250
void StreamerInterface::write( const unsigned char* data, size_t len ) const {
    hub::net::ClientSocket::write( data, len );
}

void StreamerInterface::read( unsigned char* data, size_t len ) const {
    hub::net::ClientSocket::read( data, len );
}

void StreamerInterface::close() {
    hub::net::ClientSocket::close();
}
#endif

//////////////////////////////////////////////////////////////////////

StreamViewerClient::StreamViewerClient( Server& server,
                                        int iClient,
                                        hub::net::ClientSocket&& sock ) :
    Client( server, iClient ) {

    const auto& streamers = m_server.getStreamers();

    sock.read( m_streamName );
    if ( streamers.find( m_streamName ) == streamers.end() ) {
        sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
        std::cout << headerMsg() << "unknown sensor name : '" << m_streamName << "'" << std::endl;
        return;
    }
    else { sock.write( hub::net::ClientSocket::Message::OK ); }
    assert( streamers.find( m_streamName ) != streamers.end() );

    sock.read( m_syncStreamName );
    if ( m_syncStreamName != "" && streamers.find( m_syncStreamName ) == streamers.end() ) {
        sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
        std::cout << headerMsg() << "unknown sync sensor name : '" << m_syncStreamName << "'"
                  << std::endl;
        return;
    }
    else { sock.write( hub::net::ClientSocket::Message::OK ); }
    assert( m_syncStreamName == "" || streamers.find( m_syncStreamName ) != streamers.end() );

    StreamerClient* streamer   = streamers.at( m_streamName );
    hub::SensorSpec sensorSpec = streamer->getInputSensor().m_spec;
    if ( m_syncStreamName != "" ) {
        sensorSpec += streamers.at( m_syncStreamName )->getInputSensor().m_spec;
        streamer = streamers.at( m_syncStreamName );
    }
    m_outputSensor = std::make_unique<hub::OutputSensor>( std::move( sensorSpec ),
                                                          StreamerInterface( std::move( sock ) ) );

    //    m_server.addStreamViewer( this );
    if ( m_syncStreamName == "" ) { streamer->addStreamViewer( this ); }
    else {
        assert( streamers.find( m_syncStreamName ) != streamers.end() );
        auto& syncStreamer = *streamers.at( m_syncStreamName );

        syncStreamer.addSyncStreamViewer( this );
    }

    std::string lastAcqsName = ( m_syncStreamName == "" ) ? ( "" ) : ( m_streamName );

    if ( m_syncStreamName == "" ) {
        m_mtxOutputSensor.lock();
        try {
            if ( m_outputSensor->m_spec.m_resolutions.size() == 2 ) {
                const auto& saveAcqs = streamer->getSaveAcqs( lastAcqsName );
                for ( const auto& [start, acq] : saveAcqs ) {
                    *m_outputSensor << *acq;
                }
                std::cout << headerMsg() << "sended " << saveAcqs.size() << " saved acqs"
                          << std::endl;
            }
            else {
                const auto& lastAcq = streamer->getLastAcq( lastAcqsName );
                assert( lastAcq != nullptr );
                //                if ( lastAcq.get() == nullptr ) {
                //                    m_outputSensor->getInterface().write(
                //                    hub::net::ClientSocket::Message::PING );
                //                }
                //                else {
                assert( lastAcq->getSize() == m_outputSensor->m_spec.m_acquisitionSize );
                //                    if ( m_server.getAcqPing() ) {
                *m_outputSensor << *lastAcq;
                //                    }
                //                    else {
                //                        m_outputSensor->getInterface().write(
                //                            hub::net::ClientSocket::Message::PING );
                //                    }
                //                }
            }
        }
        catch ( std::exception& e ) {
            m_mtxOutputSensor.unlock();
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
            m_isKilled = true;
//            m_server.delStreamViewer( this );
            return;
        }
        m_mtxOutputSensor.unlock();
    }

    m_thread = std::thread( [this, streamer, lastAcqsName]() {
        try {
            //            std::chrono::high_resolution_clock::
            //        std::chrono::time_point<std::chrono::high_resolution_clock> lastPingDate =
            //            std::chrono::high_resolution_clock::now();

            //            if ( !m_updateFailed ) m_mtxOutputSensor.lock();
            // check client still alive
            while ( !m_updateFailed || !m_isKilled ) {
                //                m_mtxOutputSensor.lock();
                if ( m_updateFailed || m_isKilled ) {
                    //                    m_mtxOutputSensor.unlock();
                    break;
                }

                const std::chrono::time_point<std::chrono::high_resolution_clock> now =
                    std::chrono::high_resolution_clock::now();

                const auto& lastUpdateAcqDate = streamer->getLastUpdateAcqDate( lastAcqsName );

                const int idleDuration =
                    std::chrono::duration_cast<std::chrono::milliseconds>( now - lastUpdateAcqDate )
                        .count();

                constexpr int pingPeriod = 1'000;
                if ( idleDuration > pingPeriod ) {
                    m_mtxOutputSensor.lock();
                    m_outputSensor->getInterface().write( hub::net::ClientSocket::Message::PING );
                    m_mtxOutputSensor.unlock();
                    std::this_thread::sleep_for( std::chrono::milliseconds( pingPeriod ) );
                }
                else {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds( pingPeriod - idleDuration ) );
                }

                //                const auto& lastAcq = streamer->getLastAcq( lastAcqsName );
                //                if ( lastAcq.get() == nullptr ) {
                //                    m_outputSensor->getInterface().write(
                //                    hub::net::ClientSocket::Message::PING );
                //                }
                //                else {
                ////                    assert( lastAcq->getSize() ==
                /// m_outputSensor->m_spec.m_acquisitionSize ); /                    if (
                /// m_server.getAcqPing() ) { *m_outputSensor << *lastAcq; } / else { /
                /// m_outputSensor->getInterface().write( / hub::net::ClientSocket::Message::PING );
                ////                    }
                //                }
                //                m_mtxOutputSensor.unlock();
                //                std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
                //                m_mtxOutputSensor.lock();
            } // while ( !m_updateFailed || !m_isKilled )
        }
        catch ( hub::net::Socket::exception& e ) {
            m_mtxOutputSensor.unlock();
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
        }
        catch ( std::exception& e ) {
            assert(false);
//            m_mtxOutputSensor.unlock();
//            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
//                      << std::endl;
//            throw e;
        }

        if ( m_isKilled ) { std::cout << headerMsg() << "thread killed" << std::endl; }
        else {
            std::cout << headerMsg() << "thread end (update failed)" << std::endl;
            m_isKilled = true;
//            m_server.delStreamViewer( this );
        }
    } );

    printStatusMessage("new streamViewer");
//    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
//              << "new stream viewer" << m_server.getStatus() << std::endl;
    std::cout << headerMsg() << "watching '" << m_streamName << "'" << std::endl;
//    std::cout << "-------------------------------------------------------------------------"
//                 "--------------------"
//              << std::endl;

}

StreamViewerClient::~StreamViewerClient() {
//    s_mtxCout.lock();
//    std::cout << headerMsg() << "deleted" << std::endl;
    //    assert(m_thread == nullptr);
//    if ( m_thread != nullptr ) { m_thread->detach(); }

    m_isKilled = true;
    assert(m_thread.joinable());
    m_thread.join();

    printStatusMessage("del streamViewer");
//    s_mtxCout.unlock();

//    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
//              << "del stream viewer" << m_server.getStatus() << std::endl;
//    std::cout << "-------------------------------------------------------------------------"
//                 "--------------------"
//              << std::endl;
}

std::string StreamViewerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamViewerClient] ";
}

void StreamViewerClient::update( const hub::Acquisition& acq ) {
    if ( m_updateFailed ) return;

    try {
        m_mtxOutputSensor.lock();
        *m_outputSensor << acq;
        m_mtxOutputSensor.unlock();
    }
    catch ( std::exception& e ) {
        std::cout << headerMsg()
                  << "update(Acquisition) : catch outputSensor exception : " << e.what()
                  << std::endl;
        std::cout << headerMsg() << "update(Acquisition) : end stream : '" << m_streamName << "'"
                  << std::endl;

        m_updateFailed = true;
        m_mtxOutputSensor.unlock();
    }
}

//void StreamViewerClient::killThread() {
//    if ( m_isKilled ) return;
//    m_isKilled = true;
//    if ( m_thread != nullptr ) {
//        if ( m_thread->joinable() ) {
//            m_thread->join();
//            delete m_thread;
//            m_thread = nullptr;
//        }
//    }
//}

const std::string& StreamViewerClient::getSyncStreamName() const {
    return m_syncStreamName;
}

const std::string& StreamViewerClient::getStreamName() const {
    return m_streamName;
}
