

#include "test_common.hpp"

#include <core/matrix/MatrixXD.hpp>

#include <core/Types.h>

namespace Types {
enum User : hub::TypeId_t {
    RGB8 = hub::Types::Count,
    XYZ32F,

    User_Count
};
}

struct RGB8 {
    static constexpr auto id = Types::User::RGB8;
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct XYZ32F {
    static constexpr auto id = Types::User::XYZ32F;
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

    // ResRGB().getMatrix();
    std::cout << "ResRGB matrix: " << ResRGB().getMatrix() << std::endl;
    std::cout << "ResPoint matrix: " << ResPoint().getMatrix() << std::endl;
    // ResPoint().getMatrix();

    static_assert( hub::isMatrix<hub::MatrixXD<int, 640, 480>> );

    using namespace hub;

    using MyMatrixXD = MatrixXD<int, 640, 480>;
    static_assert( MyMatrixXD::capacity() == 640 * 480 );
    static_assert( MyMatrixXD::nDim() == 2 );
    static_assert( MyMatrixXD::getDim<0>() == 640 );
    static_assert( MyMatrixXD::getDim<1>() == 480 );
    static_assert( MyMatrixXD::nType() == 1 );
    static_assert( MyMatrixXD::hasType<int>() );
    static_assert( MyMatrixXD::hasType<int, int>() );
    static_assert( !MyMatrixXD::hasType<float>() );
    static_assert( !MyMatrixXD::hasType<bool>() );
    static_assert( std::is_same<MyMatrixXD::getType<0>, int>() );
    static_assert( MyMatrixXD::size() == sizeof( int ) * 640 * 480 );

    std::cout << "MyMatrixXD: " << MyMatrixXD() << std::endl;

    static_assert( MatrixXD<int, 2>::Capacity == 2 );

    using MatrixInt = MatrixXD<int, 2>;
    static_assert( MatrixInt::Size == 8 );
    using MatrixFloat = MatrixXD<float, 2>;
    static_assert( MatrixFloat::Size == 8 );
    using MatrixDouble = MatrixXD<double, 2>;
    static_assert( MatrixDouble::Size == 16 );

    constexpr Buffer<Data_t, 2> buffer { 1, 2 };
    std::cout << "buffer: " << buffer << std::endl;

    constexpr Buffer<int, 2> buffer2 { 1, 2 };
    std::cout << "buffer2: " << buffer2 << std::endl;

#if CPP_VERSION > 14
    constexpr MatrixXD<char, 2> matrice4 { { 1, 2 } };
    std::cout << "matrice4: " << matrice4 << std::endl;

    constexpr MatrixXD<int, 2> matrice5 { { 1, 2 } };
    std::cout << "matrice5: " << matrice5 << std::endl;
#endif

    char str[] { 'B', 'o', 'u', 'y', 'j', 'o', 'u' };
    std::cout << "str: " << std::to_string( str[0] ) << std::endl;
    std::string string;
    string += str;
    static_assert( std::is_arithmetic<char>() );

    ///////////////////////////////////////////////////////////////////////////////

#if CPP_VERSION > 14
    using VectorChar = Vector<char, 10>;
    VectorChar vectorChar { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
    std::cout << "vectorChar: " << vectorChar << std::endl;
    CHECK( vectorChar.get<0>() == 1 );
    CHECK( vectorChar.get<1>() == 2 );

    auto serial = vectorChar.getMatrix();
    std::cout << "serial: " << serial << std::endl;

    CHECK( serial.hasType<char>() );
    CHECK( serial.getDims<char>() == std::vector<int> { 10 } );
#endif

    ////////////////////////////////
    /// \brief The UserClass class
    ///
    struct UserClass {
        bool a;
        bool b;
        auto toString() const { return std::to_string( a ) + " " + std::to_string( b ); }
        static constexpr auto name() { return "UserClass"; }
    };

    using UserMatrix = MatrixXD<UserClass, 2, 3>;

    ////////////////////////////////

    TEST_END()
}
