
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <sensor/Resolution.hpp>

TEST_CASE( "Resolutions test" ) {

    hub::sensor::Resolutions resolutions;
    resolutions.push_back( hub::sensor::Resolution { { 1 }, hub::sensor::Format::DISPARITY16 } );
    std::cout << resolutions << std::endl;

    hub::sensor::Format format = hub::sensor::Format::NONE;
    std::cout << format << std::endl;

    hub::sensor::Resolution resolution { { 1 }, format };
    std::cout << resolution << std::endl;
}
