/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/08/17
 */

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
