#include "Server.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <impl/server2/io/input/InputStreamServer.hpp>
#include <sensor/Sensor.hpp>

#define SERVER_MSG( str )                             \
    do {                                              \
        m_mtxPrint.lock();                            \
        std::cout << headerMsg() << str << std::endl; \
        m_mtxPrint.unlock();                          \
    } while ( false )

namespace hub {
// namespace server {


// Server::Server() {
//     assert( !m_thread.joinable() );
// }

Server::Server( int port ) : m_serverSock( port ), m_givingPort( port + 1 ) {}

Server::~Server() {
    SERVER_MSG( "~Server() started" );

    stop();
    SERVER_MSG( "~Server() ended" );
}

// void Server::detach() {
//     SERVER_MSG( "detach()" );
//     assert( m_running );
//     assert( !m_detached );
//     m_detached = true;
//     m_mtxClients.lock();
//     for ( auto& client : m_clients ) {
//         client->setServer( nullptr );
//     }
//     m_clients.clear();
//     m_mtxClients.unlock();
// }

std::string Server::headerMsg() const {
    const std::string str = "\t\033[1m[Server:0/" + std::to_string( m_nActiveClient ) + "]\033[0m ";
    return str;
}

void Server::run() {
    SERVER_MSG( "starting server" );
    m_killed = false;
    assert( !m_killed );
    SERVER_MSG( "listening port " << m_serverSock.getPort() );

    //    while ( m_nClient < m_maxClients && !m_killed ) {
    int iClient = 0;
    while ( !m_killed ) {

        assert( m_serverSock.isConnected() );
        // SERVER_MSG( "waiting new client" );
        net::ClientSocket sock = m_serverSock.waitNewClient();

        ++m_nActiveClient;
        SERVER_MSG( "new client" );

        m_mtxClients.lock();
        server::Client* newClient = initClient( std::move( sock ), ++iClient );
        if ( newClient == nullptr ) { --m_nActiveClient; }
        else {
            m_clients.push_back( newClient );
            SERVER_MSG( "new client inited" );
        }
        m_mtxClients.unlock();
    }
    if ( !m_killed ) { SERVER_MSG( "max clients attempt" ); }
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    m_running = false;
}

void Server::asyncRun() {
    assert( !m_running );
    assert( m_thread == nullptr );
    m_running = true;
    //    SERVER_MSG( "asyncRun()" );
    m_thread = new std::thread( [this]() { run(); } );
}

void Server::stop() {
    SERVER_MSG( "stopping server" );
    //    assert( m_nClient == 0 || m_nClient == m_maxClients );
    if ( m_thread != nullptr ) {
        //        SERVER_MSG( "joining main thread" );
        assert( m_thread->joinable() );
        if ( m_running ) {
            //            SERVER_MSG( "running main thread" );
            exitMainThread();
        }
        m_thread->join();
    }
    m_thread = nullptr;
    assert( !m_running );

    //    if ( !m_exiting ) {

    if ( !m_clients.empty() ) {
        SERVER_MSG( "ending connected clients" );
        m_mtxClients.lock();
        for ( auto* client : m_clients ) {
            client->end( io::StreamInterface::ServerMessage::SERVER_CLOSED );
        }
        m_mtxClients.unlock();
    }
    //    SERVER_MSG( "connected clients ended" );

    if ( !m_clients.empty() ) {
        m_mtxClients.lock();
        int iTry = 0;
        while ( !m_clients.empty() && iTry < 10 ) {
            SERVER_MSG( "waiting for clients ended (" << m_clients.size() << ")" );
            m_mtxClients.unlock();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            m_mtxClients.lock();
            ++iTry;
        }
        assert( iTry < 10 );
        m_mtxClients.unlock();
        SERVER_MSG( "all clients disconnected" );
    }

    // m_mtxPrint.lock();
    // m_mtxPrint.unlock();
    // SERVER_MSG( "~Server() unlock mtxPrint" );
    // m_mtxSreamName2streamViewers.lock();
    // m_mtxSreamName2streamViewers.unlock();
    // SERVER_MSG( "~Server() unlock mtxStreamName2streamViewers" );
    // m_mtxStreamName2streamer.lock();
    // m_mtxStreamName2streamer.unlock();
    // SERVER_MSG( "~Server() unlock mtxStreamName2streamer" );
    // m_mtxViewers.lock();
    // m_mtxViewers.unlock();
    // SERVER_MSG( "~Server() unlock mtxViewers" );

    assert( m_nActiveClient == 0 );
    assert( m_clients.empty() );
    //    m_serverSock.disconnect();
    //        assert( m_nClient <= m_maxClients );
    //    } // ( !m_exiting ) {
}


void Server::exitMainThread() {
    //    SERVER_MSG( "ending main loop" );
    m_killed = true;
    //    SERVER_MSG( "killing main loop" );
    io::StreamServer::stopServer("127.0.0.1", m_serverSock.getPort());
    //    hub::input::InputStreamServer inputStream(
    //        io::StreamServer::s_exitSignal, "127.0.0.1", m_serverSock.getPort() );
    //        "exitServer", "127.0.0.1", m_serverSock.getPort() );
    //        "exitServer", "127.0.0.1", 4042 );
}

// void Server::exit() {
//     m_exiting = true;
// }

// void Server::stop() {}

// server::Client* Server::initClient( net::ClientSocket&& sock, int iClient ) {
server::Client* Server::initClient( io::InputOutputSocket&& sock, int iClient ) {

    //    net::ClientSocket::Type clientType;
    io::StreamInterface::ClientType clientType;
    sock.read( clientType );

    switch ( clientType ) {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //    case net::ClientSocket::Type::STREAMER: {
    case io::StreamInterface::ClientType::STREAMER: {
        std::string streamName;
        sock.read( streamName );
        const auto& streamers = m_streamName2streamer;
        if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( io::StreamInterface::ServerMessage::NOT_FOUND );
        }
        else {
            sock.write( io::StreamInterface::ServerMessage::FOUND );
            std::cout << headerMsg() << "unable to start new stream, stream name : '" << streamName
                      << "' already exist" << std::endl;
            sock.close();
            return nullptr;
        }
        assert( streamers.find( streamName ) == streamers.end() );
        std::string clientIpv4 = sock.getIpv4();
        //        int clientPort = sock.getPort();
        int streamPort;
        sock.read( streamPort );
        const int port = ( streamPort == 0 ) ? m_givingPort++ : streamPort;
        sock.write( port );
        return new server::StreamerClient( this,
                                           iClient,
                                           std::move( sock ),
                                           std::move( streamName ),
                                           std::move( clientIpv4 ),
                                           port );
    }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //    case net::ClientSocket::Type::VIEWER:

    case io::StreamInterface::ClientType::VIEWER: {
        return new server::ViewerClient( this, iClient, std::move( sock ) );
    }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //    case net::ClientSocket::Type::STREAM_VIEWER: {
    case io::StreamInterface::ClientType::STREAM_VIEWER: {
        //        assert( m_server != nullptr );
        m_mtxStreamName2streamer.lock();
        const auto& streamers = m_streamName2streamer;

        //        server::StreamViewerClient* ret = nullptr;

        std::string streamName;
        sock.read( streamName );
        //        if ( streamName == io::StreamServer::s_exitSignal ) {
        //            std::cout << headerMsg() << "killing server by peer" << std::endl;
        //            sock.close();
        //            m_killed = true;
        //            //            return nullptr;
        //        }
        if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( io::StreamInterface::ServerMessage::NOT_FOUND );
            std::cout << headerMsg() << "unable to reach stream, stream name : '" << streamName
                      << "' not found" << std::endl;
            //            std::thread( [this]() { delete this; } ).detach();
            sock.close();
            //            return nullptr;
        }
        else {
            sock.write( io::StreamInterface::ServerMessage::OK );
            //            assert( streamers.find( streamName ) != streamers.end() );
            const auto& streamer = streamers.find( streamName )->second;
            const auto& ipv4     = streamer->ipv4;
            const auto& port     = streamer->port;

            sock.write( ipv4 );
            sock.write( port );

            //            ret = new server::StreamViewerClient(
            //                this, iClient, std::move( sock ), std::move( streamName ) );
        }
        m_mtxStreamName2streamer.unlock();
        //        return ret;
        return nullptr;
    }

