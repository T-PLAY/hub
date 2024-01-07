#pragma once

// #include "io/input/InputBase.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

//#include "core/Macros.hpp"

#include "SerializerT.hpp"

#ifdef HUB_USE_ZPP_BITS
#include "serializer/SerializerZppBits.hpp"
#else
// #include "serializer/SerializerImpl.hpp"
// #include "serializer/SerializerImpl2.hpp"
#include "serializer/SerializerZpp.hpp"
#endif

// #include <core/serializer/SerializerZppBits.hpp>

namespace hub {


#ifdef HUB_USE_ZPP_BITS
// using Serializer = serializer::SerializerZppBits;
using Serializer = SerializerT<serializer::SerializerZppBits>;
#else
// using Serializer = serializer::SerializerImpl2;
// using Serializer = SerializerT<serializer::SerializerImpl2>;
using Serializer = SerializerT<serializer::SerializerZpp>;
#endif

static_assert(std::is_base_of_v<serializer::SerializerI, Serializer::Base>);

} // namespace hub
