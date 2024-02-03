
#include <test_common.hpp>

#include <iostream>

#ifdef HUB_USE_EIGEN3
#    include <Eigen/Core>
#endif

TEST_CASE( "Matrix compat Eigen test" ) {
    TEST_BEGIN()

    constexpr auto width  = 20;
    constexpr auto height = 10;
    constexpr auto size   = width * height;
    unsigned char data[size] { 0 };
    for ( int i = 0; i < size; ++i ) {
        data[i] = 65 + i;
    }

    ////            assert( eigenMat.coeff( i, j ) == cvMat.data[i * height + j] );

#ifdef HUB_USE_EIGEN3
    Eigen::Matrix<unsigned char, height, width, Eigen::RowMajor> eigenMat( data );
    for ( int j = 0; j < height; ++j ) {
        for ( int i = 0; i < width; ++i ) {
            std::cout << eigenMat.coeff( j, i ) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    TEST_END()
}
