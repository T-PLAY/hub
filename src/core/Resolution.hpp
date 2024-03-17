/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2024/01/03
 */

#pragma once

#include "Format.hpp"
#include "matrix/MatrixXD.hpp"

namespace hub {

namespace resolution {

using Full_HD      = MatrixXD<format::RGB8, 1920, 1080>;
using Full_HD_Plus = MatrixXD<format::RGB8, 2160, 1080>;
using UltraWide    = MatrixXD<format::RGB8, 2560, 1080>;

} // namespace resolution

} // namespace hub
