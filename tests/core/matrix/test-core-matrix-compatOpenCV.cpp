
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <sensor/Resolution.hpp>


#ifdef HUB_USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

//#include <OpenImageIO/image_view.h>
//#include <OpenImageIO/imagebuf.h>
//#include <OpenImageIO/imageio.h>

TEST_CASE( "Matrix compat OpenCV test" ) {

    constexpr auto width  = 20;
    constexpr auto height = 10;
    constexpr auto size   = width * height;
    unsigned char data[size] { 0 };
    for ( int i = 0; i < size; ++i ) {
        data[i] = 65 + i;
    }

//    OIIO::ImageSpec imageSpec(width, height, 1, OIIO::TypeDesc::UINT8);
//    OIIO::ImageBuf imageBuf(imageSpec, data);
//    for ( int j = 0; j < height; ++j ) {
//        for ( int i = 0; i < width; ++i ) {
//            float * pixel = nullptr;
//            imageBuf.getpixel(i, j, pixel );
//            std::cout << *pixel << " ";
////            assert( eigenMat.coeff( i, j ) == cvMat.data[i * height + j] );
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
////    OIIO::image_view imageView()

    //    using EigenMat = Eigen::Matrix<unsigned char, Eigen::Dynamic>;
    //    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> eigenMat(data, height,
    //    width);

    ////////////////////////////////////

#ifdef HUB_USE_OPENCV
    cv::Mat cvMat( height, width, CV_8UC3, data ); // row major

    for ( int j = 0; j < height; ++j ) {
        for ( int i = 0; i < width; ++i ) {
            std::cout << cvMat.data[i + j * width] << " ";
//            assert( eigenMat.coeff( i, j ) == cvMat.data[i * height + j] );
        }
        std::cout << std::endl;
    }
#endif

    //    hub::sensor::Resolutions resolutions;
    //    resolutions.push_back( hub::sensor::Resolution {
    //    hub::sensor::format::DISPARITY16, 1  } ); std::cout << "resolutions: " <<
    //    resolutions << std::endl;

    //    hub::sensor::Resolution resolution { hub::sensor::format::BGR8, 1 };
    //    CHECK(resolution.nByte() == 3);

    //    hub::sensor::Resolution resolution2 { hub::sensor::format::Y8, 6 };
    //    CHECK(resolution2.nByte() == 6);

    //    hub::sensor::Resolutions resolutions2 {resolution, resolution2};
    //    hub::Size_t size = 0;
    //    for (const auto & resolution : resolutions2) {
    //        size += resolution.nByte();
    //    }
    //    CHECK(size == 9);
    //    CHECK(hub::sensor::nByte(resolutions2) == 9);

    //    const hub::sensor::Resolution resolution3 { hub::sensor::format::Y8, 6};
    //    auto resolution3_copy = resolution3;
    //    assert(resolution3 == resolution3_copy);

    //    const hub::sensor::Resolutions resolutions3 {{ hub::sensor::format::Y8, 6}};
    //    auto resolutions3_copy = resolutions3;
    //    assert(resolutions3 == resolutions3_copy);

    //    hub::sensor::Resolution resolution4 { hub::sensor::format::Y8, 6};
    //    auto resolution4_copy = resolution4;
    //    assert(resolution4 == resolution4_copy);

    //    hub::sensor::Resolutions resolutions4 {{ hub::sensor::format::Y8, 6}};
    //    auto resolutions4_copy = resolutions4;
    //    assert(resolutions4 == resolutions4_copy);

    //    hub::sensor::Resolution resolution5 { hub::sensor::format::Y8, 6};
    //    auto resolution5_copy = std::move(resolution5);
    //    assert(resolution4 == resolution5_copy);

    //    hub::sensor::Resolutions resolutions5 {{ hub::sensor::format::Y8, 6}};
    //    auto resolutions5_copy = std::move(resolutions5);
    //    assert(resolutions4 == resolutions5_copy);

    //    std::cout << "resolutions2: " << resolutions2 << std::endl;

    ////    static_assert(hub::sensor::format::RGB8. == "RGB8");

    ////    auto & format = hub::sensor::format::RGB8;

    ////    hub::sensor::Format format = hub::sensor::format::NONE;
    ////    std::cout << format << std::endl;

    ////    hub::sensor::Resolution resolution { { 1 }, format };
    //    std::cout << resolution << std::endl;
}
