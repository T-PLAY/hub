#include "ViewerClient.hpp"

#include "Server.hpp"
#include "StreamerClient.hpp"

ViewerClient::ViewerClient( Server& server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    // each already connected streamers prevent existence for this new viewer
    for ( const auto& pair : server.getStreamers() ) {
        const auto& streamer = *pair.second;
        notifyNewStreamer( streamer );
    }

    server.addViewer( this );

    std::thread thread( [this]() {
        try {
            // check client still alive
            while ( true ) {
                m_mtxSocket.lock();
                m_socket.write( hub::net::ClientSocket::Message::PING );
                m_mtxSocket.unlock();
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        }
        catch ( std::exception& e ) {
            m_mtxSocket.unlock();
            std::cout << headerMsg() << "catch viewer exception : " << e.what() << std::endl;
        }

        m_server.delViewer( this );
    } );
    thread.detach();
}

std::string ViewerClient::headerMsg() const {
    return Client::headerMsg() + "[ViewerClient] ";
}

void ViewerClient::notifyNewStreamer( const StreamerClient& streamer ) const {

    m_mtxSocket.lock();
    m_socket.write( hub::net::ClientSocket::Message::NEW_STREAMER );
    m_socket.write( streamer.getStreamName() );
    m_socket.write( streamer.getInputSensor().m_spec );
    m_mtxSocket.unlock();
}

void ViewerClient::notifyDelStreamer( const std::string& streamerName,
                                      const hub::SensorSpec& sensorSpec ) const {
    std::cout << headerMsg() << "notifyDelStreamer " << streamerName << std::endl;
    try {
        m_mtxSocket.lock();
        m_socket.write( hub::net::ClientSocket::Message::DEL_STREAMER );
        m_socket.write( streamerName );
        m_socket.write( sensorSpec );
        m_mtxSocket.unlock();
    }
    catch ( std::exception& e ) {
        m_mtxSocket.unlock();
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
    }

}
