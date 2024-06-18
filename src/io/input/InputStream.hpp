/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
/// \brief Represent is an input using stream communication.
/// A stream is a server feature able to share data comming from streamer client to differents viewer applications.
///
using InputStream = InputStreamServer2;

static_assert( std::is_base_of<Input, InputStream>::value, "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamBase, InputStream>::value,
               "Stream is base class of InputStream" );

} // namespace input
} // namespace hub
