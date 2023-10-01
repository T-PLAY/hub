#pragma once

#include "core/Macros.hpp"

#include "sensor/InputSensor.hpp"
#include "sensor/OutputSensor.hpp"
#include "client/Viewer.hpp"
//#include "ViewerInterface.hpp"

namespace hub {

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
    /// \brief createInputSensor
    /// \param streamName
    /// \param ipv4
    /// \param port
    /// \return
    ///
    SRC_API sensor::InputSensor* createInputSensor( const char* streamName,
         const char* ipv4 = input::InputStream::s_defaultIpv4.c_str(),
                                            int port = input::InputStream::s_defaultPort);

    ///
    /// \brief freeInputSensor
    /// \param inputSensor
    ///
    SRC_API void freeInputSensor( sensor::InputSensor* inputSensor );

    ///
    /// \brief getAcquisitionSize
    /// \param inputSensor
    /// \return
    ///
    SRC_API int getAcquisitionSize( const sensor::InputSensor* inputSensor );

    ///
    /// \brief getAcquisition
    /// \param inputSensor
    /// \return
    ///
    SRC_API sensor::Acquisition* getAcquisition( sensor::InputSensor* inputSensor );

    /////////////////////////////////////////////////

    ///
    /// \brief createMat4OutputSensor
    /// \param sensorName
    /// \param ipv4
    /// \param port
    /// \return
    ///
    SRC_API sensor::OutputSensor*
    createMat4OutputSensor( const char* sensorName, const char* ipv4, int port );

