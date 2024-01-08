
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <tuple>

#include <core/Any.hpp>
#include <core/Matrix.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
// #include <core/matrix/MatrixT.hpp>

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

    auto toString() const { return hub::to_string( a, b, c, ds, e ); }
    // friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
    //     //        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " "
    //     <<
    //     //        lambda.e; for ( const auto& d : lambda.ds ) {
    //     //            os << d << " ";
    //     //        }
    //     //        os << "]";
    //     return os;
    // }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
    //    Lambda() = default;
};

struct UserClass {
    bool a;
    bool b;
    auto toString() const { return std::to_string( a ) + " " + std::to_string( b ); }
    // static constexpr auto name() { return "UserClass"; }
};

// #include "core/Macros.hpp"

// class Test
//{
//   public:
//       void printHeader() {
//			std::cout << "HEADER: " << HEADER << std::endl;
//     }
// };
static hub::io::Archive s_archive;

template <class T, hub::Size_t... Ns>
void checkMatrix( const T& t ) {

    auto matrix = hub::make_matrix<T, Ns...>();
    CHECK( matrix.nType() == 1 );
    // CHECK( matrix.size() == sizeof( T ) * ( Ns * ... ) );
    CHECK( matrix.size() == hub::sizeOf<T>() * ( Ns * ... ) );
    CHECK( std::is_same_v<T, int> == matrix.template hasType<int>() );
    CHECK( std::is_same_v<T, double> == matrix.template hasType<double>() );
    CHECK( std::is_same_v<T, bool> == matrix.template hasType<bool>() );
    CHECK( std::is_same_v<T, char> == matrix.template hasType<char>() );
    CHECK( !matrix.hasValue() );

    assert( s_archive.isEnd() );
    s_archive.write( matrix );
    hub::Matrix matrix_read;
    s_archive.read( matrix_read );
    assert( matrix == matrix_read );
    assert( s_archive.isEnd() );
    std::cout << "<" << TYPE_NAME( T() ) << ">: " << matrix_read << std::endl;

    matrix.init();
    auto& data = matrix.template get<T&>();
    data       = t;
    CHECK( matrix.hasValue() );

    assert( s_archive.isEnd() );
    s_archive.write( matrix );
    s_archive.read( matrix_read );
    assert( matrix == matrix_read );
    assert( s_archive.isEnd() );
    std::cout << "<" << TYPE_NAME( T() ) << ">: " << matrix_read << std::endl;
}

template <class... Ts>
REQUIRES(, ! hub::areMatrices<Ts...>, void)
checkMatrix(const Ts&... ts) {
    auto matrix = hub::make_matrix<Ts...>();
    // std::cout << "<" << hub::typeName<Ts...>() << ">: " << matrix << std::endl;

    // CHECK( matrix.nType() == sizeof...(Ts) );
    CHECK( matrix.nType() == sizeof...( Ts ) );
    // CHECK( matrix.size() == (sizeof( Ts ) + ...));
    CHECK( matrix.size() == hub::sizeOf<Ts...>() );
    CHECK( !matrix.hasValue() );

    assert( s_archive.isEnd() );
    s_archive.write( matrix );
    hub::Matrix matrix_read;
    s_archive.read( matrix_read );
    assert( matrix == matrix_read );
    assert( s_archive.isEnd() );
    std::cout << "<" << hub::typeName<Ts...>() << ">: " << matrix_read << std::endl;

    matrix.init();
    // auto& data = matrix.template get<T&>();
    // data       = t;
    matrix.fill(ts...);
    CHECK( matrix.hasValue() );

    assert( s_archive.isEnd() );
    s_archive.write( matrix );
    s_archive.read( matrix_read );
    assert( matrix == matrix_read );
    assert( s_archive.isEnd() );
    std::cout << "<" << hub::typeName<Ts...>() << ">: " << matrix_read << std::endl;
}

