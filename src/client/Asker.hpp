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
