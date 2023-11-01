#pragma once

// #include "io/input/InputI.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

//#include "core/Macros.hpp"
#include "serializer/SerializerImpl.hpp"
#include "serializer/SerializerZppBits.hpp"

namespace hub {


#ifdef HUB_USE_ZPP_BITS
using Serializer = serializer::SerializerZppBits;
#else
using Serializer = serializer::SerializerImpl;
#endif

} // namespace hub
