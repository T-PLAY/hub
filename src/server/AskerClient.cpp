#include "AskerClient.hpp"

#include "Server.hpp"

namespace hub {
namespace server {

AskerClient::AskerClient( Server* server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ), m_sock( std::move( sock ) ) {

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            bool closeConnection = false;
            while ( !closeConnection ) {

                hub::net::ClientSocket::Message message;
                m_sock.read( message );

                switch ( message ) {
                case hub::net::ClientSocket::Message::CLOSE:
                    std::cout << headerMsg() << "closing connection" << std::endl;
                    closeConnection = true;
                    break;

                case hub::net::ClientSocket::Message::LIST_STREAMS: {
                    std::cout << headerMsg() << "listing sensors" << std::endl;

                    assert( m_server != nullptr );
                    const auto& listStreams = m_server->listStreams();
                    m_sock.write( listStreams );

                } break;

                    ////                        const auto& sensorSpec =
                    /// streamer->getInputSensor().getSpec(); /                        m_sock.write(
                    /// sensorSpec );

                case hub::net::ClientSocket::Message::GET_ACQUISITION: {
                    std::cout << headerMsg() << "get sensor acquisition" << std::endl;
                    std::string streamName;
                    m_sock.read( streamName );

                    assert( m_server != nullptr );
                    const auto& streamers = m_server->getStreamers();
                    if ( streamers.find( streamName ) != streamers.end() ) {
                        m_sock.write( hub::net::ClientSocket::Message::FOUND );

                        const auto& sensorSpec = m_server->getSensorSpec( streamName );
                        m_sock.write( sensorSpec );

                        assert( m_server != nullptr );
                        const hub::Acquisition& acq = m_server->getAcquisition( streamName );
                        m_sock.write( acq );
                    }
                    else {
                        m_sock.write( hub::net::ClientSocket::Message::NOT_FOUND );
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

    assert( m_sock.isOpen() );
    m_sock.close();
    assert( !m_sock.isOpen() );

    printStatusMessage( "del asker" );
}

std::string AskerClient::headerMsg() const {
    return Client::headerMsg() + "[Asker] ";
}

void AskerClient::end( net::ClientSocket::Message message ) {
    m_sock.close();
}

} // namespace server
} // namespace hub
