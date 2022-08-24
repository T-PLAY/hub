#include "NetClient.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

///////////////////////// STREAMER ///////////////////////////////

Streamer::Streamer( const std::string& sensorName, net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {

    std::cout << "\t[Streamer] Streamer(string, ClientSocket&&)" << std::endl;

    Interface::write( net::ClientSocket::Type::STREAMER );

    Interface::write( sensorName );

    net::ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        assert( false );
        throw net::Socket::exception(
            ( std::string( "sensor '" ) + sensorName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );
}

Streamer::Streamer( ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {}

///////////////////////// STREAM_VIEWER ///////////////////////////////

StreamViewer::StreamViewer( const std::string& sensorName,
                            const std::string& syncSensorName,
                            net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {

    std::cout << "\t[StreamViewer] StreamViewer(string, string, ClientSocket&&)" << std::endl;

    Interface::write( net::ClientSocket::Type::STREAM_VIEWER );

    Interface::write( sensorName );
    ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
        DEBUG_MSG( getHeader( mFdSock ) << "[StreamViewer] exception sensor '" << sensorName
                                        << "' is not attached to server" );
        throw ClientSocket::exception(
            ( std::string( "sensor '" ) + sensorName + "' is not attached to server" ).c_str() );
    }
    assert( mess == ClientSocket::Message::OK );

    Interface::write( syncSensorName );
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
        throw ClientSocket::exception(
            ( std::string( "sync sensor '" ) + syncSensorName + "' is not attached to server" )
                .c_str() );
    }
    assert( mess == ClientSocket::Message::OK );

    std::cout << "\t[StreamViewer] StreamViewer(string, string, ClientSocket&&) end" << std::endl;
}

StreamViewer::StreamViewer( ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {}

///////////////////////// VIEWER ///////////////////////////////

Viewer::Viewer( ClientSocket&& clientSocket,
                std::function<void( const std::string& sensorName, const SensorSpec& sensorSpec )>
                    onNewStreamer,
                std::function<void( const std::string& sensorName )> onDelStreamer ) :

    net::ClientSocket( std::move( clientSocket ) ),
    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ) {

    Interface::write( net::ClientSocket::Type::VIEWER );

    m_thread = std::thread( [this]() {
        try {
            while ( !m_stopThread ) {

                ClientSocket::Message serverMessage;
                Interface::read( serverMessage );

                switch ( serverMessage ) {

                case ClientSocket::Message::PING: {
                    // server check client connection
                    // nothing to do
                    std::cout << "[Viewer] receive ping " << std::endl;
                } break;

                case ClientSocket::Message::NEW_STREAMER: {
                    std::string sensorName;
                    Interface::read( sensorName );
                    std::cout << "[Viewer] new streamer '" << sensorName << "'" << std::endl;

                    SensorSpec sensorSpec;
                    Interface::read( sensorSpec );

                    //                    std::cout << "[Viewer] new streamer " << sensorName
                    //                              << ", format:" << sensorSpec.format
                    //                              << ", dims:" << SensorSpec::dims2string(
                    //                              sensorSpec.dims )
                    //                              << ", acquisitionSize:" <<
                    //                              sensorSpec.acquisitonSize << std::endl;
                    //                    std::cout << "[Viewer] metadata : "
                    //                              << SensorSpec::metaData2string(
                    //                              sensorSpec.metaData, true );

                    m_onNewStreamer( sensorName, sensorSpec );

                } break;

                case ClientSocket::Message::DEL_STREAMER: {
                    std::string sensorName;
                    Interface::read( sensorName );
                    std::cout << "[Viewer] del streamer '" << sensorName << "'" << std::endl;

                    m_onDelStreamer( sensorName );

                } break;

                default:
                    std::cout << "[Viewer] unknown message from server" << std::endl;
                }
            }
        }
        catch ( std::exception& e ) {
            std::cout << "[Viewer] catch exception" << std::endl;
        }
    } );
    //    m_thread.detach();
}

Viewer::~Viewer() {
    std::cout << "[Viewer] ~Viewer()" << std::endl;
    m_stopThread = true;
    assert( m_thread.joinable() );
    m_thread.join();
}

} // namespace io
} // namespace hub
