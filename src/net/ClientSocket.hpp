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

#include "ClientSocketI.hpp"
#include "system/ClientSocketSystem.hpp"

namespace hub {
namespace net {

///
/// \brief ClientSocket
///
using ClientSocket = system::ClientSocketSystem;

static_assert( std::is_base_of<ClientSocketI, ClientSocket>::value,
               "Input is base class of InputStream" );

} // namespace net
} // namespace hub
