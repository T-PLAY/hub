
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
    //    std::cout << "[StreamViewerInterface] read(data, len=" << len << ")" << std::endl;
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

    //    std::string streamName;
    sock.read( m_streamName );
    //    m_streamName = streamName;
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

    //    m_inputSensor = std::make_unique<InputSensor>( io::StreamViewerClient( std::move( sock ) )
    //    );
    try {
        m_inputSensor =
            std::make_unique<hub::InputSensor>( StreamViewerInterface( std::move( sock ) ) );
    }
    catch ( hub::net::Socket::exception& e ) {
        std::cout << headerMsg() << "InputSensor() : catch exception : " << e.what() << std::endl;
        return;
    }

    assert( streamers.find( m_streamName ) == streamers.end() );

    //    streamers[m_streamName] = this;

    std::cout << headerMsg() << "stream name = '" << m_streamName << "'" << std::endl;

    const auto& sensorSpec = m_inputSensor->m_spec;

    const size_t acquisitionSize = sensorSpec.m_acquisitionSize;
    std::cout << headerMsg() << "sensor name:'" << sensorSpec.m_sensorName << "'" << std::endl;
    std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
    std::cout << headerMsg()
              << "resolutions:" << hub::SensorSpec::resolutions2string( sensorSpec.m_resolutions )
              << std::endl;
    //    std::cout << headerMsg() << "dims:" << hub::SensorSpec::dims2string( sensorSpec.m_dims )
    //              << std::endl;
    //    std::cout << headerMsg() << "format:" << sensorSpec.m_format << std::endl;

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

    //    m_lastAcqs[""];
    m_lastAcq[""];

    std::thread thread( [this]() {
        try {
            while ( 1 ) {
                auto masterAcq = m_inputSensor->getAcquisition();
//                assert(! masterAcq.getMeasures().empty());
//                masterAcq.getMeasures().front().interpolable();

                //                continue;
                if ( masterAcq.m_start == -1 ) {
                    std::cout << headerMsg() << "receive reset acq : " << masterAcq << std::endl;
                    m_lastAcq[""].reset();
                    m_streamName2saveAcqs[""].clear();
                    m_server.newAcquisition( this, masterAcq );
                    continue;
                }

                //                std::cout << headerMsg() << "receive acq : " << masterAcq <<
                //                std::endl;

                m_mtxSyncViewers.lock();
                auto& syncViewers = m_syncViewers;
                for ( auto& pair : syncViewers ) {
                    const auto& syncViewerName = pair.first;

                    //                    std::cout << headerMsg() << "receive sync master acq : "
                    //                    << masterAcq << std::endl;

                    auto& syncAcqs = m_syncAcqs.at( syncViewerName );
                    //                    hub::Acquisition * rightAcq = nullptr;
                    //                    std::unique_ptr<hub::Acquisition> leftAcq;
                    //                    assert( rightAcq == nullptr );
                    //                    assert( leftAcq == nullptr );

                    bool foundLeft = false;
                    //                    bool leftIsRight = false;
                    //                    bool looped = false;
                    bool& isSyncthing = m_isSyncthing.at( syncViewerName );
                    do {
                        //                        m_mtxSyncAcqs.lock();
                        //                        while ( syncAcqs.empty() ) {
                        while ( syncAcqs.size() < 2 ) {
                            //                        while ( itLeftAcq == syncAcqs.end() &&
                            //                        std::next(itLeftAcq) == syncAcqs.end() ) {
                            //                            m_mtxSyncAcqs.unlock();
                            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
                            //                            m_mtxSyncAcqs.lock();
                        }
                        //                        m_mtxSyncAcqs.unlock();

                        assert( syncAcqs.size() >= 2 );

                        if ( syncAcqs.begin()->m_start == std::next( syncAcqs.begin() )->m_start ) {
//                            std::cout << headerMsg() << "detect ping acq" << std::endl;
                            syncAcqs.pop_front();
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start !=
                                std::next( syncAcqs.begin() )->m_start );

                        if ( syncAcqs.begin()->m_start >= std::next( syncAcqs.begin() )->m_start ) {
//                            std::cout << headerMsg() << "detect auto loop timestamp" << std::endl;
                            //                            std::cout << headerMsg() << "left " <<
                            //                            *syncAcqs.begin() << std::endl; std::cout
                            //                            << headerMsg() << "right " << *std::next(
                            //                            syncAcqs.begin() )
                            //                                      << std::endl;
                            //                            std::cout << headerMsg() << "master " <<
                            //                            masterAcq << std::endl;
                            syncAcqs.pop_front();
                            isSyncthing = false;
                            continue;
                        }
                        assert( syncAcqs.begin()->m_start <
                                std::next( syncAcqs.begin() )->m_start );

                        if ( syncAcqs.begin()->m_start >= masterAcq.m_start ) {
//                            std::cout << headerMsg() << "detect bad left acq" << std::endl;
                            //                            std::cout << headerMsg() << "left " <<
                            //                            *syncAcqs.begin() << std::endl; std::cout
                            //                            << headerMsg() << "right " << *std::next(
                            //                            syncAcqs.begin() )
                            //                                      << std::endl;
                            //                            std::cout << headerMsg() << "master " <<
                            //                            masterAcq << std::endl;
                            if ( !isSyncthing ) {
//                                std::cout << headerMsg() << "unable to sync master acq"
//                                          << std::endl;
                                break;
                            }
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
                    //                    const auto& syncMeasure = masterAcq.getMeasures().front();

                    hub::Acquisition acq { masterAcq.m_start, masterAcq.m_end };
                    if ( itLeftAcq->interpolable() ) {
//                        assert(itRightAcq->interpolable());
                        const double t = (masterAcq.m_start - itLeftAcq->m_start) / (double)(itRightAcq->m_start - itLeftAcq->m_start);
                        acq << hub::Acquisition::slerp(*itLeftAcq, *itRightAcq, t).getMeasures();
                    }
                    else { acq << closestAcq->getMeasures(); }
                    acq << masterAcq.getMeasures();
                    //                    acq << hub::Measure { syncMeasure.m_data,
                    //                    syncMeasure.m_size };

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
        //        m_server.m_streamers.erase( m_streamName );

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

    //    m_mtxLastAcqs.lock();
    //    m_lastAcqs.clear();
    m_streamName2saveAcqs.clear();
    //    m_mtxLastAcqs.unlock();

    m_mtxSyncViewers.lock();
    auto syncViewers = m_syncViewers;
    for ( auto& pair : syncViewers ) {
        //        const auto & sensorName = pair.first;
        auto syncViewers = pair.second;
        for ( auto* syncViewer : syncViewers ) {
            //            delStreamViewer( syncViewer );
            m_server.delStreamViewer( syncViewer );
            //            delete syncViewer;
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
    //        m_s[syncStreamViewer->m_streamName].push_back( syncStreamViewer );
    syncViewers.push_back( syncStreamViewer );
    //    m_lastAcqs[streamName];
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
    //        m_s[streamViewer->m_streamName].push_back( streamViewer );
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
    //        m_mtxSyncViewers.lock();
    if ( m_syncViewers.find( streamerName ) != m_syncViewers.end() ) {
        m_mtxSyncAcqs.lock();
        //        m_syncAcqs[streamerName].emplace_front( acq.clone() );
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

// const std::vector<std::unique_ptr<hub::Acquisition>>& StreamerClient::getLastAcqs() const {
// const std::vector<std::shared_ptr<hub::Acquisition>>&
// StreamerClient::getLastAcqs( const std::string& streamName ) const {
//    //    if ( m_syncViewers.find( streamName ) != m_syncViewers.end() ) {
//    //        m_mtxSyncAcqs.lock();
//    //        m_syncAcqs[streamName].emplace_front( acq.clone() );
//    //        m_mtxSyncAcqs.unlock();
//    //    }
//    //    if (m_lastAcqs.find(streamName) == m_lastAcqs.end()) {
//    //        m_lastAcqs[streamName] = {};
//    //    }
////    if (streamName != "") {
////        return {};
////    }
//    assert( m_lastAcqs.find( streamName ) != m_lastAcqs.end() );
//    return m_lastAcqs.at( streamName );
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
    //                    m_lastAcqs[streamViewerName].push_back(
    //                        std::make_shared<hub::Acquisition>( std::move( *bestMatchAcq ) ) );
    //    auto& saveAcqs = m_streamName2saveAcqs[streamViewerName];
    //    // save new sync acq
    //    if ( saveAcqs.find( bestMatchAcq->m_start ) == saveAcqs.end() ) {
    //        saveAcqs[bestMatchAcq->m_start] =
    //            std::make_shared<hub::Acquisition>( std::move( *bestMatchAcq ) );
    //    }
    //    auto& lastAcq = m_lastAcq[streamViewerName];
    //    auto& newAcq  = saveAcqs.at( acq.m_start );
    //    if ( lastAcq.get() != newAcq.get() ) { lastAcq = saveAcqs.at( acq.m_start ); }
    auto newAcqPtr = std::make_shared<hub::Acquisition>( std::move( newAcq ) );

    auto& saveAcqs = m_streamName2saveAcqs[streamName];
    // save new acq
    if ( saveAcqs.find( newAcq.m_start ) == saveAcqs.end() ) {
        std::cout << headerMsg() << " save new acq : [ ";
        //        << streamName << " save new acq : " << saveAcqs.size() << std::endl;
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
    //    auto& newAcq  = saveAcqs.at( acq.m_start );
    //    if ( lastAcq.get() != newAcqPtr.get() ) { lastAcq = newAcqPtr; }
    if ( lastAcq.get() != newAcqPtr.get() ) { lastAcq = newAcqPtr; }
}

////                        if ( rightAcq != nullptr ) { leftAcq =
/// std::make_unique<hub::Acquisition>(std::move(*rightAcq)); }

//                        m_mtxSyncAcqs.lock();
//                        auto nAcqs = syncAcqs.size();
////                        rightAcq = &syncAcqs.back();
////                        rightAcq =
////                            std::make_unique<hub::Acquisition>(
/// syncAcqs.back() );
//                        assert( nAcqs == syncAcqs.size() );
//                        m_mtxSyncAcqs.unlock();
//                        //                        assert(syncAcqs.back() ==
//                        nullptr); syncAcqs.pop_back();

////                        assert( rightAcq != nullptr );

////                    } while ( rightAcq->m_start < masterAcq.m_start );
//                    } while ( true );

//        std::cout << headerMsg() << "[Match] want to be close to acq : " <<
//        masterAcq << std::endl;
//                    std::unique_ptr<hub::Acquisition> bestMatchAcq;
//                    bestMatchAcq = std::make_unique<hub::Acquisition>(
//                    syncAcqs.back().clone() );

//                    // search best match acq
//                    {
//                        syncAcqs.pop_back();

//                        auto minDist = std::abs( masterAcq.m_start -
//                        bestMatchAcq->m_start );
//                        //                        std::cout << headerMsg() <<
//                        "dist = " << minDist
//                        //                        << std::endl;

//                        bool foundBestMatch = false;
//                        while ( !foundBestMatch ) {
//                            m_mtxSyncAcqs.lock();
//                            while ( syncAcqs.empty() ) {
//                                //                                std::cout <<
//                                headerMsg() << "empty
//                                //                                syncAcqs,
//                                sleep" << std::endl; m_mtxSyncAcqs.unlock();
//                                std::this_thread::sleep_for(
//                                std::chrono::milliseconds( 1 ) );
//                                m_mtxSyncAcqs.lock();
//                            }
//                            assert( !syncAcqs.empty() );
//                            m_mtxSyncAcqs.unlock();

//                            auto& acq2 = syncAcqs.back();
//                            //                std::cout << headerMsg() <<
//                            "[Match] pop acq : " <<
//                            //                acq2 << std::endl;
//                            auto dist = std::abs( masterAcq.m_start -
//                            acq2.m_start );
//                            //                            std::cout <<
//                            headerMsg() << "dist = " <<
//                            //                            dist << std::endl;
//                            if ( dist < minDist ) {
//                                minDist = dist;
//                                bestMatchAcq.release();
//                                bestMatchAcq =
//                                std::make_unique<hub::Acquisition>(
//                                acq2.clone() ); syncAcqs.pop_back();
//                            }
//                            else {
//                                // assert(minDist < 20'000); // 20 ms if (
//                                minDist > 8'000 ) {
//                                    std::cout << headerMsg() << "sync dist = "
//                                    << minDist
//                                              << std::endl;
//                                }
//                                foundBestMatch = true;
//                            }
//                        }

//                        m_mtxSyncAcqs.lock();
//                        syncAcqs.emplace_back( bestMatchAcq->clone() );
//                        //                        while ( !syncAcqs.empty() &&
//                        //                        syncAcqs.back().m_start <
//                        masterAcq.m_start ) { while ( !syncAcqs.empty() &&
//                                std::abs( syncAcqs.back().m_start -
//                                masterAcq.m_start ) > 8'000 ) {
//                            syncAcqs.pop_back();
//                        }
//                        m_mtxSyncAcqs.unlock();

//                        //                        if (masterAcq.m_start <
//                        bestMatchAcq->m_start) {
//                        //                            syncAcqs.emplace_back(
//                        bestMatchAcq->clone()
//                        //                            );
//                        //                        }
//                    }

//                std::cout << headerMsg() << "[Match] best match : " <<
//                *bestMatchAcq << std::endl; std::cout << headerMsg() <<
//                "[Match]
//                --------------------------" << std::endl;
