
#include <cassert>
#include <iostream>

#include <core/Format.hpp>
#include <core/Matrix.hpp>

/// \file
int main() {

    /// startConstruction
const auto resolutionVolume = hub::make_matrix<float>( 512, 512, 100 );
const auto resolutionCam    = hub::make_matrix<hub::format::RGB8>( 640, 480 );
const auto resolutionAll    = hub::make_matrix( resolutionVolume, resolutionCam );
    /// endConstruction

    return 0;
}
