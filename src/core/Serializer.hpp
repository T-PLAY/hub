/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

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
