
#include "StreamerClient.hpp"
#include "Server.hpp"
#include <cstring>
#include <iomanip>

namespace hub {
namespace server {

class InputStream : public hub::io::Input
{
  public:
    explicit InputStream( hub::net::ClientSocket&& clientSocket ) :
        m_clientSocket( std::move( clientSocket ) ) {}

  protected:
    void read( unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;
    bool isEnd() const override;

  private:
    hub::net::ClientSocket m_clientSocket;
};

void InputStream::read( unsigned char* data, size_t len ) {
    m_clientSocket.read( data, len );
}

void InputStream::close() {
    m_clientSocket.close();
}

bool InputStream::isOpen() const {
    return m_clientSocket.isOpen();
}

bool InputStream::isEnd() const {
    return m_clientSocket.isEnd();
}

//////////////////////////////////////////////////////////////////////////////

StreamerClient::StreamerClient( Server* server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ) {

    sock.read( m_streamName );
    assert( m_server != nullptr );
    const auto& streamers = m_server->getStreamers();
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
        m_inputSensor = std::make_unique<hub::InputSensor>( InputStream( std::move( sock ) ) );
    }
    catch ( hub::net::Socket::exception& e ) {
        std::cout << headerMsg() << "InputSensor() : catch exception : " << e.what() << std::endl;
        return;
    }

    assert( streamers.find( m_streamName ) == streamers.end() );

    std::cout << headerMsg() << "stream name = '" << m_streamName << "'" << std::endl;

    const auto& sensorSpec = m_inputSensor->getSpec();

    const size_t acquisitionSize = sensorSpec.getAcquisitionSize();
    std::cout << headerMsg() << "sensor name:'" << sensorSpec.getSensorName() << "'" << std::endl;
    std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
    std::cout << headerMsg() << "resolutions:" << sensorSpec.getResolutions() << std::endl;

    const auto& metaData = sensorSpec.getMetaData();
    for ( const auto& pair : metaData ) {
        std::cout << headerMsg() << "metaData: " << hub::SensorSpec::to_string( pair ) << std::endl;
    }
//    if ( metaData.find( "type" ) != metaData.end() ) {
//        std::cout << headerMsg() << "type detected : record stream" << std::endl;
//        const char* type = metaData.at( "type" ).getConstCharPtr();
//        if ( strcmp( type, "record" ) == 0 ) { m_isRecordStream = true; }
//    }
//    if ( metaData.find( "parent" ) != metaData.end() ) {
//        m_parent = metaData.at( "parent" ).getConstCharPtr();
//        std::cout << headerMsg() << "parent : '" << m_parent << "'" << std::endl;
//    }

//    m_lastAcq[""];
    // get record acqs before prevent viewer
    // std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    assert( m_server != nullptr );
    m_server->addStreamer( this );

