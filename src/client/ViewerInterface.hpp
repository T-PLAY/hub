#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "SensorSpec.hpp"
// #include "net/ClientSocket.hpp"
// #include "StreamViewer.hpp"
#include "InputSensor.hpp"
#include <utils/Utils.hpp>

#ifdef DEBUG_MSG
#    undef DEBUG_MSG
#endif
#define DEBUG_MSG( _params )                                              \
    do {                                                                  \
        if ( m_onLogMessage ) {                                           \
            std::stringstream _sstr;                                      \
            _sstr << m_iStreamer << ":" << _params;                       \
            m_onLogMessage( _sstr.str().c_str() );                        \
        }                                                                 \
        else { std::cout << m_iStreamer << ":" << _params << std::endl; } \
    } while ( false );

namespace hub {
namespace client {

////////////////////////////////////////////////////////////////////////////////////////////////////////

///
/// \brief The ViewerInterface class (event dispatcher)
/// can be use by client application to be aware of the events comming from server
/// client (user of this class) connection/deconnection to the server,
/// streamer connected/disconnected to the server,
/// new acquisition sended from a connected streamer.
/// Example of use : open input stream in your application (client side)
/// when new streamer is connected to server to have an interactive application.
/// Close the input stream when server or streamer are disconnected.
///
template <class InputStream = InputStream>
class SRC_API ViewerInterface
{
  protected:
    class Stream
    {
      public:
        //    template <class InputStream>
        Stream(
            ViewerInterface& viewer,
            const std::string& ipv4,
            int port,
            const std::string& streamName,
            const SensorSpec& sensorSpec,
            std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
            std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
            std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
            std::function<void( const char* logMessage )> onLogMessage );

        ~Stream();

        void startStream();
        void stopStream();

      private:
        ViewerInterface& m_viewer;
        std::thread* m_thread = nullptr;
        bool m_stopThread     = false;

        std::string m_ipv4;
        int m_port;

        //    ViewerInterface& m_viewer;
        const std::string m_streamName;
        const SensorSpec m_sensorSpec;

        //    std::thread* m_thread = nullptr;
        //    std::unique_ptr<std::thread> m_thread;
        bool m_streaming = false;

        std::function<bool( const char* streamName, const SensorSpec& )> m_onNewStreamer;
        std::function<void( const char* streamName, const SensorSpec& )> m_onDelStreamer;
        std::function<void( const char* streamName, const hub::Acquisition& )> m_onNewAcquisition;
        std::function<void( const char* logMessage )> m_onLogMessage;

        std::unique_ptr<InputSensor> m_inputSensor;
        friend class ViewerInterface<InputStream>;
        friend class ViewerInterface;

    }; // end class Stream
  public:
    ///
    /// \brief
    /// \param onNewStreamer
    /// is an event handler called when new streamer (OutputStream) is recently connected to the
    /// server.
    /// \param onDelStreamer is an event handler called when connected streamer
    /// (OutputStream) is recently disconnected from the server.
    /// \param onServerNotFound
    /// \param onServerConnected is an event handler called when viewer is recently connected to the
    /// server. \param onServerDisconnected is an event handler called when viewer is recently
    /// disconnected from the server. \param onNewAcquisition is an event handler called when new
    /// acquisition is sended from any connected OutputStream to the server. \param onSetProperty
    /// \param ipv4 is the ip of the server you want to connect.
    /// \param port is the port server of the server you want to connect.
    /// \param autoSync server suggest auto synchronisation between OutputStream able to be
    /// synchronize if enable. \param onLogMessage is an event handler to receive debug message from
    /// server.
    ///
    explicit ViewerInterface(
        const std::string & name,
        std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer = {},
        std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer = {},
        std::function<void( const char* ipv4, int port )> onServerNotFound             = {},
        std::function<void( const char* ipv4, int port )> onServerConnected            = {},
        std::function<void( const char* ipv4, int port )> onServerDisconnected         = {},
        std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition =
            {},
        std::function<
            void( const char* streamName, const char* id, int property, const Any& value )>
            onSetProperty = {},
        //        bool autoSync                                              = true,
        std::function<void( const char* logMessage )> onLogMessage = {},
        const std::string& ipv4                                    = InputStream::s_defaultIpv4,
        int port                                                   = InputStream::s_defaultIpv4 );

