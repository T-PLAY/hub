/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/13

#pragma once

#include "core/Macros.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE


namespace sensor {
class Acquisition;
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

SRC_API void freeAcquisition( sensor::Acquisition* acquisition );

SRC_API void
acquisition_getMeasure( const sensor::Acquisition* acquisition, unsigned char* data, int iMeasure = 0 );

SRC_API long long acquisition_getStart( const sensor::Acquisition* acquisition );

SRC_API void acquisition_to_string( const sensor::Acquisition* acquisition, char* str, int* strLen );


#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