    m_thread = std::thread( [this]() {
        try {
//            long long lastMasterAcqStart = 0;

//            while ( m_server != nullptr ) {
//            while ( true ) {
            while ( m_inputSensor->getInput().isOpen() ) {

//                auto acq           = m_inputSensor->getAcquisition();
                m_lastAcq = m_inputSensor->getAcquisition();
//                std::cout << "[StreamerClient] get acq : " << acq << std::endl;
                m_server->newAcquisition( this, m_lastAcq );


            } // while (true)
        }
        catch ( hub::net::Socket::exception& ex ) {
//            s_mtxCout.lock();
//            std::cout << headerMsg() << "catch exception : " << ex.what()
//                      << std::endl;
        }
//        std::cout << headerMsg() << "end streamer : '" << m_streamName << "'" << std::endl;
//        std::cout << headerMsg() << "thread end" << std::endl;

//        s_mtxCout.unlock();
        std::thread( [this]() { delete this; } ).detach();
    } );


//    printStatusMessage( "new streamer" );
}

StreamerClient::~StreamerClient() {
//    s_mtxCout.lock();

    assert( m_thread.joinable() );
    m_thread.join();

//    m_streamName2saveAcqs.clear();

//    m_mtxStreamViewers.lock();
//    auto it = m_streamViewers.begin();
//    while ( it != m_streamViewers.end() ) {
//        auto* streamViewer = *it;
//        it                 = m_streamViewers.erase( it );
//        delete streamViewer;
//    }
//    m_streamViewers.clear();
//    m_mtxStreamViewers.unlock();

//    m_mtxSyncViewers.lock();
//    auto syncViewers = m_syncViewers;
//    for ( const auto& pair : syncViewers ) {
//        auto syncViewers2 = pair.second;
//        auto it2          = syncViewers2.begin();
//        while ( it2 != syncViewers2.end() ) {
//            auto* syncViewer = *it2;
//            it2              = syncViewers2.erase( it2 );
//            delete syncViewer;
//        }
//    }
//    m_syncViewers.clear();
//    m_mtxSyncViewers.unlock();

//    m_mtxSyncAcqs.lock();
//    m_syncAcqs.clear();
//    m_mtxSyncAcqs.unlock();

//    m_isSyncthing.clear();
//    m_isLooping.clear();

    if ( m_server != nullptr ) m_server->delStreamer( this );

//    m_server->delStreamer(this);
//    printStatusMessage( "del streamer" );

//    s_mtxCout.unlock();
}

std::string StreamerClient::headerMsg() const {
    return Client::headerMsg() + "[Streamer] ";
}

const hub::InputSensor& StreamerClient::getInputSensor() const {
    return *m_inputSensor.get();
}

void StreamerClient::end()
{
//    m_inputSensor->getInput().close();
}

//void StreamerClient::addStreamViewer( StreamViewerClient* streamViewer ) {

//    const auto& streamName = streamViewer->getStreamName();
//    assert( m_streamName == streamName );
//    const auto& syncStreamName = streamViewer->getSyncStreamName();

//    if ( syncStreamName == "" ) {
//        m_mtxStreamViewers.lock();
//        assert( std::find( m_streamViewers.begin(), m_streamViewers.end(), streamViewer ) ==
//                m_streamViewers.end() );
//        m_streamViewers.push_back( streamViewer );
//        m_mtxStreamViewers.unlock();
//    }
//    else {

//        m_mtxSyncViewers.lock();
//        auto& syncViewers = m_syncViewers[syncStreamName];
//        assert( std::find( syncViewers.begin(), syncViewers.end(), streamViewer ) ==
//                syncViewers.end() );

//        if ( syncViewers.empty() ) {
//            m_syncAcqs[syncStreamName];
//            m_isSyncthing[syncStreamName] = false;
//            m_isLooping[syncStreamName]   = false;
//            m_streamName2saveAcqs[syncStreamName];
//            m_lastAcq[syncStreamName];
//        }
//        syncViewers.push_back( streamViewer );
//        m_mtxSyncViewers.unlock();
//    }
//}

// void StreamerClient::addSyncStreamViewer( StreamViewerClient* syncStreamViewer ) {

//}

//void StreamerClient::delStreamViewer( StreamViewerClient* streamViewer ) {

//    const auto& streamName = streamViewer->getStreamName();
//    assert( m_streamName == streamName );
//    const auto& syncStreamName = streamViewer->getSyncStreamName();

//    if ( syncStreamName == "" ) {
//        m_mtxStreamViewers.lock();
//        const auto startSize = m_streamViewers.size();
//        auto& streamViewers = m_streamViewers;
//        // todo fix assert
//        assert( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) !=
//                streamViewers.end() );
//        streamViewers.remove( streamViewer );
//        const auto endSize = m_streamViewers.size();
//        assert(startSize - endSize == 1);
//        m_mtxStreamViewers.unlock();
//    }
//    else {

//        m_mtxSyncViewers.lock();
//        auto& syncViewers = m_syncViewers.at( syncStreamName );
//        assert( std::find( syncViewers.begin(), syncViewers.end(), streamViewer ) !=
//                syncViewers.end() );

//        syncViewers.remove( streamViewer );
//        if ( syncViewers.empty() ) {
//            m_syncAcqs.erase( syncStreamName );
//            m_isSyncthing.erase( syncStreamName );
//            m_isLooping.erase( syncStreamName );

//            m_syncViewers.erase( syncStreamName );
//        }
//        m_mtxSyncViewers.unlock();
//    }
//}

//void StreamerClient::newAcquisition( const std::string& streamerName,
//                                     const hub::Acquisition& acq ) {

//    // no conflict queue

//    if ( m_syncViewers.find( streamerName ) != m_syncViewers.end() ) {
//        m_mtxSyncAcqs.lock();
//        m_syncAcqs[streamerName].emplace_back( acq.clone() );
//        m_mtxSyncAcqs.unlock();
//    }
//}

//const std::string& StreamerClient::getStreamName() const {
//    return m_streamName;
//}

//const std::map<std::string, std::list<StreamViewerClient*>>&
//StreamerClient::getSyncViewers() const {
//    return m_syncViewers;
//}

//const std::shared_ptr<hub::Acquisition>
//StreamerClient::getLastAcq( const std::string& streamName ) const {
//    assert( m_lastAcq.find( streamName ) != m_lastAcq.end() );
//    return m_lastAcq.at( streamName );
//}

//const std::map<long long, std::shared_ptr<hub::Acquisition>>&
//StreamerClient::getSaveAcqs( const std::string& streamName ) const {
//    assert( m_streamName2saveAcqs.find( streamName ) != m_streamName2saveAcqs.end() );
//    return m_streamName2saveAcqs.at( streamName );
//}

//void StreamerClient::saveNewAcq( const std::string& streamName, hub::Acquisition&& newAcq ) {
//    assert( m_isRecordStream );

//    auto newAcqPtr = std::make_shared<hub::Acquisition>( std::move( newAcq ) );

//    auto& saveAcqs = m_streamName2saveAcqs[streamName];
//    // save new acq
//    if ( saveAcqs.find( newAcq.getStart() ) == saveAcqs.end() ) {
//        std::cout << headerMsg() << "'" << m_streamName << "' save new acq : [ ";
//#if ( __cplusplus >= 201703L )
//        for ( const auto& [streamName2, saveAcqs2] : m_streamName2saveAcqs ) {
//#else
//        for ( const auto& pair : m_streamName2saveAcqs ) {
//            const auto & streamName2 = pair.first;
//            const auto & saveAcqs2 = pair.second;
//#endif
//            std::cout << "'" << streamName2 << "':" << saveAcqs2.size() << ", ";
//        }
//        std::cout << " ] ";
//        std::cout << std::endl;
//        saveAcqs[newAcq.getStart()] = newAcqPtr;
//    }
//    // acq already saved

//    auto& lastAcq = m_lastAcq[streamName];
//    if ( lastAcq.get() != newAcqPtr.get() ) { lastAcq = newAcqPtr; }
//}

//bool StreamerClient::isRecordStream() const {
//    return m_isRecordStream;
//}

//const std::string& StreamerClient::getParent() const {
//    return m_parent;
//}

//const std::list<StreamViewerClient*>& StreamerClient::getStreamViewers() const {
//    return m_streamViewers;
//}

// const std::chrono::time_point<std::chrono::high_resolution_clock>&
// StreamerClient::getLastUpdateAcqDate( const std::string& streamName ) const {
// }

} // server
} // hub

