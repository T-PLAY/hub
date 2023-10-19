
#include <catch2/catch_test_macros.hpp>

#include <iostream>

//#include <sensor/Resolution.hpp>
#include <sensor/Format.hpp>

TEST_CASE( "Resolution test" ) {

//    hub::sensor::Resolutions resolutions;
//    resolutions.push_back( hub::sensor::Resolution { hub::sensor::format::DISPARITY16, 1  } );
//    std::cout << resolutions << std::endl;

//    static_assert(hub::sensor::format::RGB8.nByte() == 3);
//    static_assert(hub::sensor::format::RGB8.name() == "RGB8");
//    static_assert(hub::sensor::format::RGB8. == "RGB8");

    static_assert(hub::sensor::format::RGB8.nByte == 3);
    static_assert(hub::sensor::format::RGB8.name == std::string("RGB8"));
    static_assert(! hub::sensor::format::RGB8.interpolable);

    auto & format = hub::sensor::format::RGB8;

//    format.interpolable = false;

//    hub::sensor::Format format = hub::sensor::format::NONE;
//    std::cout << format << std::endl;

//    hub::sensor::Resolution resolution { { 1 }, format };
//    std::cout << resolution << std::endl;
}