    case io::StreamInterface::ClientType::KILLER: {
        sock.close();
        if ( m_killed ) {
            //            std::cout << headerMsg() << "harakiri" << std::endl;
            std::cout << headerMsg() << "killing in the name" << std::endl;
        }
        else {
            std::cout << headerMsg() << "killing server by peer" << std::endl;
            m_killed = true;
        }
        return nullptr;
    }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //    case net::ClientSocket::Type::ASKER:
        //    case io::StreamInterface::ClientType::ASKER:
        //        return new server::AskerClient( this, iClient, std::move( sock ) );
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    default:
        assert( false );
        return nullptr;
    }
}

// void Server::setMaxClients( int maxClients ) {
//     m_maxClients = maxClients;
//     SERVER_MSG( "setting max clients: " << m_maxClients );
// }

void Server::printStatus() const {
    std::cout << headerMsg() << getStatus() << std::endl;
    // std::cout << headerMsg() << getStatus();
}

bool Server::running() const
{
    return m_running;
}


std::string Server::getStatus() const {
    //    std::string streamViewersStr = "[";

    // #if ( __cplusplus >= 201703L )
    //     for ( const auto& [streamName, streamViewers] : m_streamName2streamViewers ) {
    // #else

    //    for ( const auto& pair : m_streamName2streamViewers ) {
    //        const auto& streamName    = pair.first;
    //        const auto& streamViewers = pair.second;
    // #endif

    //        std::string str = streamName.substr( 0, 3 );
    //        if ( !streamViewers.empty() ) {
    //            streamViewersStr += "(" + str + "," + std::to_string( streamViewers.size() ) +
    //            ")";
    //        }
    //    }
    //    streamViewersStr += "]";

    //    std::string str = std::string( "status : nbStreamer:" ) +
    //                      std::to_string( m_streamName2streamer.size() ) +
    //                      ", nbViewer:" + std::to_string( m_viewers.size() ) + " " +
    //                      streamViewersStr;

    std::string str = "status : [";
    //                      +
    //                      ", nClient:" + std::to_string(m_nActiveClient);
    int i = 0;
    //    for (const auto & pair : m_streamName2streamViewers) {
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamerName = pair.first;
        const auto& streamer     = pair.second;
        //        assert( m_streamName2streamer.find( streamerName ) != m_streamName2streamer.end()
        //        ); const auto & streamer = m_streamName2streamer.at(streamerName);

        str += "'" + streamerName.substr( std::max(0, (int)streamerName.size() - 12), 12 ) + "'";
        if ( streamer->ipv4 != "127.0.0.1" ) { str += streamer->ipv4 + ":"; }
        str += std::to_string( streamer->port );
        //        if ( !streamViewers.empty() ) {

        //        if ( m_streamName2streamViewers.find( streamerName ) !=
        //        m_streamName2streamViewers.end() ) {
        //            const auto& streamViewers = m_streamName2streamViewers.at( streamerName );
        //            const auto& streamViewers = streamer.m_streamViewers;
        //            if ( !streamViewers.empty() ) {
        const auto nStreamViewer = streamer->getNStreamViewer();
        if ( nStreamViewer != 0 ) { str += "(" + std::to_string( nStreamViewer ) + ")"; }
        //        }
        //        }
        if ( i != m_streamName2streamer.size() - 1 ) str += " ";

        ++i;
    }
    str += "]";
    return str;
}

