/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31

#pragma once

#include "SerializerT.hpp"

#ifdef HUB_USE_ZPP_BITS
#    include "serializer/SerializerZppBits.hpp"
#else
#    include "serializer/SerializerZpp.hpp"
#endif

namespace hub {

#ifdef HUB_USE_ZPP_BITS
using Serializer = SerializerT<serializer::SerializerZppBits>;
#else

///
/// \brief Serializer
///
using Serializer = SerializerT<serializer::SerializerZpp>;
#endif

static_assert( std::is_base_of_v<serializer::SerializerI, Serializer::Base> );

} // namespace hub
