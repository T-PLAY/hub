
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Macros.hpp>
//#include <core/Vector.hpp>
#include <core/Traits.hpp>


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



TEST_CASE( "Macros test" ) {

    static_assert(hub::sizeof_<int>() == 4);
    static_assert(hub::sizeof_<double>() == 8);
    static_assert(hub::sizeof_<int, int>() == 8);

}
