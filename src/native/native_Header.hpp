/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/15
	
#pragma once

#include "core/Macros.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE
namespace io {
class Header;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    io::Header* createHeader( Size_t dataSize );

    SRC_API void freeHeader( io::Header* header );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
