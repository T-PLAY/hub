
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <tuple>

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

    const auto matrix = Matrix<10, int>();
    static_assert(matrix.size() == 10 * sizeof(int));
    const auto matrix2 = Matrix<int, double>();
    using R = unsigned char;
    using G = unsigned char;
    using B = unsigned char;
    const auto matrix3 = Matrix<640, 480, R, G, B>();
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
//    using MyMatrixResolution = MatrixTs<

    using MatrixInt = MatrixXD<int, 2>;
    static_assert(MatrixInt::Size == 8);
//    std::array<int, 2> matrixIntRawData{1, 2};
//    auto matrixInt_ref = MatrixInt{1, 2};
    using MatrixFloat = MatrixXD<float, 2>;
    static_assert(MatrixFloat::Size == 8);
    using MatrixDouble = MatrixXD<double, 2>;
    static_assert(MatrixDouble::Size == 16);
    using Matrices = MatrixTs<double, bool, float, double, double, MatrixInt, MatrixFloat, MatrixDouble, double>;
    static_assert(Matrices::Size == 69);
    static_assert(Matrices::nType<double>() == 5);
    static_assert(Matrices::nType<bool>() == 1);
    static_assert(Matrices::nType<float>() == 2);


//    std::tuple<MatrixInt, MatrixFloat, MatrixDouble, double, bool> tuple;

    Matrices matrices;
    std::cout << "Matrices: " << matrices << std::endl;
    static_assert(! matrices.hasType<MatrixInt, double, bool>());
    static_assert(matrices.hasType<int, double, bool>());
    static_assert(matrices.hasType<int>());
    static_assert(matrices.getOffset<double>() == 0);
//    matrices.getOffset<char>();
    static_assert(matrices.getOffset<bool>() == 8);
    static_assert(matrices.getOffset<float>() == 9);
    static_assert(matrices.getOffset<double, 1>() == 13);
    static_assert(matrices.getOffset<double, 2>() == 21);
    static_assert(matrices.getOffset<int>() == 29);
    static_assert(matrices.getOffset<float, 1>() == 37);
    static_assert(matrices.getOffset<double, 3>() == 45);
    static_assert(matrices.getOffset<double, 4>() == 61);
//    static_assert(matrices.getOffset<double, 5>() == 69);
//    static_assert(matrices.getOffset<double, 6>() == 69);
//    static_assert(matrices.getOffset<double, 5>() == 69);
//    static_assert(matrices.has<int>());

//    double doubleValue_get = matrices.get<double>();
//    doubleValue_get = 2.0;
//    std::cout << "Matrices: " << matrices << std::endl;

    double & doubleRef_get = matrices.get<double&, 0>();
    doubleRef_get = 1.0;
    std::cout << "Matrices: " << matrices << std::endl;

    bool * doublePtr_get = matrices.get<bool*>();
    *doublePtr_get = true;

    matrices.get<float&>() = 2.0f;
    matrices.get<double&, 1>() = 3.0f;
    matrices.get<double&, 2>() = 4.0f;
    matrices.get<int&>() = 5;
    matrices.get<float&, 1>() = 6.0f;
    matrices.get<double&, 3>() = 7.0f;
    matrices.get<double&, 4>() = 8.0f;

    std::cout << "Matrices: " << matrices << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    using Matrices2 = MatrixTs<char, char, char, char, char, char, char, char>;
//    Matrices2 matrices2{'g', 'a', 'u', 't', 'h', 'i', 'e', 'r'};

}
