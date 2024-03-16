

#include "test_common.hpp"

#include <core/matrix/MatrixTs.hpp>

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

    static_assert( hub::isMatrix<hub::MatrixTs<int, double>> );
    static_assert( hub::isMatrix<hub::Matrix> );

    using namespace hub;

    using MyMatrixTs = MatrixTs<int, double, float>;
    static_assert( MyMatrixTs::capacity() == 1 );
    static_assert( MyMatrixTs::nDim() == 1 );
    static_assert( MyMatrixTs::getDim<0>() == 1 );
    static_assert( MyMatrixTs::nType() == 3 );
    static_assert( MyMatrixTs::hasType<int>() );
    static_assert( MyMatrixTs::hasType<double>() );
    static_assert( MyMatrixTs::hasType<float>() );
    static_assert( !MyMatrixTs::hasType<bool>() );
    static_assert( MyMatrixTs::hasType<int, double, float>() );
    static_assert( !MyMatrixTs::hasType<int, double, float, char>() );
    static_assert( std::is_same<MyMatrixTs::getType<0>, int>() );
    static_assert( std::is_same<MyMatrixTs::getType<1>, double>() );
    static_assert( std::is_same<MyMatrixTs::getType<2>, float>() );
    static_assert( MyMatrixTs::size() == sizeof( int ) + sizeof( double ) + sizeof( float ) );

    std::cout << "MyMatrixTs: " << MyMatrixTs() << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    using MatricesChar = MatrixTs<char, char, char, char, char, char, char, char, char, int>;
    constexpr MatricesChar matricesChar {
        'g', 'a', 'u', 't', 'h', 'i', 'e', 'r', '\0', 5, 0, 0, 0 };
    std::cout << "matricesChar: " << matricesChar << std::endl;
    static_assert( matricesChar.nDim() == 1 );
    static_assert( matricesChar.getDim<0>() == 1 );
    static_assert( matricesChar.nType() == 10 );
    static_assert( matricesChar.hasType<char, int>() );
    static_assert( matricesChar.nType<char>() == 9 );
    static_assert( matricesChar.nType<int>() == 1 );
    CHECK( matricesChar.get<const char&>() == 'g' );
    CHECK( ( matricesChar.get<const char&, 0>() == 'g' ) );
    CHECK( ( matricesChar.get<const char&, 1>() == 'a' ) );
    CHECK( ( matricesChar.get<const char&, 2>() == 'u' ) );
    char* myName = matricesChar.get<char*>();
    CHECK( std::memcmp( myName, "gauthier", strlen( myName ) ) == 0 );
    CHECK( strlen( myName ) == 8 );
    std::cout << "myName: " << myName << std::endl;
    CHECK( matricesChar.get<const int&>() == 5 );

    auto serialChar = matricesChar.getMatrix();
    serialChar.setData( matricesChar.data(), matricesChar.size() );
    std::cout << "serialChar: " << serialChar << std::endl;
    CHECK( serialChar.nType() == 10 );
    CHECK( ( serialChar.hasAnyType<char, int>() ) );
    CHECK( serialChar.nType<char>() == 9 );
    CHECK( serialChar.nType<int>() == 1 );
    CHECK( serialChar.getDims<char>() == Dims { 1 } );
    CHECK( ( serialChar.getDims<char, 1>() == Dims { 1 } ) );
    CHECK( ( serialChar.getDims<char, 2>() == Dims { 1 } ) );
    CHECK( ( serialChar.getDims<char, 8>() == Dims { 1 } ) );
    CHECK( serialChar.get<const char&>() == 'g' );
    CHECK( ( serialChar.get<const char&, 0>() == 'g' ) );
    CHECK( ( serialChar.get<const char&, 1>() == 'a' ) );
    CHECK( ( serialChar.get<const char&, 2>() == 'u' ) );
    char* myName2 = serialChar.get<char*>();
    CHECK( std::memcmp( myName2, "gauthier", strlen( myName2 ) ) == 0 );
    CHECK( serialChar.get<const int&>() == 5 );

    for ( int iType = 0; iType < serialChar.nType(); ++iType ) {
        const auto& dims = serialChar.getDims( iType );
    }

    // Matrix serialChar_read;

    //////////////////////////////////////////////////////////////////////

    constexpr Buffer<Data_t, 2> buffer { 1, 2 };
    std::cout << "buffer: " << buffer << std::endl;

    constexpr Buffer<int, 2> buffer2 { 1, 2 };
    std::cout << "buffer2: " << buffer2 << std::endl;

    char str[] { 'B', 'o', 'u', 'y', 'j', 'o', 'u' };
    std::cout << "str: " << std::to_string( str[0] ) << std::endl;
    std::string string;
    string += str;
    static_assert( std::is_arithmetic<char>() );

    ///////////////////////////////////////////////////////////////////////////////

    MatrixTs<int, double> matrixUser;
    std::cout << "matrixUser: " << matrixUser << std::endl;

    auto serial2 = matrixUser.getMatrix();
    std::cout << "matrixUser: " << serial2 << std::endl;
    serial2.hasAnyType<int, double>();
    auto hasIntDouble = serial2.hasAnyType<int, double>();
    CHECK( hasIntDouble );

    CHECK( ( serial2.hasSomeType<bool, int>() ) );

    CHECK( serial2.getDims<int>() == Dims { 1 } );
    CHECK( serial2.getDims<double>() == Dims { 1 } );

    ///////////////////////////////////////////////////////////////////////////////

    TEST_END()
}
