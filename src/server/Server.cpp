#include "Server.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <Sensor.hpp>

Server::Server() {
    assert( !m_thread.joinable() );
}

Server::Server( int port ) : m_serverSock( port ) {}

Server::~Server() {
    std::cout << headerMsg() << "~Server()" << std::endl;
    assert( m_thread.joinable() );
    m_thread.join();

    for ( auto& client : m_clients ) {
        client->setServer( nullptr );
    }
}

std::string Server::headerMsg() const {
    const std::string str = "\t\033[1m[Server:0]\033[0m ";
    return str;
}

void Server::run() {
    std::cout << headerMsg() << "listening port " << m_serverSock.getPort() << std::endl;

    int iClient = 0;
    while ( iClient < m_maxClients ) {

        hub::net::ClientSocket sock = m_serverSock.waitNewClient();

        std::cout << headerMsg() << "new client" << std::endl;

        Client* newClient = initClient( std::move( sock ), ++iClient );
        m_clients.push_back( newClient );
    }
    std::cout << headerMsg() << "run() max clients attempt" << std::endl;
}

void Server::asyncRun() {
    std::cout << headerMsg() << "asyncRun()" << std::endl;
    m_thread = std::thread( [this]() { run(); } );
}

void Server::stop() {}

Client* Server::initClient( hub::net::ClientSocket&& sock, int iClient ) {

    hub::net::ClientSocket::Type clientType;
    sock.read( clientType );

    switch ( clientType ) {

    case hub::net::ClientSocket::Type::STREAMER:
        return new StreamerClient( this, iClient, std::move( sock ) );

    case hub::net::ClientSocket::Type::VIEWER:
        return new ViewerClient( this, iClient, std::move( sock ) );

    case hub::net::ClientSocket::Type::STREAM_VIEWER:
        return new StreamViewerClient( this, iClient, std::move( sock ) );

    case hub::net::ClientSocket::Type::ASKER:
        return new AskerClient( this, iClient, std::move( sock ) );

    default:
        assert( false );
        return nullptr;
    }
}

void Server::setMaxClients( int maxClients ) {
    m_maxClients = maxClients;
}

std::string Server::getStatus() {
    std::string streamViewersStr = "[";

#if ( __cplusplus >= 201703L )
    for ( const auto& [streamerName, streamer] : m_streamers ) {
#else
    for ( const auto& pair : m_streamers ) {
        const auto & streamerName = pair.first;
        const auto & streamer = pair.second;
#endif

        const auto& streamViewers = streamer->getStreamViewers();
        const auto& syncViewers   = streamer->getSyncViewers();

        std::string str = streamerName.substr( 0, 3 );
        if ( !streamViewers.empty() || !syncViewers.empty() ) {
            size_t nbViewers = 0;
            for ( const auto& pair : syncViewers ) {
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
    return str;
}

void Server::addStreamer( StreamerClient* streamer ) {

//    std::cout << "[Server] addStreamer(" << streamer << ")" << std::endl;

    const auto& streamerName = streamer->getStreamName();

    m_mtxStreamers.lock();
    assert( m_streamers.find( streamerName ) == m_streamers.end() );
    m_streamers[streamerName] = streamer;
    m_mtxStreamers.unlock();

    std::cout << headerMsg() << "prevent viewers there is a new streamer : '" << streamerName << "'"
              << std::endl;
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyNewStreamer( *streamer );
    }
}

void Server::delStreamer( StreamerClient* streamer ) {

//    std::cout << "[Server] delStreamer(" << streamer << ")" << std::endl;

    m_mtxStreamers.lock();
    int startSize = m_streamers.size();
    const std::string streamerName = streamer->getStreamName();
//    assert(m_streamers.at(streamerName) == streamer);
    assert(! m_streamers.empty());
    assert( m_streamers.find( streamerName ) != m_streamers.end() );
    m_streamers.erase( streamerName );
    int endSize = m_streamers.size();
    assert(startSize - endSize == 1);
    m_mtxStreamers.unlock();

    for ( auto* viewer : m_viewers ) {
        viewer->notifyDelStreamer( *streamer );
    }
}

void Server::addViewer( ViewerClient* viewer ) {

    // each already connected streamers prevent existence for this new viewer
//    for ( const auto& [streamerName, streamer] : m_streamers ) {
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamerName, streamer] : m_streamers ) {
#else
    for ( const auto& pair : m_streamers ) {
        const auto & streamerName = pair.first;
        const auto & streamer = pair.second;
#endif

        // notify orphans first
        if ( streamer->getParent() == "" ) { viewer->notifyNewStreamer( *streamer ); }
    }

//    for ( const auto& [streamerName, streamer] : m_streamers ) {
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamerName, streamer] : m_streamers ) {
#else
    for ( const auto& pair : m_streamers ) {
        const auto & streamerName = pair.first;
        const auto & streamer = pair.second;
#endif

        // notify children to auto synchronize with already oppened parent
        if ( streamer->getParent() != "" ) { viewer->notifyNewStreamer( *streamer ); }
    }

    m_viewers.push_back( viewer );
}

void Server::delViewer( ViewerClient* viewer ) {

    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
}

void Server::newAcquisition( StreamerClient* streamer, const hub::Acquisition& acq ) {

    const auto& streamerName = streamer->getStreamName();

    // broadcast acquisition for all streamer
    for ( const auto& pair : m_streamers ) {
        const auto& streamer2 = pair.second;

        streamer2->newAcquisition( streamerName, acq );
    }
}

std::list<std::pair<std::string, hub::SensorSpec>> Server::listStreams() const {
    std::list<std::pair<std::string, hub::SensorSpec>> ret;
//    for ( const auto& [streamName, streamer] : m_streamers ) {
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamerName, streamer] : m_streamers ) {
#else
    for ( const auto& pair : m_streamers ) {
        const auto & streamerName = pair.first;
        const auto & streamer = pair.second;
#endif

        const auto& sensorSpec = streamer->getInputSensor().m_spec;
        ret.push_back( std::make_pair( streamerName, sensorSpec ) );
    }
    return ret;
}

// const hub::SensorSpec& Server::getSensorSpec( const std::string& streamName ) {
//}

const std::shared_ptr<hub::Acquisition> Server::getAcquisition( const std::string& streamName ) {
    assert( m_streamers.find( streamName ) != m_streamers.end() );
    const auto& streamer = m_streamers.at( streamName );
    return streamer->getLastAcq( "" );
}

void Server::removeClient( Client* client ) {
    assert( std::find( m_clients.begin(), m_clients.end(), client ) != m_clients.end() );
    m_clients.remove( client );
}

const std::map<std::string, StreamerClient*>& Server::getStreamers() const {
    return m_streamers;
}
