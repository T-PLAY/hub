
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/matrix/MatrixTs.hpp>
#include <core/matrix/MatrixXD.hpp>
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

TEST_CASE( "MatrixTs test" ) {
    TEST_BEGIN()

    using ResCTypes = hub::MatrixTs<int, double, bool>;
    std::cout << "ResCTypes: " << ResCTypes() << std::endl;
    std::cout << "ResCTypes: " << ResCTypes().getMatrix() << std::endl;
    const auto resCTypes  = hub::make_matrix<int, double, bool>();
    const auto resCTypes2 = hub::make_matrix<int, bool, double>();
    std::cout << "resCTypes: " << resCTypes << std::endl;
    std::cout << std::endl;
    CHECK( resCTypes == ResCTypes().getMatrix() );
    CHECK( resCTypes2 != ResCTypes().getMatrix() );

    constexpr auto width  = 640;
    constexpr auto height = 480;
    using ResRGB          = hub::MatrixXD<RGB8, width, height>;
    using ResPoint        = hub::MatrixXD<XYZ32F, width, height>;

    std::cout << "ResRGB: " << ResRGB() << std::endl;
    std::cout << "ResRGB: " << ResRGB().getMatrix() << std::endl;
    const auto resRGB  = hub::make_matrix<RGB8, width, height>();
    const auto resRGB2 = hub::make_matrix<RGB8>( width, height );
    std::cout << "resRGB: " << resRGB << std::endl;
    std::cout << std::endl;
    CHECK( resRGB == ResRGB().getMatrix() );
    CHECK( resRGB2 == ResRGB().getMatrix() );

    std::cout << "ResPoint: " << ResPoint() << std::endl;
    std::cout << "ResPoint: " << ResPoint().getMatrix() << std::endl;
    const auto resPoint  = hub::make_matrix<XYZ32F, width, height>();
    const auto resPoint2 = hub::make_matrix<XYZ32F>( width, height );
    std::cout << "resPoint: " << resPoint << std::endl;
    std::cout << std::endl;
    CHECK( resPoint == ResPoint().getMatrix() );
    CHECK( resPoint2 == ResPoint().getMatrix() );

    using Resolution = hub::MatrixTs<ResRGB, ResPoint>;
    std::cout << "Resolution: " << Resolution() << std::endl;
    std::cout << "Resolution: " << Resolution().getMatrix() << std::endl;
    const auto resolution  = hub::make_matrix( resRGB, resPoint );
    const auto resolution2 = hub::make_matrix<ResRGB, ResPoint>();
    std::cout << "resolution: " << resolution << std::endl;
    std::cout << std::endl;
    CHECK( resolution == Resolution().getMatrix() );
    CHECK( resolution2 == Resolution().getMatrix() );

    TEST_END()
}
