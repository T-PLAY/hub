#pragma once

#include <cassert>
#include <functional>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include "ViewerHandler.hpp"
#include "core/Utils.hpp"
// #include "sensor/Acquisition.hpp"

#ifdef HUB_BUILD_SENSOR
#    include "sensor/InputSensor.hpp"
#endif

// #include "sensor/SensorSpec.hpp"

#ifdef HUB_DEBUG_VIEWER_STREAM
#    ifdef DEBUG_MSG
#        undef DEBUG_MSG
#    endif
#    define DEBUG_MSG( _params )                                              \
        do {                                                                  \
            if ( m_viewerHandler.onLogMessage ) {                             \
                std::stringstream _sstr;                                      \
                _sstr << m_iStreamer << ":" << _params;                       \
                m_viewerHandler.onLogMessage( _sstr.str().c_str() );          \
            }                                                                 \
            else { std::cout << m_iStreamer << ":" << _params << std::endl; } \
        } while ( false );
#endif

namespace hub {
namespace client {

template <class InputStream>
class ViewerStream
{
  public:
    ViewerStream(
        int iStreamer,
        const std::string& streamIpv4,
        int streamPort,
        const std::string& streamName,
        io::Header&& header,
        ViewerHandler& viewerHandler
        // const sensor::SensorSpec& sensorSpec,
        // std::function<bool( const char* streamName, const sensor::SensorSpec& )> onNewStream,
        // std::function<void( const char* streamName, const sensor::SensorSpec& )> onDelStream,
        // std::function<void( const char* streamName, const sensor::Acquisition& )>
        // onNewAcq, std::function<void( const char* logMessage )> onLogMessage );
    );

    ViewerStream( ViewerStream&& )      = delete;
    ViewerStream( const ViewerStream& ) = delete;

    ViewerStream& operator=( const ViewerStream& ) = delete;
    ViewerStream& operator=( ViewerStream&& )      = delete;

    ~ViewerStream();

    void startStream();
    void stopStream();
    bool isStreaming() const;

  private:
    std::thread* m_thread = nullptr;
    bool m_stopThread     = false;

    std::string m_streamIpv4;
    int m_streamPort;

    const std::string m_streamName;
    const io::Header m_header;
    // const sensor::SensorSpec m_sensorSpec;

    std::atomic<bool> m_streaming = false;

    // std::function<bool( const char* streamName, const sensor::SensorSpec& )> m_onNewStreamer;
    // std::function<void( const char* streamName, const sensor::SensorSpec& )> m_onDelStreamer;
    // std::function<void( const char* streamName, const sensor::Acquisition& )> m_onNewAcquisition;
    // std::function<void( const char* logMessage )> m_onLogMessage;
    ViewerHandler& m_viewerHandler;

    std::unique_ptr<InputStream> m_inputStream;
    // std::unique_ptr<sensor::InputSensor> m_inputSensor;
    // static int m_iStreamer;
    const int m_iStreamer;

}; // end class ViewerStream

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

// template <class InputStream>
// int ViewerStream<InputStream>::m_iStreamer = 0;

template <class InputStream>
ViewerStream<InputStream>::ViewerStream(
    int iStreamer,
    const std::string& streamIpv4,
    int streamPort,
    const std::string& streamName,
    io::Header&& header,
    ViewerHandler& viewerHandler
    // const sensor::SensorSpec& sensorSpec,
    // std::function<bool( const char*, const sensor::SensorSpec& )> onNewStream,
    // std::function<void( const char*, const sensor::SensorSpec& )> onDelStream,
    // std::function<void( const char*, const sensor::Acquisition& )> onNewAcq,
    // std::function<void( const char* logMessage )> onLogMessage
    ) :

