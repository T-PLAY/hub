#include "Viewer.hpp"

namespace hub {

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

} // namespace hub
