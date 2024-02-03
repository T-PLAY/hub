/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/13

#pragma once

#include "ClientSocket.hpp"
#include "ServerSocketI.hpp"
#include "system/ServerSocketSystem.hpp"

namespace hub {
namespace net {

using ServerSocket = system::ServerSocketSystem;

static_assert( std::is_base_of<ServerSocketI, ServerSocket>::value,
               "Input is base class of InputStream" );

} // namespace net
} // namespace hub
