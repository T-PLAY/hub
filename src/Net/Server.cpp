#include "Net/Server.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "IO/Client.hpp"
#include "Sensor.hpp"
#include "Socket.hpp"

namespace hub {
namespace net {

std::string Server::getStatus() const {
    std::string streamViewersStr = "[";
    //    for ( const auto& pair : mStreamers ) {
    //        const auto& sensorName = pair.first;
    //        const auto& streamer   = pair.second;
    //        std::string str        = sensorName.substr( 0, 3 );
    //        streamViewersStr += "(" + str + "," + std::to_string( streamer->mOutputSensors.size()
    //        )
    ///+ /                            "," + std::to_string( streamer->mSensor2syncViewers.size() ) +
    ///")";

    //        streamViewersStr += ",";
    //    }
    streamViewersStr += "]";

    std::string str = std::string( "status : nbStreamer:" ) + std::to_string( m_streamers.size() ) +
                      ", nbViewer:" + std::to_string( m_viewers.size() ) + " " + streamViewersStr;
    return str;
}

void Viewer::notifyNewStreamer( const Streamer& streamer ) const {
    //    mSock->write( Socket::Message::NEW_STREAMER );

    /*mSock->write(streamer.mhub::InputSensor.getSensorName());
    mSock->write(std::string(Stream::format2string[(int)streamer.mhub::InputSensor.getFormat()]));

    std::string dimStr;
    for (const auto dim : streamer.mhub::InputSensor.getDims()) {
        dimStr += std::to_string(dim) + " x ";
    }
    dimStr.resize(dimStr.size() - 3);
    mSock->write(dimStr);
    mSock->write(std::to_string(streamer.mhub::InputSensor.getAcquisitionSize()));

    mSock->write(streamer.mhub::InputSensor.getMetaData());*/

    //    mSock->write( streamer.mInputSensor );
}

// static std::string getServerHeader( int iThread ) {
//     const std::string str = "\033[1m[Server]\t\033[" + std::to_string( 31 + iThread % 7 ) +
//                             "m[server:" + std::to_string( iThread ) + "]\033[0m ";
//     return str;
// }

Client::Client( Server& server, int iClient ) : m_server( server ), m_iClient( iClient ) {}

std::string Client::headerMsg() {
    const std::string str = "\t\033[" + std::to_string( 31 + m_iClient % 7 ) + "m" +
                            "[Client:" + std::to_string( m_iClient ) + "]\033[0m";
    return str;
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

std::string Server::headerMsg() {
    const std::string str = "\033[1m[Server:0]\033[0m ";
    return str;
}

void Server::run() {
    std::cout << headerMsg() << "run() start" << std::endl;

    int iClient = 0;
    while ( iClient < m_maxClients ) {

        net::ClientSocket sock = mServerSock.waitNewClient();

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

Client* Server::initClient( ClientSocket&& sock, int iClient ) {

    net::ClientSocket::Type clientType;
    //    sock.read( reinterpret_cast<unsigned char*>( &clientType ), sizeof( clientType ) );
    sock.read( clientType );

    switch ( clientType ) {
    case net::ClientSocket::Type::STREAMER:
        return new Streamer( *this, iClient, std::move( sock ) );
    case net::ClientSocket::Type::VIEWER:
        return new Viewer( *this, iClient, std::move( sock ) );
    case net::ClientSocket::Type::STREAM_VIEWER:
        return new StreamViewer( *this, iClient, std::move( sock ) );
    default:
        assert( false );
        return nullptr;
    }
    //    //        //    m_socket( new net::ClientSocket( clientSocket  ) ) {}
}

void Server::setMaxClients( int maxThreads ) {
    m_maxClients = maxThreads;
}

Streamer::Streamer( Server& server, int iClient, ClientSocket&& sock ) : Client( server, iClient ) {

    std::string streamName;
    sock.read( streamName );
    m_streamName = streamName;
    if ( m_server.m_streamers.find( streamName ) == m_server.m_streamers.end() ) {
        sock.write( ClientSocket::Message::NOT_FOUND );
    }
    else {
        sock.write( ClientSocket::Message::FOUND );
        std::cout << headerMsg() << "stream sensor name : '" << streamName << "' already exist"
                  << std::endl;
        return;
    }
    assert( m_server.m_streamers.find( streamName ) == m_server.m_streamers.end() );

    m_inputSensor = std::make_unique<InputSensor>( io::StreamViewer( std::move( sock ) ) );

    assert( m_server.m_streamers.find( streamName ) == m_server.m_streamers.end() );
    m_server.m_streamers[streamName] = this;
    std::cout << headerMsg() << "new streamer\t\t" << m_server.getStatus() << std::endl;

    std::cout << headerMsg() << "stream name = '" << streamName << "'" << std::endl;

    // prevent all viewer there is a new streamer
    std::cout << headerMsg() << "prevent viewers there is a new streamer" << std::endl;
    for ( const auto& viewer : m_server.m_viewers ) {
        viewer->notifyNewStreamer( *this );
    }

    const auto& sensorSpec       = m_inputSensor->spec;
    const size_t acquisitionSize = sensorSpec.acquisitonSize;
    std::cout << headerMsg() << "sensor name:'" << sensorSpec.sensorName << "'" << std::endl;
    std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
    std::cout << headerMsg() << "dims:" << hub::SensorSpec::dims2string( sensorSpec.dims )
              << std::endl;
    std::cout << headerMsg() << "format:" << sensorSpec.format << std::endl;

    const auto& metadata = sensorSpec.metaData;
    for ( const auto& pair : metadata ) {
        const auto& name = pair.first;
        const auto& val  = pair.second;
#ifdef WIN32
        std::cout << headerMsg() << "metadata: " << val.type().name() << " " << name << " = '"
                  << hub::Header::any2string( val ) << "' (" << val.type().raw_name() << ")"
                  << std::endl;
#else
        std::cout << headerMsg() << "metadata: " << val.type().name() << " " << name << " = '"
                  << hub::SensorSpec::any2string( val ) << "'" << std::endl;
#endif
    }

    std::thread thread( [this, streamName]() {
        try {
            while ( 1 ) {
                auto acq = m_inputSensor->getAcquisition();
                //            std::cout << "[Streamer] receive acq : " << acq << std::endl;
                // broadcast acquisition
                for ( auto* streamViewer : m_server.m_streamViewers[streamName] ) {
                    streamViewer->send( acq );
                }
            } // while ( 1 )
        }
        catch ( std::exception& e ) {
            std::cout << headerMsg() << "in : catch inputSensor exception : " << e.what()
                      << std::endl;
        }
        std::cout << headerMsg() << "end (" << m_inputSensor->spec.sensorName << ")" << std::endl;
        m_server.m_streamers.erase( streamName );

        std::cout << headerMsg() << "thread end\t\t\t\t" << m_server.getStatus() << std::endl;
        std::cout << "-------------------------------------------------------------------------"
                     "--------------------"
                  << std::endl;
    } );
    thread.detach();
}

std::string Streamer::headerMsg() {
    return Client::headerMsg() + "[Streamer] ";
}

Viewer::Viewer( Server& server, int iClient, ClientSocket&& sock ) : Client( server, iClient ) {}

std::string Viewer::headerMsg() {
    return Client::headerMsg() + "[Viewer] ";
}

StreamViewer::StreamViewer( Server& server, int iClient, ClientSocket&& sock ) :
    Client( server, iClient ) {
    std::string streamName;
    sock.read( streamName );
    m_streamName = streamName;

    if ( m_server.m_streamers.find( streamName ) == m_server.m_streamers.end() ) {
        sock.write( ClientSocket::Message::NOT_FOUND );
        std::cout << headerMsg() << "unknown sensor name : '" << streamName << "'" << std::endl;
        return;
    }
    else { sock.write( ClientSocket::Message::OK ); }
    assert( m_server.m_streamers.find( streamName ) != m_server.m_streamers.end() );

    std::string syncSensorName;
    sock.read( syncSensorName );
    if ( syncSensorName != "" &&
         m_server.m_streamers.find( syncSensorName ) == m_server.m_streamers.end() ) {
        sock.write( ClientSocket::Message::NOT_FOUND );
        std::cout << headerMsg() << "unknown sync sensor name : '" << syncSensorName << "'"
                  << std::endl;
        return;
    }
    else { sock.write( ClientSocket::Message::OK ); }
    assert( syncSensorName == "" ||
            m_server.m_streamers.find( syncSensorName ) != m_server.m_streamers.end() );

    std::cout << headerMsg() << "new stream viewer\t" << m_server.getStatus() << std::endl;
    std::cout << headerMsg() << "watching '" << streamName << "'" << std::endl;

    Streamer* streamer    = m_server.m_streamers.at( streamName );
    SensorSpec sensorSpec = streamer->m_inputSensor->spec;
    m_outputSensor        = std::make_unique<OutputSensor>( std::move( sensorSpec ),
                                                     io::Streamer( std::move( sock ) ) );

    //    server.m_streamViewers.push_back( this );
    server.m_streamViewers[streamName].push_back( this );

    //        std::cout << headerMsg() << "end (" << m_inputSensor->spec.sensorName << ")"
    //                  << std::endl;
    //        m_server.m_streamers.erase(streamName);

    //        std::cout << headerMsg() << "thread end\t\t\t\t" << m_server.getStatus() << std::endl;
    //        std::cout <<
    //        "-------------------------------------------------------------------------"
    //                     "--------------------"
    //                  << std::endl;
}

std::string StreamViewer::headerMsg() {
    return Client::headerMsg() + "[StreamViewer] ";
}

void StreamViewer::send( const Acquisition& acq ) {
    try {
        *m_outputSensor << acq;
    }
    catch ( std::exception& e ) {
        // no sync stream viewer lost
        std::cout << headerMsg() << "out : catch outputSensor exception : " << e.what()
                  << std::endl;
        //            it = outputSensors.erase( it );

        m_server.m_streamViewers.at(m_streamName).remove(this);

        std::cout << headerMsg() << "out : end stream viewer\t" << m_server.getStatus()
                  << std::endl;
        std::cout << "-------------------------------------------------"
                     "--------------------------------------------"
                  << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////
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
//                              << "[streamer] acquisitionSize:" << acquisitionSize <<
//                              std::endl;
//                    std::cout << getServerHeader( iThread )
//                              << "[streamer] dims:" << hub::SensorSpec::dims2string(
//                              header.dims )
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
//                                  << "[streamer] metadata: " << val.type().name() << " " <<
//                                  name
//                                  << " = '" << hub::Header::any2string( val ) << "' ("
//                                  << val.type().raw_name() << ")" << std::endl;
//#else
//                        std::cout << getServerHeader( iThread )
//                                  << "[streamer] metadata: " << val.type().name() << " " <<
//                                  name
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
//                                              << "[streamer] out : catch outputSensor
//                                              exception :
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

//                        // for all synchronize viewers of this stream, send best match
//                        acquisition
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
//                                    std::this_thread::sleep_for( std::chrono::milliseconds(
//                                    100 )
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
//                                    // assert(acq.mBackendTimestamp
//                                    //                                    -
//                                    // bestMatchAcq->mBackendTimestamp
//                                    //                                    >= 0); if
//                                    // (acq.mBackendTimestamp -
//                                    // bestMatchAcq->mBackendTimestamp
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
//                                    // (acq.mBackendTimestamp -
//                                    // bestMatchAcq->mBackendTimestamp
//                                    //                                    < 0);
//                                    auto minDist = std::abs( acq.mBackendTimestamp -
//                                                             bestMatchAcq->mBackendTimestamp
//                                                             );

//                                    bool foundBestMatch = false;
//                                    while ( !foundBestMatch ) {
//                                        while ( acqs.empty() ) {}
//                                        auto& acq2 = acqs.back();
//                                        auto dist  = std::abs( acq.mBackendTimestamp -
//                                                              acq2.mBackendTimestamp );
//                                        if ( dist < minDist ) {
//                                            minDist = dist;
//                                            // std::cout << getServerHeader(iThread) <<
//                                            "[streamer]
//                                            // delete acq : \t\t" << bestMatchAcq <<
//                                            std::endl;
//                                            // bestMatchAcq
//                                            //                                            =
//                                            // std::move(acq2);
//                                            // bestMatchAcq
//                                            //                                            =
//                                            // acq2.clone();
//                                            // bestMatchAcq.reset(acq2.clone());
//                                            bestMatchAcq.release();
//                                            bestMatchAcq =
//                                                std::make_unique<hub::Acquisition>(
//                                                acq2.clone()
//                                                );
//                                            acqs.pop_back();
//                                        }
//                                        else { foundBestMatch = true; }
//                                    }

//                                    // std::cout << getServerHeader(iThread) << "[streamer]
//                                    best acq
//                                    // : \t\t" << bestMatchAcq << "\t(" << acqs.size() << ")"
//                                    <<
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

//                //                std::cout << "[" << sensorName << "] mutex lock" <<
//                std::endl; for ( const auto* viewer : mViewers ) {
//                    try {
//                        viewer->mSock->write( Socket::Message::DEL_STREAMER );
//                        // viewer->mSock->write(inputSensor.getSensorName());
//                        viewer->mSock->write( sensorName );
//                    }
//                    catch ( std::exception& e ) {
//                        std::cout << getServerHeader( iThread )
//                                  << "[streamer] in : viewer is dead, this append when "
//                                     "viewer/streamer process was stopped in same time : "
//                                  << e.what() << std::endl;
//                    }
//                }
//                //                std::cout << "[" << sensorName << "] mutex unlock" <<
//                std::endl;

//                std::cout << getServerHeader( iThread ) << "[streamer] end (" << sensorName <<
//                ")"
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
//                              << "[viewer] catch viewer exception : " << e.what() <<
//                              std::endl;
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
//                              << "[stream viewer] unknown sensor name : '" << sensorName <<
//                              "'"
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
//                              << "[stream viewer] unknown sync sensor name : '" <<
//                              syncSensorName
//                              << "'" << std::endl;
//                    break;
//                }
//                else { sock.write( Socket::Message::OK ); }
//                assert( syncSensorName == "" ||
//                        mStreamers.find( syncSensorName ) != mStreamers.end() );

//                std::cout << getServerHeader( iThread ) << "[stream viewer] new stream
//                viewer\t"
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
//                                                                             header.metaData
//                                                                             },
//                                                               ClientSocket( std::move( sock )
//                                                               )
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
////        streamViewersStr += "(" + str + "," + std::to_string(
/// streamer->mOutputSensors.size() )
///+ /                            "," + std::to_string( streamer->mSensor2syncViewers.size() ) +
///")";

////        streamViewersStr += ",";
////    }
//    streamViewersStr += "]";

//    std::string str = std::string( "status : nbStreamer:" ) + std::to_string(
//    mStreamers.size() )
//    +
//                      ", nbViewer:" + std::to_string( mViewers.size() ) + " " +
//                      streamViewersStr;
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

} // namespace net
} // namespace hub

///////////////////////////////////////////////////////////////////////////
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
//                                    // assert(acq.mBackendTimestamp
//                                    //                                    -
//                                    // bestMatchAcq->mBackendTimestamp
//                                    //                                    >= 0); if
//                                    //                                    (acq.mBackendTimestamp -
//                                    // bestMatchAcq->mBackendTimestamp
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
//                                    //                                    (acq.mBackendTimestamp -
//                                    // bestMatchAcq->mBackendTimestamp
//                                    //                                    < 0);
//                                    auto minDist = std::abs( acq.mBackendTimestamp -
//                                                             bestMatchAcq->mBackendTimestamp );

//                                    bool foundBestMatch = false;
//                                    while ( !foundBestMatch ) {
//                                        while ( acqs.empty() ) {}
//                                        auto& acq2 = acqs.back();
//                                        auto dist  = std::abs( acq.mBackendTimestamp -
//                                                              acq2.mBackendTimestamp );
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
