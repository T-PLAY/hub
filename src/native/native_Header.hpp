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
