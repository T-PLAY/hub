
#include <test_common.hpp>

#include <iostream>

#ifdef HUB_USE_OPENCV
#    include <opencv2/opencv.hpp>
#endif

TEST_CASE( "Matrix compat OpenCV test" ) {
    TEST_BEGIN()

    constexpr auto width  = 20;
    constexpr auto height = 10;
    constexpr auto size   = width * height;
    unsigned char data[size] { 0 };
    for ( int i = 0; i < size; ++i ) {
        data[i] = 65 + i;
    }

#ifdef HUB_USE_OPENCV
    cv::Mat cvMat( height, width, CV_8UC3, data ); // row major

    for ( int j = 0; j < height; ++j ) {
        for ( int i = 0; i < width; ++i ) {
            std::cout << cvMat.data[i + j * width] << " ";
        }
        std::cout << std::endl;
    }
#endif

    TEST_END()
}
