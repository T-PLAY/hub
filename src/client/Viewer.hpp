/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <Gauthier.Bouyjou@irit.fr>
 * @date 2023/03/13
 */

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
