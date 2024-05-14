/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

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
