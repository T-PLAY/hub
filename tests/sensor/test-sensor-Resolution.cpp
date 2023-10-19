
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <sensor/Resolution.hpp>

TEST_CASE( "Resolution test" ) {

    hub::sensor::Resolutions resolutions;
    resolutions.push_back( hub::sensor::Resolution { hub::sensor::format::DISPARITY16, 1  } );
    std::cout << "resolutions: " << resolutions << std::endl;

    hub::sensor::Resolution resolution { hub::sensor::format::BGR8, 1 };
    CHECK(resolution.nByte() == 3);

    hub::sensor::Resolution resolution2 { hub::sensor::format::Y8, 6 };
    CHECK(resolution2.nByte() == 6);

    hub::sensor::Resolutions resolutions2 {resolution, resolution2};
    hub::Size_t size = 0;
    for (const auto & resolution : resolutions2) {
        size += resolution.nByte();
    }
    CHECK(size == 9);
    CHECK(hub::sensor::nByte(resolutions2) == 9);

    std::cout << "resolutions2: " << resolutions2 << std::endl;

//    static_assert(hub::sensor::format::RGB8. == "RGB8");

//    auto & format = hub::sensor::format::RGB8;

//    hub::sensor::Format format = hub::sensor::format::NONE;
//    std::cout << format << std::endl;

//    hub::sensor::Resolution resolution { { 1 }, format };
//    std::cout << resolution << std::endl;
}
