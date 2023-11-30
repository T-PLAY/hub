#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"
// #include "net/ClientSocket.hpp"
// #include "StreamViewer.hpp"
// #include "ViewerInterface.hpp"
// #include "core/Utils.hpp"
// #include "sensor/InputSensor.hpp"

// #ifdef DEBUG_MSG
// #    undef DEBUG_MSG
// #endif
// #define DEBUG_MSG( _params )                                              \
//     do {                                                                  \
//         if ( m_onLogMessage ) {                                           \
//             std::stringstream _sstr;                                      \
//             _sstr << m_iStreamer << ":" << _params;                       \
//             m_onLogMessage( _sstr.str().c_str() );                        \
//         }                                                                 \
//         else { std::cout << m_iStreamer << ":" << _params << std::endl; } \
//     } while ( false );

namespace hub {
namespace client {

// template <class InputStream>
// class ViewerInterface<InputStream>::Stream
class ViewerHandler
{
  public:
    std::function<void( const char* ipv4, int port )> onServerNotFound;
    std::function<void( const char* ipv4, int port )> onServerConnected;
    std::function<void( const char* ipv4, int port )> onServerDisconnected;

    std::function<bool( const char* streamName, const sensor::SensorSpec& )> onNewStreamer;
    std::function<void( const char* streamName, const sensor::SensorSpec& )> onDelStreamer;

    std::function<void( const char* streamName, const sensor::Acquisition& )> onNewAcquisition;
    std::function<
        void( const char* streamName, const char* objectName, int property, const Any& value )>
        onSetProperty;

    std::function<void( const char* logMessage )> onLogMessage;

};

// template <class InputStream>
// int ViewerStream<InputStream>::m_iStreamer = 0;


// //////////////////////////////////////// Stream ////////////////////////////////////////

// template <class InputStream>
// // ViewerInterface<InputStream>::Stream::Stream(
// ViewerStream<InputStream>::ViewerStream(
//     // Stream::Stream(
//     // ViewerInterface::Stream::Stream(
//     // ViewerInterface<InputStream>& viewer,
//     const std::string& ipv4,
//     int port,
//     const std::string& streamName,
//     const sensor::SensorSpec& sensorSpec,
//     std::function<bool( const char*, const sensor::SensorSpec& )> onNewStreamer,
//     std::function<void( const char*, const sensor::SensorSpec& )> onDelStreamer,
//     std::function<void( const char*, const sensor::Acquisition& )> onNewAcquisition,
//     std::function<void( const char* logMessage )> onLogMessage ) :
//     //    m_viewer( viewer ),
//     // m_viewer( viewer ),
//     m_ipv4( ipv4 ),
//     m_port( port ),
//     m_streamName( streamName ),
//     m_sensorSpec( sensorSpec ),
//     m_onNewStreamer( onNewStreamer ),
//     m_onDelStreamer( onDelStreamer ),
//     m_onNewAcquisition( onNewAcquisition ),
//     m_onLogMessage( onLogMessage ) {

//     assert( m_onNewStreamer );
//     m_streaming = m_onNewStreamer( streamName.c_str(), sensorSpec );
//     if ( m_onNewAcquisition && m_streaming ) { startStream(); }
// }

// // template <class InputStream>
// // Stream<InputStream>::~Stream() {
// template <class InputStream>
// // ViewerInterface<InputStream>::Stream::~Stream() {
// ViewerStream<InputStream>::~ViewerStream() {
//     // Stream::~Stream() {
//     DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' started" );
//     if ( m_onNewAcquisition && m_streaming ) {
//         assert( m_thread != nullptr );
//         if ( m_onDelStreamer ) { m_onDelStreamer( m_streamName.c_str(), m_sensorSpec ); }
//         stopStream();
//     }

//     DEBUG_MSG( "[Viewer][Stream] ~Stream() streamer '" << m_streamName << "' ended" );
// }

// // template <class InputStream>
// // void Stream<InputStream>::startStream() {
// template <class InputStream>
// // void ViewerInterface<InputStream>::Stream::startStream() {
// void ViewerStream<InputStream>::startStream() {
//     // void Stream::startStream() {
//     DEBUG_MSG( "[Viewer][Stream] startStream" );

//     assert( m_thread == nullptr );

//     m_stopThread = false;
//     assert( m_thread == nullptr );
//     m_thread = new std::thread( [this]() {
//         try {

//             //            m_inputSensor = std::make_unique<InputSensor>( input::InputStreamServer(
//             m_inputSensor = std::make_unique<sensor::InputSensor>(
//                 InputStream( m_streamName,
//                              //                    net::ClientSocket( m_viewer.m_sock.getIpv4(),
//                              //                    m_viewer.m_sock.getPort() ) ) );
//                              m_port,
//                              m_ipv4 ) );

//             assert( m_onNewAcquisition );
//             while ( !m_stopThread ) {
//                 sensor::Acquisition acq;
//                 *m_inputSensor >> acq;
//                 m_onNewAcquisition( m_streamName.c_str(), acq );
//             }
//         }
//         catch ( std::exception& e ) {
//             DEBUG_MSG( "[Viewer][Stream] startStream() streamer '"
//                        << "'" << m_streamName << "' disconnected, catch exception " << e.what() );
//             //            m_streaming = false;
//             //            m_viewer.deleteStream(m_streamName);
//         }

//         if ( m_stopThread ) {
//             DEBUG_MSG( "[Viewer][Stream] streamer '" << m_streamName << "' thread killed " );
//         }
//         DEBUG_MSG( "[Viewer][Stream] thread end " );

//         m_streaming = false;
//         //        m_viewer.printStatus();
//     } );

//     m_streaming = true;

//     DEBUG_MSG( "[Viewer][Stream] startStream() streamer '" << m_streamName << "' inited " );
// }

// // template <class InputStream>
// // void Stream<InputStream>::stopStream() {
// template <class InputStream>
// // void ViewerInterface<InputStream>::Stream::stopStream() {
// void ViewerStream<InputStream>::stopStream() {
//     // void Stream::stopStream() {
//     DEBUG_MSG( "[Viewer][Stream] stopStream() streamer '" << m_streamName << "' started" );

//     assert( m_thread != nullptr );
//     assert( m_stopThread == false );
//     m_stopThread = true;

//     //    m_inputSensor->getInput().close();

//     assert( m_thread->joinable() );
//     m_thread->join();
//     delete m_thread;
//     m_thread = nullptr;

//     //    m_streaming = false;

//     DEBUG_MSG( "[Viewer][Stream] stopStream() streamer '" << m_streamName << "' ended" );
//     // m_viewer.printStatus();
// }

// template<class InputStream>
// bool ViewerStream<InputStream>::isStreaming() const
// {
//     return m_streaming;
// }

} // namespace client
} // namespace hub
