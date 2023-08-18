
#pragma once

#include "InputStreamServer.hpp"
#include "InputStreamMqtt.hpp"

namespace hub {
using namespace io;
namespace input {


//using InputStream = InputStreamServer;
using InputStream = InputStreamMqtt;

static_assert( std::is_base_of<InputStreamInterface, InputStream>::value,
               "InputStreamInterface is base class of InputStream" );

} // namespace input
} // namespace hub
