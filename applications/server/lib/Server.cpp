#include "Server.hpp"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <IO/Stream.hpp>
#include <Net/Socket.hpp>
#include <Sensor.hpp>

std::string Server::getStatus() {
    std::string streamViewersStr = "[";

    //    m_mtxStreamers.lock();
    for ( const auto& pair : m_streamers ) {
        const auto& streamerName = pair.first;
        const auto& streamer     = pair.second;

        auto& streamViewers = m_streamViewers[streamerName];
        //        const auto& streamer    = m_streamers.at( streamerName );
        const auto& syncViewers = streamer->getSyncViewers();

        std::string str = streamerName.substr( 0, 3 );
        if ( !streamViewers.empty() || !syncViewers.empty() ) {
            size_t nbViewers = 0;
            for ( const auto& pair : syncViewers ) {
                //                const auto & name = pair.first;
                const auto& viewers = pair.second;
                nbViewers += viewers.size();
            }
            streamViewersStr += "(" + str + "," + std::to_string( streamViewers.size() ) + "," +
                                std::to_string( syncViewers.size() ) + "," +
                                std::to_string( nbViewers ) + ")";

            streamViewersStr += ",";
        }
    }
    streamViewersStr += "]";

    std::string str = std::string( "status : nbStreamer:" ) + std::to_string( m_streamers.size() ) +
                      ", nbViewer:" + std::to_string( m_viewers.size() ) + " " + streamViewersStr;
    //    m_mtxStreamers.unlock();
    return str;
}

void Server::addStreamer( StreamerClient* streamer ) {
    //    m_mtx.lock();
    //    m_mtxStreamers.lock();

    const auto& streamerName = streamer->getStreamName();
    assert( m_streamers.find( streamerName ) == m_streamers.end() );
    m_streamers[streamerName] = streamer;

    // prevent all viewer there is a new streamer
    std::cout << headerMsg() << "prevent viewers there is a new streamer" << std::endl;
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyNewStreamer( *streamer );
    }

    //    m_mtxStreamers.unlock();
    //    m_mtx.unlock();
}

void Server::addStreamViewer( StreamViewerClient* streamViewer ) {
    //    m_mtx.lock();
    const auto& syncStreamName = streamViewer->getSyncStreamName();
    const auto& streamName     = streamViewer->getStreamName();

    if ( syncStreamName == "" ) {
        auto& streamViewers = m_streamViewers[streamName];
        assert( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) ==
                streamViewers.end() );
        streamViewers.push_back( streamViewer );
    }
    else {
        //        const auto& syncStreamName = streamViewer->m_syncStreamName;
        assert( m_streamers.find( syncStreamName ) != m_streamers.end() );
        auto& syncStreamer = *m_streamers.at( syncStreamName );

        syncStreamer.addSyncStreamViewer( streamViewer );
    }
    //    m_mtx.unlock();
}

void Server::addViewer( ViewerClient* viewer ) {

    m_viewers.push_back( viewer );
    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "new viewer" << getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;
}

void Server::delStreamer( StreamerClient* streamer ) {
    //    m_mtx.lock();
    m_mtxStreamers.lock();

    const auto streamerName = streamer->getStreamName();
    const auto& sensorSpec  = streamer->getInputSensor().m_spec;

    for ( const auto* viewer : m_viewers ) {
        viewer->notifyDelStreamer( streamerName, sensorSpec );
    }

    auto streamViewers = m_streamViewers[streamerName];
    for ( auto* streamViewer : streamViewers ) {
        delStreamViewer( streamViewer );
    }
    m_streamViewers.at( streamerName ).clear();

    //    m_mtxStreamers.lock();
    assert( m_streamers.find( streamerName ) != m_streamers.end() );
    m_streamers.erase( m_streamers.find( streamerName ) );
    //    m_mtxStreamers.unlock();

    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "del streamer" << getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;

    delete streamer;
    //    m_mtx.unlock();
    m_mtxStreamers.unlock();
}

