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
#include <sstream>
#include <string>

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

///
/// \brief The ViewerInterface class
/// can be implement for user wanted to create own viewer using own server implement (ex: Mqtt viewer using mqtt service).
/// This interface allow to compare performance of communication between different implements.
/// For example we can compare the performance with the current server/viewer implements with Mqtt protocol during the tests.
/// If you want to create your own server/viewer implements, please use this interface, we can integrate your implement
/// if this one is better than the current implement. Thank you in advance.
///
template <class InputStream>
class ViewerInterface
{
  public:

    ///
    /// \brief Construct viewer with unified parameters corresponding of viewer features
    /// \param name [in] is used to identify viewer instance from others in server logs.
    /// \param viewerHandler [in] represents user events catcher
    /// \param serverIpv4 [in] is the ip of server you want to watch
    /// \param serverPort [in] is the port of server you want to watch
    /// \param autoConnect [in] is the startup option,
    /// true if you want to check for server after creation, false if you don't want to connect to server now
    ///
    explicit ViewerInterface( const std::string& name,
                              ViewerHandler&& viewerHandler = ViewerHandler(),
                              const std::string& serverIpv4       = HUB_SERVICE_IPV4,
                              int serverPort                      = HUB_SERVICE_PORT,
                              bool autoConnect              = true );

    virtual ~ViewerInterface();

    ///
    /// \brief basic feature able to change ip during process
    /// (ex: change to backup server if first server is broken, ping server is running, ip scans).
    /// This function change the ip of the server you want to connect.
    /// \param ipv4 [in] is the new ip of server you want to watch.
    /// \warning You can't change current ip if server is connected.
    ///
    virtual void setIpv4( const std::string& ipv4 );

    ///
    /// \brief Setter function.
    /// This function change the service port of the server you want to connect.
    /// \param port [in] is the new service port you want to watch.
    /// \warning
    /// You can't change current port if server is connected.
    ///
    virtual void setPort( int port );

    ///
    /// \brief Getter function.
    /// \return The current ip of the server is trying to connect or connected.
    ///
    virtual const std::string& getIpv4() const;

    ///
    /// \brief Getter function.
    /// \return The current service port is trying to connect or connected.
    ///
    virtual int getPort() const;

    ///
    /// \brief Setter function.
    /// \param autoConnect [in] is a common feature.
    /// If auto connection is on, viewer is looking/pinging for current ip and port until server is connected.
    ///
    virtual void setAutoConnect( bool autoConnect );

    ///
    /// \brief Check for server status.
    /// \return
    /// True if the server is connected.\n
    /// False otherwise.
    ///
    virtual bool isConnected() const;

    ///
    /// \brief User of Viewer class want to watch active stream.
    /// \param streamName [in] user want to watch
    /// \warning streamName has to be active on server
    ///
    void startStream( const std::string& streamName );

    ///
    /// \brief User of Viewer class want to stop watching stream.
    /// \param streamName [in] user want to stop watching
    ///
    void stopStream( const std::string& streamName );

    ///
    /// \brief Set render options from user of Viewer class.
    /// These options are shared to other viewer of the same stream
    /// \param streamName [in] the user want to change option
    /// \param objectName [in] the user want to change option
    /// \param property [in] the user want to change property
    /// \param value [in] the user want to set
    ///
    virtual void setProperty( const std::string& streamName,
                              const std::string& objectName,
                              int property,
                              const Any& value );

    ///
    /// \brief Getter function
    /// \return The number of connected stream
    ///
    int nStream() const;

    ///
    /// \brief Getter function
    /// \return The number of watching stream.
    /// Number of watching stream is always less or equal than the number of connected stream
    ///
    int nStreaming() const;

    ///
    /// \brief Print current status of viewer
    ///
    void printStatus() const;

    ///
    /// \brief Check if stream is connected
    /// \param streamName [in] user wants to check if active or not
    /// \return true if streamName is connected\n
    /// False otherwise
    ///
    bool hasStream( const std::string& streamName );

    ///
    /// \brief Getter function
    /// \return The number of watching streams
    ///
    std::set<std::string> getActiveStreams() const;

  protected:

    ///
    /// \brief Helper function is about to add new stream (when new stream is connected to the server)
    /// \param streamName [in] is connected to the server
    /// \param streamIpv4 [in] is the sensor stream ip corresponding of the stream process (data is not passing through server)
    /// \param streamPort [in] is the sensor stream port corresponding of the stream process (data is not passing through server)
    /// \param header [in] corresponds to the stream header (additional information)
    ///
    void addStream( const std::string& streamName,
                    const std::string& streamIpv4,
                    int streamPort,
                    io::Header&& header );

    ///
    /// \brief Helper function about to remove stream (when disconnected from server)
    /// \param streamName [in] correspond of the stream name to remove
    ///
    void delStream( const std::string& streamName );

    ///
    /// \brief Unique string to identify viewer over all viewers that can be connected to the same server.
    ///
    std::string m_name;

    ///
    /// \brief Client side event dispatcher.
    ///
    ViewerHandler m_viewerHandler;

    ///
    /// \brief Current ip about to connect to server
    ///
    std::string m_serverIpv4;

    ///
    /// \brief Current port about to connect to server
    ///
    int m_serverPort;

    ///
    /// \brief Server connected status
    ///
    bool m_serverConnected = false;

    ///
    /// \brief Connected streams do not imply there are watched.
    ///
    std::map<std::string, std::unique_ptr<ViewerStream<InputStream>>> m_streams;

    ///
    /// \brief Counter of streamer, able to identify streamer in log messages
    ///
    int m_nStreamer = 0;

    ///
    /// \brief Auto connect to server (ping ip and port until server is connected)
    ///
    bool m_autoConnect;

  private:
};

//////////////////////////////////// INLINE ////////////////////////////////////

template <class InputStream>
ViewerInterface<InputStream>::ViewerInterface( const std::string& name,
                                               ViewerHandler&& viewerHandler,
                                               const std::string& serverIpv4,
                                               int serverPort,
                                               bool autoConnect ) :

    m_name( utils::getHostname() + ":" + name ),
    m_viewerHandler( std::move( viewerHandler ) ),
    m_serverIpv4( serverIpv4 ),
    m_serverPort( serverPort ),
    m_autoConnect( autoConnect ) {}

template <class InputStream>
ViewerInterface<InputStream>::~ViewerInterface() {

    assert( m_streams.empty() );
    m_streams.clear();
}

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
                                                const Any& value ) {
    // Todo implement this feature
}

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
    // DEBUG_MSG( "\033[7m[Viewer] status : server connected:"
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
int ViewerInterface<InputStream>::getPort() const {
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