//                const bool masterAcqLooped = lastMasterAcqStart > masterAcq.getStart();
//                lastMasterAcqStart         = masterAcq.getStart();

//                // broadcast acquisition for each stream viewers
//                m_mtxStreamViewers.lock();
//                auto& streamViewers = m_streamViewers;
//                auto it             = streamViewers.begin();
//                while ( it != streamViewers.end() ) {
//                    auto& streamViewer = *it++;
//                    assert( streamViewer->getSyncStreamName() == "" );
//                    m_mtxStreamViewers.unlock();
//                    streamViewer->update( masterAcq );
//                    m_mtxStreamViewers.lock();
//                }
//                m_mtxStreamViewers.unlock();

//                // receive reset acq
//                if ( masterAcq.getStart() == 0 && masterAcq.getEnd() == 0 ) {
//                    std::cout << headerMsg() << "receive (history) reset acq : " << masterAcq
//                              << std::endl;
//                    m_lastAcq[""].reset();
//                    m_streamName2saveAcqs[""].clear();
//                    assert( m_server != nullptr );
//                    m_server->newAcquisition( this, masterAcq );
//                    continue;
//                }

//                // broadcast acquisition for each sync stream viewers
//                auto it2 = m_syncViewers.begin();
//                while ( it2 != m_syncViewers.end() ) {
//#if ( __cplusplus >= 201703L )
//                    auto& [syncViewerName, syncViewers] = *it2++;
//#else
//                    auto & syncViewerName = it2->first;
//                    auto & syncViewers = it2->second;
//#endif