    ///
    /// \brief mat4OutputSensorSendAcq
    /// \param outputSensor
    /// \param input
    /// \return
    ///
    SRC_API bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input );

    ///
    /// \brief freeOutputSensor
    /// \param outputSensor
    ///
    SRC_API void freeOutputSensor( sensor::OutputSensor* outputSensor );

    ///
    /// \brief freeAcquisition
    /// \param acquisition
    ///
    SRC_API void freeAcquisition( sensor::Acquisition* acquisition );

    ///
    /// \brief acquisition_getMeasure
    /// \param acquisition
    /// \param data
    /// \param iMeasure
    ///
    SRC_API void
    acquisition_getMeasure( const sensor::Acquisition* acquisition, unsigned char* data, int iMeasure = 0 );

    ///
    /// \brief acquisition_getStart
    /// \param acquisition
    /// \return
    ///
    SRC_API long long acquisition_getStart( const sensor::Acquisition* acquisition );

    ///
    /// \brief acquisition_to_string
    /// \param acquisition
    /// \param str
    /// \param strLen
    ///
    SRC_API void acquisition_to_string( const sensor::Acquisition* acquisition, char* str, int* strLen );

    /// \brief onNewStreamerFunc
    /// \param streamName
    /// \param sensorSpec
    typedef bool ( *onNewStreamerFunc )( const char* streamName, const sensor::SensorSpec* sensorSpec );

    /// \brief onDelStreamerFunc
    /// \param streamName
    /// \param sensorSpec
    typedef void ( *onDelStreamerFunc )( const char* streamName, const sensor::SensorSpec* sensorSpec );

    /// \brief onServerConnectedFunc
    /// \param ipv4
    /// \param port
    typedef void ( *onServerConnectedFunc )( const char* ipv4, int port );

    /// \brief onServerNotFoundFunc
    /// \param ipv4
    /// \param port
    typedef void ( *onServerNotFoundFunc )( const char* ipv4, int port );

    /// \brief onServerDisconnectedFunc
    /// \param ipv4
    /// \param port
    typedef void ( *onServerDisconnectedFunc )( const char* ipv4, int port );

    /// \brief onNewAcquisitionFunc
    /// \param streamName
    /// \param acq
    typedef void ( *onNewAcquisitionFunc )( const char* streamName, const sensor::Acquisition* acq );

    /// \brief onSetPropertyFunc
    /// \param streamName
    /// \param objectName
    /// \param property
    /// \param value
    typedef void ( *onSetPropertyFunc )( const char* streamName,
                                         const char* objectName,
                                         int property,
                                         const Any* value );

    /// \brief onLogMessageFunc
    /// \param logMessage
    typedef void ( *onLogMessageFunc )( const char* logMessage );

    ///
    /// \brief createViewer
    /// \param onNewStreamer
    /// \param onDelStreamer
    /// \param onServerNotFound
    /// \param onServerConnected
    /// \param onServerDisconnected
    /// \param onNewAcquisition
    /// \param onSetProperty
    /// \param ipv4
    /// \param port
    /// \param onLogMessage
    /// \return
    ///
    SRC_API client::Viewer* createViewer(const char *name,
        onNewStreamerFunc onNewStreamer,
                                          onDelStreamerFunc onDelStreamer,
                                          onServerNotFoundFunc onServerNotFound,
                                          onServerConnectedFunc onServerConnected,
                                          onServerDisconnectedFunc onServerDisconnected,
                                          onNewAcquisitionFunc onNewAcquisition,
                                          onSetPropertyFunc onSetProperty,
                                          onLogMessageFunc onLogMessage,
         const char* ipv4 = input::InputStream::s_defaultIpv4.c_str(),
         int port = input::InputStream::s_defaultPort
        );

    ///
    /// \brief freeViewer
    /// \param viewer
    ///
    SRC_API void freeViewer( client::Viewer* viewer );

    ///
    /// \brief viewer_setIpv4
    /// \param viewer
    /// \param ipv4
    ///
    SRC_API void viewer_setIpv4( client::Viewer* viewer, const char* ipv4 );

    ///
    /// \brief viewer_setPort
    /// \param viewer
    /// \param port
    ///
    SRC_API void viewer_setPort( client::Viewer* viewer, int port );

    ///
    /// \brief viewer_getPort
    /// \param viewer
    /// \return
    ///
    SRC_API int viewer_getPort( const client::Viewer* viewer );

    ///
    /// \brief viewer_getIpv4
    /// \param viewer
    /// \param ipv4
    ///
    SRC_API void viewer_getIpv4( const client::Viewer* viewer, char* ipv4 );

    ///
    /// \brief viewer_isConnected
    /// \param viewer
    /// \return
    ///
    SRC_API bool viewer_isConnected( const client::Viewer* viewer );

    ///
    /// \brief freeSensorSpec
    /// \param sensorSpec
    ///
    SRC_API void freeSensorSpec( sensor::SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getSensorName
    /// \param sensorSpec
    /// \param sensorName
    /// \param strLen
    ///
    SRC_API void
    sensorSpec_getSensorName( const sensor::SensorSpec* sensorSpec, char* sensorName, int* strLen );

    ///
    /// \brief sensorSpec_getResolutionsSize
    /// \param sensorSpec
    /// \return
    ///
    SRC_API int sensorSpec_getResolutionsSize( const sensor::SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getResolutionSize
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getResolutionSize( const sensor::SensorSpec* sensorSpec, int iResolution = 0 );

    ///
    /// \brief sensorSpec_getResolutionsStr
    /// \param sensorSpec
    /// \param resolutionsStr
    ///
    SRC_API void sensorSpec_getResolutionsStr( const sensor::SensorSpec* sensorSpec, char* resolutionsStr );

    ///
    /// \brief sensorSpec_getFormat
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getFormat( const sensor::SensorSpec* sensorSpec, int iResolution );

    ///
    /// \brief sensorSpec_getDimensionsSize
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getDimensionsSize( const sensor::SensorSpec* sensorSpec, int iResolution );

    ///
    /// \brief sensorSpec_getDimension
    /// \param sensorSpec
    /// \param iResolution
    /// \param iDimension
    /// \return
    ///
    SRC_API int
    sensorSpec_getDimension( const sensor::SensorSpec* sensorSpec, int iResolution, int iDimension );

    ///
    /// \brief sensorSpec_getAcquisitionSize
    /// \param sensorSpec
    /// \return
    ///
    SRC_API int sensorSpec_getAcquisitionSize( const sensor::SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getMetaDataStr
    /// \param sensorSpec
    /// \param metaDataStr
    ///
    SRC_API void sensorSpec_getMetaDataStr( const sensor::SensorSpec* sensorSpec, char* metaDataStr );

    ///
    /// \brief sensorSpec_copy
    /// \param source
    /// \return
    ///
    SRC_API sensor::SensorSpec* sensorSpec_copy( const sensor::SensorSpec* source );

    ///
    /// \brief sensorSpec_getMetaData
    /// \param sensorSpec
    /// \return
    ///
    SRC_API const sensor::SensorSpec::MetaData* sensorSpec_getMetaData( const sensor::SensorSpec* sensorSpec );

    ///
    /// \brief metaData_exists
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API bool metaData_exists( const sensor::SensorSpec::MetaData* metaData, const char* metaName );

    ///
    /// \brief metaData_getString
    /// \param metaData
    /// \param metaName
    /// \param output
    /// \param strLen
    /// \return
    ///
    SRC_API bool metaData_getString( const sensor::SensorSpec::MetaData* metaData,
                                     const char* metaName,
                                     char* output,
                                     int* strLen );

    ///
    /// \brief metaData_getMat4
    /// \param metaData
    /// \param metaName
    /// \param output
    /// \return
    ///
    SRC_API bool
    metaData_getMat4( const sensor::SensorSpec::MetaData* metaData, const char* metaName, float* output );

    ///
    /// \brief metaData_getInt
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API int metaData_getInt( const sensor::SensorSpec::MetaData* metaData, const char* metaName );

    ///
    /// \brief metaData_getUInt
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API double metaData_getDouble( const sensor::SensorSpec::MetaData* metaData, const char* metaName );

    ///
    /// \brief any_getDouble
    /// \param any
    /// \return
    ///
    SRC_API double any_getDouble( const Any* any );

    ///
    /// \brief any_getInt
    /// \param any
    /// \return
    ///
    SRC_API int any_getInt( const Any* any );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
