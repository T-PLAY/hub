#pragma once

#include <InputSensor.hpp>
#include <SensorSpec.hpp>
#include <string>
#include <thread>

namespace hub {
namespace client {
namespace viewer {

template <class InputStream>
class StreamViewer
{
  public:

//    template <class InputStream>
    StreamViewer(
        const std::string& ipv4,
        int port,
        const std::string& streamName,
        const SensorSpec& sensorSpec,
        std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
        std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
        std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
        std::function<void( const char* logMessage )> onLogMessage );

    ~StreamViewer();

//    template <class InputStream>
    void startStream();

    void stopStream();

  private:
    std::thread *  m_thread = nullptr;
    bool m_stopThread = false;

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

}; // end class StreamViewer

//////////////////////////////////////////////////////////////////////////

template <class InputStream>
StreamViewer<InputStream>::StreamViewer(
    const std::string& ipv4,
    int port,
    const std::string& streamName,
    const SensorSpec& sensorSpec,
    std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const SensorSpec& )> onDelStreamer,
    std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
        std::function<void( const char* logMessage )> onLogMessage) :
//    m_viewer( viewer ),
    m_ipv4(ipv4),
    m_port(port),
    m_streamName( streamName ),
    m_sensorSpec( sensorSpec ),
    m_onNewStreamer(onNewStreamer),
    m_onDelStreamer(onDelStreamer),
    m_onNewAcquisition(onNewAcquisition),
    m_onLogMessage( onLogMessage ) {

    assert( m_onNewStreamer );
    m_streaming = m_onNewStreamer( streamName.c_str(), sensorSpec );
    if ( m_onNewAcquisition && m_streaming ) {
        startStream();
    }
}

template <class InputStream>
StreamViewer<InputStream>::~StreamViewer() {
    DEBUG_MSG( "[StreamViewer] ~StreamViewer() streamer '" << m_streamName << "' started" );
    if ( m_onNewAcquisition && m_streaming ) {
        assert( m_thread != nullptr );
        if ( m_onDelStreamer ) { m_onDelStreamer( m_streamName.c_str(), m_sensorSpec ); }
        stopStream();
    }

    DEBUG_MSG( "[StreamViewer] ~StreamViewer() streamer '" << m_streamName << "' ended" );
}

template <class InputStream>
void StreamViewer<InputStream>::startStream() {
    DEBUG_MSG( "[StreamViewer] startStream" );

    assert( m_thread == nullptr );

    m_stopThread = false;
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {

//            m_inputSensor = std::make_unique<InputSensor>( input::InputStreamServer(
            m_inputSensor = std::make_unique<InputSensor>( InputStream(
                m_streamName,
                //                    net::ClientSocket( m_viewer.m_sock.getIpv4(),
                //                    m_viewer.m_sock.getPort() ) ) );
                m_ipv4,
                m_port ) );

            assert(m_onNewAcquisition);
            while ( !m_stopThread ) {
                hub::Acquisition acq;
                *m_inputSensor >> acq;
                m_onNewAcquisition( m_streamName.c_str(), acq );
            }
        }
        catch ( std::exception& e ) {
            DEBUG_MSG( "[ViewerInterface] startStream() streamer '"
                       << "'" << m_streamName << "' disconnected, catch exception " << e.what() );
        }

        if ( m_stopThread ) {
            DEBUG_MSG( "[ViewerInterface] streamer '" << m_streamName << "' thread killed " );
        }
        DEBUG_MSG( "[ViewerInterface] thread end " );

        m_streaming = false;
    } );

    m_streaming = true;

    DEBUG_MSG( "[ViewerInterface] startStream() streamer '" << m_streamName << "' inited " );
}


template <class InputStream>
void StreamViewer<InputStream>::stopStream() {
    DEBUG_MSG( "[StreamViewer] stopStream() streamer '" << m_streamName << "' started" );

    assert( m_thread != nullptr );
    assert( m_stopThread == false );
    m_stopThread = true;

//    m_inputSensor->getInput().close();

    assert( m_thread->joinable() );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;

//    m_streaming = false;

    DEBUG_MSG( "[StreamViewer] stopStream() streamer '" << m_streamName << "' ended" );
}

} // end namespace viewer
} // namespace client
} // namespace hub
