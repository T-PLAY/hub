#include "Server.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <Sensor.hpp>

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

Server::Server( int port ) : m_serverSock( port ) {}

Server::~Server() {
    SERVER_MSG( "~Server() started" );

    //    assert( m_nClient == 0 || m_nClient == m_maxClients );
    if ( m_thread != nullptr ) {
        assert( m_thread->joinable() );
        m_thread->join();
    }

    SERVER_MSG( "~Server() ending connected clients" );
    m_mtxClients.lock();
    for ( auto* client : m_clients ) {
        client->end( io::StreamInterface::ServerMessage::SERVER_CLOSED );
    }
    m_mtxClients.unlock();
    SERVER_MSG( "~Server() connected clients ended" );

    m_mtxClients.lock();
    while ( !m_clients.empty() ) {
        SERVER_MSG( "waiting for clients ended (" << m_clients.size() << ")" );
        m_mtxClients.unlock();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        m_mtxClients.lock();
    }
    m_mtxClients.unlock();
    SERVER_MSG( "~Server() clients disconnected" );

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

    SERVER_MSG( "~Server() ended" );
    assert( m_nActiveClient == 0 );
    assert( m_clients.empty() );
    assert( m_nClient <= m_maxClients );
}

void Server::detach() {
    SERVER_MSG( "detach()" );
    assert( m_running );
    assert( !m_detached );
    m_detached = true;
    m_mtxClients.lock();
    for ( auto& client : m_clients ) {
        client->setServer( nullptr );
    }
    m_clients.clear();
    m_mtxClients.unlock();
}

std::string Server::headerMsg() const {
    const std::string str = "\t\033[1m[Server:0/" + std::to_string( m_nActiveClient ) + "]\033[0m ";
    return str;
}

void Server::run() {
    assert( !m_running );
    m_running = true;
    SERVER_MSG( "listening port " << m_serverSock.getPort() );

    while ( m_nClient < m_maxClients && !m_killed ) {

        hub::net::ClientSocket sock = m_serverSock.waitNewClient();

        ++m_nActiveClient;
        SERVER_MSG( "new client" );

        m_mtxClients.lock();
        server::Client* newClient = initClient( std::move( sock ), ++m_nClient );
        if ( newClient == nullptr ) { --m_nActiveClient; }
        else {
            m_clients.push_back( newClient );
            SERVER_MSG( "new client inited" );
        }
        m_mtxClients.unlock();
    }
    if ( !m_killed ) { SERVER_MSG( "run() max clients attempt" ); }
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    m_running = false;
}

void Server::asyncRun() {
    assert( !m_running );
    assert( m_thread == nullptr );
    SERVER_MSG( "asyncRun()" );
    m_thread = new std::thread( [this]() { run(); } );
}

// void Server::stop() {}

