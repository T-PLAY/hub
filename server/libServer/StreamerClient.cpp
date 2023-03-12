
#include "StreamerClient.hpp"
#include "Server.hpp"
#include <cstring>
// #include <iostream>
// #include <string>
#include <iomanip>
// #include <iterator>

// std::mutex StreamerClient::s_mtxCout;

class InputStream : public hub::io::Input
{
  public:
    explicit InputStream( hub::net::ClientSocket&& clientSocket ) :
        //        hub::net::ClientSocket( std::move( clientSocket ) )
        m_clientSocket( std::move( clientSocket ) ) {}

    //#ifdef WIN32 // msvc warning C4250
  protected:
    //    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;
    bool isEnd() const override;
    //#endif
  private:
    hub::net::ClientSocket m_clientSocket;
};

//#ifdef WIN32 // msvc warning C4250
// void InputStream::write( const unsigned char* data, size_t len ) const {
////    hub::net::ClientSocket::write( data, len );
//    m_clientSocket.write(data, len);
//}

void InputStream::read( unsigned char* data, size_t len ) {
    //    hub::net::ClientSocket::read( data, len );
    m_clientSocket.read( data, len );
}

void InputStream::close() {
    //    hub::net::ClientSocket::close();
    m_clientSocket.close();
}

bool InputStream::isOpen() const {
    //    return hub::net::ClientSocket::isOpen();
    return m_clientSocket.isOpen();
}

