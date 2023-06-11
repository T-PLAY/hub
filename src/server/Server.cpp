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
namespace server {

Server::Server() {
    assert( !m_thread.joinable() );
}

Server::Server( int port ) : m_serverSock( port ) {}

Server::~Server() {
    //    m_mtxPrint.lock();
    //    std::cout << headerMsg() << "~Server()" << std::endl;
    //    m_mtxPrint.unlock();
    SERVER_MSG( "~Server() started" );

    assert( m_nClient == 0 || m_nClient == m_maxClients );
    //    if (m_isRunning) {
    assert( m_thread.joinable() );
    m_thread.join();
    //    }

    //    if ( !m_detached ) {
    //    m_mtxClients.lock();

    m_mtxClients.lock();
    for ( auto* client : m_clients ) {
        //        client->setServer( nullptr );
        //        delete client;
        client->end( net::ClientSocket::Message::SERVER_CLOSED );
    }
    m_mtxClients.unlock();

    //    m_mtxClients.unlock();

    //    if ( !m_detached ) {
    m_mtxClients.lock();
    while ( !m_clients.empty() ) {
        SERVER_MSG( "waiting for clients ended (" << m_clients.size() << ")" );
        m_mtxClients.unlock();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        m_mtxClients.lock();
    }
    m_mtxClients.unlock();
    //    }
    //    }

    //    for ( auto& client : m_clients ) {
    //        client->setServer( nullptr );
    //    }
    //    SERVER_MSG( "server ended" );
    SERVER_MSG( "~Server() ended" );
}

void Server::detach() {
    //    assert( false );
    SERVER_MSG( "detach()" );
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
    const std::string str = "\t\033[1m[Server:0]\033[0m ";
    return str;
}

void Server::run() {
    //    m_isRunning = true;
    SERVER_MSG( "listening port " << m_serverSock.getPort() );

    //    int iClient = 0;
    while ( m_nClient < m_maxClients ) {

        hub::net::ClientSocket sock = m_serverSock.waitNewClient();

        SERVER_MSG( "new client" );

        m_mtxClients.lock();
        Client* newClient = initClient( std::move( sock ), ++m_nClient );
        if ( newClient != nullptr ) m_clients.push_back( newClient );
        m_mtxClients.unlock();
    }
    SERVER_MSG( "run() max clients attempt" );
    //    m_isRunning = false;
}

void Server::asyncRun() {
    SERVER_MSG( "asyncRun()" );
    m_thread = std::thread( [this]() { run(); } );
}

// void Server::stop() {}

Client* Server::initClient( hub::net::ClientSocket&& sock, int iClient ) {

    hub::net::ClientSocket::Type clientType;
    sock.read( clientType );

    switch ( clientType ) {

    case hub::net::ClientSocket::Type::STREAMER: {
        std::string streamName;
        sock.read( streamName );
        //        assert( m_server != nullptr );
        const auto& streamers = m_streamName2streamer;
        if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
        }
        else {
            sock.write( hub::net::ClientSocket::Message::FOUND );
            std::cout << headerMsg() << "unable to start new stream, stream name : '" << streamName
                      << "' already exist" << std::endl;
            sock.close();
            return nullptr;
        }
        assert( streamers.find( streamName ) == streamers.end() );
        return new StreamerClient( this, iClient, std::move( sock ), std::move( streamName ) );
    }

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
    for ( const auto& [streamName, streamViewers] : m_streamName2streamViewers ) {
#else
    //    for ( const auto& pair : m_streamName2streamer ) {

    for ( const auto& pair : m_streamName2streamViewers ) {
        const auto& streamName    = pair.first;
        const auto& streamViewers = pair.second;
#endif

        //        const auto& streamViewers = streamer->getStreamViewers();
        //        const auto& streamViewers = m_streamName2streamViewers;
        //        const auto& syncViewers   = streamer->getSyncViewers();

        std::string str = streamName.substr( 0, 3 );
        //        if ( !streamViewers.empty() || !syncViewers.empty() ) {
        if ( !streamViewers.empty() ) {
            //            size_t nbViewers = 0;
            //            for ( const auto& pair : syncViewers ) {
            //                const auto& viewers = pair.second;
            //                nbViewers += viewers.size();
            //            }
            streamViewersStr += "(" + str + "," + std::to_string( streamViewers.size() ) + ")";
            //+ "," +
            //                                std::to_string( syncViewers.size() ) + "," +
            //                                std::to_string( nbViewers ) + ")";

            //            streamViewersStr += ",";
        }
    }
    streamViewersStr += "]";

    std::string str = std::string( "status : nbStreamer:" ) +
                      std::to_string( m_streamName2streamer.size() ) +
                      ", nbViewer:" + std::to_string( m_viewers.size() ) + " " + streamViewersStr;
    return str;
}

void Server::addStreamer( StreamerClient* streamer ) {

    //    std::cout << "[Server] addStreamer(" << streamer << ")" << std::endl;

//    const auto& streamName = streamer->m_streamName;
    const auto& streamName = streamer->getStreamName();

    //    m_mtxStreamers.lock();
    assert( m_streamName2streamer.find( streamName ) == m_streamName2streamer.end() );
    m_streamName2streamer[streamName] = streamer;
    //    m_mtxStreamers.unlock();

    SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'" );
    m_mtxViewers.lock();
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyNewStreamer( streamName, streamer->getInputSensor().getSpec() );
    }
    m_mtxViewers.unlock();

    streamer->printStatusMessage( "new streamer" );
}

void Server::addStreamViewer( StreamViewerClient* streamViewer ) {
    const auto& streamName = streamViewer->m_streamName;

    auto& streamer = m_streamName2streamer.at( streamName );
    if ( streamer->isPackedStream() ) {
        for ( const auto& packedAcq : streamer->getPackedAcqs() ) {
            streamViewer->update( packedAcq );
        }
    }
    else {
        const auto& lastAcq = streamer->getLastAcq();
        assert( !lastAcq.isEmpty() );
//        if (! lastAcq.isEmpty()) {
//        streamViewer->update( streamer->getLastAcq() );
            streamViewer->update( lastAcq );
//        }
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

void Server::addViewer( ViewerClient* viewer ) {

    // each already connected streamers prevent existence for this new viewer
//    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#else
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamName = pair.first;
        const auto& streamer   = pair.second;
#endif

        // notify orphans first
        //        if ( streamer->getParent() == "" ) {
        viewer->notifyNewStreamer( streamName, streamer->getInputSensor().getSpec() );
        //        }
    }

    ////    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
    // #if ( __cplusplus >= 201703L )
    //     for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
    // #else
    //     for ( const auto& pair : m_streamName2streamer ) {
    //         const auto& streamName = pair.first;
    //         const auto& streamer     = pair.second;
    // #endif

    //        // notify children to auto synchronize with already oppened parent
    ////        if ( streamer->getParent() != "" ) {
    //            viewer->notifyNewStreamer( *streamer );
    ////        }
    //    }

    m_mtxViewers.lock();
    m_viewers.push_back( viewer );
    m_mtxViewers.unlock();
}

void Server::delStreamer( StreamerClient* streamer ) {
    //    m_mtxClients.lock();

    //    std::cout << "[Server] delStreamer(" << streamer << ")" << std::endl;

    //    m_mtxStreamers.lock();
    //    int startSize                  = m_streamName2streamer.size();
    const std::string streamName = streamer->getStreamName();
    //    assert(m_streamName2streamer.at(streamName) == streamer);
    m_mtxStreamName2streamer.lock();
    assert( !m_streamName2streamer.empty() );
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    m_streamName2streamer.erase( streamName );
    m_mtxStreamName2streamer.unlock();
    //    int endSize = m_streamName2streamer.size();
    //    assert( startSize - endSize == 1 );
    //    m_mtxStreamers.unlock();

    m_mtxSreamName2streamViewers.lock();
    if ( m_streamName2streamViewers.find( streamName ) != m_streamName2streamViewers.end() ) {
        auto& streamViewers = m_streamName2streamViewers.at( streamName );
        for ( auto& streamViewer : streamViewers ) {
            streamViewer->end( net::ClientSocket::Message::STREAMER_CLOSED );
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
        m_mtxSreamName2streamViewers.unlock();
    }

    m_mtxViewers.lock();
    for ( auto* viewer : m_viewers ) {
        viewer->notifyDelStreamer( streamName, streamer->getInputSensor().getSpec() );
    }
    m_mtxViewers.unlock();

    m_mtxPrint.lock();
    std::cout << streamer->headerMsg() << "end streamer : '" << streamName << "'" << std::endl;
    streamer->printStatusMessage( "del streamer" );
    m_mtxPrint.unlock();
    //    m_mtxClients.unlock();
}

void Server::delStreamViewer( StreamViewerClient* streamViewer ) {
    std::cout << streamViewer->headerMsg() << "delStreamViewer() start" << std::endl;
    //    m_mtxClients.lock();
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
    //    m_mtxClients.unlock();
    std::cout << streamViewer->headerMsg() << "delStreamViewer() end" << std::endl;
}

void Server::delViewer( ViewerClient* viewer ) {
    //    m_mtxClients.lock();

    m_mtxViewers.lock();
    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
    m_mtxViewers.unlock();

    m_mtxPrint.lock();
    viewer->printStatusMessage( "del viewer" );
    m_mtxPrint.unlock();

    //    m_mtxClients.lock();
}

void Server::newAcquisition( StreamerClient* streamer, const Acquisition& acq ) {
    assert( !acq.isEmpty() );

//    const auto& streamName = streamer->m_streamName;
    const auto& streamName = streamer->getStreamName();

    // broadcast acquisition for all streamer
    //    for ( auto& streamViewer : m_streamName2streamViewers[streamName] ) {
    ////        const auto& streamer2 = pair.second;
    //        streamViewer->update(acq);
    ////        streamer2->newAcquisition( streamName, acq );
    //    }

    m_mtxSreamName2streamViewers.lock();
    auto& streamViewers = m_streamName2streamViewers[streamName];
    auto it             = streamViewers.begin();
    while ( it != streamViewers.end() ) {
        auto* streamViewer = *it;
        try {
            streamViewer->update( acq );
            //            *( streamViewer->m_outputSensor ) << acq;
            ++it;
        }
        catch ( std::exception& ex ) {
            SERVER_MSG( streamViewer->headerMsg()
                        << "newAcquisition() catch exception : " << ex.what() );
            //            std::cout << streamViewer->headerMsg()
            //                      << "update(Acquisition) : catch outputSensor exception : " <<
            //                      ex.what()
            //                      << std::endl;
            //            std::cout << streamViewer->headerMsg() << "update(Acquisition) failed with
            //            stream '"
            //                      << streamName << "'" << std::endl;
            //            delStreamViewer(streamViewer);
            it = streamViewers.erase( it );
            //            delete streamViewer;
        }
    }
    m_mtxSreamName2streamViewers.unlock();
    assert( !acq.isEmpty() );
}

std::list<std::pair<std::string, hub::SensorSpec>> Server::listStreams() const {
    std::list<std::pair<std::string, hub::SensorSpec>> ret;
    //    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
    m_mtxStreamName2streamer.lock();
#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#else
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamName = pair.first;
        const auto& streamer   = pair.second;
#endif

        const auto& sensorSpec = streamer->getInputSensor().getSpec();
        ret.push_back( std::make_pair( streamName, sensorSpec ) );
    }
    m_mtxStreamName2streamer.unlock();
    return ret;
}

// const hub::SensorSpec& Server::getSensorSpec( const std::string& streamName ) {
//}

hub::Acquisition Server::getAcquisition( const std::string& streamName ) const {
    m_mtxStreamName2streamer.lock();
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    const auto& streamer = m_streamName2streamer.at( streamName );
    m_mtxStreamName2streamer.unlock();
    //    return hub::Acquisition();
//    assert( !streamer->m_lastAcq.isEmpty() );
    //    return streamer->getLastAcq( "" );
//    return streamer->m_lastAcq;
    return streamer->getLastAcq();
}

void Server::removeClient( Client* client ) {
    m_mtxClients.lock();
    assert( std::find( m_clients.begin(), m_clients.end(), client ) != m_clients.end() );
    m_clients.remove( client );
    m_mtxClients.unlock();
}

const std::map<std::string, StreamerClient*>& Server::getStreamers() const {
    return m_streamName2streamer;
}

const SensorSpec& Server::getSensorSpec( const std::string& streamName ) const {
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
//    return m_streamName2streamer.at( streamName )->m_inputSensor->getSpec();
    return m_streamName2streamer.at( streamName )->getInputSensor().getSpec();
}

void Server::setProperty(const std::string &streamName, const std::string &objectName, int property, const Any &value)
{
    for ( const auto& viewer : m_viewers ) {
        viewer->notifyProperty( streamName, objectName, property, value);
    }
}

} // namespace server
} // namespace hub