    virtual ~ViewerInterface();

    //    virtual void connectToServer() = 0;
    //    template <class InputStream>
    //    virtual void routine() = 0;
    //    virtual void routine() = 0;

    ///
    /// \brief setIpv4
    /// function change the ip of the server you want to connect.
    /// \param ipv4
    /// is the new ip you want to use.
    /// \warning
    /// You can't change current ip if server is connected.
    ///
    void setIpv4( const std::string& ipv4 );

    ///
    /// \brief setPort
    /// function change the service port of the server you want to connect.
    /// \param port
    /// is the new service port you want to use.
    /// \warning
    /// You can't change current port if server is connected.
    ///
    void setPort( int port );

    ///
    /// \brief getIpv4
    /// is a getter function.
    /// \return
    /// the current ip of the server you want to connect.
    ///
    const std::string& getIpv4() const;

    ///
    /// \brief getPort
    /// is a getter function.
    /// \return
    /// the current service port you want to connect.
    ///
    const int& getPort() const;

    //    ///
    //    /// \brief setAutoSync
    //    /// is used to use or not the auto synchronization suggestion when
    //    /// new sensor is connecting to the server.
    //    /// \param newAutoSync
    //    /// enable/disable auto synchronization.
    //    ///
    //    void setAutoSync( bool newAutoSync );

    ///
    /// \brief isConnected
    /// check server connection.
    /// \return
    /// true if the server is connected.\n
    /// false otherwise.
    ///
    bool isConnected() const;

    ///
    /// \brief startStream
    /// \param streamName
    ///
    //    template <class InputStream>
    //    template <class InputStream>
    void startStream( const std::string& streamName );

    ///
    /// \brief stopStream
    /// \param streamName
    ///
    void stopStream( const std::string& streamName );

    ///
    /// \brief setProperty
    /// \param streamName
    /// \param objectName
    /// \param property
    /// \param value
    ///
    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const hub::Any& value );

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
  protected:
    void printStatus() const;

