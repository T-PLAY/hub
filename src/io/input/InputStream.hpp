/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/17

#pragma once

#include "io/input/InputStreamServer2.hpp"

namespace hub {
namespace input {

///
/// \brief InputStream
///
using InputStream = InputStreamServer2;

static_assert( std::is_base_of<Input, InputStream>::value, "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamBase, InputStream>::value,
               "Stream is base class of InputStream" );

} // namespace input
} // namespace hub
