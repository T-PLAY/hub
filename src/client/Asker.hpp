/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/13

#pragma once

#include "asker/AskerServer2.hpp"

namespace hub {
namespace client {

///
/// \brief Asker
///
using Asker = AskerServer2;

static_assert( std::is_base_of<client::AskerInterface, Asker>::value,
               "AskerInterface is base class of Asker" );

} // namespace client
} // namespace hub
