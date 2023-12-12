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
// #include "sensor/InputSensor.hpp"
// #include "sensor/SensorSpec.hpp"

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

template <class InputStream>
class ViewerStream
{
  public:
    ViewerStream(
        int iStreamer,
        const std::string& streamIpv4,
        int streamPort,
        const std::string& streamName,
        Datas_t&& header,
        ViewerHandler& viewerHandler
        // const sensor::SensorSpec& sensorSpec,
        // std::function<bool( const char* streamName, const sensor::SensorSpec& )> onNewStream,
        // std::function<void( const char* streamName, const sensor::SensorSpec& )> onDelStream,
        // std::function<void( const char* streamName, const sensor::Acquisition& )>
        // onNewAcquisition, std::function<void( const char* logMessage )> onLogMessage );
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
    const Datas_t m_header;
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
    Datas_t&& header,
    ViewerHandler& viewerHandler
    // const sensor::SensorSpec& sensorSpec,
    // std::function<bool( const char*, const sensor::SensorSpec& )> onNewStream,
    // std::function<void( const char*, const sensor::SensorSpec& )> onDelStream,
    // std::function<void( const char*, const sensor::Acquisition& )> onNewAcquisition,
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
// m_onNewAcquisition( onNewAcquisition ),
// m_onLogMessage( onLogMessage )
{

    assert( m_viewerHandler.onNewStream );
    // m_streaming = m_onNewStreamer( streamName.c_str(), sensorSpec );
    // const bool clientWantStream = m_onNewStreamer( streamName.c_str(), sensorSpec );
    const bool clientWantToWatchStream =
        m_viewerHandler.onNewStream( streamName.c_str(), m_header );
    // if ( m_viewerHandler.onNewAcquisition && clientWantToWatchStream ) { startStream(); }
    if ( m_viewerHandler.onNewData && clientWantToWatchStream ) { startStream(); }
}

template <class InputStream>
ViewerStream<InputStream>::~ViewerStream() {
    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' started" );
    // if ( m_viewerHandler.onNewAcquisition && m_streaming ) {
    if ( m_viewerHandler.onNewData && m_streaming ) {
        assert( m_thread != nullptr );
        // if ( m_viewerHandler.onDelStream ) { m_viewerHandler.onDelStream( m_streamName.c_str(),
        // m_header ); }
        if ( m_viewerHandler.onDelStream ) { m_viewerHandler.onDelStream( m_streamName.c_str() ); }
        stopStream();
    }

    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' ended" );
}

template <class InputStream>
void ViewerStream<InputStream>::startStream() {
    DEBUG_MSG( "[Viewer][Stream] startStream" );

    assert( m_thread == nullptr );
    assert( !m_streaming );

    m_stopThread = false;
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {

            DEBUG_MSG( "[Viewer][Stream] starting streaming thread" );
            // m_inputSensor = std::make_unique<sensor::InputSensor>(
            // InputStream( m_streamName, m_streamPort, m_streamIpv4 ) );
            // m_inputStream = std::make_unique<InputStream>( m_streamName, m_streamPort,
            // m_streamIpv4 );
            m_inputStream = std::make_unique<InputStream>( m_streamPort, m_streamIpv4 );
            assert( m_inputStream->isOpen() );
            // InputStream inputStream(m_streamName, m_streamPort, m_streamIpv4);

            DEBUG_MSG( "[Viewer][Stream] inputStream inited" );

            // assert( m_viewerHandler.onInitStream );
            // m_viewerHandler.onInitStream(m_streamName.c_str(), *m_inputStream);

            // hub::Datas_t retainedData;
            // m_inputStream->read(retainedData);
            // hub::Datas_t retainedData(m_header.size());
            // m_inputStream->read(retainedData.data(), retainedData.size());
            assert( m_inputStream->getHeader() == m_header );

            // assert( m_viewerHandler.onNewAcquisition );
            assert( m_viewerHandler.onNewData );

            assert( !m_streaming );
            m_streaming = true;

            // auto acq = m_inputSensor->acqMsg();
            while ( !m_stopThread ) {
                assert( m_inputStream->isOpen() );
                // sensor::Acquisition acq;
                // *m_inputSensor >> acq;
                // m_viewerHandler.onNewAcquisition( m_streamName.c_str(), acq );
                m_viewerHandler.onNewData( m_streamName.c_str(), *m_inputStream );
            }
        }
        catch ( std::exception& e ) {
            DEBUG_MSG( "[Viewer][Stream] startStream() streamer '"
                       << "'" << m_streamName << "' disconnected, catch exception " << e.what() );
        }

        if ( m_stopThread ) {
            DEBUG_MSG( "[Viewer][Stream] streamer '" << m_streamName << "' thread killed" );
        }
        DEBUG_MSG( "[Viewer][Stream] thread end" );

        m_streaming = false;
    } );

    // m_streaming = true;
    while ( !m_streaming ) {
        DEBUG_MSG( "[Viewer][Stream] waiting for start streaming ..." );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    DEBUG_MSG( "[Viewer][Stream] startStream() streamer '" << m_streamName << "' inited" );
}

template <class InputStream>
void ViewerStream<InputStream>::stopStream() {
    DEBUG_MSG( "[Viewer][Stream] stopStream() streamer '" << m_streamName << "' started" );

    assert( m_thread != nullptr );
    assert( m_stopThread == false );
    m_stopThread = true;

    assert( m_thread->joinable() );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;

    DEBUG_MSG( "[Viewer][Stream] stopStream() streamer '" << m_streamName << "' ended" );
}

template <class InputStream>
bool ViewerStream<InputStream>::isStreaming() const {
    return m_streaming;
}

} // namespace client
} // namespace hub