    void addStream( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void deleteStream( const std::string& streamName );
    //    std::thread m_thread;
    //    bool m_stopThread = false;

    std::string m_name;
    std::string m_ipv4;
    int m_port;

    std::function<bool( const char* streamName, const SensorSpec& )> m_onNewStreamer;
    std::function<void( const char* streamName, const SensorSpec& )> m_onDelStreamer;
    std::function<void( const char* ipv4, int port )> m_onServerNotFound;
    std::function<void( const char* ipv4, int port )> m_onServerConnected;
    std::function<void( const char* ipv4, int port )> m_onServerDisconnected;
    std::function<void( const char* streamName, const hub::Acquisition& )> m_onNewAcquisition;
    std::function<
        void( const char* streamName, const char* objectName, int property, const Any& value )>
        m_onSetProperty;

    //    net::ClientSocket m_sock;
    //    hub::Input& m_input;
    bool m_serverConnected = false;
    std::function<void( const char* logMessage )> m_onLogMessage;

    //    std::map<std::string, std::unique_ptr<StreamViewer>> m_streams;
    std::map<std::string, std::unique_ptr<Stream>> m_streams;

    static int m_iStreamer;
    //    std::map<std::string, StreamViewer> m_streams;

    friend class Stream;

  private:
};

template <class InputStream>
int ViewerInterface<InputStream>::m_iStreamer = 0;

//////////////////////////////////////// Stream ////////////////////////////////////////

template <class InputStream>
ViewerInterface<InputStream>::Stream::Stream(
    // Stream::Stream(
    // ViewerInterface::Stream::Stream(
    ViewerInterface& viewer,
    const std::string& ipv4,
    int port,
    const std::string& streamName,
    const SensorSpec& sensorSpec,
    std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const SensorSpec& )> onDelStreamer,
    std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
    std::function<void( const char* logMessage )> onLogMessage ) :
    //    m_viewer( viewer ),
    m_viewer( viewer ),
    m_ipv4( ipv4 ),
    m_port( port ),
    m_streamName( streamName ),
    m_sensorSpec( sensorSpec ),
    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onNewAcquisition( onNewAcquisition ),
    m_onLogMessage( onLogMessage ) {

    assert( m_onNewStreamer );
    m_streaming = m_onNewStreamer( streamName.c_str(), sensorSpec );
    if ( m_onNewAcquisition && m_streaming ) { startStream(); }
}

// template <class InputStream>
// Stream<InputStream>::~Stream() {
template <class InputStream>
ViewerInterface<InputStream>::Stream::~Stream() {
    // Stream::~Stream() {
    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' started" );
    if ( m_onNewAcquisition && m_streaming ) {
        assert( m_thread != nullptr );
        if ( m_onDelStreamer ) { m_onDelStreamer( m_streamName.c_str(), m_sensorSpec ); }
        stopStream();
    }

    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' ended" );
}

// template <class InputStream>
// void Stream<InputStream>::startStream() {
template <class InputStream>
void ViewerInterface<InputStream>::Stream::startStream() {
    // void Stream::startStream() {
    DEBUG_MSG( "[Viewer][Stream] startStream" );

    assert( m_thread == nullptr );

    m_stopThread = false;
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {

            //            m_inputSensor = std::make_unique<InputSensor>( input::InputStreamServer(
            m_inputSensor = std::make_unique<InputSensor>(
                InputStream( m_streamName,
                             //                    net::ClientSocket( m_viewer.m_sock.getIpv4(),
                             //                    m_viewer.m_sock.getPort() ) ) );
                             m_ipv4,
                             m_port ) );

            assert( m_onNewAcquisition );
            while ( !m_stopThread ) {
                hub::Acquisition acq;
                *m_inputSensor >> acq;
                m_onNewAcquisition( m_streamName.c_str(), acq );
            }
        }
        catch ( std::exception& e ) {
            DEBUG_MSG( "[Viewer][Stream] startStream() streamer '"
                       << "'" << m_streamName << "' disconnected, catch exception " << e.what() );
//            m_streaming = false;
//            m_viewer.deleteStream(m_streamName);
        }

        if ( m_stopThread ) {
            DEBUG_MSG( "[Viewer][Stream] streamer '" << m_streamName << "' thread killed " );
        }
        DEBUG_MSG( "[Viewer][Stream] thread end " );

        m_streaming = false;
//        m_viewer.printStatus();

    } );

    m_streaming = true;

    DEBUG_MSG( "[Viewer][Stream] startStream() streamer '" << m_streamName << "' inited " );
}

// template <class InputStream>
// void Stream<InputStream>::stopStream() {
template <class InputStream>
void ViewerInterface<InputStream>::Stream::stopStream() {
    // void Stream::stopStream() {
    DEBUG_MSG( "[Viewer][Stream] stopStream() streamer '" << m_streamName << "' started" );

    assert( m_thread != nullptr );
    assert( m_stopThread == false );
    m_stopThread = true;

    //    m_inputSensor->getInput().close();

    assert( m_thread->joinable() );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;

    //    m_streaming = false;

    DEBUG_MSG( "[Viewer][Stream] stopStream() streamer '" << m_streamName << "' ended" );
    m_viewer.printStatus();
}

//////////////////////////////////////// Viewer ////////////////////////////////////////

