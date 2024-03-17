/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <Gauthier.Bouyjou@irit.fr>
 * @date 2023/03/13
 */

#pragma once

#include "ClientSocket.hpp"
#include "ServerSocketI.hpp"
#include "system/ServerSocketSystem.hpp"

namespace hub {
namespace net {

///
/// \brief ServerSocket
///
using ServerSocket = system::ServerSocketSystem;

static_assert( std::is_base_of<ServerSocketI, ServerSocket>::value,
               "Input is base class of InputStream" );

} // namespace net
} // namespace hub
