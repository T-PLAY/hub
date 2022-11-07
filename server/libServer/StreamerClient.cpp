
#include "StreamerClient.hpp"
#include "Server.hpp"
#include <cstring>
//#include <iostream>
//#include <string>
#include <iomanip>
//#include <iterator>

// std::mutex StreamerClient::s_mtxCout;

class StreamViewerInterface : public hub::io::InputInterface, public hub::net::ClientSocket
{
  public:
    StreamViewerInterface( hub::net::ClientSocket&& clientSocket ) :
        hub::net::ClientSocket( std::move( clientSocket ) ) {}

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() const override;
    bool isOpen() const override;
    bool isEnd() const override;
#endif
};

#ifdef WIN32 // msvc warning C4250
void StreamViewerInterface::write( const unsigned char* data, size_t len ) const {
    hub::net::ClientSocket::write( data, len );
}

void StreamViewerInterface::read( unsigned char* data, size_t len ) const {
    hub::net::ClientSocket::read( data, len );
}

void StreamViewerInterface::close() const {
    hub::net::ClientSocket::close();
}

bool StreamViewerInterface::isOpen() const
{
    return hub::net::ClientSocket::isOpen();
}

bool StreamViewerInterface::isEnd() const
{
    return hub::net::ClientSocket::isEnd();
}
#endif

//////////////////////////////////////////////////////////////////////////////

StreamerClient::StreamerClient( Server& server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ) {

    //    m_mtx.lock();

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
    //    if ( metaData.find( "type" ) != metaData.end() ) {
    //        std::cout << headerMsg() << "type detected : record stream" << std::endl;
    //        const char* type = std::any_cast<const char*>( metaData.at( "type" ) );
    //        if ( strcmp( type, "record" ) == 0 ) { m_isRecordStream = true; }
    //    }
    if ( metaData.find( "parent" ) != metaData.end() ) {
        m_parent = std::any_cast<const char*>( metaData.at( "parent" ) );
        std::cout << headerMsg() << "parent : '" << m_parent << "'" << std::endl;
    }

    m_lastAcq[""];

    m_thread = std::thread( [this]() {
        try {
            while ( true ) {
                auto masterAcq = m_inputSensor->getAcquisition();

                if ( masterAcq.m_start == -1 ) {
                    std::cout << headerMsg() << "receive (history) reset acq : " << masterAcq
                              << std::endl;
                    m_lastAcq[""].reset();
                    m_streamName2saveAcqs[""].clear();
                    m_server.newAcquisition( this, masterAcq );
                    continue;
                }

                // broadcast acquisition for each stream viewers
                auto streamViewers = m_streamViewers;
                for ( const auto& streamViewer : streamViewers ) {
                    assert( streamViewer->getSyncStreamName() == "" );
                    streamViewer->update( masterAcq );
                }

                // broadcast acquisition for each sync stream viewers
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

                        // do not synchronize ping acq
                        if ( syncAcqs.begin()->m_start == std::next( syncAcqs.begin() )->m_start ) {
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start !=
                                std::next( syncAcqs.begin() )->m_start );

                        // autoloop detected (player)
                        if ( syncAcqs.begin()->m_start >= std::next( syncAcqs.begin() )->m_start ) {
                            syncAcqs.pop_front();
                            isSyncthing = false;
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start <
                                std::next( syncAcqs.begin() )->m_start );

                        // unable to synchronize these acquisitions (too far from master acq)
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
                    // if too far then abort synchronize
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
                    m_lastUpdateAcqDate[""] = std::chrono::high_resolution_clock::now();
                }
                else { saveNewAcq( "", std::move( masterAcq ) ); }

            } // while (true)
        }
        catch ( hub::net::Socket::exception& e ) {
            s_mtxCout.lock();
            std::cout << headerMsg() << "in : catch inputSensor exception : " << e.what()
                      << std::endl;
            //            s_mtxCout.unlock();
        }
        std::cout << headerMsg() << "end streamer : '" << m_inputSensor->m_spec.m_sensorName << "'"
                  << std::endl;

        std::cout << headerMsg() << "thread end" << std::endl;
        s_mtxCout.unlock();

        //        m_server.delStreamer( this );
        std::thread( [this]() { delete this; } ).detach();
        //        delete this;
    } );
    //    thread.detach();

    // get record acqs before prevent viewer
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    m_server.addStreamer( this );

    printStatusMessage( "new streamer" );
    //    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
    //              << "new streamer" << m_server.getStatus() << std::endl;
    //    std::cout << "-------------------------------------------------------------------------"
    //                 "--------------------"
    //              << std::endl;

    //    m_mtx.unlock();
}