    m_iStreamer( iStreamer ),
    m_streamIpv4( streamIpv4 ),
    m_streamPort( streamPort ),
    m_streamName( streamName ),
    m_header( std::move( header ) ),
    m_viewerHandler( viewerHandler )
// m_sensorSpec( sensorSpec ),
// m_onNewStreamer( onNewStream ),
// m_onDelStreamer( onDelStream ),
// m_onNewAcquisition( onNewAcq ),
// m_onLogMessage( onLogMessage )
{

    assert( m_viewerHandler.onNewStream || m_viewerHandler.onNewSensor );
    // m_streaming = m_onNewStreamer( streamName.c_str(), sensorSpec );
    // const bool clientWantStream = m_onNewStreamer( streamName.c_str(), sensorSpec );

    // raw stream
    if ( m_header.getUserDefined().empty() ) {
        const bool clientWantToWatchStream =
            m_viewerHandler.onNewStream( streamName.c_str(), m_header );
        if ( m_viewerHandler.onNewData && clientWantToWatchStream ) { startStream(); }
    }
    // sensor stream
    else {
        hub::io::Memory memory(header.getUserDefined());
        hub::sensor::SensorSpec sensorSpec;
        memory.read(sensorSpec);
        const bool clientWantToWatchStream =
            m_viewerHandler.onNewSensor( streamName.c_str(), sensorSpec );
        if ( m_viewerHandler.onNewAcq && clientWantToWatchStream ) { startStream(); }
    }
}

template <class InputStream>
ViewerStream<InputStream>::~ViewerStream() {
#ifdef HUB_DEBUG_VIEWER_STREAM
    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' started" );
#endif
    // if ( m_viewerHandler.onNewAcq && m_streaming ) {
    if ( m_viewerHandler.onNewData || m_viewerHandler.onNewAcq ) {
        if ( m_streaming ) {
            assert( m_thread != nullptr );
            // if ( m_viewerHandler.onDelStream ) { m_viewerHandler.onDelStream(
            // m_streamName.c_str(), m_header ); }
            stopStream();
        }
        if ( m_viewerHandler.onDelStream ) { m_viewerHandler.onDelStream( m_streamName.c_str() ); }
        // if ( m_viewerHandler.onDelSensor ) { m_viewerHandler.onDelSensor( m_streamName.c_str() ); }
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
            // m_inputSensor = std::make_unique<sensor::InputSensor>(
            // InputStream( m_streamName, m_streamPort, m_streamIpv4 ) );
            // m_inputStream = std::make_unique<InputStream>( m_streamName, m_streamPort,
            // m_streamIpv4 );
            m_inputStream = std::make_unique<InputStream>( m_streamPort, m_streamIpv4 );
            assert( m_inputStream->isOpen() );
            // InputStream inputStream(m_streamName, m_streamPort, m_streamIpv4);

#ifdef HUB_DEBUG_VIEWER_STREAM
            // DEBUG_MSG( "[Viewer][Stream] inputStream inited" );
#endif

            // assert( m_viewerHandler.onInitStream );
            // m_viewerHandler.onInitStream(m_streamName.c_str(), *m_inputStream);

            // hub::io::Header retainedData;
            // m_inputStream->read(retainedData);
            // hub::io::Header retainedData(m_header.size());
            // m_inputStream->read(retainedData.data(), retainedData.size());
            assert( m_inputStream->getHeader() == m_header );
            assert( m_header.getDataSize() > 0 );

            // raw stream
            if ( m_header.getUserDefined().empty() ) {

                // assert( m_viewerHandler.onNewAcq );
                assert( m_viewerHandler.onNewData );

                Datas_t datas( m_header.getDataSize() );

                assert( !m_streaming );
                m_streaming = true;

                // auto acq = m_inputSensor->acqMsg();
                while ( !m_stopThread ) {
                    assert( m_inputStream->isOpen() );
                    // sensor::Acquisition acq;
                    // *m_inputSensor >> acq;
                    m_inputStream->read( datas.data(), datas.size() );
                    // m_viewerHandler.onNewAcq( m_streamName.c_str(), acq );
                    // m_viewerHandler.onNewData( m_streamName.c_str(), *m_inputStream );
                    m_viewerHandler.onNewData( m_streamName.c_str(), datas );
                }
            }
            // sensor stream
            else {
                assert( m_viewerHandler.onNewAcq );

                // Datas_t datas( m_header.getDataSize() );
                sensor::InputSensor inputSensor( *m_inputStream );
                auto acq = inputSensor.acqMsg();

                assert( !m_streaming );
                m_streaming = true;

                while ( !m_stopThread ) {
                    assert( m_inputStream->isOpen() );
                    // sensor::Acquisition acq;
                    inputSensor >> acq;
                    // std::cout << "[ViewerStream] on new acq : " << acq << std::endl;
                    // m_inputStream->read( datas.data(), datas.size() );
                    m_viewerHandler.onNewAcq( m_streamName.c_str(), acq );
                    // m_viewerHandler.onNewData( m_streamName.c_str(), *m_inputStream );
                    // m_viewerHandler.onNewData( m_streamName.c_str(), datas );
                }
            }
        }
        catch ( std::exception& ex ) {
            DEBUG_MSG( "[Viewer][Stream] startStream() streamer '"
                       << "'" << m_streamName << "' disconnected, catch exception " << ex.what() );
        }

#ifdef HUB_DEBUG_VIEWER_STREAM
        if ( m_stopThread ) {
            DEBUG_MSG( "[Viewer][Stream] streamer '" << m_streamName << "' thread killed" );
        }
        DEBUG_MSG( "[Viewer][Stream] thread end" );
#endif

        m_streaming = false;
    } );

    // m_streaming = true;
    while ( !m_streaming ) {
#ifdef HUB_DEBUG_VIEWER_STREAM
        // DEBUG_MSG( "[Viewer][Stream] waiting for start streaming ..." );
#endif
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