void Server::delStreamViewer( StreamViewerClient* streamViewer ) {
    //    m_mtx.lock();

    const auto& syncStreamName = streamViewer->getSyncStreamName();
    const auto& streamName     = streamViewer->getStreamName();

    if ( syncStreamName == "" ) {
        assert( m_streamViewers.find( streamName ) != m_streamViewers.end() );
        auto& streamViewers = m_streamViewers.at( streamName );
        streamViewers.remove( streamViewer );
    }
    else {
        //        assert( m_streamers.find( streamViewer->m_streamName ) != m_streamers.end() );
        if ( m_streamers.find( syncStreamName ) != m_streamers.end() ) {
            auto& streamer = *m_streamers.at( syncStreamName );

            streamer.delStreamViewer( streamViewer );
        }
    }

    streamViewer->killThread();

    delete streamViewer;
    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "del stream viewer" << getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;
    //    m_mtx.unlock();
}

void Server::delViewer( ViewerClient* viewer ) {
    //    m_mtx.lock();
    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "del viewer" << getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;
    //    m_mtx.unlock();
}

void Server::newAcquisition( StreamerClient* streamer, hub::Acquisition acq ) {

    const auto& streamerName = streamer->getStreamName();
    //    const auto streamViewers = m_streamViewers[streamerName];

    // broadcast acquisition for each live viewer (no sync)
    //        m_mtxStreamViewers.lock();
    auto streamViewers = m_streamViewers[streamerName];
    //    for ( const auto& streamViewer : m_streamViewers[streamerName] ) {
    for ( const auto& streamViewer : streamViewers ) {
        assert( streamViewer->getSyncStreamName() == "" );
        //        if ( streamViewer->m_syncStreamName == "" ) {
        //            assert( false );
        //            const auto& syncStreamName = streamViewer->m_syncStreamName;
        //            assert( m_streamers.find( syncStreamName ) != m_streamers.end() );
        //            auto& syncStreamer = m_streamers.at( syncStreamName );
        //            syncStreamer->m_syncAcqs[streamerName].emplace_front( acq.clone() );
        //            std::cout << headerMsg() << "newAcquisition(" << streamerName << ") :
        //            syncStreamName = " <<  syncStreamName << std::endl;
        //        }
        //        else {
        streamViewer->update( acq );
        //        }
        //        }
    }
    //    m_mtxStreamViewers.unlock();

    // broadcast acquisition for each synchronize stream
    for ( const auto& pair : m_streamers ) {
        //        const auto& otherStreamerName = pair.first;
        const auto& streamer = pair.second;

        streamer->newAcquisition( streamerName, acq );
    }
}

const std::vector<std::shared_ptr<hub::Acquisition>>&
Server::getLastAcqs( const std::string& streamName ) {
    m_mtxStreamers.lock();
    assert( m_streamers.find( streamName ) != m_streamers.end() );
    //    auto lastAcq = m_streamers.at( streamName )->getLastAcq();
    const auto& streamer = m_streamers.at( streamName );
    //    streamer->m_mtxLastAcqs.lock();
    const auto& lastAcqs = streamer->getLastAcqs();
    m_mtxStreamers.unlock();
    return lastAcqs;
}

void Server::setAcqPing( bool newAcqPing ) {
    m_acqPing = newAcqPing;
}

const std::map<std::string, StreamerClient*>& Server::getStreamers() const {
    //    m_mtxStreamers.lock();
    //    m_mtxStreamers.unlock();
    return m_streamers;
}

Server::Server() {
    assert( !m_thread.joinable() );
}

Server::Server( int port ) : mServerSock( port ) {}

Server::~Server() {
    std::cout << headerMsg() << "~Server()" << std::endl;
    //    assert( !m_thread.joinable() );
    m_thread.join();
}

std::string Server::headerMsg() const {
    const std::string str = "\t\033[1m[Server:0]\033[0m ";
    return str;
}

