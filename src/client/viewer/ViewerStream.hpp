#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include "core/Utils.hpp"
#include "sensor/Acquisition.hpp"
#include "sensor/InputSensor.hpp"
#include "sensor/SensorSpec.hpp"

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

template <class InputStream>
class ViewerStream
{
  public:
    ViewerStream(
        int iStreamer,
        const std::string& ipv4,
        int port,
        const std::string& streamName,
        const sensor::SensorSpec& sensorSpec,
        std::function<bool( const char* streamName, const sensor::SensorSpec& )> onNewStreamer,
        std::function<void( const char* streamName, const sensor::SensorSpec& )> onDelStreamer,
        std::function<void( const char* streamName, const sensor::Acquisition& )> onNewAcquisition,
        std::function<void( const char* logMessage )> onLogMessage );

    ~ViewerStream();

    void startStream();
    void stopStream();
    bool isStreaming() const;

  private:
    std::thread* m_thread = nullptr;
    bool m_stopThread     = false;

    std::string m_ipv4;
    int m_port;

    const std::string m_streamName;
    const sensor::SensorSpec m_sensorSpec;

    bool m_streaming = false;

    std::function<bool( const char* streamName, const sensor::SensorSpec& )> m_onNewStreamer;
    std::function<void( const char* streamName, const sensor::SensorSpec& )> m_onDelStreamer;
    std::function<void( const char* streamName, const sensor::Acquisition& )> m_onNewAcquisition;
    std::function<void( const char* logMessage )> m_onLogMessage;

    std::unique_ptr<sensor::InputSensor> m_inputSensor;
    // static int m_iStreamer;
    const int m_iStreamer;

}; // end class ViewerStream

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

// template <class InputStream>
// int ViewerStream<InputStream>::m_iStreamer = 0;

template <class InputStream>
ViewerStream<InputStream>::ViewerStream(
    int iStreamer,
    const std::string& ipv4,
    int port,
    const std::string& streamName,
    const sensor::SensorSpec& sensorSpec,
    std::function<bool( const char*, const sensor::SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const sensor::SensorSpec& )> onDelStreamer,
    std::function<void( const char*, const sensor::Acquisition& )> onNewAcquisition,
    std::function<void( const char* logMessage )> onLogMessage ) :

    m_iStreamer(iStreamer),
    m_ipv4( ipv4 ),
    m_port( port ),
    m_streamName( streamName ),
    m_sensorSpec( sensorSpec ),
    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onNewAcquisition( onNewAcquisition ),
    m_onLogMessage( onLogMessage ) {

    assert( m_onNewStreamer );
    // m_streaming = m_onNewStreamer( streamName.c_str(), sensorSpec );
    const bool acceptStreamer = m_onNewStreamer( streamName.c_str(), sensorSpec );
    if ( m_onNewAcquisition && acceptStreamer ) { startStream(); }
}

template <class InputStream>
ViewerStream<InputStream>::~ViewerStream() {
    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' started" );
    if ( m_onNewAcquisition && m_streaming ) {
        assert( m_thread != nullptr );
        if ( m_onDelStreamer ) { m_onDelStreamer( m_streamName.c_str(), m_sensorSpec ); }
        stopStream();
    }

    DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' ended" );
}

template <class InputStream>
void ViewerStream<InputStream>::startStream() {
    DEBUG_MSG( "[Viewer][Stream] startStream" );

    assert( m_thread == nullptr );

    m_stopThread = false;
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {

            m_inputSensor = std::make_unique<sensor::InputSensor>(
                InputStream( m_streamName, m_port, m_ipv4 ) );

            assert( m_onNewAcquisition );

            assert(! m_streaming);
            m_streaming = true;
            auto acq = m_inputSensor->acqMsg();
            while ( !m_stopThread ) {
                // sensor::Acquisition acq;
                *m_inputSensor >> acq;
                m_onNewAcquisition( m_streamName.c_str(), acq );
            }
        }
        catch ( std::exception& e ) {
            DEBUG_MSG( "[Viewer][Stream] startStream() streamer '"
                       << "'" << m_streamName << "' disconnected, catch exception " << e.what() );
        }

        if ( m_stopThread ) {
            DEBUG_MSG( "[Viewer][Stream] streamer '" << m_streamName << "' thread killed " );
        }
        DEBUG_MSG( "[Viewer][Stream] thread end " );

        m_streaming = false;
    } );

    // m_streaming = true;
    // while (! m_streaming) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // }
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    DEBUG_MSG( "[Viewer][Stream] startStream() streamer '" << m_streamName << "' inited " );
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
