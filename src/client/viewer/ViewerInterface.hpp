/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include "ViewerHandler.hpp"
#include "ViewerStream.hpp"
#include "core/Utils.hpp"

#ifdef DEBUG_MSG
#    undef DEBUG_MSG
#endif
#define DEBUG_MSG( _params )                                                   \
    do {                                                                       \
        if ( m_viewerHandler.onLogMessage ) {                                  \
            std::stringstream _sstr;                                           \
            _sstr << m_nStreamer << "/" << m_streams.size() << ":" << _params; \
            m_viewerHandler.onLogMessage( _sstr.str().c_str() );               \
        }                                                                      \
        else {                                                                 \
            std::cout << m_nStreamer << ":" << _params << std::endl;           \
        }                                                                      \
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
class ViewerInterface
{
  protected:
  public:
    ///
    /// \brief
    /// \param name
    /// \param viewerHandler
    /// \param ipv4 is the ip of the server you want to connect.
    /// \param port is the port server of the server you want to connect.
    /// \param autoConnect
    ///
    explicit ViewerInterface( const std::string& name,
                              ViewerHandler&& viewerHandler = ViewerHandler(),
                              const std::string& ipv4       = HUB_SERVICE_IPV4,
                              int port                      = HUB_SERVICE_PORT,
                              bool autoConnect              = true );

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

    ///
    /// \brief setAutoConnect
    /// \param autoConnect
    ///
    virtual void setAutoConnect( bool autoConnect );

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

    ///
    /// \brief nStream
    /// \return
    ///
    int nStream() const;

    ///
    /// \brief nStreaming
    /// \return
    ///
    int nStreaming() const;

    ///
    /// \brief printStatus
    ///
    void printStatus() const;

    ///
    /// \brief hasStream
    /// \param streamName
    /// \return
    ///
    bool hasStream( const std::string& streamName );

    ///
    /// \brief getActiveStreams
    /// \return
    ///
    std::set<std::string> getActiveStreams() const;


  private:
  protected:
    ///
    /// \brief addStream
    /// \param streamName
    /// \param streamIpv4
    /// \param streamPort
    /// \param header
    ///
    void addStream( const std::string& streamName,
                    const std::string& streamIpv4,
                    int streamPort,
                    io::Header&& header );

    ///
    /// \brief delStream
    /// \param streamName
    ///
    void delStream( const std::string& streamName );

    ///
    /// \brief m_name
    ///
    std::string m_name;

    ///
    /// \brief m_viewerHandler
    ///
    ViewerHandler m_viewerHandler;

    ///
    /// \brief m_serverIpv4
    ///
    std::string m_serverIpv4;

    ///
    /// \brief m_serverPort
    ///
    int m_serverPort;

    ///
    /// \brief m_serverConnected
    ///
    bool m_serverConnected = false;

    ///
    /// \brief m_streams
    ///
    std::map<std::string, std::unique_ptr<ViewerStream<InputStream>>> m_streams;

    ///
    /// \brief m_nStreamer
    ///
    int m_nStreamer = 0;

    ///
    /// \brief m_autoConnect
    ///
    bool m_autoConnect;

  private:
};

//////////////////////////////////////// Viewer ////////////////////////////////////////

template <class InputStream>
ViewerInterface<InputStream>::ViewerInterface( const std::string& name,
                                               ViewerHandler&& viewerHandler,
                                               const std::string& ipv4,
                                               int port,
                                               bool autoConnect ) :

    m_name( utils::getHostname() + ":" + name ),
    m_viewerHandler( std::move( viewerHandler ) ),
    m_serverIpv4( ipv4 ),
    m_serverPort( port ),
    m_autoConnect( autoConnect ) {}

template <class InputStream>
ViewerInterface<InputStream>::~ViewerInterface() {

    assert( m_streams.empty() );
    m_streams.clear();
}

/////////////////////////////////////////////////////

template <class InputStream>
void ViewerInterface<InputStream>::addStream( const std::string& streamName,
                                              const std::string& streamIpv4,
                                              int streamPort,
                                              io::Header&& header ) {
    assert( m_streams.find( streamName ) == m_streams.end() );

#ifndef HUB_NON_BUILD_SENSOR
    if ( m_viewerHandler.onNewStream || m_viewerHandler.onNewSensor ) {
#else
    if ( m_viewerHandler.onNewStream ) {
#endif

        m_streams[streamName] = std::make_unique<ViewerStream<InputStream>>( m_nStreamer++,
                                                                             streamIpv4,
                                                                             streamPort,
                                                                             streamName,
                                                                             std::move( header ),
                                                                             m_viewerHandler );
        printStatus();
    }
}

template <class InputStream>
void ViewerInterface<InputStream>::delStream( const std::string& streamName ) {

#ifndef HUB_NON_BUILD_SENSOR
    if ( m_viewerHandler.onNewStream || m_viewerHandler.onNewSensor ) {
#else
    if ( m_viewerHandler.onNewStream ) {
#endif
        assert( m_streams.find( streamName ) != m_streams.end() );
        m_streams.erase( streamName );
        printStatus();
    }
}

template <class InputStream>
void ViewerInterface<InputStream>::startStream( const std::string& streamName ) {
#ifndef HUB_NON_BUILD_SENSOR
    assert( m_viewerHandler.onNewStream || m_viewerHandler.onNewSensor );
#else
    assert( m_viewerHandler.onNewStream );
#endif
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.startStream();
    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::stopStream( const std::string& streamName ) {
    DEBUG_MSG( "[Viewer] stopStream by user " << streamName );

    assert( m_viewerHandler.onDelStream );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.stopStream();
    printStatus();
}

template <class InputStream>
void ViewerInterface<InputStream>::setProperty( const std::string& streamName,
                                                const std::string& id,
                                                int property,
                                                const Any& value ) {}

template <class InputStream>
int ViewerInterface<InputStream>::nStream() const {
    return m_streams.size();
}

template <class InputStream>
int ViewerInterface<InputStream>::nStreaming() const {
    int ret = 0;
    for ( const auto& pair : m_streams ) {
        const auto& stream = pair.second;

        if ( stream->isStreaming() ) { ++ret; }
    }
    return ret;
}

template <class InputStream>
void ViewerInterface<InputStream>::printStatus() const {
    std::string str;
    for ( const auto& pair : m_streams ) {
        auto streamName = pair.first;
        streamName      = streamName.substr( std::max( 0, (int)streamName.size() - 12 ), 12 );

        const auto& stream = pair.second;
        if ( stream == nullptr ) { str += " (" + streamName + ",null)"; }
        else {
            str += " (" + streamName + "," + std::to_string( stream->isStreaming() ) + ")";
        }
    }
    // Todo fix for Hololens, no unicode \u001B
    //DEBUG_MSG( "\033[7m[Viewer] status : server connected:"
    DEBUG_MSG( "[Viewer] status : server connected:"
               << m_serverConnected << ", nStreamer:" << m_streams.size() << str );
}

template <class InputStream>
bool ViewerInterface<InputStream>::hasStream( const std::string& streamName ) {
    return m_streams.find( streamName ) != m_streams.end();
}

template <class InputStream>
std::set<std::string> ViewerInterface<InputStream>::getActiveStreams() const {
    std::set<std::string> streamNames;
    for ( const auto& [streamName, inputStream] : m_streams ) {
        if ( inputStream->isStreaming() ) { streamNames.insert( streamName ); }
    }
    return streamNames;
}



template <class InputStream>
void ViewerInterface<InputStream>::setIpv4( const std::string& ipv4 ) {
    DEBUG_MSG( "[Viewer] setIpv4 " << ipv4 );
    assert( !m_serverConnected );
    m_serverIpv4 = ipv4;
}

template <class InputStream>
void ViewerInterface<InputStream>::setPort( int port ) {
    DEBUG_MSG( "[Viewer] setPort " << port );
    assert( !m_serverConnected );
    m_serverPort = port;
}

template <class InputStream>
const std::string& ViewerInterface<InputStream>::getIpv4() const {
    return m_serverIpv4;
}

template <class InputStream>
const int& ViewerInterface<InputStream>::getPort() const {
    return m_serverPort;
}

template <class InputStream>
void ViewerInterface<InputStream>::setAutoConnect( bool autoConnect ) {
    m_autoConnect = autoConnect;
}

template <class InputStream>
bool ViewerInterface<InputStream>::isConnected() const {
    return m_serverConnected;
}

} // namespace client
} // namespace hub
