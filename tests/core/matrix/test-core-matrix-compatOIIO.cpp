
#include <test_common.hpp>

#include <iostream>

TEST_CASE( "Matrix compat OIIO test" ) {
    TEST_BEGIN()

    constexpr auto width  = 20;
    constexpr auto height = 10;
    constexpr auto size   = width * height;
    unsigned char data[size] { 0 };
    for ( int i = 0; i < size; ++i ) {
        data[i] = 65 + i;
    }

    ////            assert( eigenMat.coeff( i, j ) == cvMat.data[i * height + j] );
    ////    OIIO::image_view imageView()

    TEST_END()
}
