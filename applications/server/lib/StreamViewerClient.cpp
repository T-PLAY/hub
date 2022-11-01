
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

    const StreamerClient* streamer = streamers.at( m_streamName );
    hub::SensorSpec sensorSpec     = streamer->getInputSensor().m_spec;
    if ( m_syncStreamName != "" ) {
        sensorSpec += streamers.at( m_syncStreamName )->getInputSensor().m_spec;
        streamer = streamers.at( m_syncStreamName );
    }
    m_outputSensor = std::make_unique<hub::OutputSensor>( std::move( sensorSpec ),
                                                          StreamerInterface( std::move( sock ) ) );

    m_server.addStreamViewer( this );

    std::string lastAcqsName = ( m_syncStreamName == "" ) ? ( "" ) : ( m_streamName );

    if ( m_syncStreamName == "" && m_outputSensor->m_spec.m_resolutions.size() == 2 ) {
        m_mtxOutputSensor.lock();
        const auto& lastAcqs = streamer->getSaveAcqs( lastAcqsName );
        try {
            for ( const auto& [start, acq] : lastAcqs ) {
                *m_outputSensor << *acq;
            }
        }
        catch ( std::exception& e ) {
            m_mtxOutputSensor.unlock();
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
            m_server.delStreamViewer( this );
            return;
        }
        m_mtxOutputSensor.unlock();
    }

    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "new stream viewer" << m_server.getStatus() << std::endl;
    std::cout << headerMsg() << "watching '" << m_streamName << "'" << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;

    m_thread = new std::thread( [this, streamer, lastAcqsName]() {
        try {
            // check client still alive
            while ( !m_updateFailed || !m_isKilled ) {
                m_mtxOutputSensor.lock();
                if ( m_updateFailed || m_isKilled ) {
                    m_mtxOutputSensor.unlock();
                    break;
                }
                const auto& lastAcq = streamer->getLastAcq( lastAcqsName );
                if ( lastAcq.get() == nullptr ) {
                    m_outputSensor->getInterface().write( hub::net::ClientSocket::Message::PING );
                }
                else {
                    assert( lastAcq->getSize() == m_outputSensor->m_spec.m_acquisitionSize );
                    if ( m_server.m_acqPing ) {
                        *m_outputSensor << *lastAcq;
                    }
                    else {
                        m_outputSensor->getInterface().write(
                            hub::net::ClientSocket::Message::PING );
                    }

                }
                m_mtxOutputSensor.unlock();
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }
        }
        catch ( hub::net::Socket::exception& e ) {
            m_mtxOutputSensor.unlock();
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
        }
        catch ( std::exception& e ) {
            m_mtxOutputSensor.unlock();
            std::cout << headerMsg() << "thread : catch stream viewer exception : " << e.what()
                      << std::endl;
            throw e;
        }


        if ( m_isKilled ) { std::cout << headerMsg() << "thread : thread killed" << std::endl; }
        else {
            std::cout << headerMsg() << "thread : thread end" << std::endl;
            m_isKilled = true;
            m_server.delStreamViewer( this );
        }
    } );
}

StreamViewerClient::~StreamViewerClient() {
    std::cout << headerMsg() << "deleted" << std::endl;
    if ( m_thread != nullptr ) {
        m_thread->detach();
    }
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
