#include "Viewer.hpp"

#include "Net/ClientSocket.hpp"

//#define DEBUG_VIEWER

namespace hub {

Viewer::Viewer(std::function<void(const std::string&, const SensorSpec&)> onNewStreamer,
    std::function<void(const std::string&, const SensorSpec&)> onDelStreamer,
    std::function<void()> onServerConnected,
    std::function<void()> onServerDisconnected,
    const std::string& ipv4,
    int port)
    : //    net::ClientSocket( std::move( clientSocket ) ),
    m_onNewStreamer(onNewStreamer)
    , m_onDelStreamer(onDelStreamer)
    , m_onServerConnected(onServerConnected)
    , m_onServerDisconnected(onServerDisconnected)
    , m_ipv4(ipv4)
    , m_port(port)
{

//    SensorSpec sensorSpec;
    //    m_onNewStreamer( "hello", sensorSpec );
    //    m_onDelStreamer( "goodbye", sensorSpec );
    //    m_onNewStreamer( m_ipv4, sensorSpec );
    //    m_thread = std::thread([]() {
    //    m_thread = std::thread([]() {
    //        int a = 5;
    //    });
    //    m_thread.detach();

    //        return;

    m_thread = std::thread([this]() {
        //        while ( !m_stopThread ) {
        try {
            //            std::string ip = "127.0.0.1";
            //                                    net::ClientSocket sock( ip.c_str(), m_port );
            net::ClientSocket sock(m_ipv4, m_port);
            //                                    net::ClientSocket sock( "127.0.0.1", m_port );
            //                                    net::ClientSocket sock( "127.0.0.1", 4042 );
            //            net::ClientSocket sock( "127.0.0.1", 4043 );
            m_serverConnected = true;
            sock.write(net::ClientSocket::Type::VIEWER);

            if (m_onServerConnected)
                m_onServerConnected();

//            SensorSpec sensorSpec;

            while (!m_stopThread) {
                net::ClientSocket::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {

                case net::ClientSocket::Message::PING: {
                    // server check client connection
                    // nothing to do
#ifdef DEBUG_VIEWER
                    DEBUG_MSG("[Viewer] receive ping ");
#endif
                } break;

                case net::ClientSocket::Message::NEW_STREAMER: {
                    std::string streamName;
                    sock.read(streamName);
#ifdef DEBUG_VIEWER
                    DEBUG_MSG("[Viewer] new streamer '" << streamName << "'");
#endif

                                        SensorSpec sensorSpec;
                    sock.read(sensorSpec);

                    //                    std::cout << "[Viewer] new streamer " << streamName
                    //                              << ", format:" << sensorSpec.format
                    //                              << ", dims:" << SensorSpec::dims2string(
                    //                              sensorSpec.dims )
                    //                              << ", acquisitionSize:" <<
                    //                              sensorSpec.acquisitonSize << std::endl;
                    //                    std::cout << "[Viewer] metadata : "
                    //                              << SensorSpec::metaData2string(
                    //                              sensorSpec.metaData, true );

                    m_onNewStreamer(streamName, sensorSpec);

                } break;

                case net::ClientSocket::Message::DEL_STREAMER: {
                    std::string streamName;
                    sock.read(streamName);
                    SensorSpec sensorSpec;
                    sock.read(sensorSpec);
#ifdef DEBUG_VIEWER
                    DEBUG_MSG("[Viewer] del streamer '" << streamName << "'");
#endif

                    m_onDelStreamer(streamName, sensorSpec);

                } break;

                default: {
#ifdef DEBUG_VIEWER
                    DEBUG_MSG("[Viewer] unknown message from server");
#endif
                }
                } // switch (serverMessage)

            } // while (! m_stopThread)
        } catch (std::exception& e) {
#ifdef DEBUG_VIEWER
            DEBUG_MSG("[Viewer] catch exception " << e.what());
#endif
        }
        if (m_serverConnected && m_onServerDisconnected)
            m_onServerDisconnected();
        m_serverConnected = false;

        //        } // while (! m_stopThread)
    }); // thread
    //    m_thread.detach();
}

Viewer::~Viewer()
{
#ifdef DEBUG_VIEWER
    DEBUG_MSG("[Viewer] ~Viewer()");
#endif
    m_stopThread = true;
    assert(m_thread.joinable());
    m_thread.join();
#ifdef DEBUG_VIEWER
    DEBUG_MSG("[Viewer] ~Viewer() done");
#endif
}

} // namespace hub

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