void Server::addStreamer( server::StreamerClient* streamer ) {

    const auto& streamName = streamer->streamName;

    assert( m_streamName2streamer.find( streamName ) == m_streamName2streamer.end() );
    m_streamName2streamer[streamName] = streamer;

    if ( !m_viewers.empty() ) {
        SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'" );
        m_mtxViewers.lock();
        //        assert(streamer->getInputSensor() != nullptr);
        for ( const auto& viewer : m_viewers ) {
            //            viewer->notifyNewStreamer( streamName,
            //            streamer->getInputSensor()->getSpec()
            //            );
            viewer->notifyNewStreamer( streamName );
        }
        m_mtxViewers.unlock();
    }

    streamer->printStatusMessage( "new streamer" );
}

void Server::newStreamViewer( server::StreamerClient* streamer ) {
    const auto& streamName = streamer->streamName;

   m_mtxPrint.lock();
   std::cout << streamer->headerMsg() << "new stream viewer watching '" << streamName << "'"
             << std::endl;
   streamer->printStatusMessage( "new streamViewer" );
   m_mtxPrint.unlock();
}

// void Server::newInputSensor( server::StreamerClient* streamer ) {
//     const auto& streamName = streamer->getStreamName();

//    SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'" );
//    m_mtxViewers.lock();
//    //    assert(inputSensor);
//    for ( const auto& viewer : m_viewers ) {
//        viewer->notifyNewStreamer( streamName, streamer->getInputSensor()->getSpec() );
//    }
//    m_mtxViewers.unlock();
//}

