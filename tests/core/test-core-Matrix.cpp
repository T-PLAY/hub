
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

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        //        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " <<
        //        lambda.e; for ( const auto& d : lambda.ds ) {
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

struct UserClass {
    bool a;
    bool b;
    auto toString() const { return std::to_string( a ) + " " + std::to_string( b ); }
    static constexpr auto name() { return "UserClass"; }
};

// #include "core/Macros.hpp"

// class Test
//{
//   public:
//       void printHeader() {
//			std::cout << "HEADER: " << HEADER << std::endl;
//     }
// };

TEST_CASE( "Matrix test" ) {
    TEST_BEGIN()

    //     std::cout << typeid(int).hash_code() << std::endl;

    //    // check compat with 32/64 bits operating systems
    //    assert(typeid(int).hash_code() == 6253375586064260614ul);
    //    assert(typeid(double).hash_code() == 14494284460613645429ul);
    //    assert(typeid(UserClass).hash_code() == 2193770348167473685ul);
    //    assert(typeid(long long).hash_code() == 13272411544345499535ul);

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
