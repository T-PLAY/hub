

#include "test_common.hpp"

#include <tuple>

#include <core/Any.hpp>
#include <core/Matrix.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>

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
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

struct UserClass {
    bool a;
    bool b;
    auto toString() const { return std::to_string( a ) + " " + std::to_string( b ); }
};

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
REQUIRES(, !hub::areMatrices<Ts...>, void )
checkMatrix( const Ts&... ts ) {
    auto matrix = hub::make_matrix<Ts...>();

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
    matrix.fill( ts... );
    CHECK( matrix.hasValue() );

    assert( s_archive.isEnd() );
    s_archive.write( matrix );
    s_archive.read( matrix_read );
    assert( matrix == matrix_read );
    assert( s_archive.isEnd() );
    std::cout << "<" << hub::typeName<Ts...>() << ">: " << matrix_read << std::endl;
}

template <class... Matrices>
REQUIRES(, hub::areMatrices<Matrices...>, void )
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

    checkMatrix<int, double, bool>( 5, 2.0, true );
    checkMatrix<std::vector<int>, 1>( { 1, 2 } );
    checkMatrix<UserClass, 10>( UserClass { true, false } );

    auto matrix  = hub::make_matrix<int, 3>();
    auto matrix2 = hub::make_matrix<double, 2>();
    checkMatrix( matrix, matrix2 );

    matrix.init();
    matrix.get<int&>()    = 1;
    matrix.get<int*>()[1] = 1;
    matrix.get<int*>()[2] = 1;
    matrix2.init();
    matrix2.get<double&>()    = 1.0;
    matrix2.get<double*>()[1] = 1.0;
    checkMatrix( matrix, matrix2 );

    TEST_END()
}
