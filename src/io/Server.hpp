#pragma once

#ifdef HUB_BUILD_IMPL_SERVER2
#include "impl/server2/Server.hpp"
#endif

namespace hub {

#if defined( HUB_BUILD_IMPL_SERVER2 )
using Server = impl2::Server;
#endif


} // namespace hub
