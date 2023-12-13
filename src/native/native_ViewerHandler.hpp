#pragma once

#include "core/Macros.hpp"

// #include "client/viewer/ViewerHandler.hpp"

namespace hub {

class Any;

namespace io {
class Header;
}

namespace client {
class ViewerHandler;
}

///
/// @brief native
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void ( *onServerNotFoundFunc )( const char* ipv4, int port );

    typedef void ( *onServerConnectedFunc )( const char* ipv4, int port );

    typedef void ( *onServerDisconnectedFunc )( const char* ipv4, int port );

    // typedef bool ( *onNewStreamFunc )( const char* streamName,
    // const sensor::SensorSpec* sensorSpec );
    typedef bool ( *onNewStreamFunc )( const char* streamName, const io::Header* header );

    typedef void ( *onNewDataFunc )( const char* streamName, const Datas_t* datas );

    typedef void ( *onDelStreamFunc )( const char* streamName );
    // const sensor::SensorSpec* sensorSpec );

    // typedef void ( *onNewAcquisitionFunc )( const char* streamName,
    // const sensor::Acquisition* acq );

    typedef void ( *onSetPropertyFunc )( const char* streamName,
                                         const char* objectName,
                                         int property,
                                         const Any* value );

    typedef void ( *onLogMessageFunc )( const char* logMessage );

    SRC_API client::ViewerHandler*
    createViewerHandler( onServerNotFoundFunc onServerNotFound,
                         onServerConnectedFunc onServerConnected,
                         onServerDisconnectedFunc onServerDisconnected,
                         onNewStreamFunc onNewStreamer,
                         onNewDataFunc onNewData,
                         onDelStreamFunc onDelStreamer,
                         // onNewAcquisitionFunc onNewAcquisition,
                         onSetPropertyFunc onSetProperty,
                         onLogMessageFunc onLogMessage );

    ///
    /// \brief freeViewer
    /// \param viewerHandler
    ///
    SRC_API void freeViewerHandler( client::ViewerHandler* viewerHandler );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
