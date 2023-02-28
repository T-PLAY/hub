#include "AskerClient.hpp"

#include "Server.hpp"

AskerClient::AskerClient( Server& server, int iClient, hub::net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    //    server.addViewer( this );

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            //            while ( m_socket.isOpen() ) {
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

                    const auto& listStreams = m_server.listStreams();
                    m_socket.write( listStreams );

                    ////                    std::list<hub::SensorSpec> sensorSpecs;
                    //                    const auto & streamers = m_server.getStreamers();
                    ////                    m_socket.write((int)streamers.size());
                    //                    for (const auto & [streamName, streamer] : streamers) {
                    //                        const auto & sensorSpec =
                    //                        streamer->getInputSensor().m_spec;
                    //                        m_socket.write(sensorSpec);
                    //                    }
                    ////                    m_socket.write(sensorSpecs);

                } break;

                case hub::net::ClientSocket::Message::GET_SENSOR_SPEC: {
                    std::cout << headerMsg() << "get sensor spec" << std::endl;
                    std::string streamName;
                    m_socket.read( streamName );

                    const auto& streamers = m_server.getStreamers();
                    if ( streamers.find( streamName ) != streamers.end() ) {
                        m_socket.write( hub::net::ClientSocket::Message::FOUND );

                        const auto& streamer = streamers.at( streamName );

                        //					    const auto & sensorSpec =
                        // m_server.getSensorSpec(streamName);
                        const auto& sensorSpec = streamer->getInputSensor().m_spec;
                        m_socket.write( sensorSpec );
                    }
                    else {
                        m_socket.write( hub::net::ClientSocket::Message::NOT_FOUND );
                    }

                    //                    const auto & sensorSpec =
                    //                    m_server.getSensorSpec(streamName); m_socket.write(
                    //                    sensorSpec );
                } break;

                case hub::net::ClientSocket::Message::GET_ACQUISITION: {
                    std::cout << headerMsg() << "get sensor acquisition" << std::endl;
                    std::string streamName;
                    m_socket.read( streamName );

                    const auto& streamers = m_server.getStreamers();
                    if ( streamers.find( streamName ) != streamers.end() ) {
                        m_socket.write( hub::net::ClientSocket::Message::FOUND );

                        const auto& streamer = streamers.at( streamName );

                        //					    const auto & sensorSpec =
                        // m_server.getSensorSpec(streamName);
                        const auto& sensorSpec = streamer->getInputSensor().m_spec;
                        m_socket.write( sensorSpec );

                        const hub::Acquisition& acq = *m_server.getAcquisition( streamName ).get();
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

                //                m_mtxSocket.lock();
                //                m_socket.write( hub::net::ClientSocket::Message::PING );
                //                m_mtxSocket.unlock();
                //                // ping viewer client to know if the connection of this one still
                //                alive std::this_thread::sleep_for( std::chrono::milliseconds( 100
                //                ) );
            }
            std::thread( [this]() { delete this; } ).detach();
        }
        catch ( std::exception& e ) {
            //            m_mtxSocket.unlock();

            //            s_mtxCout.lock();
            std::cout << headerMsg() << "catch asker exception : " << e.what() << std::endl;
            //            delete this;
            //            m_server.delViewer( this );
            std::thread( [this]() { delete this; } ).detach();
        }
    } );
    //    thread.detach();

    printStatusMessage( "new asker" );

    //    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
    //              << "new viewer" << m_server.getStatus() << std::endl;
    //    std::cout << "-------------------------------------------------------------------------"
    //                 "--------------------"
    //              << std::endl;
}

AskerClient::~AskerClient() {
    //    s_mtxCout.lock();

    assert( m_thread.joinable() );
    m_thread.join();

    //    m_server.delViewer( this );
    printStatusMessage( "del asker" );
    //    std::cout << std::left << std::setw( g_margin2 ) << headerMsg() << std::setw( g_margin )
    //              << "del viewer" << m_server.getStatus() << std::endl;
    //    std::cout << "-------------------------------------------------------------------------"
    //                 "--------------------"
    //              << std::endl;
    //    m_mtxSocket.unlock();
    //    s_mtxCout.unlock();
}

std::string AskerClient::headerMsg() const {
    return Client::headerMsg() + "[Asker] ";
}
