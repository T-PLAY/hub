#include "ServerImpl2.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "io/Stream.hpp"

#define SERVER_MSG( str )                             \
    do {                                              \
        m_mtxPrint.lock();                            \
        std::cout << headerMsg() << str << std::endl; \
        m_mtxPrint.unlock();                          \
    } while ( false )

namespace hub {
namespace server {

ServerImpl2::ServerImpl2( int port ) : m_serverSock( port ), m_givingPort( port + 1 ) {}

ServerImpl2::~ServerImpl2() {
    // SERVER_MSG( "~ServerImpl2() started" );

    stop();
    SERVER_MSG( "closing port " << m_serverSock );
    SERVER_MSG( "stoping server" );
    // SERVER_MSG( "~ServerImpl2() ended" );
}

std::string ServerImpl2::headerMsg() const {
    const std::string str = "\t\033[1m[Server:" + std::to_string( m_iClient ) + "/" +
                            std::to_string( m_nActiveClient ) + "]\033[0m ";
    return str;
}

void ServerImpl2::run() {
    SERVER_MSG( "starting server" );
    m_killed = false;
    assert( !m_killed );
    SERVER_MSG( "listening port " << m_serverSock.getPort() );

    while ( !m_killed ) {

        assert( m_serverSock.isConnected() );
        net::ClientSocket sock = m_serverSock.waitNewClient();

        ++m_nActiveClient;
        SERVER_MSG( "new client" );

        m_mtxClients.lock();
        Client2* newClient = initClient( io::InputOutputSocket{std::move( sock )}, ++m_iClient );
        if ( newClient == nullptr ) { --m_nActiveClient; }
        else {
            m_clients.push_back( newClient );
            SERVER_MSG( "new client inited" );
            newClient->notifyInited();
        }
        m_mtxClients.unlock();
    }
    m_running = false;
}

void ServerImpl2::asyncRun() {
    assert( !m_running );
    assert( m_thread == nullptr );
    m_running = true;
    m_thread  = new std::thread( [this]() { run(); } );
}

void ServerImpl2::stop() {
    SERVER_MSG( "stopping server" );
    if ( m_thread != nullptr ) {
        assert( m_thread->joinable() );
        if ( m_running ) { exitMainThread(); }
        m_thread->join();
    }
    m_thread = nullptr;
    assert( !m_running );

    if ( !m_clients.empty() ) {
        SERVER_MSG( "ending connected clients" );
        m_mtxClients.lock();
        for ( auto* client : m_clients ) {
            client->end( hub::io::StreamBase::ServerMessage::SERVER_CLOSED );
        }
        m_mtxClients.unlock();
    }

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

    assert( m_nActiveClient == 0 );
    assert( m_clients.empty() );
}

void ServerImpl2::exitMainThread() {
    m_killed = true;
    io::Stream::stopServer( "127.0.0.1", m_serverSock.getPort() );
}

Client2* ServerImpl2::initClient( hub::io::InputOutputSocket&& sock, int iClient ) {

    hub::io::StreamBase::ClientType clientType;
    sock.read( clientType );

    switch ( clientType ) {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case hub::io::StreamBase::ClientType::STREAMER: {
        std::string streamName;
        sock.read( streamName );
        const auto& streamers = m_streamName2streamer;
        if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( hub::io::StreamBase::ServerMessage::NOT_FOUND );
        }
        else {
            sock.write( hub::io::StreamBase::ServerMessage::FOUND );
            std::cout << headerMsg() << "unable to start new stream, stream name : '" << streamName
                      << "' already exist" << std::endl;
            sock.close();
            return nullptr;
        }
        assert( streamers.find( streamName ) == streamers.end() );
        std::string streamIpv4 = sock.getIpv4();
        int streamPort;
        sock.read( streamPort );
        streamPort = ( streamPort == 0 ) ? m_givingPort++ : streamPort;
        sock.write( streamPort );
        return new StreamerClient2( this,
                                    iClient,
                                    std::move( sock ),
                                    std::move( streamName ),
                                    std::move( streamIpv4 ),
                                    streamPort );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case hub::io::StreamBase::ClientType::VIEWER: {
        return new ViewerClient2( this, iClient, std::move( sock ) );
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case hub::io::StreamBase::ClientType::STREAM_VIEWER: {
        m_mtxStreamName2streamer.lock();
        const auto& streamers = m_streamName2streamer;

        std::string streamName;
        sock.read( streamName );
        if ( streamers.find( streamName ) == streamers.end() ) {
            sock.write( hub::io::StreamBase::ServerMessage::NOT_FOUND );
            std::cout << headerMsg() << "unable to reach stream, stream name : '" << streamName
                      << "' not found" << std::endl;
            sock.close();
        }
        else {
            sock.write( hub::io::StreamBase::ServerMessage::OK );
            const auto& streamer   = streamers.find( streamName )->second;
            const auto& streamIpv4 = streamer->m_streamIpv4;
            const auto& streamPort = streamer->m_streamPort;

            sock.write( streamIpv4 );
            sock.write( streamPort );
        }
        m_mtxStreamName2streamer.unlock();
        return nullptr;
    }

    case hub::io::StreamBase::ClientType::KILLER: {
        sock.close();
        if ( m_killed ) { std::cout << headerMsg() << "killing in the name" << std::endl; }
        else {
            std::cout << headerMsg() << "killing server by peer" << std::endl;
            m_killed = true;
        }
        return nullptr;
    }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    default:
        assert( false );
        return nullptr;
    }
}

void ServerImpl2::printStatus() const {
    std::cout << headerMsg() << getStatus() << std::endl;
}

bool ServerImpl2::running() const {
    return m_running;
}

int ServerImpl2::nStreamer() const {
    return m_streamName2streamer.size();
}

int ServerImpl2::nClient() const {
    return m_clients.size();
}

std::string ServerImpl2::getStatus() const {

    std::string str = "viewer:" + std::to_string( m_viewers.size() ) + ", streamer:[";

    size_t i = 0;
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamerName = pair.first;
        const auto& streamer     = pair.second;

        str += "'" + streamerName.substr( std::max( 0, (int)streamerName.size() - 12 ), 12 ) + "'";
        if ( streamer->m_streamIpv4 != "127.0.0.1" ) { str += streamer->m_streamIpv4 + ":"; }
        str += std::to_string( streamer->m_streamPort );

        const auto nStreamViewer = streamer->getNStreamViewer();
        if ( nStreamViewer != 0 ) { str += "(" + std::to_string( nStreamViewer ) + ")"; }
        if ( i != m_streamName2streamer.size() - 1 ) str += " ";

        ++i;
    }
    str += "]";
    return str;
}

void ServerImpl2::addStreamer( StreamerClient2* streamer ) {

    const auto& streamName = streamer->m_streamName;

    assert( m_streamName2streamer.find( streamName ) == m_streamName2streamer.end() );
    m_streamName2streamer[streamName] = streamer;

    streamer->printStatusMessage( "new streamer" );

    if ( !m_viewers.empty() ) {

        SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'" );
        m_mtxViewers.lock();
        for ( const auto& viewer : m_viewers ) {
            viewer->notifyNewStreamer( streamer );
        }
        m_mtxViewers.unlock();

        // SERVER_MSG( "prevent viewers there is a new streamer : '" << streamName << "'
    }
}

void ServerImpl2::addStreamViewer( StreamerClient2* streamer ) {
    const auto& streamName = streamer->m_streamName;

    m_mtxPrint.lock();
    std::cout << streamer->headerMsg() << "new stream viewer watching '" << streamName << "'"
              << std::endl;
    streamer->printStatusMessage( "new streamViewer" );
    m_mtxPrint.unlock();
}

void ServerImpl2::delStreamViewer( StreamerClient2* streamer ) {
    const auto& streamName = streamer->m_streamName;

    m_mtxPrint.lock();
    std::cout << streamer->headerMsg() << "del stream viewer watching '" << streamName << "'"
              << std::endl;
    streamer->printStatusMessage( "del streamViewer" );
    m_mtxPrint.unlock();
}

void ServerImpl2::addViewer( ViewerClient2* viewer ) {

#if ( __cplusplus >= 201703L )
    for ( const auto& [streamName, streamer] : m_streamName2streamer ) {
#else
    for ( const auto& pair : m_streamName2streamer ) {
        const auto& streamName = pair.first;
        const auto& streamer   = pair.second;
#endif

        viewer->notifyNewStreamer( streamer );
    }

    m_mtxViewers.lock();
    m_viewers.push_back( viewer );
    m_mtxViewers.unlock();
}

void ServerImpl2::delStreamer( const StreamerClient2* streamer ) {

    const std::string streamName = streamer->m_streamName;
    m_mtxStreamName2streamer.lock();
    assert( !m_streamName2streamer.empty() );
    assert( m_streamName2streamer.find( streamName ) != m_streamName2streamer.end() );
    // const auto nActiveClient = m_nActiveClient;
    m_streamName2streamer.erase( streamName );
    m_mtxStreamName2streamer.unlock();

    m_mtxViewers.lock();
    for ( auto* viewer : m_viewers ) {
        viewer->notifyDelStreamer( streamName );
    }
    m_mtxViewers.unlock();

    std::cout << streamer->headerMsg() << "end streamer : '" << streamName << "'" << std::endl;
    streamer->printStatusMessage( "del streamer" );
}

void ServerImpl2::delViewer( ViewerClient2* viewer ) {

    m_mtxViewers.lock();
    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );
    m_mtxViewers.unlock();

    m_mtxPrint.lock();
    viewer->printStatusMessage( "del viewer" );
    m_mtxPrint.unlock();
}

void ServerImpl2::removeClient( Client2* client ) {
    m_mtxClients.lock();
    assert( std::find( m_clients.begin(), m_clients.end(), client ) != m_clients.end() );
    m_clients.remove( client );
    --m_nActiveClient;
    m_mtxClients.unlock();
}

} // namespace server
} // namespace hub