void Server::run() {
    std::cout << headerMsg() << "listening port " << mServerSock.getPort() << std::endl;

    int iClient = 0;
    while ( iClient < m_maxClients ) {

        hub::net::ClientSocket sock = mServerSock.waitNewClient();

        std::cout << headerMsg() << "new client" << std::endl;

        Client* newClient = initClient( std::move( sock ), ++iClient );
        m_clients.push_back( newClient );

        //        io::Client* newClient = io::Client::init( std::move( sock ) );
        //        newClient->startAsyncRoutine( this, iClient );
    }
    std::cout << headerMsg() << "run() max clients attempt" << std::endl;
}

void Server::asyncRun() {
    std::cout << headerMsg() << "asyncRun()" << std::endl;
    m_thread = std::thread( [this]() { run(); } );
    //    m_thread.detach();
}

void Server::stop() {
    //    m_wantToStop = true;
    //    assert( ! m_thread.joinable() );
    //    m_thread.join();
}

Client* Server::initClient( hub::net::ClientSocket&& sock, int iClient ) {

    hub::net::ClientSocket::Type clientType;
    //    sock.read( reinterpret_cast<unsigned char*>( &clientType ), sizeof( clientType ) );
    sock.read( clientType );

    switch ( clientType ) {
    case hub::net::ClientSocket::Type::STREAMER:
        return new StreamerClient( *this, iClient, std::move( sock ) );
    case hub::net::ClientSocket::Type::VIEWER:
        return new ViewerClient( *this, iClient, std::move( sock ) );
    case hub::net::ClientSocket::Type::STREAM_VIEWER:
        return new StreamViewerClient( *this, iClient, std::move( sock ) );
    default:
        assert( false );
        return nullptr;
    }
    //    //        //    m_socket( new net::ClientSocket( clientSocket  ) ) {}
}

void Server::setMaxClients( int maxThreads ) {
    m_maxClients = maxThreads;
}

//////////////////////////////////////// old ///////////////////////////////////

//        std::thread thread( [this, iThread, sock = std::move( sock )]() mutable {
//            std::cout << getServerHeader( iThread ) << "new thread\t\t\t\t" << getStatus()
//                      << std::endl;

//            ClientSocket::Type clientType;
//            sock.read( clientType );

//            switch ( clientType ) {
//            ////////////////////////////////////////////////////////////////////////////////////////////////
//            case ClientSocket::Type::STREAMER: {

//                std::string sensorName;
//                sock.read( sensorName );
//                if ( mStreamers.find( sensorName ) == mStreamers.end() ) {
//                    sock.write( Socket::Message::NOT_FOUND );
//                }
//                else {
//                    sock.write( Socket::Message::FOUND );
//                    std::cout << getServerHeader( iThread )
//                              << "[stream viewer] stream sensor name : '" << sensorName
//                              << "' already exist" << std::endl;
//                    break;
//                }
//                assert( mStreamers.find( sensorName ) == mStreamers.end() );

////                SensorSpec sensorSpec;
////                sock.read(sensorSpec);
//                Streamer streamer {
//                    hub::InputSensor( std::move( sock ) ), sensorName, {}, {}, nullptr, {} };
//                // sensor is unique
//                const auto& inputSensor  = streamer.mInputSensor;
//                auto& outputSensors      = streamer.mOutputSensors;
//                auto& sensor2syncViewers = streamer.mSensor2syncViewers;
//                auto& sensor2acqs        = streamer.mSensor2acqs;
//                auto& syncMaster         = streamer.mSyncMaster;

//                // sensor is unique

//                assert( mStreamers.find( sensorName ) == mStreamers.end() );
//                mStreamers[sensorName] = &streamer;
//                std::cout << getServerHeader( iThread ) << "[streamer] new streamer\t\t"
//                          << getStatus() << std::endl;

//                std::cout << getServerHeader( iThread ) << "[streamer] sensor name = '"
//                          << sensorName << "'" << std::endl;

