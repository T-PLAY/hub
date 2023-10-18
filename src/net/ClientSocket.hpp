#pragma once

#include "ClientSocketInterface.hpp"

#include "system/ClientSocketSystem.hpp"

namespace hub {
namespace net {

using ClientSocket = system::ClientSocketSystem;

static_assert( std::is_base_of<ClientSocketInterface, ClientSocket>::value,
               "Input is base class of InputStream" );

} // namespace net
} // namespace hub
