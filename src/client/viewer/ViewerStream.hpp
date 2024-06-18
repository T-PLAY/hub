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

#include <atomic>
#include <cassert>
#include <functional>
#include <string>
#include <thread>

#include "ViewerHandler.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#    include "sensor/InputSensor.hpp"
#endif

#ifdef HUB_DEBUG_VIEWER_STREAM
#    ifdef DEBUG_MSG
#        undef DEBUG_MSG
#    endif
#    define DEBUG_MSG( _params )                                         \
        do {                                                             \
            if ( m_viewerHandler.onLogMessage ) {                        \
                std::stringstream _sstr;                                 \
                _sstr << m_iStreamer << ":" << _params;                  \
                m_viewerHandler.onLogMessage( _sstr.str().c_str() );     \
            }                                                            \
            else {                                                       \
                std::cout << m_iStreamer << ":" << _params << std::endl; \
            }                                                            \
        } while ( false );
#endif

namespace hub {
namespace client {

///
/// \brief The ViewerStream class
/// is the helper class to define what is a stream for viewer client implement
/// (only used by ViewerInterface)
///
template <class InputStream>
class ViewerStream
{
  public:
    ///
    /// \brief Stream is generalized by these parameters
    /// \param iStreamer [in] is the id of stream for log messages
    /// \param streamIpv4 [in] is the ip of stream
    /// \param streamPort [in] is the port of stream
    /// \param streamName [in] is the name of stream
    /// \param header [in] is the header of stream
    /// \param viewerHandler [ref] is the common handler of viewer class user
    ///
    ViewerStream( int iStreamer,
                  const std::string& streamIpv4,
                  int streamPort,
                  const std::string& streamName,
                  io::Header&& header,
                  hub::client::ViewerHandler& viewerHandler );

    ViewerStream( ViewerStream&& )      = delete;
    ViewerStream( const ViewerStream& ) = delete;

    ViewerStream& operator=( const ViewerStream& ) = delete;
    ViewerStream& operator=( ViewerStream&& ) = delete;

    ~ViewerStream();

    ///
    /// \brief Start collecting data from stream and shared events to viewer class user
    ///
    void startStream();

    ///
    /// \brief Stop collecting data from stream
    ///
    void stopStream();

    ///
    /// \brief Status function
    /// \return true if the stream is started\n
    /// false otherwise
    ///
    bool isStreaming() const;

  private:
    std::thread* m_thread = nullptr;
    bool m_stopThread     = false;

    std::string m_streamIpv4;
    int m_streamPort;

    const std::string m_streamName;
    const io::Header m_header;

    std::atomic<bool> m_streaming = false;

    ViewerHandler& m_viewerHandler;

    std::unique_ptr<InputStream> m_inputStream;
    const int m_iStreamer;

};

//////////////////////////////////// INLINE ////////////////////////////////////

template <class InputStream>
ViewerStream<InputStream>::ViewerStream( int iStreamer,
                                         const std::string& streamIpv4,
                                         int streamPort,
                                         const std::string& streamName,
                                         io::Header&& header,
                                         ViewerHandler& viewerHandler ) :

