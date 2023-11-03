

#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <sensor/Resolution.hpp>
//#include <sensor/Resolutions.hpp>
//#include <sensor/resolution/Format.hpp>

#include <core/io/Archive.hpp>


TEST_CASE( "Resolution test" ) {


    //    hub::sensor::Resolutions resolutions;
    //    resolutions.push_back( hub::sensor::Resolution {
    //    hub::sensor::resolution::format::DISPARITY16, 1  } ); std::cout << "resolutions: " <<
    //    resolutions << std::endl;

//    using MyResolution = hub::sensor::Resolution<unsigned char, width, height>;

    using Resolution = hub::sensor::ResolutionT<int, 640, 480>;
    static_assert(Resolution::nByte() == 640 * 480 * sizeof(int));
    static_assert(Resolution::nDim() == 2);
    static_assert(Resolution::nResolution() == 1);
    static_assert(Resolution::getDim(0) == 640);
    static_assert(Resolution::getDim(1) == 480);
    static_assert(Resolution::getDim(2) == 0);
    static_assert(Resolution::width() == 640);
    static_assert(Resolution::height() == 480);

//    static_assert(Resolution::n<0>() == 640);
//    static_assert(Resolution::n<1>() == 480);
//    constexpr std::string_view str = "<int:640x480>";
//    static_assert(Resolution::typeName() == str);
    static_assert(std::is_same_v<Resolution::typeFormat, int>);
    std::cout << "resolution: " << Resolution() << std::endl;

    struct UserFormat {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        static constexpr auto name() { return "userFormat"; };
        static constexpr auto interpolable() { return true; };
    };
//    std::cout << "RGB8 hash code: " << typeid(RGB8).hash_code() << std::endl;
//    struct BGR8 {
//        unsigned char b;
//        unsigned char g;
//        unsigned char r;
//    };
//    std::cout << "BGR8 hash code: " << typeid(BGR8).hash_code() << std::endl;

    constexpr auto size = 1;
    using Resolution1 = hub::sensor::ResolutionT<UserFormat>;
    static_assert(Resolution1::nByte() == sizeof(UserFormat) * size);
    static_assert(Resolution1::nDim() == 1);
    static_assert(Resolution1::nResolution() == 1);
    static_assert(Resolution1::getDim(0) == size);
    static_assert(Resolution1::length() == size);
    static_assert(std::is_same_v<Resolution1::typeFormat, UserFormat>);
    std::cout << "resolution1: " << Resolution1() << std::endl;

    constexpr auto format1 = Resolution1::getFormat();
    static_assert(format1.interpolable == true);
    static_assert(format1.nByte == 3);
    static_assert(format1.name == "userFormat");

    constexpr auto nDim1 = Resolution1::getNDim();
    static_assert(nDim1.capacity() == 1);
    static_assert(nDim1.size() == 1);
    static_assert(nDim1.ns[0] == 1);


    using Resolution2 = hub::sensor::ResolutionT<hub::sensor::resolution::format::RGB8, 640, 480>;
    static_assert(Resolution2::width() == 640);
    static_assert(Resolution2::height() == 480);
    std::cout << "resolution2: " << Resolution2() << std::endl;

    using Resolution3 = hub::sensor::ResolutionT<hub::sensor::resolution::format::RGB8, 1, 2, 3>;
    static_assert(Resolution3::nx() == 1);
    static_assert(Resolution3::ny() == 2);
    static_assert(Resolution3::nz() == 3);
    std::cout << "resolution3: " << Resolution3() << std::endl;


    using Resolution4 = hub::sensor::ResolutionT<hub::sensor::resolution::format::RGB8, 1, 2, 3, 4>;
    static_assert(Resolution4::nx() == 1);
    static_assert(Resolution4::ny() == 2);
    static_assert(Resolution4::nz() == 3);
    static_assert(Resolution4::nt() == 4);
    std::cout << "resolution4: " << Resolution4() << std::endl;

    using Resolution5 = hub::sensor::ResolutionT<hub::sensor::resolution::format::RGB8, 1, 2, 3, 4, 5>;
    static_assert(Resolution5::n<0>() == 1);
    static_assert(Resolution5::n<1>() == 2);
    static_assert(Resolution5::n<2>() == 3);
    static_assert(Resolution5::n<3>() == 4);
    static_assert(Resolution5::n<4>() == 5);
    std::cout << "resolution5: " << Resolution5() << std::endl;
//    static_assert(Resolution4::n<4>() == 2);

//    using Resolutions = hub::sensor::ResolutionsT<Resolution>;
//    std::cout << "resolutions: " << Resolutions() << std::endl;


//    using Resolutions2 = hub::sensor::ResolutionsT<Resolution, Resolution2, Resolution3>;
//    std::cout << "resolutions2: " << Resolutions2() << std::endl;

    hub::sensor::resolution::Format format3{5, std::string_view(TYPE_NAME(int)), false};
    std::cout << "format: " << format3 << std::endl;
    std::cout << "format: " << Resolution().getFormat() << std::endl;

    hub::io::Archive archive;
    archive.write(Resolution().getFormat());

    hub::sensor::resolution::Format format;
    archive.read(format);

    return;

//    const char * str = "fuck";
//    constexpr std::string_view str = "fuck";
    constexpr hub::sensor::resolution::Format format2{3, UserFormat::name(), false};

    constexpr hub::sensor::resolution::Format format4{3, "RGB8", false};
    constexpr hub::sensor::resolution::NDim nDim{1, 2};
    constexpr hub::sensor::Resolution resolution{format4, nDim};

//    Resolution resolution;
//    archive.read(resolution);

    //    hub::sensor::Resolution resolution { hub::sensor::resolution::format::BGR8, 1 };
    //    CHECK(resolution.nByte() == 3);

    //    hub::sensor::Resolution resolution2 { hub::sensor::resolution::format::Y8, 6 };
    //    CHECK(resolution2.nByte() == 6);

    //    hub::sensor::Resolutions resolutions2 {resolution, resolution2};
    //    hub::Size_t size = 0;
    //    for (const auto & resolution : resolutions2) {
    //        size += resolution.nByte();
    //    }
    //    CHECK(size == 9);
    //    CHECK(hub::sensor::nByte(resolutions2) == 9);

    //    const hub::sensor::Resolution resolution3 { hub::sensor::resolution::format::Y8, 6};
    //    auto resolution3_copy = resolution3;
    //    assert(resolution3 == resolution3_copy);

    //    const hub::sensor::Resolutions resolutions3 {{ hub::sensor::resolution::format::Y8, 6}};
    //    auto resolutions3_copy = resolutions3;
    //    assert(resolutions3 == resolutions3_copy);

    //    hub::sensor::Resolution resolution4 { hub::sensor::resolution::format::Y8, 6};
    //    auto resolution4_copy = resolution4;
    //    assert(resolution4 == resolution4_copy);

    //    hub::sensor::Resolutions resolutions4 {{ hub::sensor::resolution::format::Y8, 6}};
    //    auto resolutions4_copy = resolutions4;
    //    assert(resolutions4 == resolutions4_copy);

    //    hub::sensor::Resolution resolution5 { hub::sensor::resolution::format::Y8, 6};
    //    auto resolution5_copy = std::move(resolution5);
    //    assert(resolution4 == resolution5_copy);

    //    hub::sensor::Resolutions resolutions5 {{ hub::sensor::resolution::format::Y8, 6}};
    //    auto resolutions5_copy = std::move(resolutions5);
    //    assert(resolutions4 == resolutions5_copy);

    //    std::cout << "resolutions2: " << resolutions2 << std::endl;

    ////    static_assert(hub::sensor::resolution::format::RGB8. == "RGB8");

    ////    auto & format = hub::sensor::resolution::format::RGB8;

    ////    hub::sensor::Format format = hub::sensor::resolution::format::NONE;
    ////    std::cout << format << std::endl;

    ////    hub::sensor::Resolution resolution { { 1 }, format };
    //    std::cout << resolution << std::endl;
}
