#pragma once

#ifdef HUB_BUILD_SERVER
// #include "core/Macros.hpp"
#include "server/impl/ServerImpl.hpp"
#include "server/impl2/ServerImpl2.hpp"
#endif

namespace hub {

#ifdef HUB_BUILD_SERVER
// using Server = server::impl::ServerImpl;
using Server = server::impl2::ServerImpl2;
#endif


} // namespace hub
