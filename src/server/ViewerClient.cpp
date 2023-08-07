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
            // ping viewer client to know if the connection of this one still alive

            net::ClientSocket::Message message;
            m_socket.read( message );
            while ( message == net::ClientSocket::Message::SET_PROPERTY ) {
                std::string streamName;
                std::string objectName;
                int property;
                Any value;
                m_socket.read( streamName );
                m_socket.read( objectName );
                m_socket.read( property );
                m_socket.read( value );

                m_server->setProperty( streamName, objectName, property, value );

                m_socket.read( message );
            };
            assert( message == net::ClientSocket::Message::VIEWER_CLOSED );
            m_viewerClosed = true;

            if ( m_socket.isOpen() )
                m_socket.write( net::ClientSocket::Message::VIEWER_CLIENT_CLOSED );
        }
        catch ( std::exception& ex ) {
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }
        std::thread( [this]() { delete this; } ).detach();
    } );

    printStatusMessage( "new viewer" );
}

ViewerClient::~ViewerClient() {

    assert( m_thread.joinable() );
    m_thread.join();

    assert( m_server != nullptr );
    m_server->delViewer( this );

    if ( m_socket.isOpen() ) {
        if ( !m_viewerClosed ) {
            m_socket.write( net::ClientSocket::Message::VIEWER_CLIENT_CLOSED );
        }
        while ( !m_viewerClosed ) {
            std::cout << "[ViewerClient] close() waiting for server/viewer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }

        m_socket.close();
    }
}

std::string ViewerClient::headerMsg() const {
    return Client::headerMsg() + "[Viewer] ";
}

void ViewerClient::notifyNewStreamer( const std::string& streamName,
                                      const SensorSpec& sensorSpec ) {

    m_socket.write( hub::net::ClientSocket::Message::NEW_STREAMER );

    m_socket.write( streamName );
    m_socket.write( sensorSpec );
}

void ViewerClient::notifyDelStreamer( const std::string& streamName,
                                      const SensorSpec& sensorSpec ) {
    // void ViewerClient::notifyDelStreamer( const StreamerClient& streamer ) {
    if ( m_viewerClosed ) return;

    try {
        m_socket.write( hub::net::ClientSocket::Message::DEL_STREAMER );
        m_socket.write( streamName );
        m_socket.write( sensorSpec );
    }
    catch ( std::exception& e ) {
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        std::thread( [this]() { delete this; } ).detach();
    }
}

void ViewerClient::end( net::ClientSocket::Message message ) {
    std::cout << headerMsg() << "end(" << message << ")" << std::endl;
    assert( m_socket.isOpen() );
    m_socket.write( message );
}

void ViewerClient::notifyProperty( const std::string& streamName,
                                   const std::string& objectName,
                                   int property,
                                   const Any& value ) {

    assert( m_socket.isOpen() );
    m_socket.write( net::ClientSocket::Message::SET_PROPERTY );
    m_socket.write( streamName );
    m_socket.write( objectName );
    m_socket.write( property );
    m_socket.write( value );
}

} // namespace server
} // namespace hub
