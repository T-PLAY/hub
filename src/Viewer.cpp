#include "Viewer.hpp"

#include "Net/ClientSocket.hpp"

namespace hub {

// Viewer::Viewer( ClientSocket&& clientSocket,
//                 std::function<void( const std::string& streamName, const SensorSpec& sensorSpec
//                 )>
//                     onNewStreamer,
//                 std::function<void( const std::string& streamName )> onDelStreamer ) :

//    net::ClientSocket( std::move( clientSocket ) ),
//    m_onNewStreamer( onNewStreamer ),
//    m_onDelStreamer( onDelStreamer ) {

//    Interface::write( net::ClientSocket::Type::VIEWER );

//    m_thread = std::thread( [this]() {
//        try {
//            while ( !m_stopThread ) {

//                ClientSocket::Message serverMessage;
//                Interface::read( serverMessage );

//                switch ( serverMessage ) {

//                case ClientSocket::Message::PING: {
//                    // server check client connection
//                    // nothing to do
//                    std::cout << "[Viewer] receive ping " << std::endl;
//                } break;

//                case ClientSocket::Message::NEW_STREAMER: {
//                    std::string streamName;
//                    Interface::read( streamName );
//                    std::cout << "[Viewer] new streamer '" << streamName << "'" << std::endl;

//                    SensorSpec sensorSpec;
//                    Interface::read( sensorSpec );

//                    //                    std::cout << "[Viewer] new streamer " << streamName
//                    //                              << ", format:" << sensorSpec.format
//                    //                              << ", dims:" << SensorSpec::dims2string(
//                    //                              sensorSpec.dims )
//                    //                              << ", acquisitionSize:" <<
//                    //                              sensorSpec.acquisitonSize << std::endl;
//                    //                    std::cout << "[Viewer] metadata : "
//                    //                              << SensorSpec::metaData2string(
//                    //                              sensorSpec.metaData, true );

//                    m_onNewStreamer( streamName, sensorSpec );

//                } break;

//                case ClientSocket::Message::DEL_STREAMER: {
//                    std::string streamName;
//                    Interface::read( streamName );
//                    SensorSpec sensorSpec;
//                    Interface::read( sensorSpec );
//                    std::cout << "[Viewer] del streamer '" << streamName << "'" << std::endl;

//                    m_onDelStreamer( streamName );

//                } break;

//                default:
//                    std::cout << "[Viewer] unknown message from server" << std::endl;
//                }
//            }
//        }
//        catch ( std::exception& e ) {
//            std::cout << "[Viewer] catch exception" << std::endl;
//        }
//    } );
//    //    m_thread.detach();
//}

Viewer::Viewer( const std::string& ipv4,
                const int& port,
                std::function<void( const std::string&, const SensorSpec& )> onNewStreamer,
                std::function<void( const std::string&, const SensorSpec& )> onDelStreamer,
                std::function<void()> onServerConnected,
                std::function<void()> onServerDisconnected ) :
    //    net::ClientSocket( std::move( clientSocket ) ),
    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_ipv4( ipv4 ),
    m_port( port ) {

	SensorSpec sensorSpec;
    m_onNewStreamer( "hello", sensorSpec );
    m_onDelStreamer( "goodbye", sensorSpec );

    m_thread = std::thread( [this]() {
        //        while ( !m_stopThread ) {
        try {
            net::ClientSocket sock( m_ipv4, m_port );
            sock.write( net::ClientSocket::Type::VIEWER );

            if (m_onServerConnected)
				m_onServerConnected();

            while ( !m_stopThread ) {
                net::ClientSocket::Message serverMessage;
                sock.read( serverMessage );

                switch ( serverMessage ) {

                case net::ClientSocket::Message::PING: {
                    // server check client connection
                    // nothing to do
                    //                        std::cout << "[Viewer] receive ping " << std::endl;
                } break;

                case net::ClientSocket::Message::NEW_STREAMER: {
                    std::string streamName;
                    sock.read( streamName );
                    std::cout << "[Viewer] new streamer '" << streamName << "'" << std::endl;

                    SensorSpec sensorSpec;
                    sock.read( sensorSpec );

                    //                    std::cout << "[Viewer] new streamer " << streamName
                    //                              << ", format:" << sensorSpec.format
                    //                              << ", dims:" << SensorSpec::dims2string(
                    //                              sensorSpec.dims )
                    //                              << ", acquisitionSize:" <<
                    //                              sensorSpec.acquisitonSize << std::endl;
                    //                    std::cout << "[Viewer] metadata : "
                    //                              << SensorSpec::metaData2string(
                    //                              sensorSpec.metaData, true );

                    m_onNewStreamer( streamName, sensorSpec );

                } break;

                case net::ClientSocket::Message::DEL_STREAMER: {
                    std::string streamName;
                    sock.read( streamName );
                    SensorSpec sensorSpec;
                    sock.read( sensorSpec );
                    std::cout << "[Viewer] del streamer '" << streamName << "'" << std::endl;

                    m_onDelStreamer( streamName, sensorSpec );

                } break;

                default:
                    std::cout << "[Viewer] unknown message from server" << std::endl;
                } // switch (serverMessage)

            } // while (! m_stopThread)
        }
        catch ( std::exception& e ) {
            std::cout << "[Viewer] catch exception " << e.what() << std::endl;
        }
        if (m_onServerDisconnected)
			m_onServerDisconnected();

        //        } // while (! m_stopThread)
    } ); // thread
    //    m_thread.detach();
}

Viewer::~Viewer() {
    std::cout << "[Viewer] ~Viewer()" << std::endl;
    m_stopThread = true;
    assert( m_thread.joinable() );
    m_thread.join();
    std::cout << "[Viewer] ~Viewer() done" << std::endl;
}

} // namespace hub