//                try {

//                    // prevent all viewer there is a new streamer
//                    std::cout << getServerHeader( iThread )
//                              << "[streamer] prevent viewers there is a new streamer" <<
//                              std::endl;
//                    for ( const auto& viewer : mViewers ) {
//                        viewer->notifyNewStreamer( streamer );
//                    }

//                    const auto& header           = inputSensor.getHeader();
//                    const size_t acquisitionSize = header.acquisitonSize;
//                    std::cout << getServerHeader( iThread )
//                              << "[streamer] acquisitionSize:" << acquisitionSize << std::endl;
//                    std::cout << getServerHeader( iThread )
//                              << "[streamer] dims:" << hub::SensorSpec::dims2string( header.dims )
//                              << std::endl;
//                    // std::cout << getServerHeader(iThread) << "[streamer] height:" <<
//                    // header.getDims().at(1) << std::endl;
//                    std::cout << getServerHeader( iThread )
//                              << "[streamer] format:" << header.format << std::endl;
//                    const auto& metadata = header.metaData;
//                    for ( const auto& pair : metadata ) {
//                        const auto& name = pair.first;
//                        const auto& val  = pair.second;
//#ifdef WIN32
//                        std::cout << getServerHeader( iThread )
//                                  << "[streamer] metadata: " << val.type().name() << " " << name
//                                  << " = '" << hub::Header::any2string( val ) << "' ("
//                                  << val.type().raw_name() << ")" << std::endl;
//#else
//                        std::cout << getServerHeader( iThread )
//                                  << "[streamer] metadata: " << val.type().name() << " " << name
//                                  << " = '" << hub::SensorSpec::any2string( val ) << "'" <<
//                                  std::endl;
//#endif
//                    }

//                    //                    Stream::hub::Acquisition acq;
//                    // for each new stream acquistion
//                    while ( true ) {

//                        auto acq = inputSensor.getAcquisition();
//                        //                        inputSensor >> acq;

//                        // broadcast data
//                        // send new acquisition for all no sync stream viewers of this stream
//                        {
//                            auto it = outputSensors.begin();
//                            while ( it != outputSensors.end() ) {
//                                const auto& outputSensor = *it;
//                                try {
//                                    // std::cout << getServerHeader(iThread) << "[streamer]
//                                    receive
//                                    // and send acq : \t" << acq << std::endl;
//                                    outputSensor << acq;

//                                    ++it;
//                                }
//                                catch ( std::exception& e ) {
//                                    // no sync stream viewer lost
//                                    std::cout << getServerHeader( iThread )
//                                              << "[streamer] out : catch outputSensor exception :
//                                              "
//                                              << e.what() << std::endl;
//                                    it = outputSensors.erase( it );

//                                    std::cout << getServerHeader( iThread )
//                                              << "[streamer] out : end stream viewer\t"
//                                              << getStatus() << std::endl;
//                                    std::cout <<
//                                    "-------------------------------------------------"
//                                                 "--------------------------------------------"
//                                              << std::endl;
//                                }
//                            }
//                        }

//                        // save new acquisition for all sync stream viewers of this stream
//                        if ( syncMaster != nullptr ) {
//                            Streamer* root = syncMaster;
//                            while ( root->mSyncMaster != nullptr )
//                                root = root->mSyncMaster;

//                            // std::cout << getServerHeader(iThread) << "[streamer] save acq :
//                            \t\t"
//                            // << acq << std::endl;
//                            root->mSensor2acqs[sensorName].emplace_front( acq.clone() );
//                        }

//                        // for all synchronize viewers of this stream, send best match acquisition
//                        // according to the current acquisition
//                        {
//                            auto it = sensor2syncViewers.begin();
//                            while ( it != sensor2syncViewers.end() ) {
//                                auto& pair                 = *it;
//                                const auto& syncSensorName = pair.first;
//                                auto& outputSensors        = pair.second;