server::Client* Server::initClient( hub::net::ClientSocket&& sock, int iClient ) {

//    hub::net::ClientSocket::Type clientType;
    io::StreamInterface::ClientType clientType;
    sock.read( clientType );

    switch ( clientType ) {
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    case hub::net::ClientSocket::Type::STREAMER: {
    case io::StreamInterface::ClientType::STREAMER: {
        std::string streamName;
        sock.read( streamName );
        const auto& streamers = m_streamName2streamer;
        if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( hub::io::StreamInterface::ServerMessage::NOT_FOUND );
        }
        else {
            sock.write( hub::io::StreamInterface::ServerMessage::FOUND );
            std::cout << headerMsg() << "unable to start new stream, stream name : '" << streamName
                      << "' already exist" << std::endl;
            sock.close();
            return nullptr;
        }
        assert( streamers.find( streamName ) == streamers.end() );
        return new server::StreamerClient(
            this, iClient, std::move( sock ), std::move( streamName ) );
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    case hub::net::ClientSocket::Type::VIEWER:
    case io::StreamInterface::ClientType::VIEWER:
        return new server::ViewerClient( this, iClient, std::move( sock ) );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    case hub::net::ClientSocket::Type::STREAM_VIEWER: {
    case io::StreamInterface::ClientType::STREAM_VIEWER: {
        //        assert( m_server != nullptr );
        m_mtxStreamName2streamer.lock();
        const auto& streamers = m_streamName2streamer;

        server::StreamViewerClient* ret = nullptr;

        std::string streamName;
        sock.read( streamName );
        if ( streamName == "killServer" ) {
            std::cout << headerMsg() << "killing server by peer" << std::endl;
            sock.close();
            m_killed = true;
            //            return nullptr;
        }
        else if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( hub::io::StreamInterface::ServerMessage::NOT_FOUND );
            std::cout << headerMsg() << "unable to reach stream, stream name : '" << streamName
                      << "' not found" << std::endl;
            //            std::thread( [this]() { delete this; } ).detach();
            sock.close();
            //            return nullptr;
        }
        else {
            sock.write( hub::io::StreamInterface::ServerMessage::OK );
            assert( streamers.find( streamName ) != streamers.end() );
            ret = new server::StreamViewerClient(
                this, iClient, std::move( sock ), std::move( streamName ) );
        }

        m_mtxStreamName2streamer.unlock();
        return ret;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    case hub::net::ClientSocket::Type::ASKER:
    case io::StreamInterface::ClientType::ASKER:
        return new server::AskerClient( this, iClient, std::move( sock ) );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    default:
        assert( false );
        return nullptr;
    }
}

void Server::setMaxClients( int maxClients ) {
    m_maxClients = maxClients;
    SERVER_MSG( "setting max clients: " << m_maxClients );
}

void Server::printStatus() const {
    std::cout << headerMsg() << getStatus() << std::endl;
}

std::string Server::getStatus() const {
    std::string streamViewersStr = "[";

#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamViewers] : m_streamName2streamViewers ) {
#else

    for ( const auto& pair : m_streamName2streamViewers ) {
        const auto& streamName    = pair.first;
        const auto& streamViewers = pair.second;
#endif

        std::string str = streamName.substr( 0, 3 );
        if ( !streamViewers.empty() ) {
            streamViewersStr += "(" + str + "," + std::to_string( streamViewers.size() ) + ")";
        }
    }
    streamViewersStr += "]";

    std::string str = std::string( "status : nbStreamer:" ) +
                      std::to_string( m_streamName2streamer.size() ) +
                      ", nbViewer:" + std::to_string( m_viewers.size() ) + " " + streamViewersStr
        //                      +
        //                      ", nClient:" + std::to_string(m_nActiveClient);
        ;
    return str;
}

void Server::addStreamer( server::StreamerClient* streamer ) {

    const auto& streamName = streamer->getStreamName();

    assert( m_streamName2streamer.find( streamName ) == m_streamName2streamer.end() );
    m_streamName2streamer[streamName] = streamer;

    //    SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'" );
    //    m_mtxViewers.lock();
    //    assert(streamer->getInputSensor() != nullptr);
    //    for ( const auto& viewer : m_viewers ) {
    //        viewer->notifyNewStreamer( streamName, streamer->getInputSensor()->getSpec() );
    //    }
    //    m_mtxViewers.unlock();

    streamer->printStatusMessage( "new streamer" );
}

void Server::newInputSensor( server::StreamerClient* streamer ) {
    const auto& streamName = streamer->getStreamName();

    SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'" );
    m_mtxViewers.lock();
    //    assert(inputSensor);
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyNewStreamer( streamName, streamer->getInputSensor()->getSpec() );
    }
    m_mtxViewers.unlock();
}

void Server::addStreamViewer( server::StreamViewerClient* streamViewer ) {
    const auto& streamName = streamViewer->m_streamName;

    const auto* streamer = m_streamName2streamer.at( streamName );
    if ( streamer->isPackedStream() ) {
        for ( const auto& packedAcq : streamer->getPackedAcqs() ) {
            streamViewer->update( packedAcq );
        }
    }
    else {
        //        const auto& lastAcq = streamer->getLastAcq();
        //        assert( !lastAcq.isEmpty() );
        //        streamViewer->update( lastAcq );
    }

    m_mtxSreamName2streamViewers.lock();
    m_streamName2streamViewers[streamName].push_back( streamViewer );
    m_mtxSreamName2streamViewers.unlock();

    m_mtxPrint.lock();
    std::cout << streamViewer->headerMsg() << "new stream viewer watching '" << streamName << "'"
              << std::endl;
    streamViewer->printStatusMessage( "new streamViewer" );
    m_mtxPrint.unlock();
}

void Server::addViewer( server::ViewerClient* viewer ) {

    // each already connected streamers prevent existence for this new viewer
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#else
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamName = pair.first;
        const auto& streamer   = pair.second;
#endif

        assert( streamer->getInputSensor() != nullptr );
        // notify orphans first
        viewer->notifyNewStreamer( streamName, streamer->getInputSensor()->getSpec() );
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

    const std::string streamName = streamer->getStreamName();
    m_mtxStreamName2streamer.lock();
    assert( !m_streamName2streamer.empty() );
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    m_streamName2streamer.erase( streamName );
    m_mtxStreamName2streamer.unlock();

    m_mtxSreamName2streamViewers.lock();
    if ( m_streamName2streamViewers.find( streamName ) != m_streamName2streamViewers.end() ) {
        auto& streamViewers = m_streamName2streamViewers.at( streamName );
        for ( auto& streamViewer : streamViewers ) {
            streamViewer->end( io::StreamInterface::ServerMessage::STREAMER_CLOSED );
        }
        m_mtxSreamName2streamViewers.unlock();

        m_mtxSreamName2streamViewers.lock();
        while ( !streamViewers.empty() ) {
            SERVER_MSG( "waiting for streamViewers closing : " << streamViewers.size()
                                                               << " still alive" );
            m_mtxSreamName2streamViewers.unlock();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            m_mtxSreamName2streamViewers.lock();
        }
        SERVER_MSG( "streamViewers all closed" );
    }
    m_mtxSreamName2streamViewers.unlock();

    m_mtxViewers.lock();
    if ( streamer->getInputSensor() != nullptr ) {
        assert( streamer->getInputSensor() != nullptr );
        for ( auto* viewer : m_viewers ) {
            viewer->notifyDelStreamer( streamName, streamer->getInputSensor()->getSpec() );
        }
    }
    m_mtxViewers.unlock();

    m_mtxPrint.lock();
    std::cout << streamer->headerMsg() << "end streamer : '" << streamName << "'" << std::endl;
    streamer->printStatusMessage( "del streamer" );
    m_mtxPrint.unlock();
}

void Server::delStreamViewer( server::StreamViewerClient* streamViewer ) {
    std::cout << streamViewer->headerMsg() << "delStreamViewer() start" << std::endl;
    const auto& streamName = streamViewer->m_streamName;

    m_mtxSreamName2streamViewers.lock();
    auto& streamViewers = m_streamName2streamViewers.at( streamName );
    if ( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) !=
         streamViewers.end() ) {
        streamViewers.remove( streamViewer );
    }
    m_mtxSreamName2streamViewers.unlock();

    m_mtxPrint.lock();
    std::cout << streamViewer->headerMsg() << "end streamViewer watched : '" << streamName << "'"
              << std::endl;
    streamViewer->printStatusMessage( "del streamViewer" );
    m_mtxPrint.unlock();
    std::cout << streamViewer->headerMsg() << "delStreamViewer() end" << std::endl;
}

