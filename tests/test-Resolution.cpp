
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <Resolution.hpp>

TEST_CASE( "Resolutions test" ) {

    hub::Resolutions resolutions;
    resolutions.push_back( hub::Resolution { { 1 }, hub::Format::DISPARITY16 } );
    std::cout << resolutions << std::endl;

    hub::Format format = hub::Format::NONE;
    std::cout << format << std::endl;

    hub::Resolution resolution { { 1 }, format };
    std::cout << resolution << std::endl;
}
