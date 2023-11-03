#pragma once

#include "ServerSocketI.hpp"

#include "system/ServerSocketSystem.hpp"
//#include "boost/ServerSocketBoost.hpp"

// user friendly
#include "ClientSocket.hpp"

namespace hub {
namespace net {

using ServerSocket = system::ServerSocketSystem;
//using ServerSocket = boost::ServerSocketBoost;

static_assert( std::is_base_of<ServerSocketI, ServerSocket>::value,
               "Input is base class of InputStream" );

} // namespace net
} // namespace hub
