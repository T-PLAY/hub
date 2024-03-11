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

    ///
    /// \brief onServerNotFoundFunc
    ///
    typedef void ( *onServerNotFoundFunc )( const char* ipv4, int port );

    ///
    /// \brief onServerConnectedFunc
    ///
    typedef void ( *onServerConnectedFunc )( const char* ipv4, int port );

    ///
    /// \brief onServerDisconnectedFunc
    ///
    typedef void ( *onServerDisconnectedFunc )( const char* ipv4, int port );

    ///
    /// \brief onNewStreamFunc
    ///
    typedef bool ( *onNewStreamFunc )( const char* streamName, const io::Header* header );
#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief onNewSensorFunc
    ///
    typedef bool ( *onNewSensorFunc )( const char* streamName,
                                       const sensor::SensorSpec* sensorSpec );
#endif

    ///
    /// \brief onNewDataFunc
    ///
    typedef void ( *onNewDataFunc )( const char* streamName, const Datas_t* datas );
#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief onNewAcqFunc
    ///
    typedef void ( *onNewAcqFunc )( const char* streamName, const sensor::Acquisition* acq );
#endif

    ///
    /// \brief onDelStreamFunc
    ///
    typedef void ( *onDelStreamFunc )( const char* streamName );

    ///
    /// \brief onSetPropertyFunc
    ///
    typedef void ( *onSetPropertyFunc )( const char* streamName,
                                         const char* objectName,
                                         int property,
                                         const Any* value );

    ///
    /// \brief onServerNotFoundFunc
    ///
    typedef void ( *onLogMessageFunc )( const char* logMessage );

    ///
    /// \brief createViewerHandler
    /// \param onServerNotFound
    /// \param onServerConnected
    /// \param onServerDisconnected
    /// \param onNewStream
    /// \param onNewSensor
    /// \param onNewData
    /// \param onNewAcq
    /// \param onDelStream
    /// \param onSetProperty
    /// \param onLogMessage
    /// \return
    ///
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

    ///
    /// \brief printStatus
    /// \return
    ///
    SRC_API int printStatus();

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