// void Server::addStreamViewer( server::StreamViewerClient* streamViewer ) {
//     const auto& streamName = streamViewer->m_streamName;

//    //    const auto* streamer = m_streamName2streamer.at( streamName );
//    //    if ( streamer->isPackedStream() ) {
//    //        for ( const auto& packedAcq : streamer->getPackedAcqs() ) {
//    //            streamViewer->update( packedAcq );
//    //        }
//    //    }
//    //    else {
//    //        //        const auto& lastAcq = streamer->getLastAcq();
//    //        //        assert( !lastAcq.isEnd() );
//    //        //        streamViewer->update( lastAcq );
//    //    }

////    m_mtxSreamName2streamViewers.lock();
////    m_streamName2streamViewers[streamName].push_back( streamViewer );
////    m_mtxSreamName2streamViewers.unlock();

//    m_mtxPrint.lock();
//    std::cout << streamViewer->headerMsg() << "new stream viewer watching '" << streamName << "'"
//              << std::endl;
//    streamViewer->printStatusMessage( "new streamViewer" );
//    m_mtxPrint.unlock();
//}


void Server::addViewer( server::ViewerClient* viewer ) {

    // each already connected streamers prevent existence for this new viewer
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#else
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamName = pair.first;
        const auto& streamer   = pair.second;
#endif

        //        const auto* inputSensor = streamer->getInputSensor();
        //        if ( inputSensor != nullptr ) {
        //            assert( streamer->getInputSensor() != nullptr );
        viewer->notifyNewStreamer( streamName );
        //        }
    }

    ////    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
    // #if ( __cplusplus >= 201703L )
    // #else
    // #endif

    ////        if ( streamer->getParent() != "" ) {
    ////        }

    m_mtxViewers.lock();
    m_viewers.push_back( viewer );
    m_mtxViewers.unlock();
}

void Server::delStreamer( server::StreamerClient* streamer ) {

    const std::string streamName = streamer->streamName;
    m_mtxStreamName2streamer.lock();
    assert( !m_streamName2streamer.empty() );
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    m_streamName2streamer.erase( streamName );
    m_mtxStreamName2streamer.unlock();

    //    m_mtxSreamName2streamViewers.lock();
    //    if ( m_streamName2streamViewers.find( streamName ) != m_streamName2streamViewers.end() ) {
    //        auto& streamViewers = m_streamName2streamViewers.at( streamName );
    //        for ( auto& streamViewer : streamViewers ) {
    //            streamViewer->end( io::StreamInterface::ServerMessage::STREAMER_CLOSED );
    //        }
    //        m_mtxSreamName2streamViewers.unlock();

    //        m_mtxSreamName2streamViewers.lock();
    //        int iTry = 0;
    //        while ( !streamViewers.empty() && iTry < 10 ) {
    //            //            SERVER_MSG( "waiting for streamViewers closing : " <<
    //            streamViewers.size()
    //            //                                                               << " still alive"
    //            ); std::cout << headerMsg()
    //                      << "waiting for streamViewers closing : " << streamViewers.size()
    //                      << " still alive" << std::endl;
    //            m_mtxSreamName2streamViewers.unlock();
    //            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //            m_mtxSreamName2streamViewers.lock();
    //            ++iTry;
    //        }
    //        assert( iTry < 10 );
    //        m_streamName2streamViewers.erase( streamName );
    //        //        SERVER_MSG( "streamViewers all closed" );
    //        //    std::cout << headerMsg() << "streamViewers all closed" << std::endl;
    //    }
    //    m_mtxSreamName2streamViewers.unlock();

    m_mtxViewers.lock();
    //    if ( streamer->getInputSensor() != nullptr ) {
    //        assert( streamer->getInputSensor() != nullptr );
    for ( auto* viewer : m_viewers ) {
        //            viewer->notifyDelStreamer( streamName, streamer->getInputSensor()->getSpec()
        //            );
        viewer->notifyDelStreamer( streamName );
    }
    //    }
    m_mtxViewers.unlock();

    //    m_mtxPrint.lock();
    std::cout << streamer->headerMsg() << "end streamer : '" << streamName << "'" << std::endl;
    streamer->printStatusMessage( "del streamer" );
    //    m_mtxPrint.unlock();
}