//                                assert( !outputSensors.empty() );
//                                auto& acqs = sensor2acqs[syncSensorName];
//                                while ( acqs.empty() ) {
//                                    std::cout << getServerHeader( iThread )
//                                              << "[streamer] empty acqs, sleep" << std::endl;
//                                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 )
//                                    );
//                                }

//                                //                                Stream::hub::Acquisition
//                                //                                bestMatchAcq =
//                                //                                std::move(acqs.back());
//                                std::unique_ptr<hub::Acquisition> bestMatchAcq;
//                                //                                Stream::hub::Acquisition &
//                                //                                bestMatchAcq =
//                                //                                acqs.back().clone();
//                                bestMatchAcq =
//                                    std::make_unique<hub::Acquisition>( acqs.back().clone() );
//                                // search best match acq
//                                {
//                                    acqs.pop_back();
//                                    // assert(acq.start
//                                    //                                    -
//                                    // bestMatchAcq->start
//                                    //                                    >= 0); if
//                                    //                                    (acq.start -
//                                    // bestMatchAcq->start
//                                    //                                    >= 0) {
//                                    ////                                        while
//                                    (acqs.empty())
//                                    ///{ };
//                                    // bestMatchAcq.release();
//                                    //                                        bestMatchAcq =
//                                    //
//                                    std::make_unique<Stream::hub::Acquisition>(acqs.back().clone());
//                                    //                                        acqs.pop_back();
//                                    //                                    }
//                                    //                                    assert
//                                    //                                    (acq.start -
//                                    // bestMatchAcq->start
//                                    //                                    < 0);
//                                    auto minDist = std::abs( acq.start -
//                                                             bestMatchAcq->start );

//                                    bool foundBestMatch = false;
//                                    while ( !foundBestMatch ) {
//                                        while ( acqs.empty() ) {}
//                                        auto& acq2 = acqs.back();
//                                        auto dist  = std::abs( acq.start -
//                                                              acq2.start );
//                                        if ( dist < minDist ) {
//                                            minDist = dist;
//                                            // std::cout << getServerHeader(iThread) <<
//                                            "[streamer]
//                                            // delete acq : \t\t" << bestMatchAcq << std::endl;
//                                            // bestMatchAcq
//                                            //                                            =
//                                            // std::move(acq2);
//                                            // bestMatchAcq
//                                            //                                            =
//                                            // acq2.clone();
//                                            // bestMatchAcq.reset(acq2.clone());
//                                            bestMatchAcq.release();
//                                            bestMatchAcq =
//                                                std::make_unique<hub::Acquisition>( acq2.clone()
//                                                );
//                                            acqs.pop_back();
//                                        }
//                                        else { foundBestMatch = true; }
//                                    }

//                                    // std::cout << getServerHeader(iThread) << "[streamer] best
//                                    acq
//                                    // : \t\t" << bestMatchAcq << "\t(" << acqs.size() << ")" <<
//                                    // std::endl;
//                                    std::cout << "timestamp start diff : " << minDist / 1000.0
//                                              << " milliseconds" << std::endl;

//                                    acqs.emplace_back( bestMatchAcq->clone() );
//                                }

//                                // broadcast best match acquisition fo each stream viewers
//                                auto it2 = outputSensors.begin();
//                                while ( it2 != outputSensors.end() ) {
//                                    auto& outputSensor = *it2;

//                                    try {
//                                        outputSensor << *bestMatchAcq;
//                                        // std::cout << "----------" << std::endl;
//                                        ++it2;
//                                    }
//                                    catch ( std::exception& e ) {
//                                        std::cout
//                                            << getServerHeader( iThread )
//                                            << "[viewer] catch sync viewer exception : " <<
//                                            e.what()
//                                            << std::endl;
//                                        it2 = outputSensors.erase( it2 );
//                                    }
//                                }

