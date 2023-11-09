
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Traits.hpp>
#include <core/Any.hpp>
#include <core/io/Archive.hpp>
#include <core/Matrix.hpp>


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

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
//        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " << lambda.e;
//        for ( const auto& d : lambda.ds ) {
//            os << d << " ";
//        }
//        os << "]";
        return os;
    }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
    //    Lambda() = default;
};



 TEST_CASE( "Matrix test" ) {

    using namespace hub;

    using MyMatrixTs = MatrixTs<int, double, float>;
    static_assert(MyMatrixTs::capacity() == 1);
    static_assert(MyMatrixTs::nDim() == 1);
    static_assert(MyMatrixTs::getDim<0>() == 1);
    static_assert(MyMatrixTs::nType() == 3);
    static_assert(MyMatrixTs::hasType<int>());
    static_assert(MyMatrixTs::hasType<double>());
    static_assert(MyMatrixTs::hasType<float>());
    static_assert(! MyMatrixTs::hasType<bool>());
    static_assert(MyMatrixTs::hasType<int, double, float>());
    static_assert(! MyMatrixTs::hasType<int, double, float, char>());
    static_assert(std::is_same<MyMatrixTs::getType<0>, int>());
    static_assert(std::is_same<MyMatrixTs::getType<1>, double>());
    static_assert(std::is_same<MyMatrixTs::getType<2>, float>());
    static_assert(MyMatrixTs::size() == sizeof(int) + sizeof(double) + sizeof(float));

    std::cout << "MyMatrixTs: " << MyMatrixTs() << std::endl;


    using MyMatrixXD = MatrixXD<int, 640, 480>;
    static_assert(MyMatrixXD::capacity() == 640 * 480);
    static_assert(MyMatrixXD::nDim() == 2);
    static_assert(MyMatrixXD::getDim<0>() == 640);
    static_assert(MyMatrixXD::getDim<1>() == 480);
    static_assert(MyMatrixXD::nType() == 1);
    static_assert(MyMatrixXD::hasType<int>());
    static_assert(MyMatrixXD::hasType<int, int>());
    static_assert(! MyMatrixXD::hasType<float>());
    static_assert(! MyMatrixXD::hasType<bool>());
    static_assert(std::is_same<MyMatrixXD::getType<0>, int>());
    static_assert(MyMatrixXD::size() == sizeof(int) * 640 * 480);

    std::cout << "MyMatrixXD: " << MyMatrixXD() << std::endl;

//    MatrixXD<int>;
//    MatrixXD<int, 1>;
    static_assert(MatrixXD<int, 2>::Capacity == 2);

//    MatrixTs<int>;
    static_assert(MatrixTs<int, double>::nType() == 2);

    constexpr auto matrix = Matrix<10, int>();
    static_assert(matrix.size() == 10 * sizeof(int));
    constexpr auto matrix2 = Matrix<int, double>();
    using R = unsigned char;
    using G = unsigned char;
    using B = unsigned char;
    constexpr auto matrix3 = Matrix<640, 480, R, G, B>();
    static_assert(matrix3.width() == 640);
    static_assert(matrix3.size() == 640 * 480 * 3);
    auto matrix4 = Matrix<2, MAX_STACK_SIZE, unsigned char>();
    auto matrix5 = Matrix<MatrixXD<int, 2>, double, float>();
    static_assert(matrix5.size() == sizeof(int) * 2 + sizeof(double) + sizeof(float));
//    using MyMatrix5 = MatrixXD<unsigned char, 2, MAX_STACK_SIZE>;
//    static constexpr auto matrix5 = MyMatrix5();
//    constexpr auto matrix5 = MyMatrix5();
//    std::cout << "matrix5: " << matrix5 << std::endl;
//    static_assert(matrix4.nx() == 640);
//    constexpr auto matrix5 = Matrix<640, 480, 3, 2, int, bool>();
//    static_assert(matrix5.nt() == 2);
//    constexpr auto matrix6 = Matrix<640, 480, 3, 2, 2, int, bool>();
//    static_assert(matrix6.n<4>() == 2);

    std::vector<int> vints { 1, 2, 3};
    std::vector<char> vchar { 'a', 'b', 'c'};
    std::array<int, 5> array { 1, 2, 3, 4, 5};
    std::span span {array};
    std::string string = "gauthier";


//    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
//    hub::io::Archive archive;
//    std::any std_any;
//    assert(! std_any.has_value());
//    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
}
