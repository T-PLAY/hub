
#include <cassert>
#include <iostream>

#include <core/Format.hpp>
// #include <core/Matrix.hpp>
#include <core/matrix/MatrixXD.hpp>

/// \file
int main() {

    /// startConstruction
using ResolutionVolume = hub::MatrixXD<float, 512, 512, 100>;
using ResolutionCam = hub::MatrixXD<hub::format::RGB8, 640, 480>;
    /// endConstruction

    return 0;
}
