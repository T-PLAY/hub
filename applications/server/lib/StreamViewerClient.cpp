
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
    //    std::cout << "[StreamerInterface ----------------------------------" << std::endl;
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

    const StreamerClient* streamer         = streamers.at( m_streamName );
    const hub::SensorSpec sensorSpec = streamer->getInputSensor().m_spec;
    //    m_outputSensor        = std::make_unique<OutputSensor>( std::move( sensorSpec ),
    //                                                     io::Streamer( std::move( sock ) ) );
    m_outputSensor = std::make_unique<hub::OutputSensor>( std::move( sensorSpec ),
                                                          StreamerInterface( std::move( sock ) ) );

    //    server.m_streamViewers.push_back( this );
    //    server.m_streamViewers[m_streamName].push_back( this );
    m_server.addStreamViewer( this );

    const auto& lastAcqs = m_server.getLastAcqs( m_streamName );
    try {
        for ( const auto& acq : lastAcqs ) {
            *m_outputSensor << *acq;
        }
        //        m_server.releaseGetLastAcqs(m_streamName);
    }
    catch ( std::exception& e ) {
        //        m_server.releaseGetLastAcqs(m_streamName);
        std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                  << std::endl;
        m_server.delStreamViewer( this );
        return;
    }

    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "new stream viewer" << m_server.getStatus() << std::endl;
    std::cout << headerMsg() << "watching '" << m_streamName << "'" << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;

    //    std::thread thread( [this, streamer]() {
    m_thread = new std::thread( [this, streamer]() {
        try {
            // check client still alive
            while ( !m_updateFailed || !m_isKilled ) {
                //                std::cout << headerMsg() << "thread : start loop" << std::endl;
                m_mtxOutputSensor.lock();
                //                std::cout << headerMsg() << "thread : writing ping" << std::endl;
                //                const auto& lastAcqs = m_server.getLastAcqs( m_streamName );
                //                const auto * lastAcq = m_server.getLastAcq(m_streamName);
                //                streamer->m_mtxLastAcqs.lock();
                //                std::cout << headerMsg() << "thread : start getLastAcq" <<
                //                std::endl; const auto lastAcq = streamer->getLastAcq();
                streamer->m_mtxLastAcqs.lock();
                if ( m_updateFailed || m_isKilled ) {
                    streamer->m_mtxLastAcqs.unlock();
                    m_mtxOutputSensor.unlock();
                    break;
                }
                const auto& lastAcqs = streamer->getLastAcqs();
                //                std::cout << headerMsg() << "thread : end getLastAcq" <<
                //                std::endl; if ( lastAcq == nullptr ) {
                if ( lastAcqs.empty() ) {
                    m_outputSensor->getInterface().write( hub::net::ClientSocket::Message::PING );
                    //                    std::cout << headerMsg() << "thread : ping wrote" <<
                    //                    std::endl;
                }
                else {
                    if ( m_server.m_acqPing ) {
                        *m_outputSensor << *lastAcqs.back();
                        //                        *m_outputSensor << *lastAcq;
                        //                        for (const auto & acq : lastAcqs) {
                        //                        *m_outputSensor << *acq;
                        //                        }
                    }
                    else {
                        m_outputSensor->getInterface().write(
                            hub::net::ClientSocket::Message::PING );
                    }

                    //                    std::cout << headerMsg() << "thread : acq wrote" <<
                    //                    std::endl;
                }
                streamer->m_mtxLastAcqs.unlock();
                //                streamer->m_mtxLastAcqs.unlock();
                m_mtxOutputSensor.unlock();
                //                m_server.releaseGetLastAcqs(m_streamName);
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                //                std::cout << headerMsg() << "thread : end sleep" << std::endl;
            }
        }
        catch ( hub::net::Socket::exception& e ) {
            //            streamer->m_mtxLastAcqs.unlock();
            streamer->m_mtxLastAcqs.unlock();
            m_mtxOutputSensor.unlock();
            //            m_server.releaseGetLastAcqs(m_streamName);
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
        }
        catch ( std::exception& e ) {
            //            streamer->m_mtxLastAcqs.unlock();
            streamer->m_mtxLastAcqs.unlock();
            m_mtxOutputSensor.unlock();
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
            throw e;
        }

        //        if ( !m_isKilled ) { m_server.delStreamViewer( this ); }

        if ( m_isKilled ) { std::cout << headerMsg() << "thread : thread killed" << std::endl; }
        else {
            std::cout << headerMsg() << "thread : thread end" << std::endl;
            //        if ( !m_updateFailed ) { m_server.delStreamViewer( this ); }
            m_isKilled = true;
            m_server.delStreamViewer( this );
        }
    } );
    //        m_thread.detach();
    //    thread.detach();
}

StreamViewerClient::~StreamViewerClient() {
    //    std::cout << headerMsg() << "destroying start" << std::endl;
    //    assert( m_thread.joinable() );
    //    m_isKilled = true;
    //        m_thread.join();
    std::cout << headerMsg() << "deleted" << std::endl;
    //    if (! m_isKilled)
    if ( m_thread != nullptr ) {
        m_thread->detach();
        //        if (m_thread->joinable()) {
        //            m_thread->join();
        //            m_mtxOutputSensor.unlock();
        //        }
    }
    //    m_mtxOutputSensor.unlock();
}

std::string StreamViewerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamViewerClient] ";
}

void StreamViewerClient::update( const hub::Acquisition& acq ) {
    //    std::cout << headerMsg() << "update(Acquisition) receive acq : " << acq << std::endl;
    //    if ( m_syncStreamName != "" ) { m_acquisitions.emplace_front( acq.clone() ); }
    //    else {
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

        //            m_server.m_streamers.erase(m_streamName);

        //        std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw(
        //        g_margin )
        //                  << "thread end" << std::endl;
        //        std::cout <<
        //        "-------------------------------------------------------------------------"
        //                     "--------------------"
        //                  << std::endl;

        m_updateFailed = true;
        //        m_server.delStreamViewer( this );
        m_mtxOutputSensor.unlock();
    }
    //    }
}

void StreamViewerClient::killThread() {
    if ( m_isKilled ) return;
    m_isKilled = true;
    if ( m_thread != nullptr ) {
        if ( m_thread->joinable() ) {
            m_thread->join();
            delete m_thread;
            m_thread = nullptr;
        }
    }
}

const std::string& StreamViewerClient::getSyncStreamName() const {
    return m_syncStreamName;
}

const std::string& StreamViewerClient::getStreamName() const {
    return m_streamName;
}
