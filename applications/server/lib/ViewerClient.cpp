
#include "ViewerClient.hpp"
#include "StreamerClient.hpp"

#include "Server.hpp"

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
                m_socket.write( hub::net::ClientSocket::Message::PING );
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                //                std::cout << headerMsg() << "ping viewer" << std::endl;
            }
        }
        catch ( std::exception& e ) {
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
    //    mSock->write( Socket::Message::NEW_STREAMER );

    m_socket.write( hub::net::ClientSocket::Message::NEW_STREAMER );
    m_socket.write( streamer.getStreamName() );
    m_socket.write( streamer.getInputSensor().m_spec );

    /*mSock->write(streamer.mhub::InputSensor.getSensorName());
    mSock->write(std::string(Stream::format2string[(int)streamer.mhub::InputSensor.getFormat()]));

    std::string dimStr;
    for (const auto dim : streamer.mhub::InputSensor.getDims()) {
        dimStr += std::to_string(dim) + " x ";
    }
    dimStr.resize(dimStr.size() - 3);
    mSock->write(dimStr);
    mSock->write(std::to_string(streamer.mhub::InputSensor.getAcquisitionSize()));

    mSock->write(streamer.mhub::InputSensor.getMetaData());*/

    //    mSock->write( streamer.mInputSensor );
}

void ViewerClient::notifyDelStreamer( const std::string& streamerName,
                                      const hub::SensorSpec& sensorSpec ) const {
    std::cout << headerMsg() << "notifyDelStreamer " << streamerName << std::endl;
    try {
        m_socket.write( hub::net::ClientSocket::Message::DEL_STREAMER );
        m_socket.write( streamerName );
        m_socket.write( sensorSpec );
    }
    catch ( std::exception& e ) {
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
    }
}
