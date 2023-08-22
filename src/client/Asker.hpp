#pragma once


#include "AskerMqtt.hpp"

namespace hub {
namespace client {

using Asker = AskerMqtt;

static_assert( std::is_base_of<client::AskerInterface, Asker>::value,
               "AskerInterface is base class of Asker" );

} // namespace client
} // namespace hub
