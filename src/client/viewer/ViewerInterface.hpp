#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

// #include "sensor/Acquisition.hpp"
// #include "sensor/SensorSpec.hpp"
// #include "net/ClientSocket.hpp"
// #include "StreamViewer.hpp"
// #include "sensor/InputSensor.hpp"
#include "ViewerHandler.hpp"
#include "ViewerStream.hpp"
#include "core/Utils.hpp"

#ifdef DEBUG_MSG
#    undef DEBUG_MSG
#endif
#define DEBUG_MSG( _params )                                              \
    do {                                                                  \
        if ( m_viewerHandler.onLogMessage ) {                             \
            std::stringstream _sstr;                                      \
            _sstr << m_iStreamer << ":" << _params;                       \
            m_viewerHandler.onLogMessage( _sstr.str().c_str() );          \
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
template <class InputStream>
class SRC_API ViewerInterface
{
  protected:
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
    explicit ViewerInterface( const std::string& name,
                              ViewerHandler&& viewerHandler = ViewerHandler(),
                              const std::string& ipv4       = InputStream::s_defaultIpv4,
                              int port                      = InputStream::s_defaultIpv4 );

    virtual ~ViewerInterface();

    ///
    /// \brief setIpv4
    /// function change the ip of the server you want to connect.
    /// \param ipv4
    /// is the new ip you want to use.
    /// \warning
    /// You can't change current ip if server is connected.
    ///
    virtual void setIpv4( const std::string& ipv4 );

    ///
    /// \brief setPort
    /// function change the service port of the server you want to connect.
    /// \param port
    /// is the new service port you want to use.
    /// \warning
    /// You can't change current port if server is connected.
    ///
    virtual void setPort( int port );

    ///
    /// \brief getIpv4
    /// is a getter function.
    /// \return
    /// the current ip of the server you want to connect.
    ///
    virtual const std::string& getIpv4() const;

    ///
    /// \brief getPort
    /// is a getter function.
    /// \return
    /// the current service port you want to connect.
    ///
    virtual const int& getPort() const;

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
    virtual bool isConnected() const;

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
    virtual void setProperty( const std::string& streamName,
                              const std::string& objectName,
                              int property,
                              const Any& value );

           ///////////////////////////////////////////////////////////////////////////////////////////////


    int nStream() const;

  private:
  protected:
    void printStatus() const;

    void addStream( const std::string& streamName, const sensor::SensorSpec& sensorSpec );
    void deleteStream( const std::string& streamName );

    std::string m_name;
    ViewerHandler m_viewerHandler;
    std::string m_ipv4;
    int m_port;

    bool m_serverConnected = false;

    std::map<std::string, std::unique_ptr<ViewerStream<InputStream>>> m_streams;

    static int m_iStreamer;

  private:
};

template <class InputStream>
int ViewerInterface<InputStream>::m_iStreamer = 0;

//////////////////////////////////////// Viewer ////////////////////////////////////////

template <class InputStream>
ViewerInterface<InputStream>::ViewerInterface( const std::string& name,
                                               ViewerHandler&& viewerHandler,
                                               const std::string& ipv4,
                                               int port ) :

    m_name( utils::getHostname() + ":" + name ),
    m_viewerHandler( std::move( viewerHandler ) ),
    m_ipv4( ipv4 ),
    m_port( port ) {}


template <class InputStream>
ViewerInterface<InputStream>::~ViewerInterface() {
    //    DEBUG_MSG( "[ViewerInterface] ~ViewerInterface()" );
    //    assert(! m_stopThread);
    //    m_stopThread = true;

    //    if ( m_sock.isOpen() ) { m_sock.write( io::StreamInterface::ClientMessage::VIEWER_CLOSED
    //    ); }

    //    assert( m_thread.joinable() );
    //    m_thread.join();

    assert( m_streams.empty() );
    m_streams.clear();
    //    DEBUG_MSG( "[ViewerInterface] ~ViewerInterface() done" );
    //    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::startStream( const std::string& streamName ) {
    assert( m_viewerHandler.onNewStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.startStream();
    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::stopStream( const std::string& streamName ) {
    DEBUG_MSG( "[Viewer] stopStream by user " << streamName );

    assert( m_viewerHandler.onDelStreamer );
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
    //        m_sock.write( io::StreamInterface::ClientMessage::SET_PROPERTY );
    //        m_sock.write( streamName );
    //        m_sock.write( id );
    //        m_sock.write( property );
    //        m_sock.write( value );
    //    }
}

template<class InputStream>
int ViewerInterface<InputStream>::nStream() const
{
    return m_streams.size();
}

template <class InputStream>
void ViewerInterface<InputStream>::printStatus() const {
    std::string str;
    for ( const auto& pair : m_streams ) {
        const auto& streamName = pair.first;
        const auto& stream     = pair.second;
        if ( stream == nullptr ) { str += "(" + streamName + ",null)"; }
        else { str += "(" + streamName + "," + std::to_string( stream->isStreaming() ) + ")"; }
    }
    DEBUG_MSG( "\033[7m[Viewer] status : server connected:" << m_serverConnected
                                                            << ", nbStreamer:" << m_streams.size()
                                                            << " " << str << "\033[0m" );
}

template <class InputStream>
void ViewerInterface<InputStream>::addStream( const std::string& streamName,
                                              const sensor::SensorSpec& sensorSpec ) {
    assert( m_streams.find( streamName ) == m_streams.end() );

    if ( m_viewerHandler.onNewStreamer ) {

        m_streams[streamName] =
            std::make_unique<ViewerStream<InputStream>>(
                m_ipv4,
                m_port,
                streamName,
                sensorSpec,
                m_viewerHandler.onNewStreamer,
                m_viewerHandler.onDelStreamer,
                m_viewerHandler.onNewAcquisition,
                m_viewerHandler.onLogMessage );
        printStatus();
    }
}

template <class InputStream>
void ViewerInterface<InputStream>::deleteStream( const std::string& streamName ) {

    if ( m_viewerHandler.onNewStreamer ) {
        assert( m_viewerHandler.onDelStreamer );
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