//                                if ( outputSensors.empty() ) {
//                                    mStreamers.at( syncSensorName )->mSyncMaster = nullptr;
//                                    assert( it2 == outputSensors.end() );
//                                    it = sensor2syncViewers.erase( it );
//                                    std::cout << getServerHeader( iThread )
//                                              << "[streamer] out : end sync viewer\t" <<
//                                              getStatus()
//                                              << std::endl;
//                                }
//                                else { ++it; }
//                            } // while (it != sensor2syncViewers.end())
//                        }
//                    } // while (true)
//                }
//                catch ( std::exception& e ) {
//                    std::cout << getServerHeader( iThread )
//                              << "[streamer] in : catch inputSensor exception : " << e.what()
//                              << std::endl;
//                }

//                // if player kill many stream in same time (concurrency)
//                mMtx.lock();
//                mStreamers.erase( sensorName );

//                for ( const auto& pair : sensor2syncViewers ) {
//                    const auto& syncSensorName                   = pair.first;
//                    mStreamers.at( syncSensorName )->mSyncMaster = nullptr;
//                }

//                //                std::cout << "[" << sensorName << "] mutex lock" << std::endl;
//                for ( const auto* viewer : mViewers ) {
//                    try {
//                        viewer->mSock->write( Socket::Message::DEL_STREAMER );
//                        //                    viewer->mSock->write(inputSensor.getSensorName());
//                        viewer->mSock->write( sensorName );
//                    }
//                    catch ( std::exception& e ) {
//                        std::cout << getServerHeader( iThread )
//                                  << "[streamer] in : viewer is dead, this append when "
//                                     "viewer/streamer process was stopped in same time : "
//                                  << e.what() << std::endl;
//                    }
//                }
//                //                std::cout << "[" << sensorName << "] mutex unlock" << std::endl;

//                std::cout << getServerHeader( iThread ) << "[streamer] end (" << sensorName << ")"
//                          << std::endl;
//                mMtx.unlock();
//            } break;

//            ////////////////////////////////////////////////////////////////////////////////////////////////
//            case ClientSocket::Type::VIEWER: {

//                Viewer viewer { &sock };

//                // each already connected streamers prevent existence for this new viewer
//                for ( const auto& pair : mStreamers ) {
//                    const auto& streamer = *pair.second;
//                    viewer.notifyNewStreamer( streamer );
//                }

//                mViewers.push_back( &viewer );
//                std::cout << getServerHeader( iThread ) << "[viewer] new viewer\t\t\t"
//                          << getStatus() << std::endl;

//                try {
//                    // check client still alive
//                    while ( true ) {
//                        sock.write( Socket::Message::PING );
//                        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//                    }
//                }
//                catch ( std::exception& e ) {
//                    std::cout << getServerHeader( iThread )
//                              << "[viewer] catch viewer exception : " << e.what() << std::endl;
//                }

//                mViewers.remove( &viewer );
//                std::cout << getServerHeader( iThread ) << "[viewer] end" << std::endl;

//                sock.clear();

//            } break;

//            ////////////////////////////////////////////////////////////////////////////////////////////////
//            case ClientSocket::Type::STREAM_VIEWER: {

//                std::string sensorName;
//                sock.read( sensorName );
//                if ( mStreamers.find( sensorName ) == mStreamers.end() ) {
//                    sock.write( Socket::Message::NOT_FOUND );
//                    std::cout << getServerHeader( iThread )
//                              << "[stream viewer] unknown sensor name : '" << sensorName << "'"
//                              << std::endl;
//                    break;
//                }
//                else { sock.write( Socket::Message::OK ); }
//                assert( mStreamers.find( sensorName ) != mStreamers.end() );

//                std::string syncSensorName;
//                sock.read( syncSensorName );
//                if ( syncSensorName != "" &&
//                     mStreamers.find( syncSensorName ) == mStreamers.end() ) {
//                    sock.write( Socket::Message::NOT_FOUND );
//                    std::cout << getServerHeader( iThread )
//                              << "[stream viewer] unknown sync sensor name : '" << syncSensorName
//                              << "'" << std::endl;
//                    break;
//                }
//                else { sock.write( Socket::Message::OK ); }
//                assert( syncSensorName == "" ||
//                        mStreamers.find( syncSensorName ) != mStreamers.end() );

