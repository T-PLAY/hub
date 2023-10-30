#pragma once

// #include "io/input/InputI.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

//#include "core/Macros.hpp"
#include "serializer/SerializerImpl.hpp"
#include "serializer/SerializerZppBits.hpp"

namespace hub {

using Serializer = serializer::SerializerZppBits;
//using Serializer = serializer::SerializerImpl;

} // namespace hub