bool InputStream::isEnd() const {
    //    return hub::net::ClientSocket::isEnd();
    return m_clientSocket.isEnd();
}
//#endif

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
        m_inputSensor = std::make_unique<hub::InputSensor>( InputStream( std::move( sock ) ) );
    }
    catch ( hub::net::Socket::exception& e ) {
        std::cout << headerMsg() << "InputSensor() : catch exception : " << e.what() << std::endl;
        return;
    }

    assert( streamers.find( m_streamName ) == streamers.end() );

    std::cout << headerMsg() << "stream name = '" << m_streamName << "'" << std::endl;

    const auto& sensorSpec = m_inputSensor->m_spec;

    const size_t acquisitionSize = sensorSpec.getAcquisitionSize();
    std::cout << headerMsg() << "sensor name:'" << sensorSpec.getSensorName() << "'" << std::endl;
    std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
    std::cout
        << headerMsg()
        //              << "resolutions:" << hub::resolutions2string( sensorSpec.getResolutions() )
        << "resolutions:" << sensorSpec.getResolutions() << std::endl;

    const auto& metaData = sensorSpec.getMetaData();
    for ( const auto& pair : metaData ) {
        std::cout << headerMsg() << "metaData: " << hub::SensorSpec::to_string( pair ) << std::endl;
    }
    if ( metaData.find( "type" ) != metaData.end() ) {
        std::cout << headerMsg() << "type detected : record stream" << std::endl;
        // todo any
//        const char* type = std::any_cast<const char*>( metaData.at( "type" ) );
        const char* type = metaData.at("type").getConstCharPtr();
        if ( strcmp( type, "record" ) == 0 ) { m_isRecordStream = true; }
    }
    if ( metaData.find( "parent" ) != metaData.end() ) {
        // todo any
//        m_parent = std::any_cast<const char*>( metaData.at( "parent" ) );
        m_parent = metaData.at("parent").getConstCharPtr();
        std::cout << headerMsg() << "parent : '" << m_parent << "'" << std::endl;
    }

    m_lastAcq[""];

    m_thread = std::thread( [this]() {
        try {
            long long lastMasterAcqStart = 0;

            while ( true ) {

                auto&& masterAcq = m_inputSensor->getAcquisition();
                //                std::cout << "[StreamerClient] receive master acq : " << masterAcq
                //                << std::endl;
                const bool masterAcqLooped = lastMasterAcqStart > masterAcq.m_start;
                lastMasterAcqStart         = masterAcq.m_start;

                // broadcast acquisition for each stream viewers
                m_mtxStreamViewers.lock();
                auto& streamViewers = m_streamViewers;
                auto it             = streamViewers.begin();
                //                for ( const auto& streamViewer : streamViewers ) {
                while ( it != streamViewers.end() ) {
                    auto& streamViewer = *it++;
                    assert( streamViewer->getSyncStreamName() == "" );
                    m_mtxStreamViewers.unlock();
                    streamViewer->update( masterAcq );
                    m_mtxStreamViewers.lock();
                }
                m_mtxStreamViewers.unlock();

                // receive reset acq
                if ( masterAcq.m_start == 0 && masterAcq.m_end == 0 ) {
                    std::cout << headerMsg() << "receive (history) reset acq : " << masterAcq
                              << std::endl;
                    m_lastAcq[""].reset();
                    m_streamName2saveAcqs[""].clear();
                    m_server.newAcquisition( this, masterAcq );
                    continue;
                }

                // broadcast acquisition for each sync stream viewers
                //                m_mtxSyncViewers.lock();
                //                auto& syncViewers = m_syncViewers;
                auto it2 = m_syncViewers.begin();
                while ( it2 != m_syncViewers.end() ) {
                    //                for ( auto& [syncViewerName, syncViewers] : m_syncViewers ) {
                    auto& [syncViewerName, syncViewers] = *it2++;
                    //                    const auto& syncViewerName = pair.first;

                    auto& syncAcqs = m_syncAcqs.at( syncViewerName );

                    bool foundLeft    = false;
                    bool& isSyncthing = m_isSyncthing.at( syncViewerName );
                    bool& isLooping   = m_isLooping.at( syncViewerName );

                    //                    if ( isSyncthing || isLooping ) {
                    //                    if (! isLooping) {
                    //                    const auto& lastMasterAcq = getLastAcq( syncViewerName );
                    //                    if ( lastMasterAcq != nullptr ) {
                    //                        masterAcqLooped = lastMasterAcq->m_start >
                    //                        masterAcq.m_start;
                    //                            isLooping = true;
                    //                        isSyncthing = false;
                    //                        assert(! isLooping);
                    //                    }
                    //                    }

                    // sync acq looping
                    if ( isLooping ) {
                        assert( !isSyncthing );
                        if ( masterAcqLooped ) {
                            std::cout << "[StreamerClient] master acq loop detected, stop looping"
                                      << std::endl;
                            isLooping = false;
                        }
                        else {
                            //							    assert( m_lastAcq.find( syncViewerName )
                            //!= m_lastAcq.end() );
                            //    						  auto & lastAcq =
                            //    m_lastAcq.at(syncViewerName);
                            //                            m_lastAcq[syncViewerName] =
                            //                                std::make_shared<hub::Acquisition>(
                            //                                masterAcq.clone() );
                            //                            m_lastAcq[syncViewerName] =
                            //                            std::make_shared<hub::Acquisition>(
                            //                                masterAcq.clone() );
                            //                                hub::Acquisition { masterAcq.m_start,
                            //                                masterAcq.m_end } );
                            //                            assert(! isSyncthing);
                            std::cout << "[StreamerClient] searching master acq loop" << std::endl;
                            break;
                        }
                        //					    assert(! isSyncthing);
                    }
                    //                    else {
                    if ( masterAcqLooped ) {
                        //                            isSyncthing = false;
                        isLooping   = true;
                        isSyncthing = false;
                        std::cout << "[StreamerClient] master acq loop deteted, searching for "
                                     "sync acq loop"
                                  << std::endl;
                    }
                    //                    }
                    //                }

                    do {
                        m_mtxSyncAcqs.lock();
                        while ( syncAcqs.size() < 2 ) {
                            m_mtxSyncAcqs.unlock();
                            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
                            m_mtxSyncAcqs.lock();
                        }
                        assert( syncAcqs.size() >= 2 );
                        m_mtxSyncAcqs.unlock();

                        //        m_mtxSyncAcqs.lock();

                        // do not synchronize ping acq
                        if ( syncAcqs.begin()->m_start == std::next( syncAcqs.begin() )->m_start ) {
                            std::cout << "[StreamerClient] receive ping acq" << std::endl;
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start !=
                                std::next( syncAcqs.begin() )->m_start );

                        // autoloop detected (player)
                        if ( syncAcqs.begin()->m_start >= std::next( syncAcqs.begin() )->m_start ) {
                            std::cout << "[StreamerClient] sync acq loop deteted" << std::endl;
                            syncAcqs.pop_front();

                            if ( isLooping ) {
                                assert( !isSyncthing );
                                assert( masterAcqLooped );
                                //                                m_lastAcq[syncViewerName] =
                                //                                    std::make_shared<hub::Acquisition>(
                                //                                    masterAcq.clone() );
                                //                                m_lastAcq[syncViewerName] =
                                //                                std::make_shared<hub::Acquisition>(
                                //                                            masterAcq.clone());
                                //                                    hub::Acquisition {
                                //                                    masterAcq.m_start,
                                //                                    masterAcq.m_end } );
                                std::cout << "[StreamerClient] stop looping, sync acq loop found"
                                          << std::endl;
                                isLooping = false;
                                break;
                            }
                            else {
                                assert( !masterAcqLooped );
                                assert( !isLooping );
                                //                                m_lastAcq[syncViewerName] =
                                //                                std::make_shared<hub::Acquisition>(
                                //                                    hub::Acquisition {
                                //                                    masterAcq.m_start,
                                //                                    masterAcq.m_end } );
                                //                    hub::Acquisition mergedAcq {
                                //                    masterAcq.m_start, masterAcq.m_end };
                                std::cout << "[StreamerClient] start looping, sync acq loop found"
                                          << std::endl;
                                //                                isSyncthing = false;
                                isLooping   = true;
                                isSyncthing = false;
                                break;
                            }
                            //                            continue;
                        }
                        assert( syncAcqs.begin()->m_start <
                                std::next( syncAcqs.begin() )->m_start );

                        if ( isLooping ) {
                            std::cout
                                << "[StreamerClient] continue looping, sync acq loop not found"
                                << std::endl;
                            syncAcqs.pop_front();
                            continue;
                        }

                        // unable to synchronize master acq, these acquisitions are too far in the
                        // future compare to acq
                        if ( syncAcqs.begin()->m_start >= masterAcq.m_start ) {
                            std::cout << "[StreamerClient] unable to sync future acqs, "
                                         "syncAcq.start - masterAcq = "
                                      << syncAcqs.begin()->m_start - masterAcq.m_start
                                      << ", syncAcq : " << *syncAcqs.begin()
                                      << ", masterAcq : " << masterAcq << std::endl;
                            //                            if ( isLooping ) {
                            //                                break;
                            //                            } // no match possible after loop with
                            //                            oldest master acq
                            if ( !isSyncthing ) { break; }
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start < masterAcq.m_start );

                        // unable to synchronize master acq, these acquisitions are too far in the
                        // past compare to master acq
                        if ( std::next( syncAcqs.begin() )->m_start <= masterAcq.m_start ) {
                            //                            std::cout << "[StreamerClient] unable to
                            //                            sync past acqs "
                            //                                      << syncAcqs.begin()->m_start -
                            //                                      masterAcq.m_start << std::endl;

                            //                            if ( isLooping && isSyncthing ) { break; }
                            //                                                        if (
                            //                                                        !isSyncthing )
                            //                                                        { break; }
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( std::next( syncAcqs.begin() )->m_start > masterAcq.m_start );

                        foundLeft = true;
                        //                        if ( isLooping ) { isLooping = false; }
                        if ( !isSyncthing ) {
                            std::cout << "[StreamerClient] syncthing" << std::endl;
                            isSyncthing = true;
                        }

                        //                    } while ( !foundLeft );
                    } while ( false );

                    if ( !foundLeft ) continue;
                    //                    std::cout << "[StreamerClient] found corresponding" <<
                    //                    std::endl;

                    const auto itLeftAcq  = syncAcqs.begin();
                    const auto itRightAcq = std::next( itLeftAcq );

                    assert( itLeftAcq->m_start < masterAcq.m_start );
                    assert( itRightAcq->m_start >= masterAcq.m_start );

                    const auto closestAcq = ( std::abs( itLeftAcq->m_start - masterAcq.m_start ) >
                                              std::abs( itRightAcq->m_start - masterAcq.m_start ) )
                                                ? ( itRightAcq )
                                                : ( itLeftAcq );

                    const auto dist = std::abs( closestAcq->m_start - masterAcq.m_start );

                    // assert(minDist < 20'000); // 20 ms
                    // if too far then abort synchronize
                    // consider constant period of acquistion rate
                    auto maxDist = ( itRightAcq->m_start - itLeftAcq->m_start ) / 2;
                    //                    if ( m_lastAcq.find( syncViewerName ) != m_lastAcq.end() )
                    //                    {
                    // find acceptable corresponding acquisition if interpolation is not possible
                    if ( !itLeftAcq->isInterpolable() ) {
                        const auto& lastMasterAcq = getLastAcq( syncViewerName );
                        if ( lastMasterAcq != nullptr ) {
                            maxDist = std::min(
                                dist, std::abs( lastMasterAcq->m_start - masterAcq.m_start ) );
                        }
                    }
                    //                    }
                    if ( dist > maxDist ) {
                        std::cout << headerMsg() << "sync dist = abs( "
                                  << closestAcq->m_start - masterAcq.m_start << " ) > " << maxDist
                                  << std::endl;
                        continue;
                    }

                    assert( masterAcq.getMeasures().size() == 1 );
                    assert( closestAcq->getMeasures().size() == 1 );

                    //                    hub::Acquisition mergedAcq { masterAcq.m_start,
                    //                    masterAcq.m_end };
                    hub::Acquisition mergedAcq = masterAcq.clone();
                    if ( itLeftAcq->isInterpolable() ) {
                        const double t = ( masterAcq.m_start - itLeftAcq->m_start ) /
                                         (double)( itRightAcq->m_start - itLeftAcq->m_start );
                        mergedAcq
                            << hub::Acquisition::slerp( *itLeftAcq, *itRightAcq, t ).getMeasures();
                    }
                    else {
                        mergedAcq << closestAcq->getMeasures();
                    }
                    //                    mergedAcq << masterAcq.getMeasures();

                    //                    const auto syncViewers = pair.second;
                    assert( !syncViewers.empty() );
                    int nMergedSyncViewers = 0;
                    m_mtxSyncViewers.lock();
                    assert( m_syncViewers.find( syncViewerName ) != m_syncViewers.end() );

                    auto it3 = syncViewers.begin();
                    while ( it3 != syncViewers.end() ) {

                        //                    auto it3 = m_syncViewers.at(syncViewerName).begin();
                        //                    while (it3 != m_syncViewers.at(syncViewerName).end())
                        //                    {

                        //                    for ( auto& syncViewer : syncViewers ) {
                        auto& syncViewer = *it3++;
                        m_mtxSyncViewers.unlock();
                        if ( syncViewer->shoudMergeSyncAcqs() ) {
                            const auto updated = syncViewer->update( mergedAcq );
                            //                            if ( syncViewer->update( mergedAcq ) )
                            //                            ++nMergedSyncViewers;
                            if ( updated ) { ++nMergedSyncViewers; }
                            else {
                                // todo : check last failed viewer
                                m_mtxSyncViewers.lock();
                                break;
                            }
                        }
                        else {
                            assert( false );
                            syncViewer->update( masterAcq );
                        }
                        m_mtxSyncViewers.lock();
                    }
                    m_mtxSyncViewers.unlock();

                    if ( m_isRecordStream && nMergedSyncViewers > 0 ) {
                        saveNewAcq( syncViewerName, std::move( mergedAcq ) );
                    }
                    //                    m_lastUpdateAcqDate[syncViewerName] =
                    //                    std::chrono::high_resolution_clock::now();

                } // for ( auto& pair : syncViewers )
                //                m_mtxSyncViewers.unlock();

                m_server.newAcquisition( this, masterAcq );

                //                if ( masterAcq.getMeasures().size() == 1 ) {
                if ( !m_isRecordStream ) {
                    //                    auto & lastAcq = m_lastAcq[""];
                    //                    lastAcq.reset();
                    //                    lastAcq. = std::move(masterAcq);
                    //                    lastAcq = std::make_shared<hub::Acquisition>( std::move(
                    //                    masterAcq ) );
                    m_lastAcq[""] = std::make_shared<hub::Acquisition>( std::move( masterAcq ) );
                }
                else {
                    saveNewAcq( "", std::move( masterAcq ) );
                }
                //                m_lastUpdateAcqDate[""] =
                //                std::chrono::high_resolution_clock::now();

            } // while (true)
        }
        catch ( hub::net::Socket::exception& e ) {
            //            m_guard = std::lock_guard<std::mutex>(s_mtxCout);
            s_mtxCout.lock();
            std::cout << headerMsg() << "in : catch inputSensor exception : " << e.what()
                      << std::endl;
            //            s_mtxCout.unlock();
        }
        std::cout << headerMsg() << "end streamer : '" << m_inputSensor->m_spec.getSensorName()
                  << "'" << std::endl;

        std::cout << headerMsg() << "thread end" << std::endl;
        //        s_mtxCout.unlock();

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
    s_mtxCout.lock();

    assert( m_thread.joinable() );
    m_thread.join();
    //    delete m_thread;
    //    m_thread = nullptr;

    //    m_server.delStreamer(this);

    m_streamName2saveAcqs.clear();

    m_mtxStreamViewers.lock();
    auto it = m_streamViewers.begin();
    while ( it != m_streamViewers.end() ) {
        //    for ( auto* streamViewer : m_streamViewers ) {
        //        m_server.delStreamViewer( streamViewer );
        auto* streamViewer = *it;
        it                 = m_streamViewers.erase( it );
        //        it = m_streamViewers.erase(it);
        delete streamViewer;
    }
    m_streamViewers.clear();
    m_mtxStreamViewers.unlock();

    m_mtxSyncViewers.lock();
    auto syncViewers = m_syncViewers;
    for ( const auto& pair : syncViewers ) {
        auto syncViewers2 = pair.second;
        auto it2          = syncViewers2.begin();
        while ( it2 != syncViewers2.end() ) {
            //        for ( auto* syncViewer : syncViewers ) {
            auto* syncViewer = *it2;
            it2              = syncViewers2.erase( it2 );
            //            m_server.delStreamViewer( syncViewer );
            delete syncViewer;
        }
    }
    m_syncViewers.clear();
    m_mtxSyncViewers.unlock();

    m_mtxSyncAcqs.lock();
    m_syncAcqs.clear();
    m_mtxSyncAcqs.unlock();

    m_isSyncthing.clear();
    m_isLooping.clear();

    m_server.delStreamer( this );
    printStatusMessage( "del streamer" );

    s_mtxCout.unlock();

    //    std::cout << headerMsg() << "streamer : '" << m_streamName << "' deleted" << std::endl;
    //    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
    //              << "del streamer" << m_server.getStatus() << std::endl;
    //    std::cout << "-------------------------------------------------------------------------"
    //                 "--------------------"
    //              << std::endl;
    //    m_mtxStreamViewers.lock();
    //    m_mtxSyncViewers.lock();
    //    m_mtxSyncAcqs.lock();

    //        m_mtx.unlock();
}

std::string StreamerClient::headerMsg() const {
    return Client::headerMsg() + "[Streamer] ";
}

const hub::InputSensor& StreamerClient::getInputSensor() const {
    return *m_inputSensor.get();
}

void StreamerClient::addStreamViewer( StreamViewerClient* streamViewer ) {

    const auto& streamName = streamViewer->getStreamName();
    assert( m_streamName == streamName );
    const auto& syncStreamName = streamViewer->getSyncStreamName();

    if ( syncStreamName == "" ) {
        m_mtxStreamViewers.lock();
        assert( std::find( m_streamViewers.begin(), m_streamViewers.end(), streamViewer ) ==
                m_streamViewers.end() );
        m_streamViewers.push_back( streamViewer );
        m_mtxStreamViewers.unlock();
    }
    else {

        m_mtxSyncViewers.lock();
        auto& syncViewers = m_syncViewers[syncStreamName];
        assert( std::find( syncViewers.begin(), syncViewers.end(), streamViewer ) ==
                syncViewers.end() );

        if ( syncViewers.empty() ) {
            m_syncAcqs[syncStreamName];
            m_isSyncthing[syncStreamName] = false;
            m_isLooping[syncStreamName]   = false;
            m_streamName2saveAcqs[syncStreamName];
            m_lastAcq[syncStreamName];
        }
        syncViewers.push_back( streamViewer );
        m_mtxSyncViewers.unlock();
    }
}

// void StreamerClient::addSyncStreamViewer( StreamViewerClient* syncStreamViewer ) {
//     const auto& streamName = syncStreamViewer->getStreamName();

//}

void StreamerClient::delStreamViewer( StreamViewerClient* streamViewer ) {

    //    const auto& streamName = streamViewer->getStreamName();
    const auto& streamName = streamViewer->getStreamName();
    assert( m_streamName == streamName );
    const auto& syncStreamName = streamViewer->getSyncStreamName();

    if ( syncStreamName == "" ) {
        //    m_mtxSyncViewers.lock();
        m_mtxStreamViewers.lock();
        auto& streamViewers = m_streamViewers;
        // todo fix assert
        assert( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) !=
                streamViewers.end() );
        streamViewers.remove( streamViewer );
        m_mtxStreamViewers.unlock();
    }
    else {

        m_mtxSyncViewers.lock();
        auto& syncViewers = m_syncViewers.at( syncStreamName );
        assert( std::find( syncViewers.begin(), syncViewers.end(), streamViewer ) !=
                syncViewers.end() );

        syncViewers.remove( streamViewer );
        if ( syncViewers.empty() ) {
            m_syncAcqs.erase( syncStreamName );
            m_isSyncthing.erase( syncStreamName );
            m_isLooping.erase( syncStreamName );
            //            m_streamName2saveAcqs.erase( syncStreamName );
            //            m_lastAcq.erase( syncStreamName );

            m_syncViewers.erase( syncStreamName );
        }
        m_mtxSyncViewers.unlock();
    }
    //    if ( streamViewers.empty() ) {
    //        m_syncViewers.erase( streamName );
    //        m_syncAcqs.erase( streamName );
    //        m_isSyncthing.erase( streamName );
    //        m_streamName2saveAcqs.erase( streamName );
    //        m_lastAcq.erase( streamName );
    //    }
    //    m_mtxSyncViewers.unlock();
}

void StreamerClient::newAcquisition( const std::string& streamerName,
                                     const hub::Acquisition& acq ) {

    // no conflict queue

    //        m_mtxSyncViewers.lock();
    if ( m_syncViewers.find( streamerName ) != m_syncViewers.end() ) {
        //        std::cout << "[StreamerClient] new sync acquisition (" << streamerName << ", " <<
        //        acq << ")" << std::endl;
        m_mtxSyncAcqs.lock();
        m_syncAcqs[streamerName].emplace_back( acq.clone() );
        m_mtxSyncAcqs.unlock();
    }
    //        m_mtxSyncViewers.unlock();
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
    assert( m_isRecordStream );

    auto newAcqPtr = std::make_shared<hub::Acquisition>( std::move( newAcq ) );

    auto& saveAcqs = m_streamName2saveAcqs[streamName];
    // save new acq
    if ( saveAcqs.find( newAcq.m_start ) == saveAcqs.end() ) {
        std::cout << headerMsg() << "'" << m_streamName << "' save new acq : [ ";
        for ( const auto& [streamName2, saveAcqs2] : m_streamName2saveAcqs ) {
            std::cout << "'" << streamName2 << "':" << saveAcqs2.size() << ", ";
        }
        std::cout << " ] ";
        std::cout << std::endl;
        saveAcqs[newAcq.m_start] = newAcqPtr;
    }
    // acq already saved
    //    else {}

    auto& lastAcq = m_lastAcq[streamName];
    if ( lastAcq.get() != newAcqPtr.get() ) { lastAcq = newAcqPtr; }
}

bool StreamerClient::isRecordStream() const {
    return m_isRecordStream;
}

const std::string& StreamerClient::getParent() const {
    return m_parent;
}

const std::list<StreamViewerClient*>& StreamerClient::getStreamViewers() const {
    return m_streamViewers;
}

// const std::chrono::time_point<std::chrono::high_resolution_clock>&
// StreamerClient::getLastUpdateAcqDate( const std::string& streamName ) const {
//     while ( m_lastUpdateAcqDate.find( streamName ) == m_lastUpdateAcqDate.end() ) {
//         std::cout << "[StreamerClient] wait for acq date of stream : " << streamName <<
//         std::endl; std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//     }
//     assert( m_lastUpdateAcqDate.find( streamName ) != m_lastUpdateAcqDate.end() );
//     return m_lastUpdateAcqDate.at( streamName );
// }
