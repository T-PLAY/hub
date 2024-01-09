/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/07
	
#pragma once

#include "core/Macros.hpp"

// #include "sensor/SensorSpec.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE
class MetaData;
// namespace sensor {
// class SensorSpec;
// class MetaData;
// class SensorSpec::MetaData;
// namespace SensorSpec {
// class MetaData;
// }
// }
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



    SRC_API bool metaData_exists( const MetaData* metaData,
                                  const char* metaName );

    SRC_API bool metaData_getString( const MetaData* metaData,
                                     const char* metaName,
                                     char* output,
                                     int* strLen );

    SRC_API bool metaData_getMat4( const MetaData* metaData,
                                   const char* metaName,
                                   float* output );

    SRC_API int metaData_getInt( const MetaData* metaData,
                                 const char* metaName );

    SRC_API double metaData_getDouble( const MetaData* metaData,
                                       const char* metaName );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
