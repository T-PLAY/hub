

#include "test_common.hpp"

#include <tuple>

#include <core/Any.hpp>
#include <core/Traits.hpp>
#include <core/matrix/MatrixT.hpp>

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, c, ds, e );
    }

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) { return os; }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

struct UserClass {
    bool a;
    bool b;
    auto toString() const { return std::to_string( a ) + " " + std::to_string( b ); }
    static constexpr auto name() { return "UserClass"; }
};

TEST_CASE( "MatrixT test" ) {
    TEST_BEGIN()

    //

    static_assert( hub::isMatrix<hub::MatrixXD<int, 640, 480>> );
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

    static_assert( MatrixTs<int, double>::nType() == 2 );

#if CPP_VERSION >= 20
    const auto matrix = MatrixT<10, int>();
    static_assert( matrix.size() == 10 * sizeof( int ) );
    const auto matrix2 = MatrixT<int, double>();
    using R            = unsigned char;
    using G            = unsigned char;
    using B            = unsigned char;
    const auto matrix3 = MatrixT<640, 480, R, G, B>();
    static_assert( matrix3.width() == 640 );
    static_assert( matrix3.size() == 640 * 480 * 3 );
    auto matrix4 = MatrixT<2, MAX_STACK_SIZE, unsigned char>();
    auto matrix5 = MatrixT<MatrixXD<int, 2>, double, float>();
    static_assert( matrix5.size() == sizeof( int ) * 2 + sizeof( double ) + sizeof( float ) );
#endif

    using MatrixInt = MatrixXD<int, 2>;
    static_assert( MatrixInt::Size == 8 );
    using MatrixFloat = MatrixXD<float, 2>;
    static_assert( MatrixFloat::Size == 8 );
    using MatrixDouble = MatrixXD<double, 2>;
    static_assert( MatrixDouble::Size == 16 );
    using Matrices =
        MatrixTs<double, bool, float, double, double, MatrixInt, MatrixFloat, MatrixDouble, double>;
    static_assert( Matrices::Size == 69 );
    static_assert( Matrices::nType<double>() == 5 );
    static_assert( Matrices::nType<bool>() == 1 );
    static_assert( Matrices::nType<float>() == 2 );

    Matrices matrices;
    std::cout << "Matrices: " << matrices << std::endl;
    static_assert( !matrices.hasType<MatrixInt, double, bool>() );
    static_assert( matrices.hasType<int, double, bool>() );
    static_assert( matrices.hasType<int>() );
    static_assert( matrices.getOffset<double>() == 0 );
    static_assert( matrices.getOffset<bool>() == 8 );
    static_assert( matrices.getOffset<float>() == 9 );
    static_assert( matrices.getOffset<double, 1>() == 13 );
    static_assert( matrices.getOffset<double, 2>() == 21 );
    static_assert( matrices.getOffset<int>() == 29 );
    static_assert( matrices.getOffset<float, 1>() == 37 );
    static_assert( matrices.getOffset<double, 3>() == 45 );
    static_assert( matrices.getOffset<double, 4>() == 61 );

    double& doubleRef_get = matrices.get<double&, 0>();
    doubleRef_get         = 1.0;
    std::cout << "Matrices: " << matrices << std::endl;

    bool* doublePtr_get = matrices.get<bool*>();
    *doublePtr_get      = true;

    matrices.get<float&>()     = 2.0f;
    matrices.get<double&, 1>() = 3.0f;
    matrices.get<double&, 2>() = 4.0f;
    matrices.get<int&>()       = 5;
    matrices.get<float&, 1>()  = 6.0f;
    matrices.get<double&, 3>() = 7.0f;
    matrices.get<double&, 4>() = 8.0f;

    std::cout << "Matrices: " << matrices << std::endl;

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

    //////////////////////////////////////////////////////////////////////

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

    using UserMatrix = MatrixXD<UserClass, 2, 3>;

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

    ////////////////////////////////

    constexpr int width    = 512;
    constexpr int height   = 512;
    using ResolutionCam    = hub::MatrixXD<hub::format::RGB8, width, height>;
    using Resolution       = hub::MatrixTs<hub::format::Dof6, ResolutionCam>;

    static_assert( Resolution::getOffset<hub::format::Dof6>() == 0 );
    static_assert( Resolution::getOffset<hub::format::RGB8>() == 28);
    static_assert( Resolution::Size == 512 * 512 * 3 + 28);

    using Resolution2       = hub::MatrixTs<ResolutionCam, hub::format::Dof6>;
    static_assert( Resolution2::getOffset<hub::format::Dof6>() == 512 * 512 * 3 );
    static_assert( Resolution2::getOffset<hub::format::RGB8>() == 0);
    static_assert( Resolution2::Size == 512 * 512 * 3 + 28);


    TEST_END()
}
