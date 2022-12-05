#pragma once

#include "InputSensor.hpp"
#include "Macros.hpp"
#include "Viewer.hpp"

namespace hub {
namespace native {

extern "C"
{
    SRC_API InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port );
    SRC_API void freeInputSensor( InputSensor* inputSensor );
    SRC_API int getAcquisitionSize( const InputSensor* inputSensor );

    SRC_API Acquisition* getAcquisition( const InputSensor* inputSensor );
    SRC_API void freeAcquisition( Acquisition* acquisition );
    SRC_API void
    acquisition_getMeasure( const Acquisition* acquisition, unsigned char* data, int iMeasure = 0 );
    SRC_API long long acquisition_getStart( const Acquisition* acquisition );

    typedef bool ( *onNewStreamerFunc )( const char* streamName, const SensorSpec* sensorSpec );
    typedef void ( *onDelStreamerFunc )( const char* streamName, const SensorSpec* sensorSpec );
    typedef void ( *onServerConnectedFunc )( const char* ipv4, int port );
    typedef void ( *onServerDisconnectedFunc )( const char* ipv4, int port );
    typedef void ( *onNewAcquisitionFunc )( const char* streamName, const Acquisition* acq );

    SRC_API Viewer* createViewer( onNewStreamerFunc onNewStreamer,
                                  onDelStreamerFunc onDelStreamer,
                                  onServerConnectedFunc onServerConnected,
                                  onServerDisconnectedFunc onServerDisconnected,
                                  onNewAcquisitionFunc onNewAcquisition,
                                  const char* ipv4,
                                  int port );
    SRC_API void freeViewer( Viewer* viewer );
    SRC_API void viewer_setIpv4(Viewer * viewer, const char * ipv4);
    SRC_API void viewer_setPort(Viewer * viewer, int port);
    SRC_API bool viewer_isConnected(Viewer * viewer);

    SRC_API void
    sensorSpec_getSensorName( const SensorSpec* sensorSpec, char* sensorName, int* strLen );
    SRC_API int sensorSpec_getResolutionsSize( const SensorSpec* sensorSpec );
    SRC_API int sensorSpec_getResolutionSize( const SensorSpec* sensorSpec, int iResolution = 0 );
    SRC_API void sensorSpec_getResolutionsStr( const SensorSpec* sensorSpec, char* resolutionsStr );
    SRC_API int sensorSpec_getFormat( const SensorSpec* sensorSpec, int iResolution );
    SRC_API int sensorSpec_getDimensionsSize( const SensorSpec* sensorSpec, int iResolution );
    SRC_API int
    sensorSpec_getDimension( const SensorSpec* sensorSpec, int iResolution, int iDimension );
    SRC_API int sensorSpec_getAcquisitionSize( const SensorSpec* sensorSpec );
    SRC_API void sensorSpec_getMetaDataStr( const SensorSpec* sensorSpec, char* metaDataStr );
    SRC_API SensorSpec* sensorSpec_copy( const SensorSpec* source );
    SRC_API void freeSensorSpec( SensorSpec* dest );

    SRC_API const SensorSpec::MetaData* sensorSpec_getMetaData( const SensorSpec* sensorSpec );
    SRC_API bool metaData_exists( const SensorSpec::MetaData* metaData, const char* metaName );
    SRC_API bool metaData_getString( const SensorSpec::MetaData* metaData,
                                     const char* metaName,
                                     char* output,
                                     int* strLen );
    SRC_API bool
    metaData_getMat4( const SensorSpec::MetaData* metaData, const char* metaName, float* output );
    SRC_API int metaData_getInt( const SensorSpec::MetaData* metaData, const char* metaName );
    SRC_API unsigned int metaData_getUInt( const SensorSpec::MetaData* metaData,
                                           const char* metaName );
}

} // namespace native
} // namespace hub