#pragma once

#include "core/Macros.hpp"

// #include "sensor/SensorSpec.hpp"

namespace hub {

namespace sensor {
class SensorSpec;
// class SensorSpec::MetaData;
// namespace SensorSpec {
// class MetaData;
// }
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


    SRC_API const sensor::SensorSpec::MetaData*
    sensorSpec_getMetaData( const sensor::SensorSpec* sensorSpec );

    SRC_API bool metaData_exists( const sensor::SensorSpec::MetaData* metaData,
                                  const char* metaName );

    SRC_API bool metaData_getString( const sensor::SensorSpec::MetaData* metaData,
                                     const char* metaName,
                                     char* output,
                                     int* strLen );

    SRC_API bool metaData_getMat4( const sensor::SensorSpec::MetaData* metaData,
                                   const char* metaName,
                                   float* output );

    SRC_API int metaData_getInt( const sensor::SensorSpec::MetaData* metaData,
                                 const char* metaName );

    SRC_API double metaData_getDouble( const sensor::SensorSpec::MetaData* metaData,
                                       const char* metaName );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
