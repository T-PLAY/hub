#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "SensorSpec.hpp"
//#include "net/ClientSocket.hpp"
#include "StreamViewer.hpp"

namespace hide {
#ifdef DEBUG_MSG
#    undef DEBUG_MSG
#endif
#define DEBUG_MSG( _params )                        \
    do {                                            \
        if ( m_onLogMessage ) {                     \
            std::stringstream _sstr;                \
            _sstr << _params;                       \
            m_onLogMessage( _sstr.str().c_str() );  \
        }                                           \
        else { std::cout << _params << std::endl; } \
    } while ( false );
}

namespace hub {
namespace client {


using namespace viewer;

//namespace viewer {
//class Stream;
//}

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
template <class InputStream>
class SRC_API ViewerInterface
{
  public:
    ///
    /// \brief
    /// \param onNewStreamer
    /// is an event handler called when new streamer (OutputStream) is recently connected to the
    /// server.
    /// \param onDelStreamer is an event handler called when connected streamer
    /// (OutputStream) is recently disconnected from the server.
    /// \param onServerNotFound
    /// \param onServerConnected is an event handler called when viewer is recently connected to the server.
    /// \param onServerDisconnected is an event handler called when viewer is recently disconnected from
    /// the server.
    /// \param onNewAcquisition is an event handler called when new acquisition is
    /// sended from any connected OutputStream to the server.
    /// \param onSetProperty
    /// \param ipv4 is the ip of the server you want to connect.
    /// \param port is the port server of the server you want to connect.
    /// \param autoSync server suggest auto synchronisation between OutputStream able to be synchronize if enable.
    /// \param onLogMessage is an event handler to receive debug message from server.
    ///
    explicit ViewerInterface(
        const std::string & ipv4,
        int port,
        std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer = {},
        std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer = {},
        std::function<void( const char* ipv4, int port )> onServerNotFound             = {},
        std::function<void( const char* ipv4, int port )> onServerConnected            = {},
        std::function<void( const char* ipv4, int port )> onServerDisconnected         = {},
        std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition =
            {},
        std::function<
            void( const char* streamName, const char* id, int property, const Any& value )>
            onSetProperty                                          = {},
//        bool autoSync                                              = true,
        std::function<void( const char* logMessage )> onLogMessage = {} );

    virtual ~ViewerInterface();

//    virtual void connectToServer() = 0;
//    template <class InputStream>
//    virtual void routine() = 0;
    virtual void routine() = 0;

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
    std::thread m_thread;
    bool m_stopThread = false;

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
    std::map<std::string, std::unique_ptr<StreamViewer<InputStream>>> m_streams;
//    std::map<std::string, StreamViewer> m_streams;

//    friend class viewer::StreamViewer;

  private:
};

////////////////////////////////////////////////////////////////////////////


template <class InputStream>
ViewerInterface<InputStream>::ViewerInterface(
    const std::string& ipv4,
    int port,
    std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const SensorSpec& )> onDelStreamer,
    std::function<void( const char*, int )> onServerNotFound,
    std::function<void( const char*, int )> onServerConnected,
    std::function<void( const char*, int )> onServerDisconnected,
    std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
    std::function<void( const char*, const char*, int, const Any& )> onSetProperty,
    std::function<void( const char* )> onLogMessage ) :

    m_ipv4(ipv4),
    m_port(port),
    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerNotFound( onServerNotFound ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_onNewAcquisition( onNewAcquisition ),
    m_onSetProperty( onSetProperty ),
//    m_sock( ipv4, port, false ),
    m_onLogMessage( onLogMessage ) {

    m_thread = std::thread( [this]() {
        routine();
    } );  // thread
//    m_thread.detach();
}


template <class InputStream>
ViewerInterface<InputStream>::~ViewerInterface() {
    DEBUG_MSG( "[ViewerInterface] ~ViewerInterface()" );
//    assert(! m_stopThread);
//    m_stopThread = true;

//    if ( m_sock.isOpen() ) { m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED ); }

//    assert( m_thread.joinable() );
//    m_thread.join();

    m_streams.clear();
    DEBUG_MSG( "[ViewerInterface] ~ViewerInterface() done" );
}

template <class InputStream>
void ViewerInterface<InputStream>::startStream( const std::string& streamName ) {
    assert( m_onNewStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.startStream();
}

template <class InputStream>
void ViewerInterface<InputStream>::stopStream( const std::string& streamName ) {
    DEBUG_MSG( "[ViewerInterface] stopStream by user " << streamName );

    assert( m_onDelStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.stopStream();
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
void ViewerInterface<InputStream>::setIpv4( const std::string& ipv4 ) {
    DEBUG_MSG( "[ViewerInterface] setIpv4 " << ipv4 );
    assert( !m_serverConnected );
    m_ipv4 = ipv4;
//    m_sock.setIpv4( ipv4 );
}

template <class InputStream>
void ViewerInterface<InputStream>::setPort( int port ) {
    DEBUG_MSG( "[ViewerInterface] setPort " << port );
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
