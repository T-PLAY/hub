#include "AskerClient.hpp"

#include "Server.hpp"

namespace hub {
namespace server {

AskerClient::AskerClient( Server* server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            bool closeConnection = false;
            while ( !closeConnection ) {

                hub::net::ClientSocket::Message message;
                m_socket.read( message );

                switch ( message ) {
                case hub::net::ClientSocket::Message::CLOSE:
                    std::cout << headerMsg() << "closing connection" << std::endl;
                    closeConnection = true;
                    break;

                case hub::net::ClientSocket::Message::LIST_STREAMS: {
                    std::cout << headerMsg() << "listing sensors" << std::endl;

                    assert( m_server != nullptr );
                    const auto& listStreams = m_server->listStreams();
                    m_socket.write( listStreams );

                } break;

                case hub::net::ClientSocket::Message::GET_SENSOR_SPEC: {
                    std::cout << headerMsg() << "get sensor spec" << std::endl;
                    std::string streamName;
                    m_socket.read( streamName );

                    assert( m_server != nullptr );
                    const auto& streamers = m_server->getStreamers();
                    if ( streamers.find( streamName ) != streamers.end() ) {
                        m_socket.write( hub::net::ClientSocket::Message::FOUND );

                        const auto& streamer = streamers.at( streamName );

//                        const auto& sensorSpec = streamer->getInputSensor().getSpec();
//                        m_socket.write( sensorSpec );
                    }
                    else {
                        m_socket.write( hub::net::ClientSocket::Message::NOT_FOUND );
                    }

                } break;

                case hub::net::ClientSocket::Message::GET_ACQUISITION: {
                    std::cout << headerMsg() << "get sensor acquisition" << std::endl;
                    std::string streamName;
                    m_socket.read( streamName );

                    assert( m_server != nullptr );
                    const auto& streamers = m_server->getStreamers();
                    if ( streamers.find( streamName ) != streamers.end() ) {
                        m_socket.write( hub::net::ClientSocket::Message::FOUND );

                        const auto& streamer = streamers.at( streamName );

//                        const auto& sensorSpec = streamer->getInputSensor().getSpec();
//                        m_socket.write( sensorSpec );

                        assert( m_server != nullptr );
                        const hub::Acquisition& acq = *m_server->getAcquisition( streamName ).get();
                        m_socket.write( acq );
                    }
                    else {
                        m_socket.write( hub::net::ClientSocket::Message::NOT_FOUND );
                    }

                } break;

                default:
                    std::cout << headerMsg() << "[AskerClient] unknown message : " << message
                              << std::endl;
                    break;
                }
            }
            std::thread( [this]() { delete this; } ).detach();
        }
        catch ( std::exception& e ) {

            std::cout << headerMsg() << "catch asker exception : " << e.what() << std::endl;
            std::thread( [this]() { delete this; } ).detach();
        }
    } );

    printStatusMessage( "new asker" );
}

AskerClient::~AskerClient() {

    assert( m_thread.joinable() );
    m_thread.join();

    printStatusMessage( "del asker" );
}

std::string AskerClient::headerMsg() const {
    return Client::headerMsg() + "[Asker] ";
}

} // server
} // hub
