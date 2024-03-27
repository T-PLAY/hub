/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

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