//                std::cout << getServerHeader( iThread ) << "[stream viewer] new stream viewer\t"
//                          << getStatus() << std::endl;
//                std::cout << getServerHeader( iThread ) << "[stream viewer] watching '"
//                          << sensorName << "'" << std::endl;

//                Streamer* streamer = mStreamers.at( sensorName );

//                try {
//                    const auto& inputSensor = streamer->mInputSensor;
//                    const auto& header      = inputSensor.getHeader();
//                    if ( syncSensorName == "" ) {

//                        // here
//                        streamer->mOutputSensors.emplace_back( hub::SensorSpec { sensorName,
//                                                                             header.format,
//                                                                             header.dims,
//                                                                             header.metaData },
//                                                               ClientSocket( std::move( sock ) )
//                                                               );
//                    }
//                    else {
//                        Streamer* syncMaster = mStreamers.at( syncSensorName );

//                        // here
//                        syncMaster->mSensor2syncViewers[sensorName].emplace_back(
//                            hub::SensorSpec { sensorName,
//                                          header.format,
//                                          header.dims,
//                                          header.metaData },
//                            ClientSocket( std::move( sock ) ) );

//                        assert( streamer->mSyncMaster == nullptr ||
//                                streamer->mSyncMaster == syncMaster );
//                        streamer->mSyncMaster = syncMaster;
//                    }
//                }
//                catch ( std::exception& e ) {
//                    std::cout << getServerHeader( iThread )
//                              << "[stream viewer] failed to initialize viewer : " << e.what()
//                              << std::endl;
//                }

//                std::cout << getServerHeader( iThread ) << "[stream viewer] end" << std::endl;
//            } break;

//            default:
//                std::cout << getServerHeader( iThread ) << "unknown client type" << std::endl;
//            }

//            std::cout << getServerHeader( iThread ) << "thread end\t\t\t\t" << getStatus()
//                      << std::endl;
//            std::cout <<
//            "-------------------------------------------------------------------------"
//                         "--------------------"
//                      << std::endl;
//        } );
//        thread.detach();

//        ++iThread;
//    } // while (true)
//}

// std::string Server::getStatus() const {
//     std::string streamViewersStr = "[";
////    for ( const auto& pair : mStreamers ) {
////        const auto& sensorName = pair.first;
////        const auto& streamer   = pair.second;
////        std::string str        = sensorName.substr( 0, 3 );
////        streamViewersStr += "(" + str + "," + std::to_string( streamer->mOutputSensors.size() )
///+ /                            "," + std::to_string( streamer->mSensor2syncViewers.size() ) +
///")";

////        streamViewersStr += ",";
////    }
//    streamViewersStr += "]";

//    std::string str = std::string( "status : nbStreamer:" ) + std::to_string( mStreamers.size() )
//    +
//                      ", nbViewer:" + std::to_string( mViewers.size() ) + " " + streamViewersStr;
//    return str;
//}

// void Viewer::notifyNewStreamer( const Streamer& streamer ) const {
////    mSock->write( Socket::Message::NEW_STREAMER );

//    /*mSock->write(streamer.mhub::InputSensor.getSensorName());
//    mSock->write(std::string(Stream::format2string[(int)streamer.mhub::InputSensor.getFormat()]));

//    std::string dimStr;
//    for (const auto dim : streamer.mhub::InputSensor.getDims()) {
//        dimStr += std::to_string(dim) + " x ";
//    }
//    dimStr.resize(dimStr.size() - 3);
//    mSock->write(dimStr);
//    mSock->write(std::to_string(streamer.mhub::InputSensor.getAcquisitionSize()));

//    mSock->write(streamer.mhub::InputSensor.getMetaData());*/

////    mSock->write( streamer.mInputSensor );
//}
