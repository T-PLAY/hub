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
            while ( m_socket.isOpen() ) {
                m_mtxSocket.lock();
                m_socket.write( hub::net::ClientSocket::Message::PING );
                m_mtxSocket.unlock();
                // ping viewer client to know if the connection of this one still alive
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        }
        catch ( std::exception& e ) {
            m_mtxSocket.unlock();

            std::cout << headerMsg() << "catch viewer exception : " << e.what() << std::endl;
            std::thread( [this]() { delete this; } ).detach();
        }
    } );

    printStatusMessage( "new viewer" );
}

ViewerClient::~ViewerClient() {

    assert( m_thread.joinable() );
    m_thread.join();

    assert( m_server != nullptr );
    m_server->delViewer( this );
    printStatusMessage( "del viewer" );
}

std::string ViewerClient::headerMsg() const {
    return Client::headerMsg() + "[Viewer] ";
}

void ViewerClient::notifyNewStreamer( const StreamerClient& streamer ) {

    m_mtxSocket.lock();
    m_socket.write( hub::net::ClientSocket::Message::NEW_STREAMER );
    m_socket.write( streamer.getStreamName() );
    m_socket.write( streamer.getInputSensor().m_spec );
    m_mtxSocket.unlock();
}

void ViewerClient::notifyDelStreamer( const StreamerClient& streamer ) {
    std::cout << headerMsg() << "notifyDelStreamer " << streamer.getStreamName() << std::endl;
    try {
        m_mtxSocket.lock();
        m_socket.write( hub::net::ClientSocket::Message::DEL_STREAMER );
        m_socket.write( streamer.getStreamName() );
        m_socket.write( streamer.getInputSensor().m_spec );
        m_mtxSocket.unlock();
    }
    catch ( std::exception& e ) {
        m_mtxSocket.unlock();
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        std::thread( [this]() { delete this; } ).detach();
    }
}

} // server
} // hub
