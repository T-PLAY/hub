#pragma once

#include "core/Macros.hpp"

// #include "sensor/SensorSpec.hpp"

namespace hub {


#ifndef HUB_CPP_SOURCE

class MetaData;

namespace sensor {
class SensorSpec;
}
#endif

///
/// @brief native
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    SRC_API void freeSensorSpec( sensor::SensorSpec* sensorSpec );

    SRC_API void
    sensorSpec_getSensorName( const sensor::SensorSpec* sensorSpec, char* sensorName, int* strLen );

    // SRC_API int sensorSpec_getResolutionsSize( const sensor::SensorSpec* sensorSpec );

    // SRC_API int sensorSpec_getResolutionSize( const sensor::SensorSpec* sensorSpec,
    //                                           int iResolution = 0 );

    // SRC_API void sensorSpec_getResolutionsStr( const sensor::SensorSpec* sensorSpec,
    //                                            char* resolutionsStr );

    // SRC_API int sensorSpec_getFormat( const sensor::SensorSpec* sensorSpec, int iResolution );

    // SRC_API int sensorSpec_getDimensionsSize( const sensor::SensorSpec* sensorSpec,
    //                                           int iResolution );

    // SRC_API int sensorSpec_getDimension( const sensor::SensorSpec* sensorSpec,
    //                                      int iResolution,
    //                                      int iDimension );

    SRC_API int sensorSpec_getAcquisitionSize( const sensor::SensorSpec* sensorSpec );

    SRC_API void sensorSpec_getMetaDataStr( const sensor::SensorSpec* sensorSpec,
                                            char* metaDataStr );

    SRC_API sensor::SensorSpec* sensorSpec_copy( const sensor::SensorSpec* source );

    SRC_API const MetaData*
    sensorSpec_getMetaData( const sensor::SensorSpec* sensorSpec );


#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
