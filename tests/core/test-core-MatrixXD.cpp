
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"


#include <core/matrix/MatrixXD.hpp>
// #include <core/matrix/MatrixTs.hpp>
// #include <core/Matrix.hpp>


struct RGB8 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct XYZ32F {
    float x;
    float y;
    float z;
};

TEST_CASE( "MatrixXD test" ) {
    TEST_BEGIN()

    constexpr auto width  = 640;
    constexpr auto height = 480;
    using ResRGB          = hub::MatrixXD<RGB8, width, height>;
    using ResPoint        = hub::MatrixXD<XYZ32F, width, height>;

    std::cout << "ResRGB: " << ResRGB() << std::endl;
    std::cout << "ResPoint: " << ResPoint() << std::endl;

    // using Resolution      = hub::MatrixTs<ResRGB, ResPoint>;
    // using Resolution      = hub::MatrixTs<int, double>;
    // using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    // using Resolution = ResPoint;
    // using Resolution = ResRGB;
    // using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    // ResRGB().getMatrix();
    std::cout << "ResRGB matrix: " << ResRGB().getMatrix() << std::endl;
    std::cout << "ResPoint matrix: " << ResPoint().getMatrix() << std::endl;
    // ResPoint().getMatrix();
    // hub::make_matrix<Resolution>();
    //     std::cout << typeid(int).hash_code() << std::endl;

    TEST_END()
}