//                    auto& syncAcqs = m_syncAcqs.at( syncViewerName );

//                    bool foundLeft    = false;
//                    bool& isSyncthing = m_isSyncthing.at( syncViewerName );
//                    bool& isLooping   = m_isLooping.at( syncViewerName );

//                    // sync acq looping
//                    if ( isLooping ) {
//                        assert( !isSyncthing );
//                        if ( masterAcqLooped ) {
//                            std::cout << "[StreamerClient] master acq loop detected, stop looping"
//                                      << std::endl;
//                            isLooping = false;
//                        }
//                        else {
//                            std::cout << "[StreamerClient] searching master acq loop" << std::endl;
//                            break;
//                        }
//                    }
//                    if ( masterAcqLooped ) {
//                        isLooping   = true;
//                        isSyncthing = false;
//                        std::cout << "[StreamerClient] master acq loop deteted, searching for "
//                                     "sync acq loop"
//                                  << std::endl;
//                    }

//                    do {
//                        m_mtxSyncAcqs.lock();
//                        while ( syncAcqs.size() < 2 ) {
//                            m_mtxSyncAcqs.unlock();
//                            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
//                            m_mtxSyncAcqs.lock();
//                        }
//                        assert( syncAcqs.size() >= 2 );
//                        m_mtxSyncAcqs.unlock();

//                        // do not synchronize ping acq
//                        if ( syncAcqs.begin()->getStart() == std::next( syncAcqs.begin() )->getStart() ) {
//                            std::cout << "[StreamerClient] receive ping acq" << std::endl;
//                            syncAcqs.pop_front();
//                            continue;
//                        }
//                        assert( syncAcqs.begin()->getStart() !=
//                                std::next( syncAcqs.begin() )->getStart() );

//                        // autoloop detected (player)
//                        if ( syncAcqs.begin()->getStart() >= std::next( syncAcqs.begin() )->getStart() ) {
//                            std::cout << "[StreamerClient] sync acq loop deteted" << std::endl;
//                            syncAcqs.pop_front();

//                            if ( isLooping ) {
//                                assert( !isSyncthing );
//                                assert( masterAcqLooped );
//                                std::cout << "[StreamerClient] stop looping, sync acq loop found"
//                                          << std::endl;
//                                isLooping = false;
//                                break;
//                            }
//                            else {
//                                assert( !masterAcqLooped );
//                                assert( !isLooping );
//                                std::cout << "[StreamerClient] start looping, sync acq loop found"
//                                          << std::endl;
//                                isLooping   = true;
//                                isSyncthing = false;
//                                break;
//                            }
//                        }
//                        assert( syncAcqs.begin()->getStart() <
//                                std::next( syncAcqs.begin() )->getStart() );

//                        if ( isLooping ) {
//                            std::cout
//                                << "[StreamerClient] continue looping, sync acq loop not found"
//                                << std::endl;
//                            syncAcqs.pop_front();
//                            continue;
//                        }

//                        // unable to synchronize master acq, these acquisitions are too far in the
//                        // future compare to acq
//                        if ( syncAcqs.begin()->getStart() >= masterAcq.getStart() ) {
//                            std::cout << "[StreamerClient] unable to sync future acqs, "
//                                         "syncAcq.start - masterAcq = "
//                                      << syncAcqs.begin()->getStart() - masterAcq.getStart()
//                                      << ", syncAcq : " << *syncAcqs.begin()
//                                      << ", masterAcq : " << masterAcq << std::endl;

//                            if ( !isSyncthing ) { break; }
//                            syncAcqs.pop_front();
//                            continue;
//                        }
//                        assert( syncAcqs.begin()->getStart() < masterAcq.getStart() );

//                        // unable to synchronize master acq, these acquisitions are too far in the
//                        // past compare to master acq
//                        if ( std::next( syncAcqs.begin() )->getStart() <= masterAcq.getStart() ) {
//                            syncAcqs.pop_front();
//                            continue;
//                        }
//                        assert( std::next( syncAcqs.begin() )->getStart() > masterAcq.getStart() );