// void Server::delStreamViewer( server::StreamViewerClient* streamViewer ) {
//     std::cout << streamViewer->headerMsg() << "delStreamViewer() start" << std::endl;
//     const auto& streamName = streamViewer->m_streamName;

////    m_mtxSreamName2streamViewers.lock();
////    auto& streamViewers = m_streamName2streamViewers.at( streamName );
////    if ( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) !=
////         streamViewers.end() ) {
////        streamViewers.remove( streamViewer );
////    }
////    m_mtxSreamName2streamViewers.unlock();

//    //    m_mtxPrint.lock();
//    std::cout << streamViewer->headerMsg() << "end streamViewer watched : '" << streamName << "'"
//              << std::endl;
//    //    m_mtxPrint.unlock();
//    std::cout << streamViewer->headerMsg() << "delStreamViewer() end" << std::endl;
//    streamViewer->printStatusMessage( "del streamViewer" );
//}

void Server::delViewer( server::ViewerClient* viewer ) {

    m_mtxViewers.lock();
    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
    m_mtxViewers.unlock();

    m_mtxPrint.lock();
    viewer->printStatusMessage( "del viewer" );
    m_mtxPrint.unlock();
}

// void Server::newAcquisition( const server::StreamerClient* streamer, const sensor::Acquisition&
// acq ) {
////    assert( !acq.isEnd() );

//    const auto& streamName = streamer->getStreamName();

//    // broadcast acquisition for all streamer
//    ////        const auto& streamer2 = pair.second;
//    ////        streamer2->newAcquisition( streamName, acq );

//    m_mtxSreamName2streamViewers.lock();
//    auto& streamViewers = m_streamName2streamViewers[streamName];
//    auto it             = streamViewers.begin();
//    while ( it != streamViewers.end() ) {
//        auto* streamViewer = *it;
//        try {
//            streamViewer->update( acq );
//            ++it;
//        }
//        catch ( std::exception& ex ) {
//            SERVER_MSG( streamViewer->headerMsg()
//                        << "newAcquisition() catch exception : " << ex.what() );
//            it = streamViewers.erase( it );
//        }
//    }
//    m_mtxSreamName2streamViewers.unlock();
////    assert( !acq.isEnd() );
//}

// std::list<std::pair<std::string, sensor::SensorSpec>> Server::listStreams() const {
//     std::list<std::pair<std::string, sensor::SensorSpec>> ret;
//     m_mtxStreamName2streamer.lock();
// #if ( __cplusplus >= 201703L )
//     for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
// #else
//     for ( const auto& pair : m_streamName2streamer ) {
//         const auto& streamName = pair.first;
//         const auto& streamer   = pair.second;
// #endif

//        const auto* inputSensor = streamer->getInputSensor();

////        if ( inputSensor != nullptr ) {
////            assert( streamer->getInputSensor() != nullptr );
//            const auto& sensorSpec = inputSensor->getSpec();
//            ret.push_back( std::make_pair( streamName, sensorSpec ) );
////        }
//    }
//    m_mtxStreamName2streamer.unlock();
//    return ret;
//}

// sensor::Acquisition Server::getAcquisition( const std::string& streamName ) const {
//     m_mtxStreamName2streamer.lock();
//     assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
//     const auto* streamer = m_streamName2streamer.at( streamName );
//     m_mtxStreamName2streamer.unlock();
//     return streamer->getLastAcq();
// }

void Server::removeClient( server::Client* client ) {
    m_mtxClients.lock();
    assert( std::find( m_clients.begin(), m_clients.end(), client ) != m_clients.end() );
    m_clients.remove( client );
    --m_nActiveClient;
    m_mtxClients.unlock();
}

// const std::map<std::string, server::StreamerClient*>& Server::getStreamers() const {
//     return m_streamName2streamer;
// }

// const SensorSpec& Server::getSensorSpec( const std::string& streamName ) const {
//     assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
//     return m_streamName2streamer.at( streamName )->getInputSensor().getSpec();
// }

// const sensor::InputSensor* Server::getInputSensor( const std::string& streamName ) const {
//     assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
//     return m_streamName2streamer.at( streamName )->getInputSensor();
// }

// void Server::setProperty( const std::string& streamName,
//                           const std::string& objectName,
//                           int property,
//                           const Any& value ) {
//     for ( const auto& viewer : m_viewers ) {
//         viewer->notifyProperty( streamName, objectName, property, value );
//     }
// }

//} // namespace server
} // namespace hub