template <class InputStream>
ViewerInterface<InputStream>::ViewerInterface(
    const std::string & name,
    std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const SensorSpec& )> onDelStreamer,
    std::function<void( const char*, int )> onServerNotFound,
    std::function<void( const char*, int )> onServerConnected,
    std::function<void( const char*, int )> onServerDisconnected,
    std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
    std::function<void( const char*, const char*, int, const Any& )> onSetProperty,
    std::function<void( const char* )> onLogMessage,
    const std::string& ipv4,
    int port ) :

    m_name(utils::getHostname() + ":" + name),
    m_ipv4( ipv4 ),
    m_port( port ),
    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerNotFound( onServerNotFound ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_onNewAcquisition( onNewAcquisition ),
    m_onSetProperty( onSetProperty ),
    //    m_sock( ipv4, port, false ),
    m_onLogMessage( onLogMessage ) {

    //    m_thread = std::thread( [this]() {
    //    routine();
    //    } );  // thread
    //    m_thread.detach();
    //    printStatus();
}

template <class InputStream>
ViewerInterface<InputStream>::~ViewerInterface() {
    //    DEBUG_MSG( "[ViewerInterface] ~ViewerInterface()" );
    //    assert(! m_stopThread);
    //    m_stopThread = true;

    //    if ( m_sock.isOpen() ) { m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED ); }

    //    assert( m_thread.joinable() );
    //    m_thread.join();

    assert( m_streams.empty() );
    m_streams.clear();
    //    DEBUG_MSG( "[ViewerInterface] ~ViewerInterface() done" );
    //    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::startStream( const std::string& streamName ) {
    assert( m_onNewStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.startStream();
    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::stopStream( const std::string& streamName ) {
    DEBUG_MSG( "[Viewer] stopStream by user " << streamName );

    assert( m_onDelStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.stopStream();
    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::setProperty( const std::string& streamName,
                                                const std::string& id,
                                                int property,
                                                const Any& value ) {
    //    if ( m_sock.isOpen() ) {
    //        m_sock.write( net::ClientSocket::Message::SET_PROPERTY );
    //        m_sock.write( streamName );
    //        m_sock.write( id );
    //        m_sock.write( property );
    //        m_sock.write( value );
    //    }
}

template <class InputStream>
void ViewerInterface<InputStream>::printStatus() const {
    std::string str;
    for ( const auto& pair : m_streams ) {
        const auto& streamName = pair.first;
        const auto& stream     = pair.second;
        str += "(" + streamName + "," + std::to_string( stream->m_streaming ) + ")";
    }
    DEBUG_MSG( "\033[7m[Viewer] status : server connected:" << m_serverConnected
                                                            << ", nbStreamer:" << m_streams.size()
                                                            << " " << str << "\033[0m" );
    //    for (int i = 0; i < 50; ++i) {
    //        DEBUG_MSG( i << "\033[" << i << "m[Viewer] status : server connected:" <<
    //        m_serverConnected << ", nbStreamer:" << m_streams.size() << " " << str << "\033[0m" );
    //    }
}

template <class InputStream>
void ViewerInterface<InputStream>::addStream( const std::string& streamName,
                                              const SensorSpec& sensorSpec ) {
    assert( m_streams.find( streamName ) == m_streams.end() );

    if ( m_onNewStreamer ) {
        //                    assert(m_onDelStreamer);

        m_streams[streamName] =
            //                            std::make_unique<viewer::StreamViewer<input::InputStreamMqtt>>(
            std::make_unique<Stream>( *this,
                                      m_ipv4,
                                      m_port,
                                      streamName,
                                      sensorSpec,
                                      m_onNewStreamer,
                                      m_onDelStreamer,
                                      m_onNewAcquisition,
                                      m_onLogMessage );
        printStatus();
    }
}

template <class InputStream>
void ViewerInterface<InputStream>::deleteStream( const std::string& streamName ) {

    if ( m_onNewStreamer ) {
        assert( m_onDelStreamer );
        //                        assert( m_streams.find( streamName ) !=
        //                        m_streams.end() );
        if ( m_streams.find( streamName ) != m_streams.end() ) { m_streams.erase( streamName ); }
        printStatus();
    }
}

template <class InputStream>
void ViewerInterface<InputStream>::setIpv4( const std::string& ipv4 ) {
    DEBUG_MSG( "[Viewer] setIpv4 " << ipv4 );
    assert( !m_serverConnected );
    m_ipv4 = ipv4;
    //    m_sock.setIpv4( ipv4 );
}

template <class InputStream>
void ViewerInterface<InputStream>::setPort( int port ) {
    DEBUG_MSG( "[Viewer] setPort " << port );
    assert( !m_serverConnected );
    m_port = port;
    //    m_sock.setPort( port );
}

template <class InputStream>
const std::string& ViewerInterface<InputStream>::getIpv4() const {
    return m_ipv4;
    //    return m_sock.getIpv4();
}

template <class InputStream>
const int& ViewerInterface<InputStream>::getPort() const {
    return m_port;
    //    return m_sock.getPort();
}

// void ViewerInterface::setAutoSync( bool newAutoSync ) {
//     DEBUG_MSG( "[ViewerInterface] setAutoSync " << newAutoSync );
// }

template <class InputStream>
bool ViewerInterface<InputStream>::isConnected() const {
    return m_serverConnected;
}

} // namespace client
} // namespace hub
