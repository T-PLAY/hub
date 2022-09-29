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
    SRC_API bool getData( const InputSensor* inputSensor, unsigned char* data, int iMeasure = 0 );
    //    SRC_API bool getAcquisition( InputSensor* inputSensor,
    //                                 long long* start,
    //                                 long long* end,
    //                                 unsigned char* data );
    SRC_API Viewer*
    createViewer( void ( *onNewStreamer )( const char* streamName, const SensorSpec* sensorSpec ),
                  void ( *onDelStreamer )( const char* streamName, const SensorSpec* sensorSpec ),
                  void ( *onServerConnected )(),
                  void ( *onServerDisconnected )(),
                  const char* ipv4,
                  int port );
    //                  const char* ipv4 = hub::net::s_defaultServiceIp.c_str(),
    //                  int port         = hub::net::s_defaultServicePort );
    SRC_API void freeViewer( Viewer* viewer );

    SRC_API void sensorSpec_getSensorName( const SensorSpec* sensorSpec, char* sensorName );
    SRC_API int sensorSpec_getResolutionsSize( const SensorSpec* sensorSpec );
    SRC_API void sensorSpec_getResolutionsStr( const SensorSpec* sensorSpec, char* resolutionsStr );
    SRC_API int sensorSpec_getFormat( const SensorSpec* sensorSpec, int iResolution );
    SRC_API int sensorSpec_getDimensionsSize( const SensorSpec* sensorSpec, int iResolution );
    SRC_API int
    sensorSpec_getDimension( const SensorSpec* sensorSpec, int iResolution, int iDimension );
//    SRC_API void sensorSpec_getDimensionStr( const SensorSpec* sensorSpec, int iResolution, int iDimension, char* sensorName );
    SRC_API int sensorSpec_getAcquisitionSize( const SensorSpec* sensorSpec );
    SRC_API void sensorSpec_getMetaDataStr( const SensorSpec* sensorSpec, char* metaDataStr );
    SRC_API SensorSpec * sensorSpec_copy( const SensorSpec* source);
    SRC_API void freeSensorSpec( SensorSpec* dest);

    SRC_API const SensorSpec::MetaData * sensorSpec_getMetaData( const SensorSpec* sensorSpec );
    SRC_API bool metaData_getString( const SensorSpec::MetaData * metaData, const char * metaName, char* output );

}

} // namespace native
} // namespace hub