void Server::delViewer( server::ViewerClient* viewer ) {

    m_mtxViewers.lock();
    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
    m_mtxViewers.unlock();

    m_mtxPrint.lock();
    viewer->printStatusMessage( "del viewer" );
    m_mtxPrint.unlock();
}

void Server::newAcquisition( const server::StreamerClient* streamer, const Acquisition& acq ) {
    assert( !acq.isEmpty() );

    const auto& streamName = streamer->getStreamName();

    // broadcast acquisition for all streamer
    ////        const auto& streamer2 = pair.second;
    ////        streamer2->newAcquisition( streamName, acq );

    m_mtxSreamName2streamViewers.lock();
    auto& streamViewers = m_streamName2streamViewers[streamName];
    auto it             = streamViewers.begin();
    while ( it != streamViewers.end() ) {
        auto* streamViewer = *it;
        try {
            streamViewer->update( acq );
            ++it;
        }
        catch ( std::exception& ex ) {
            SERVER_MSG( streamViewer->headerMsg()
                        << "newAcquisition() catch exception : " << ex.what() );
            it = streamViewers.erase( it );
        }
    }
    m_mtxSreamName2streamViewers.unlock();
    assert( !acq.isEmpty() );
}

std::list<std::pair<std::string, hub::SensorSpec>> Server::listStreams() const {
    std::list<std::pair<std::string, hub::SensorSpec>> ret;
    m_mtxStreamName2streamer.lock();
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#else
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamName = pair.first;
        const auto& streamer   = pair.second;
#endif

        assert( streamer->getInputSensor() != nullptr );
        const auto& sensorSpec = streamer->getInputSensor()->getSpec();
        ret.push_back( std::make_pair( streamName, sensorSpec ) );
    }
    m_mtxStreamName2streamer.unlock();
    return ret;
}

hub::Acquisition Server::getAcquisition( const std::string& streamName ) const {
    m_mtxStreamName2streamer.lock();
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    const auto* streamer = m_streamName2streamer.at( streamName );
    m_mtxStreamName2streamer.unlock();
    return streamer->getLastAcq();
}

void Server::removeClient( server::Client* client ) {
    m_mtxClients.lock();
    assert( std::find( m_clients.begin(), m_clients.end(), client ) != m_clients.end() );
    m_clients.remove( client );
    --m_nActiveClient;
    m_mtxClients.unlock();
}

const std::map<std::string, server::StreamerClient*>& Server::getStreamers() const {
    return m_streamName2streamer;
}

// const SensorSpec& Server::getSensorSpec( const std::string& streamName ) const {
//     assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
//     return m_streamName2streamer.at( streamName )->getInputSensor().getSpec();
// }

const InputSensor* Server::getInputSensor( const std::string& streamName ) const {
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    return m_streamName2streamer.at( streamName )->getInputSensor();
}

void Server::setProperty( const std::string& streamName,
                          const std::string& objectName,
                          int property,
                          const Any& value ) {
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyProperty( streamName, objectName, property, value );
    }
}

//} // namespace server
} // namespace hub
