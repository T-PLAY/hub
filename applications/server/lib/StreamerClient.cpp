
#include "StreamerClient.hpp"
#include "Server.hpp"
#include <cstring>
//#include <iostream>
//#include <string>
#include <iomanip>
//#include <iterator>

class StreamViewerInterface : public hub::io::InputInterface, public hub::net::ClientSocket
{
  public:
    StreamViewerInterface( hub::net::ClientSocket&& clientSocket ) :
        hub::net::ClientSocket( std::move( clientSocket ) ) {}

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() override;
#endif
};

#ifdef WIN32 // msvc warning C4250
void StreamViewerInterface::write( const unsigned char* data, size_t len ) const {
    hub::net::ClientSocket::write( data, len );
}

void StreamViewerInterface::read( unsigned char* data, size_t len ) const {
    hub::net::ClientSocket::read( data, len );
}

void StreamViewerInterface::close() {
    hub::net::ClientSocket::close();
}
#endif

//////////////////////////////////////////////////////////////////////////////

StreamerClient::StreamerClient( Server& server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ) {

    m_mtx.lock();

    sock.read( m_streamName );
    const auto& streamers = m_server.getStreamers();
    if ( streamers.find( m_streamName ) == streamers.end() ) {
        sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
    }
    else {
        sock.write( hub::net::ClientSocket::Message::FOUND );
        std::cout << headerMsg() << "stream sensor name : '" << m_streamName << "' already exist"
                  << std::endl;
        return;
    }
    assert( streamers.find( m_streamName ) == streamers.end() );

    try {
        m_inputSensor =
            std::make_unique<hub::InputSensor>( StreamViewerInterface( std::move( sock ) ) );
    }
    catch ( hub::net::Socket::exception& e ) {
        std::cout << headerMsg() << "InputSensor() : catch exception : " << e.what() << std::endl;
        return;
    }

    assert( streamers.find( m_streamName ) == streamers.end() );

    std::cout << headerMsg() << "stream name = '" << m_streamName << "'" << std::endl;

    const auto& sensorSpec = m_inputSensor->m_spec;

    const size_t acquisitionSize = sensorSpec.m_acquisitionSize;
    std::cout << headerMsg() << "sensor name:'" << sensorSpec.m_sensorName << "'" << std::endl;
    std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
    std::cout << headerMsg()
              << "resolutions:" << hub::SensorSpec::resolutions2string( sensorSpec.m_resolutions )
              << std::endl;

    const auto& metaData = sensorSpec.m_metaData;
    for ( const auto& pair : metaData ) {
        std::cout << headerMsg() << "metaData: " << hub::SensorSpec::metaData2string( pair )
                  << std::endl;
    }
    if ( metaData.find( "type" ) != metaData.end() ) {
        std::cout << headerMsg() << "type detected : record stream" << std::endl;
        const char* type = std::any_cast<const char*>( metaData.at( "type" ) );
        if ( strcmp( type, "record" ) == 0 ) { m_isRecordStream = true; }
    }

    m_lastAcq[""];

    std::thread thread( [this]() {
        try {
            while ( 1 ) {
                auto masterAcq = m_inputSensor->getAcquisition();

                if ( masterAcq.m_start == -1 ) {
                    std::cout << headerMsg() << "receive reset acq : " << masterAcq << std::endl;
                    m_lastAcq[""].reset();
                    m_streamName2saveAcqs[""].clear();
                    m_server.newAcquisition( this, masterAcq );
                    continue;
                }

                m_mtxSyncViewers.lock();
                auto& syncViewers = m_syncViewers;
                for ( auto& pair : syncViewers ) {
                    const auto& syncViewerName = pair.first;

                    auto& syncAcqs = m_syncAcqs.at( syncViewerName );

                    bool foundLeft    = false;
                    bool& isSyncthing = m_isSyncthing.at( syncViewerName );
                    do {
                        while ( syncAcqs.size() < 2 ) {
                            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
                        }

                        assert( syncAcqs.size() >= 2 );

                        if ( syncAcqs.begin()->m_start == std::next( syncAcqs.begin() )->m_start ) {
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start !=
                                std::next( syncAcqs.begin() )->m_start );

                        if ( syncAcqs.begin()->m_start >= std::next( syncAcqs.begin() )->m_start ) {
                            syncAcqs.pop_front();
                            isSyncthing = false;
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start <
                                std::next( syncAcqs.begin() )->m_start );

                        if ( syncAcqs.begin()->m_start >= masterAcq.m_start ) {
                            if ( !isSyncthing ) { break; }
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start < masterAcq.m_start );

                        if ( std::next( syncAcqs.begin() )->m_start >= masterAcq.m_start ) {
                            foundLeft = true;
                            if ( !isSyncthing ) isSyncthing = true;
                        }
                        else { syncAcqs.pop_front(); }

                    } while ( !foundLeft );

                    if ( !foundLeft ) continue;

                    const auto itLeftAcq  = syncAcqs.begin();
                    const auto itRightAcq = std::next( itLeftAcq );

                    assert( itLeftAcq->m_start < masterAcq.m_start );
                    assert( itRightAcq->m_start >= masterAcq.m_start );

                    const auto closestAcq = ( std::abs( itLeftAcq->m_start - masterAcq.m_start ) >
                                              std::abs( itRightAcq->m_start - masterAcq.m_start ) )
                                                ? ( itRightAcq )
                                                : ( itLeftAcq );
                    const auto dist       = std::abs( closestAcq->m_start - masterAcq.m_start );

                    // assert(minDist < 20'000); // 20 ms
                    const auto maxDist = ( itRightAcq->m_start - itLeftAcq->m_start ) / 2;
                    if ( dist > maxDist ) {
                        std::cout << headerMsg() << "sync dist = abs( "
                                  << closestAcq->m_start - masterAcq.m_start << " ) > " << maxDist
                                  << std::endl;
                        continue;
                    }

                    assert( masterAcq.getMeasures().size() == 1 );
                    assert( closestAcq->getMeasures().size() == 1 );

                    hub::Acquisition acq { masterAcq.m_start, masterAcq.m_end };
                    if ( itLeftAcq->interpolable() ) {
                        const double t = ( masterAcq.m_start - itLeftAcq->m_start ) /
                                         (double)( itRightAcq->m_start - itLeftAcq->m_start );
                        acq << hub::Acquisition::slerp( *itLeftAcq, *itRightAcq, t ).getMeasures();
                    }
                    else { acq << closestAcq->getMeasures(); }
                    acq << masterAcq.getMeasures();

                    const auto streamViewers = pair.second;
                    assert( !streamViewers.empty() );
                    for ( auto& streamViewer : streamViewers ) {
                        streamViewer->update( acq );
                    }

                    saveNewAcq( syncViewerName, std::move( acq ) );

                } // for ( auto& pair : syncViewers )
                m_mtxSyncViewers.unlock();

                m_server.newAcquisition( this, masterAcq );

                if ( masterAcq.getMeasures().size() == 1 ) {
                    m_lastAcq[""] = std::make_shared<hub::Acquisition>( std::move( masterAcq ) );
                }
                else { saveNewAcq( "", std::move( masterAcq ) ); }
            }
        }
        catch ( hub::net::Socket::exception& e ) {
            std::cout << headerMsg() << "in : catch inputSensor exception : " << e.what()
                      << std::endl;
        }
        std::cout << headerMsg() << "end (" << m_inputSensor->m_spec.m_sensorName << ")"
                  << std::endl;

        std::cout << headerMsg() << "thread end" << std::endl;

        m_server.delStreamer( this );
    } );
    thread.detach();

    // get record acqs before prevent viewer
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    m_server.addStreamer( this );
    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "new streamer" << m_server.getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;

    m_mtx.unlock();
}

