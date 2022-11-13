#include "Server.hpp"

#include <cassert>
#include <cstring>
//#include <iomanip>
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

    for ( const auto& [streamerName, streamer] : m_streamers ) {
//        const auto& streamerName = pair.first;
//        const auto& streamer     = pair.second;

        //        auto& streamViewers     = m_streamViewers[streamerName];
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

//void printStatus( const std::string& message ) {}

void Server::addStreamer( StreamerClient* streamer ) {

    const auto& streamerName = streamer->getStreamName();

    assert( m_streamers.find( streamerName ) == m_streamers.end() );
    m_streamers[streamerName] = streamer;

        std::cout << headerMsg() << "prevent viewers there is a new streamer : '" << streamerName
                  << std::endl;
        for ( const auto& viewer : m_viewers ) {
            viewer->notifyNewStreamer( *streamer );
        }

//    const auto& parentName = streamer->getParent();
    //    const auto& syncStream =
    //        ( m_streamers.find( parent ) != m_streamers.end() ) ? ( parent ) : ( "" );
    //    for ( const auto& viewer : m_viewers ) {
    //        viewer->notifyNewStreamer( *streamer, "" );
    //    }

    // prevent the father to link to his child
    //    std::cout << headerMsg() << "prevent viewers there is a new streamer : '" << streamerName
    //              << "' -> '" << streamerParent << "'" << std::endl;

//    // if parent exist, attach parent to his child
//    if ( m_streamers.find( parentName ) != m_streamers.end() ) {
//        const auto& parent = m_streamers.at( parentName );

//        std::cout << headerMsg()
//                  << "prevent viewers there is a new father streamer linked to his child : '"
//                  << parentName << "' (parent) <- '" << streamerName << "'" << std::endl;
//        for ( const auto& viewer : m_viewers ) {
//            //        viewer->notifyNewStreamer( *streamer, syncStream );
//            viewer->notifyNewStreamer( parentName, parent->getInputSensor().m_spec, streamerName );
//        }
//    }
//    // however prevent there is new orphan
//    else {
//        std::cout << headerMsg() << "prevent viewers there is a new streamer : '" << streamerName
//                  << std::endl;
//        for ( const auto& viewer : m_viewers ) {
//            viewer->notifyNewStreamer( streamerName, streamer->getInputSensor().m_spec, "" );
//        }
//    }

//    for ( const auto& pair : m_streamers ) {
//        const auto& sonStreamer     = *pair.second;
//        const auto& sonStreamerName = sonStreamer.getStreamName();

//        const auto& parentOfChild = sonStreamer.getParent();
//        // for each sons of the new streamer, synchronize it
//        if ( parentOfChild == streamerName ) {
//            std::cout << headerMsg()
//                      << "prevent viewers there is a new father streamer linked to his child : '"
//                      << streamerName << "' <- '" << sonStreamerName << "' (son)" << std::endl;
//            for ( const auto* viewer : m_viewers ) {
//                //                viewer->notifyDelStreamer( sonStreamer.getStreamName(),
//                //                sonStreamer.getInputSensor().m_spec); viewer->notifyNewStreamer(
//                //                sonStreamer, syncStream );
//                //                viewer->notifyDelStreamer( streamerName,
//                //                streamer->getInputSensor().m_spec);
//                viewer->notifyDelStreamer(
//                    sonStreamerName, sonStreamer.getInputSensor().m_spec, "" );
//                viewer->notifyNewStreamer(
//                    streamerName, streamer->getInputSensor().m_spec, sonStreamerName );
//            }
//        }
//        //        const auto& syncStream =
//        //            ( m_streamers.find( parentOfChild ) != m_streamers.end() ) ? ( parent ) : ( ""
//        //            );
//    }
}

void Server::delStreamer( StreamerClient* streamer ) {
    //    m_mtxStreamers.lock();

    const auto streamerName = streamer->getStreamName();
//    const auto sensorSpec   = streamer->getInputSensor().m_spec;

    //    auto streamViewers = m_streamViewers[streamerName];
    //    for ( auto* streamViewer : streamViewers ) {
    //        delStreamViewer( streamViewer );
    //    }
    //    m_streamViewers.at( streamerName ).clear();

    assert( m_streamers.find( streamerName ) != m_streamers.end() );
    m_streamers.erase( m_streamers.find( streamerName ) );

    //    std::thread thread( [streamer]() { delete streamer; } );
    //    thread.detach();

        for ( const auto* viewer : m_viewers ) {
            viewer->notifyDelStreamer( *streamer);
        }

//    const auto& parentName = streamer->getParent();
//    //    const auto& syncStream =
//    //        ( m_streamers.find( parent ) != m_streamers.end() ) ? ( parent ) : ( "" );
//    //    for ( const auto& viewer : m_viewers ) {
//    //        viewer->notifyNewStreamer( *streamer, "" );
//    //    }

//    // prevent the father to link to his child
//    //    std::cout << headerMsg() << "prevent viewers there is a new streamer : '" << streamerName
//    //              << "' -> '" << parentName << "'" << std::endl;

//    // if parent exist, attach parent to his child
//    if ( m_streamers.find( parentName ) != m_streamers.end() ) {
//        const auto& parent = m_streamers.at( parentName );

//        std::cout << headerMsg()
//                  << "prevent viewers there is a new father streamer linked to his child : '"
//                  << parentName << "' (parent) <- '" << streamerName << "'" << std::endl;
//        for ( const auto& viewer : m_viewers ) {
//            //        viewer->notifyNewStreamer( *streamer, syncStream );
//            //            viewer->notifyNewStreamer( *m_streamers.at( parentName ), streamerName );
//            viewer->notifyDelStreamer( parentName, parent->getInputSensor().m_spec, streamerName );
//        }
//    }
//    // however prevent there is new orphan
//    else {
//        std::cout << headerMsg() << "prevent viewers there is a new streamer : '" << streamerName
//                  << std::endl;
//        for ( const auto& viewer : m_viewers ) {
//            //            viewer->notifyNewStreamer( *streamer, "" );
//            viewer->notifyDelStreamer( streamerName, streamer->getInputSensor().m_spec, "" );
//        }
//    }

//    //    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
//    //              << "del streamer" << getStatus() << std::endl;
//    //    std::cout << "-------------------------------------------------------------------------"
//    //                 "--------------------"
//    //              << std::endl;

//        m_mtxStreamers.unlock();
}

// void Server::addStreamViewer( StreamViewerClient* streamViewer ) {
//     const auto& syncStreamName = streamViewer->getSyncStreamName();
//     const auto& streamName     = streamViewer->getStreamName();

//    if ( syncStreamName == "" ) {
////        auto& streamViewers = m_streamViewers[streamName];
//        assert(m_streamers.find(streamName) != m_streamers.end());
//        auto& streamer = *m_streamers.at(streamName);
////        assert( std::find( streamViewers.begin(), streamViewers.end(), streamViewer ) ==
////                streamViewers.end() );
//        streamer.addStreamViewer(streamViewer);
////        streamViewers.push_back( streamViewer );
//    }
//    else {
//        assert( m_streamers.find( syncStreamName ) != m_streamers.end() );
//        auto& syncStreamer = *m_streamers.at( syncStreamName );

//        syncStreamer.addSyncStreamViewer( streamViewer );
//    }
//}

void Server::addViewer( ViewerClient* viewer ) {

    // each already connected streamers prevent existence for this new viewer
    for ( const auto& [streamerName, streamer] : m_streamers ) {
        //        const auto& streamer = *pair.second;

        // notify orphans first
        if (streamer->getParent() == "") {
            viewer->notifyNewStreamer( *streamer );
        }

//        const auto& parentName = streamer->getParent();
//        //        const auto& syncStream =
//        //            ( m_streamers.find( parentName ) != m_streamers.end() ) ? ( parentName ) : (
//        //            "" );

//        if ( m_streamers.find( parentName ) != m_streamers.end() ) {
//            const auto& parent = *m_streamers.at( parentName );

//            viewer->notifyNewStreamer( parentName, parent.getInputSensor().m_spec, streamerName );
//        }
//        else { viewer->notifyNewStreamer( streamerName, streamer->getInputSensor().m_spec, "" ); }
    }

    for ( const auto& [streamerName, streamer] : m_streamers ) {
        // notify children to auto synchronize with already oppened parent
        if (streamer->getParent() != "") {
            viewer->notifyNewStreamer( *streamer );
        }
    }

    m_viewers.push_back( viewer );
}

void Server::delViewer( ViewerClient* viewer ) {

    assert( std::find( m_viewers.begin(), m_viewers.end(), viewer ) != m_viewers.end() );
    m_viewers.remove( viewer );

    //    std::thread thread( [viewer]() { delete viewer; } );
    //    thread.detach();
    //    delete viewer;
}

// void Server::delStreamViewer( StreamViewerClient* streamViewer ) {

//    const auto& syncStreamName = streamViewer->getSyncStreamName();
//    const auto& streamName     = streamViewer->getStreamName();

//    if ( syncStreamName == "" ) {
//        assert( m_streamViewers.find( streamName ) != m_streamViewers.end() );
//        auto& streamViewers = m_streamViewers.at( streamName );
//        streamViewers.remove( streamViewer );
//    }
//    else {
//        if ( m_streamers.find( syncStreamName ) != m_streamers.end() ) {
//            auto& streamer = *m_streamers.at( syncStreamName );

//            streamer.delStreamViewer( streamViewer );
//        }
//    }

//    streamViewer->killThread();

//    delete streamViewer;
//    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
//              << "del stream viewer" << getStatus() << std::endl;
//    std::cout << "-------------------------------------------------------------------------"
//                 "--------------------"
//              << std::endl;
//}

void Server::newAcquisition( StreamerClient* streamer, const hub::Acquisition& acq ) {

    const auto& streamerName = streamer->getStreamName();

    //    // broadcast acquisition for each live viewer (no sync)
    //    auto streamViewers = m_streamViewers[streamerName];
    //    for ( const auto& streamViewer : streamViewers ) {
    //        assert( streamViewer->getSyncStreamName() == "" );
    //        streamViewer->update( acq );
    //    }

    // broadcast acquisition for all streamer
    for ( const auto& pair : m_streamers ) {
        const auto& streamer = pair.second;

        streamer->newAcquisition( streamerName, acq );
    }
}

// void Server::setAcqPing( bool newAcqPing ) {
//     m_acqPing = newAcqPing;
// }

const std::map<std::string, StreamerClient*>& Server::getStreamers() const {
    return m_streamers;
}

Server::Server() {
    assert( !m_thread.joinable() );
}

Server::Server( int port ) : m_serverSock( port ) {}

Server::~Server() {
    std::cout << headerMsg() << "~Server()" << std::endl;
    assert( m_thread.joinable() );
    m_thread.join();
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

void Server::setMaxClients( int maxClients ) {
    m_maxClients = maxClients;
}

// bool Server::getAcqPing() const
//{
//     return m_acqPing;
// }
