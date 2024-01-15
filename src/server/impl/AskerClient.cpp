#include "AskerClient.hpp"

#ifdef HUB_USE_SERVER

#include "ServerImpl.hpp"

namespace hub {
namespace server {
namespace impl {

AskerClient::AskerClient( ServerImpl* server, int iClient, net::ClientSocket&& sock ) :
    Client( server, iClient ), m_sock( std::move( sock ) ) {

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            bool closeConnection = false;
            while ( !closeConnection ) {

                io::StreamBase::ClientMessage message;
                m_sock.read( message );

                switch ( message ) {
                case io::StreamBase::ClientMessage::ASKER_CLIENT_CLOSED:
                    std::cout << headerMsg() << "closing connection" << std::endl;
                    closeConnection = true;
                    break;

                case io::StreamBase::ClientMessage::ASKER_CLIENT_GET_LIST_STREAMS: {
                    std::cout << headerMsg() << "listing sensors" << std::endl;

                    // todo server
                    //                    assert( m_server != nullptr );
                    //                    const auto& listStreams = m_server->listStreams();
                    //                    m_sock.write( listStreams );

                } break;

                case io::StreamBase::ClientMessage::ASKER_CLIENT_GET_ACQ: {
                    std::cout << headerMsg() << "get sensor acquisition" << std::endl;
                    std::string streamName;
                    m_sock.read( streamName );

                    assert( m_server != nullptr );
                    const auto& streamers = m_server->getStreamers();
                    if ( streamers.find( streamName ) != streamers.end() ) {
                        m_sock.write( io::StreamBase::ServerMessage::FOUND );

                        const auto* inputSensor = m_server->getInputSensor( streamName );
                        assert( inputSensor != nullptr );
                        const auto& sensorSpec = inputSensor->getSpec();
                        //                        const auto& sensorSpec = m_server->getSensorSpec(
                        //                        streamName );
                        m_sock.write( sensorSpec );

                        assert( m_server != nullptr );
                        const sensor::Acquisition& acq = m_server->getAcquisition( streamName );
                        m_sock.write( acq );
                    }
                    else { m_sock.write( io::StreamBase::ServerMessage::NOT_FOUND ); }

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

void AskerClient::end( io::StreamBase::ServerMessage message ) {
    m_sock.close();
}

} // namespace impl
} // namespace server
} // namespace hub

#endif