//                        foundLeft = true;

//                        if ( !isSyncthing ) {
//                            std::cout << "[StreamerClient] syncthing" << std::endl;
//                            isSyncthing = true;
//                        }

//                    } while ( false );

//                    if ( !foundLeft ) continue;

//                    const auto itLeftAcq  = syncAcqs.begin();
//                    const auto itRightAcq = std::next( itLeftAcq );

//                    assert( itLeftAcq->getStart() < masterAcq.getStart() );
//                    assert( itRightAcq->getStart() >= masterAcq.getStart() );

//                    const auto closestAcq = ( std::abs( itLeftAcq->getStart() - masterAcq.getStart() ) >
//                                              std::abs( itRightAcq->getStart() - masterAcq.getStart() ) )
//                                                ? ( itRightAcq )
//                                                : ( itLeftAcq );

//                    const auto dist = std::abs( closestAcq->getStart() - masterAcq.getStart() );

//                    // assert(minDist < 20'000); // 20 ms
//                    // if too far then abort synchronize
//                    // consider constant period of acquistion rate
//                    auto maxDist = ( itRightAcq->getStart() - itLeftAcq->getStart() ) / 2;

//                    // find acceptable corresponding acquisition if interpolation is not possible
//                    if ( !itLeftAcq->isInterpolable() ) {
//                        const auto& lastMasterAcq = getLastAcq( syncViewerName );
//                        if ( lastMasterAcq != nullptr ) {
//                            maxDist = std::min(
//                                dist, std::abs( lastMasterAcq->getStart() - masterAcq.getStart() ) );
//                        }
//                    }
//                    if ( dist > maxDist ) {
//                        std::cout << headerMsg() << "sync dist = abs( "
//                                  << closestAcq->getStart() - masterAcq.getStart() << " ) > " << maxDist
//                                  << std::endl;
//                        continue;
//                    }

//                    assert( masterAcq.getMeasures().size() == 1 );
//                    assert( closestAcq->getMeasures().size() == 1 );

//                    hub::Acquisition mergedAcq = masterAcq.clone();
//                    if ( itLeftAcq->isInterpolable() ) {
//                        const double t = ( masterAcq.getStart() - itLeftAcq->getStart() ) /
//                                         (double)( itRightAcq->getStart() - itLeftAcq->getStart() );
//                        mergedAcq
//                            << hub::Acquisition::slerp( *itLeftAcq, *itRightAcq, t ).getMeasures();
//                    }
//                    else {
//                        mergedAcq << closestAcq->getMeasures();
//                    }

//                    assert( !syncViewers.empty() );
//                    int nMergedSyncViewers = 0;
//                    m_mtxSyncViewers.lock();
//                    assert( m_syncViewers.find( syncViewerName ) != m_syncViewers.end() );

//                    auto it3 = syncViewers.begin();
//                    while ( it3 != syncViewers.end() ) {

//                        auto& syncViewer = *it3++;
//                        m_mtxSyncViewers.unlock();
//                        if ( syncViewer->shoudMergeSyncAcqs() ) {
//                            const auto updated = syncViewer->update( mergedAcq );
//                            if ( updated ) { ++nMergedSyncViewers; }
//                            else {
//                                // todo : check last failed viewer
//                                m_mtxSyncViewers.lock();
//                                break;
//                            }
//                        }
//                        else {
//                            assert( false );
//                            syncViewer->update( masterAcq );
//                        }
//                        m_mtxSyncViewers.lock();
//                    }
//                    m_mtxSyncViewers.unlock();

//                    if ( m_isRecordStream && nMergedSyncViewers > 0 ) {
//                        saveNewAcq( syncViewerName, std::move( mergedAcq ) );
//                    }

//                } // for ( auto& pair : syncViewers )

//                if ( m_server != nullptr ) m_server->newAcquisition( this, masterAcq );

//                if ( !m_isRecordStream ) {
//                    m_lastAcq[""] = std::make_shared<hub::Acquisition>( std::move( masterAcq ) );
//                }
//                else {
//                    saveNewAcq( "", std::move( masterAcq ) );
//                }
