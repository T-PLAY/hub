/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/17

#pragma once

#include "core/Output.hpp"

#include "io/output/OutputStreamServer2.hpp"

namespace hub {
namespace output {

///
/// \brief OutputStream
///
using OutputStream = OutputStreamServer2;

static_assert( std::is_base_of<Output, OutputStream>::value,
               "Output is base class of OutputStream" );
static_assert( std::is_base_of<io::StreamBase, OutputStream>::value,
               "StreamInterface is base class of OutputStream" );

} // namespace output
} // namespace hub
