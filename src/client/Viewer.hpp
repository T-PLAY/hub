/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/13

#pragma once

#include "viewer/ViewerServer2.hpp"

#include "io/input/InputStream.hpp"

namespace hub {
namespace client {

using Viewer = ViewerServer2;

static_assert( std::is_base_of<client::ViewerInterface<input::InputStream>, Viewer>::value,
               "ViewerInterface is base class of Viewer" );

} // namespace client
} // namespace hub
