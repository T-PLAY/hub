
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <sensor/Resolution.hpp>

//#include <OpenImageIO/image_view.h>
//#include <OpenImageIO/imagebuf.h>
//#include <OpenImageIO/imageio.h>

TEST_CASE( "Matrix compat OIIO test" ) {

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

}
