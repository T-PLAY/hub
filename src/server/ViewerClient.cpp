#include "ViewerClient.hpp"

#include "Server.hpp"
#include "StreamerClient.hpp"

namespace hub {
namespace server {

ViewerClient::ViewerClient( Server* server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    assert( m_server != nullptr );
    m_server->addViewer( this );

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            //            while ( m_socket.isOpen() ) {
            //                m_mtxSocket.lock();
            //                m_socket.write( hub::net::ClientSocket::Message::PING );
            //                m_mtxSocket.unlock();
            // ping viewer client to know if the connection of this one still alive
            //                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            net::ClientSocket::Message message;
            m_socket.read( message );
            assert( message == net::ClientSocket::Message::VIEWER_CLOSED );
            m_viewerClosed = true;

            if ( m_socket.isOpen() )
                m_socket.write( net::ClientSocket::Message::VIEWER_CLIENT_CLOSED );
            //            }
        }
        catch ( std::exception& ex ) {
            //            m_mtxSocket.unlock();
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }
        std::thread( [this]() { delete this; } ).detach();
    } );

    printStatusMessage( "new viewer" );
}

ViewerClient::~ViewerClient() {
    //    std::cout << "[ViewerClient] ~ViewerClient()" << std::endl;

    assert( m_thread.joinable() );
    m_thread.join();

    assert( m_server != nullptr );
    m_server->delViewer( this );

//    assert( m_socket.isOpen() );
    if ( m_socket.isOpen() ) {
        if ( !m_viewerClosed ) {
            m_socket.write( net::ClientSocket::Message::VIEWER_CLIENT_CLOSED );
        }
        while ( !m_viewerClosed ) {
            std::cout << "[ViewerClient] close() waiting for server/viewer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }

        //    if (m_socket.isOpen())
        m_socket.close();
    }
    //    printStatusMessage( "del viewer" );
}

std::string ViewerClient::headerMsg() const {
    return Client::headerMsg() + "[Viewer] ";
}

void ViewerClient::notifyNewStreamer( const std::string& streamName,
                                      const SensorSpec& sensorSpec ) {

    //    m_mtxSocket.lock();
    m_socket.write( hub::net::ClientSocket::Message::NEW_STREAMER );

    //    const auto & streamName = m_server.get
    //    m_socket.write( streamer.getStreamName() );
    //    m_socket.write( streamer.getInputSensor().getSpec() );
    m_socket.write( streamName );
    m_socket.write( sensorSpec );
    //    m_mtxSocket.unlock();
}

void ViewerClient::notifyDelStreamer( const std::string& streamName,
                                      const SensorSpec& sensorSpec ) {
    // void ViewerClient::notifyDelStreamer( const StreamerClient& streamer ) {
    //     std::cout << headerMsg() << "notifyDelStreamer " << streamer.getStreamName() <<
    //     std::endl;
    if (m_viewerClosed)
        return;

    try {
        //        m_mtxSocket.lock();
        m_socket.write( hub::net::ClientSocket::Message::DEL_STREAMER );
        //        m_socket.write( streamer.getStreamName() );
        //        m_socket.write( streamer.getInputSensor().getSpec() );
        m_socket.write( streamName );
        m_socket.write( sensorSpec );
        //        m_mtxSocket.unlock();
    }
    catch ( std::exception& e ) {
        //        m_mtxSocket.unlock();
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        std::thread( [this]() { delete this; } ).detach();
    }
}

void ViewerClient::end( net::ClientSocket::Message message ) {
    assert( m_socket.isOpen() );
    //    m_socket.write(net::ClientSocket::Message::VIEWER_CLIENT_CLOSED);
    //    m_socket.write(net::ClientSocket::Message::SERVER_CLOSED);
    m_socket.write( message );
    //	m_socket.write( hub::net::ClientSocket::Message::PING );
    //    m_socket.close();
}

} // namespace server
} // namespace hub
