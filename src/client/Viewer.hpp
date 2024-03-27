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

#include "viewer/ViewerServer2.hpp"

#include "io/input/InputStream.hpp"

namespace hub {
namespace client {

///
/// \brief Viewer
///
using Viewer = ViewerServer2;

static_assert( std::is_base_of<client::ViewerInterface<input::InputStream>, Viewer>::value,
               "ViewerInterface is base class of Viewer" );

} // namespace client
} // namespace hub
