
#include <test_common.hpp>

#include <OpenImageIO/image_view.h>
#include <OpenImageIO/imagebuf.h>

TEST_CASE( "Matrix compat OIIO test" ) {
    TEST_BEGIN()

   // Todo check if hub matrix is compatible with OIIO matrix

    constexpr auto width  = 20;
    constexpr auto height = 10;
    constexpr auto size   = width * height;
    unsigned char data[size] { 0 };
    for ( int i = 0; i < size; ++i ) {
        data[i] = 65 + i;
    }

    OIIO::ImageBuf buff;

    TEST_END()
}
