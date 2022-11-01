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

    for ( const auto& pair : m_streamers ) {
        const auto& streamerName = pair.first;
        const auto& streamer     = pair.second;

        auto& streamViewers     = m_streamViewers[streamerName];
        const auto& syncViewers = streamer->getSyncViewers();

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

    const auto& streamerName = streamer->getStreamName();
    assert( m_streamers.find( streamerName ) == m_streamers.end() );
    m_streamers[streamerName] = streamer;

    // prevent all viewer there is a new streamer
    std::cout << headerMsg() << "prevent viewers there is a new streamer" << std::endl;
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyNewStreamer( *streamer );
    }
}

void Server::addStreamViewer( StreamViewerClient* streamViewer ) {
    const auto& syncStreamName = streamViewer->getSyncStreamName();
    const auto& streamName     = streamViewer->getStreamName();

    if ( syncStreamName == "" ) {
        auto& streamViewers = m_streamViewers[streamName];
        assert( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) ==
                streamViewers.end() );
        streamViewers.push_back( streamViewer );
    }
    else {
        assert( m_streamers.find( syncStreamName ) != m_streamers.end() );
        auto& syncStreamer = *m_streamers.at( syncStreamName );

        syncStreamer.addSyncStreamViewer( streamViewer );
    }
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

    assert( m_streamers.find( streamerName ) != m_streamers.end() );
    m_streamers.erase( m_streamers.find( streamerName ) );

    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "del streamer" << getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;

    delete streamer;
    m_mtxStreamers.unlock();
}

void Server::delStreamViewer( StreamViewerClient* streamViewer ) {

    const auto& syncStreamName = streamViewer->getSyncStreamName();
    const auto& streamName     = streamViewer->getStreamName();

    if ( syncStreamName == "" ) {
        assert( m_streamViewers.find( streamName ) != m_streamViewers.end() );
        auto& streamViewers = m_streamViewers.at( streamName );
        streamViewers.remove( streamViewer );
    }
    else {
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
}

void Server::delViewer( ViewerClient* viewer ) {
    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
              << "del viewer" << getStatus() << std::endl;
    std::cout << "-------------------------------------------------------------------------"
                 "--------------------"
              << std::endl;
}

void Server::newAcquisition( StreamerClient* streamer, const hub::Acquisition& acq ) {

    const auto& streamerName = streamer->getStreamName();

    // broadcast acquisition for each live viewer (no sync)
    auto streamViewers = m_streamViewers[streamerName];
    for ( const auto& streamViewer : streamViewers ) {
        assert( streamViewer->getSyncStreamName() == "" );
        streamViewer->update( acq );
    }

    // broadcast acquisition for each synchronize stream
    for ( const auto& pair : m_streamers ) {
        const auto& streamer = pair.second;

        streamer->newAcquisition( streamerName, acq );
    }
}

// const std::vector<std::shared_ptr<hub::Acquisition>>&
// Server::getLastAcqs( const std::string& streamName ) {
// }

void Server::setAcqPing( bool newAcqPing ) {
    m_acqPing = newAcqPing;
}

const std::map<std::string, StreamerClient*>& Server::getStreamers() const {
    return m_streamers;
}

Server::Server() {
    assert( !m_thread.joinable() );
}

Server::Server( int port ) : mServerSock( port ) {}

Server::~Server() {
    std::cout << headerMsg() << "~Server()" << std::endl;
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
        return new StreamerClient( *this, iClient, std::move( sock ) );
    case hub::net::ClientSocket::Type::VIEWER:
        return new ViewerClient( *this, iClient, std::move( sock ) );
    case hub::net::ClientSocket::Type::STREAM_VIEWER:
        return new StreamViewerClient( *this, iClient, std::move( sock ) );
    default:
        assert( false );
        return nullptr;
    }
}

void Server::setMaxClients( int maxThreads ) {
    m_maxClients = maxThreads;
}

//////////////////////////////////////// old ///////////////////////////////////

//#ifdef WIN32
//#else
//#endif

//}

// std::string Server::getStatus() const {
///+ /                            "," + std::to_string( streamer->mSensor2syncViewers.size() ) +
///")";

//}

// void Viewer::notifyNewStreamer( const Streamer& streamer ) const {

//}