StreamerClient::~StreamerClient() {
    m_mtx.lock();

    m_streamName2saveAcqs.clear();

    m_mtxSyncViewers.lock();
    auto syncViewers = m_syncViewers;
    for ( auto& pair : syncViewers ) {
        auto syncViewers = pair.second;
        for ( auto* syncViewer : syncViewers ) {
            m_server.delStreamViewer( syncViewer );
        }
    }
    m_syncViewers.clear();
    m_mtxSyncViewers.unlock();

    m_syncAcqs.clear();
    m_isSyncthing.clear();
    std::cout << headerMsg() << "deleted" << std::endl;

    m_mtx.unlock();
}

std::string StreamerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamerClient] ";
}

const hub::InputSensor& StreamerClient::getInputSensor() const {
    return *m_inputSensor.get();
}

void StreamerClient::addSyncStreamViewer( StreamViewerClient* syncStreamViewer ) {
    const auto& streamName = syncStreamViewer->getStreamName();

    m_mtxSyncViewers.lock();

    auto& syncViewers = m_syncViewers[streamName];
    assert( std::find( syncViewers.begin(), syncViewers.end(), syncStreamViewer ) ==
            syncViewers.end() );
    syncViewers.push_back( syncStreamViewer );
    m_syncAcqs[streamName];
    m_isSyncthing[streamName] = false;
    m_streamName2saveAcqs[streamName];
    m_lastAcq[streamName];

    m_mtxSyncViewers.unlock();
}