template <class... Matrices>
REQUIRES(, hub::areMatrices<Matrices...>, void)
checkMatrix( const Matrices&... matrices ) {
    auto matrix = hub::make_matrix( matrices... );
    std::cout << "<" << hub::typeName<Matrices...>() << ">: " << matrix << std::endl;

    CHECK( matrix.nType() == sizeof...( Matrices ) );
    // CHECK( matrix.size() == (sizeof( Matrices ) + ...));
    CHECK( matrix.size() == hub::sizeOf( matrices... ) );
    // CHECK( !matrix.hasValue() );

    assert( s_archive.isEnd() );
    s_archive.write( matrix );
    hub::Matrix matrix_read;
    s_archive.read( matrix_read );
    assert( matrix == matrix_read );
    assert( s_archive.isEnd() );

    std::cout << "<" << hub::typeName<Matrices...>() << ">: " << matrix_read << std::endl;
}

TEST_CASE( "Matrix test" ) {
    TEST_BEGIN()

    // hub::io::Archive archive;
    // checkMatrix<int, 640, 480>( 5 );
    checkMatrix<int, double, bool>(5, 2.0, true);
    // return;
    checkMatrix<std::vector<int>, 1>( { 1, 2 } );
    checkMatrix<UserClass, 10>( UserClass { true, false } );

    auto matrix  = hub::make_matrix<int, 3>();
    auto matrix2 = hub::make_matrix<double, 2>();
    checkMatrix( matrix, matrix2 );

    matrix.init();
    matrix.get<int&>() = 1;
    matrix.get<int*>()[1] = 1;
    matrix.get<int*>()[2] = 1;
    // matrix.get<int*>()[3] = 1;
    // matrix.get<int*>()[4] = 1;
    matrix2.init();
    matrix2.get<double&>() = 1.0;
    matrix2.get<double*>()[1] = 1.0;
    // matrix.get<int&, 1>() = 2;
    // matrix.get<int&, 2>() = 3;
    // matrix.get<int&, 3>() = 4;
    checkMatrix( matrix, matrix2 );
    // hub::make_matrix<int>(5, 2);
    // hub::make_matrix(5, 2);

    // CHECK( matrix.nType() == 3 );
    // CHECK( matrix.size() == sizeof( int ) + sizeof( double ) + sizeof( bool ) );
    // CHECK( matrix.hasType<int>() );
    // CHECK( matrix.hasType<double>() );
    // CHECK( matrix.hasType<bool>() );
    // CHECK( !matrix.hasType<char>() );
    // CHECK( !matrix.hasValue() );

    // static_assert( !hub::packable_v<hub::Matrix> );
    // static_assert( hub::isPacket<hub::Matrix> );
    // archive.write( matrix );
    // hub::Matrix matrix_read;

    // auto matrix2 = hub::make_matrix<int>( 640, 480 );
    // CHECK( matrix2.nType() == 1 );
    // CHECK( matrix2.size() == sizeof( int ) * 640 * 480 );
    // CHECK( matrix2.hasType<int>() );
    // CHECK( !matrix2.hasType<double>() );
    // CHECK( !matrix2.hasType<bool>() );
    // CHECK( !matrix2.hasType<char>() );
    // CHECK( !matrix2.hasValue() );

    // auto matrix3 = hub::make_matrix<UserClass>( 640, 480 );
    // CHECK( matrix3.nType() == 1 );
    // CHECK( matrix3.size() == sizeof( UserClass ) * 640 * 480 );
    // CHECK( !matrix3.hasType<int>() );
    // CHECK( matrix3.hasType<UserClass>() );
    // CHECK( !matrix3.hasType<double>() );
    // CHECK( !matrix3.hasType<bool>() );
    // CHECK( !matrix3.hasType<char>() );
    // CHECK( !matrix3.hasValue() );
    // // matrix3.init();
    // auto & userClass = matrix3.get<UserClass&>();

    //    // check compat with 32/64 bits operating systems

    //    Test myTest;
    //      myTest.printHeader();

    //    std::cout << "FILE_NAME: " << FILE_NAME << std::endl;
    //    std::cout << "FILE_NAME_WITHOUT_EXTENSIONS: " << FILE_NAME_WITHOUT_EXTENSION << std::endl;
    // return 0;
    //

    //    serial2.get<int&>() = 5;
    //    std::cout << "matrixUser: " << matrixUser << std::endl;

    ////////////////////////////////

    //    hub::io::Archive archive;
    //    archive.write(matrices3);
    TEST_END()
}
