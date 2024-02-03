/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/07

#pragma once

#include "core/Macros.hpp"

namespace hub {

class Any;

namespace io {
class Header;
}

namespace client {
class ViewerHandler;
}

namespace sensor {
class SensorSpec;
class Acquisition;
} // namespace sensor

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

    typedef bool ( *onNewStreamFunc )( const char* streamName, const io::Header* header );
#ifndef HUB_NON_BUILD_SENSOR
    typedef bool ( *onNewSensorFunc )( const char* streamName,
                                       const sensor::SensorSpec* sensorSpec );
#endif

    typedef void ( *onNewDataFunc )( const char* streamName, const Datas_t* datas );
#ifndef HUB_NON_BUILD_SENSOR
    typedef void ( *onNewAcqFunc )( const char* streamName, const sensor::Acquisition* acq );
#endif

    typedef void ( *onDelStreamFunc )( const char* streamName );

    typedef void ( *onSetPropertyFunc )( const char* streamName,
                                         const char* objectName,
                                         int property,
                                         const Any* value );

    typedef void ( *onLogMessageFunc )( const char* logMessage );

    SRC_API client::ViewerHandler*
    createViewerHandler( onServerNotFoundFunc onServerNotFound,
                         onServerConnectedFunc onServerConnected,
                         onServerDisconnectedFunc onServerDisconnected,
                         onNewStreamFunc onNewStream,
#ifndef HUB_NON_BUILD_SENSOR
                         onNewSensorFunc onNewSensor,
#endif
                         onNewDataFunc onNewData,
#ifndef HUB_NON_BUILD_SENSOR
                         onNewAcqFunc onNewAcq,
#endif
                         onDelStreamFunc onDelStream,
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
