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

#include "Format.hpp"
#include "matrix/MatrixXD.hpp"

namespace hub {

namespace resolution {

using Full_HD      = MatrixXD<format::RGB8, 1920, 1080>;
using Full_HD_Plus = MatrixXD<format::RGB8, 2160, 1080>;
using UltraWide    = MatrixXD<format::RGB8, 2560, 1080>;

} // namespace resolution

} // namespace hub
