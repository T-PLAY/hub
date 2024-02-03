/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2024/01/03

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
