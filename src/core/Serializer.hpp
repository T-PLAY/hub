#pragma once

// #include "io/input/InputBase.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

//#include "core/Macros.hpp"

#ifdef HUB_USE_ZPP_BITS
#include "serializer/SerializerZppBits.hpp"
#else
#include "serializer/SerializerImpl.hpp"
#endif

// #include <core/serializer/SerializerZppBits.hpp>

namespace hub {


#ifdef HUB_USE_ZPP_BITS
using Serializer = serializer::SerializerZppBits;
#else
using Serializer = serializer::SerializerImpl;
#endif

} // namespace hub
