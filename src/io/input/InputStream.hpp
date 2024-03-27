/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

#pragma once

#include "io/input/InputStreamServer2.hpp"

namespace hub {
namespace input {

///
/// \brief InputStream
///
using InputStream = InputStreamServer2;

static_assert( std::is_base_of<Input, InputStream>::value, "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamBase, InputStream>::value,
               "Stream is base class of InputStream" );

} // namespace input
} // namespace hub
