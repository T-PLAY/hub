/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/13

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