void StreamerClient::delStreamViewer( StreamViewerClient* streamViewer ) {

    const auto& streamName = streamViewer->getStreamName();

    m_mtxSyncViewers.lock();
    auto& syncViewers = m_syncViewers[streamName];
    assert( std::find( syncViewers.begin(), syncViewers.end(), streamViewer ) !=
            syncViewers.end() );
    syncViewers.remove( streamViewer );
    if ( syncViewers.empty() ) {
        m_syncViewers.erase( streamName );
        m_syncAcqs.erase( streamName );
        m_isSyncthing.erase( streamName );
        m_streamName2saveAcqs.erase( streamName );
        m_lastAcq.erase( streamName );
    }
    m_mtxSyncViewers.unlock();
}

void StreamerClient::newAcquisition( const std::string& streamerName,
                                     const hub::Acquisition& acq ) {
    if ( m_syncViewers.find( streamerName ) != m_syncViewers.end() ) {
        m_mtxSyncAcqs.lock();
        m_syncAcqs[streamerName].emplace_back( acq.clone() );
        m_mtxSyncAcqs.unlock();
    }
}

const std::string& StreamerClient::getStreamName() const {
    return m_streamName;
}

const std::map<std::string, std::list<StreamViewerClient*>>&
StreamerClient::getSyncViewers() const {
    return m_syncViewers;
}

// const std::vector<std::unique_ptr<hub::Acquisition>>& StreamerClient::getLastAcqs() const {
// const std::vector<std::shared_ptr<hub::Acquisition>>&
// StreamerClient::getLastAcqs( const std::string& streamName ) const {
//}

const std::shared_ptr<hub::Acquisition>
StreamerClient::getLastAcq( const std::string& streamName ) const {
    assert( m_lastAcq.find( streamName ) != m_lastAcq.end() );
    return m_lastAcq.at( streamName );
}

const std::map<long long, std::shared_ptr<hub::Acquisition>>&
StreamerClient::getSaveAcqs( const std::string& streamName ) const {
    assert( m_streamName2saveAcqs.find( streamName ) != m_streamName2saveAcqs.end() );
    return m_streamName2saveAcqs.at( streamName );
}

void StreamerClient::saveNewAcq( const std::string& streamName, hub::Acquisition&& newAcq ) {
    auto newAcqPtr = std::make_shared<hub::Acquisition>( std::move( newAcq ) );

    auto& saveAcqs = m_streamName2saveAcqs[streamName];
    // save new acq
    if ( saveAcqs.find( newAcq.m_start ) == saveAcqs.end() ) {
        std::cout << headerMsg() << " save new acq : [ ";
        for ( const auto& [streamName, saveAcqs] : m_streamName2saveAcqs ) {
            std::cout << streamName << ":" << saveAcqs.size() << ", ";
        }
        std::cout << " ] ";
        std::cout << std::endl;
        saveAcqs[newAcq.m_start] = newAcqPtr;
    }
    // acq already saved
    else {}

    auto& lastAcq = m_lastAcq[streamName];
    if ( lastAcq.get() != newAcqPtr.get() ) { lastAcq = newAcqPtr; }
}

/// std::make_unique<hub::Acquisition>(std::move(*rightAcq)); }

/// syncAcqs.back() );