StreamerClient::~StreamerClient() {
    //    m_mtx.lock();

    assert( m_thread.joinable() );
    m_thread.join();
    //    delete m_thread;
    //    m_thread = nullptr;

    //    m_server.delStreamer(this);

    m_streamName2saveAcqs.clear();

    s_mtxCout.lock();

    auto it = m_streamViewers.begin();
    while ( it != m_streamViewers.end() ) {
        //    for ( auto* streamViewer : m_streamViewers ) {
        //        m_server.delStreamViewer( streamViewer );
        auto* streamViewer = *it;
        m_streamViewers.erase( it++ );
        //        it = m_streamViewers.erase(it);
        delete streamViewer;
    }
    m_streamViewers.clear();

    m_mtxSyncViewers.lock();
    auto syncViewers = m_syncViewers;
    for ( auto& pair : syncViewers ) {
        auto syncViewers = pair.second;
        auto it          = syncViewers.begin();
        while ( it != syncViewers.end() ) {
            //        for ( auto* syncViewer : syncViewers ) {
            auto* syncViewer = *it;
            syncViewers.erase( it++ );
            //            m_server.delStreamViewer( syncViewer );
            delete syncViewer;
        }
    }
    m_syncViewers.clear();
    m_mtxSyncViewers.unlock();

    m_syncAcqs.clear();
    m_isSyncthing.clear();

    m_server.delStreamer( this );
    printStatusMessage( "del streamer" );

    s_mtxCout.unlock();

    //    std::cout << headerMsg() << "streamer : '" << m_streamName << "' deleted" << std::endl;
    //    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
    //              << "del streamer" << m_server.getStatus() << std::endl;
    //    std::cout << "-------------------------------------------------------------------------"
    //                 "--------------------"
    //              << std::endl;

    //    m_mtx.unlock();
}

std::string StreamerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamerClient] ";
}

const hub::InputSensor& StreamerClient::getInputSensor() const {
    return *m_inputSensor.get();
}

void StreamerClient::addStreamViewer( StreamViewerClient* streamViewer ) {
    assert( std::find( m_streamViewers.begin(), m_streamViewers.end(), streamViewer ) ==
            m_streamViewers.end() );
    m_streamViewers.push_back( streamViewer );
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
        std::cout << headerMsg() << "save new acq : [ ";
        for ( const auto& [streamName, saveAcqs] : m_streamName2saveAcqs ) {
            std::cout << "'" << streamName << "':" << saveAcqs.size() << ", ";
        }
        std::cout << " ] ";
        std::cout << std::endl;
        saveAcqs[newAcq.m_start] = newAcqPtr;
    }
    // acq already saved
    //    else {}

    auto& lastAcq = m_lastAcq[streamName];
    if ( lastAcq.get() != newAcqPtr.get() ) { lastAcq = newAcqPtr; }
    m_lastUpdateAcqDate[streamName] = std::chrono::high_resolution_clock::now();
}

const std::string &StreamerClient::getParent() const
{
    return m_parent;
}

const std::list<StreamViewerClient*>& StreamerClient::getStreamViewers() const {
    return m_streamViewers;
}

const std::chrono::time_point<std::chrono::high_resolution_clock>&
StreamerClient::getLastUpdateAcqDate( const std::string& streamName ) const {
    assert( m_lastUpdateAcqDate.find( streamName ) != m_lastUpdateAcqDate.end() );
    return m_lastUpdateAcqDate.at( streamName );
}