    m_iStreamer( iStreamer ),
    m_streamIpv4( streamIpv4 ),
    m_streamPort( streamPort ),
    m_streamName( streamName ),
    m_header( std::move( header ) ),
    m_viewerHandler( viewerHandler ) {

#ifndef HUB_NON_BUILD_SENSOR
    assert( m_viewerHandler.onNewStream || m_viewerHandler.onNewSensor );
#else
    assert( m_viewerHandler.onNewStream );
#endif

    if ( m_header.getUserDefined().empty() ) {
        const bool clientWantToWatchStream =
            m_viewerHandler.onNewStream( streamName.c_str(), m_header );
        if ( m_viewerHandler.onNewData && clientWantToWatchStream ) { startStream(); }
    }
#ifndef HUB_NON_BUILD_SENSOR
    else {
        hub::io::Memory memory( header.getUserDefined() );
        hub::sensor::SensorSpec sensorSpec;
        memory.read( sensorSpec );
        const bool clientWantToWatchStream =
            m_viewerHandler.onNewSensor( streamName.c_str(), sensorSpec );
        if ( m_viewerHandler.onNewAcq && clientWantToWatchStream ) { startStream(); }
    }
#endif
}

template <class InputStream>
ViewerStream<InputStream>::~ViewerStream() {
#ifdef HUB_DEBUG_VIEWER_STREAM
    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' started" );
#endif
#ifndef HUB_NON_BUILD_SENSOR
    if ( m_viewerHandler.onNewData || m_viewerHandler.onNewAcq ) {
#else
    if ( m_viewerHandler.onNewData ) {
#endif
        if ( m_streaming ) {
            assert( m_thread != nullptr );
            stopStream();
        }
        if ( m_viewerHandler.onDelStream ) { m_viewerHandler.onDelStream( m_streamName.c_str() ); }
    }

#ifdef HUB_DEBUG_VIEWER_STREAM
    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' ended" );
#endif
}

template <class InputStream>
void ViewerStream<InputStream>::startStream() {
#ifdef HUB_DEBUG_VIEWER_STREAM
    // DEBUG_MSG( "[Viewer][Stream] startStream" );
#endif

    assert( m_thread == nullptr );
    assert( !m_streaming );

    m_stopThread = false;
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {

#ifdef HUB_DEBUG_VIEWER_STREAM
            // DEBUG_MSG( "[Viewer][Stream] starting streaming thread" );
#endif
            // assert(m_inputStream == nullptr);
            // if (m_inputStream == nullptr) {
            m_inputStream = std::make_unique<InputStream>( m_streamPort, m_streamIpv4 );
            // }
            assert( m_inputStream->isOpen() );

#ifdef HUB_DEBUG_VIEWER_STREAM
            // DEBUG_MSG( "[Viewer][Stream] inputStream inited" );
#endif

            assert( m_inputStream->getHeader() == m_header );
            assert( m_header.getDataSize() > 0 );

            if ( m_header.getUserDefined().empty() ) {

                assert( m_viewerHandler.onNewData );

                Datas_t datas( m_header.getDataSize() );

                assert( !m_streaming );
                m_streaming = true;

                while ( !m_stopThread ) {
                    assert( m_inputStream->isOpen() );
                    m_inputStream->read( datas.data(), datas.size() );
                    m_viewerHandler.onNewData( m_streamName.c_str(), datas );
                }
            }
#ifndef HUB_NON_BUILD_SENSOR
            else {
                assert( m_viewerHandler.onNewAcq );

                // Datas_t datas( m_header.getDataSize() );
                sensor::InputSensor inputSensor( *m_inputStream );
                auto acq = inputSensor.acqMsg();

                assert( !m_streaming );
                m_streaming = true;

                while ( !m_stopThread ) {
                    assert( m_inputStream->isOpen() );
                    inputSensor >> acq;
                    m_viewerHandler.onNewAcq( m_streamName.c_str(), acq );
                }
            }
#endif
            // m_inputStream->close();
            // m_inputStream.release();
        }
#ifdef DEBUG
        catch ( std::exception& ex ) {
            DEBUG_MSG( "[Viewer][Stream] startStream() streamer '"
                       << "'" << m_streamName << "' disconnected, catch exception " << ex.what() );
        }
#else
        catch ( std::exception& ) {
        }

#endif

#ifdef HUB_DEBUG_VIEWER_STREAM
        if ( m_stopThread ) {
            DEBUG_MSG( "[Viewer][Stream] streamer '" << m_streamName << "' thread killed" );
        }
        DEBUG_MSG( "[Viewer][Stream] thread end" );
#endif

        m_streaming = false;
    } );

    while ( !m_streaming ) {
#ifdef HUB_DEBUG_VIEWER_STREAM
        // DEBUG_MSG( "[Viewer][Stream] waiting for start streaming ..." );
#endif
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }

#ifdef HUB_DEBUG_VIEWER_STREAM
    DEBUG_MSG( "[Viewer][Stream] streaming '" << m_streamName << "'" );
#endif
}

template <class InputStream>
void ViewerStream<InputStream>::stopStream() {
#ifdef HUB_DEBUG_VIEWER_STREAM
    DEBUG_MSG( "[Viewer][Stream] stop stream '" << m_streamName << "' started" );
#endif

    assert( m_thread != nullptr );
    assert( m_stopThread == false );
    m_stopThread = true;

    assert( m_thread->joinable() );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;

#ifdef HUB_DEBUG_VIEWER_STREAM
    DEBUG_MSG( "[Viewer][Stream] stop stream '" << m_streamName << "' ended" );
#endif
}

template <class InputStream>
bool ViewerStream<InputStream>::isStreaming() const {
    return m_streaming;
}

} // namespace client
} // namespace hub
