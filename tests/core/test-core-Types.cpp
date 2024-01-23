
#include "test_common.hpp"

#include <core/Types.h>

namespace hub {
namespace Types {
enum User : hub::TypeId_t {
    User0 = hub::Types::Count,
    User1,
};
} // namespace Types
} // namespace hub

struct User0 {
    static constexpr auto id = hub::Types::User::User0;
};

struct User1 {
    static constexpr auto id = hub::Types::User::User1;
};

TEST_CASE( "TypeId test" ) {
    TEST_BEGIN()

    constexpr auto typeId = TYPE_ID( int );
    static_assert( typeId == hub::Types::INT );

    static_assert( TYPE_ID( int ) == hub::Types::INT );
    static_assert( TYPE_ID( bool ) == hub::Types::BOOL );
    static_assert( TYPE_ID( hub::format::Mat4 ) == hub::Types::Cpp_Count );

    static_assert( TYPE_ID( hub::format::Mat4 ) == hub::Types::MAT4 );
    static_assert( TYPE_ID( hub::format::Density ) == hub::Types::DENSITY );
    static_assert( TYPE_ID( hub::format::Distance ) == hub::Types::DISTANCE );
    static_assert( TYPE_ID( hub::format::RGB8 ) == hub::Types::RGB8 );
    static_assert( TYPE_ID( hub::format::BGR8 ) == hub::Types::BGR8 );
    static_assert( TYPE_ID( hub::format::Y8 ) == hub::Types::Y8 );
    static_assert( TYPE_ID( hub::format::Y16 ) == hub::Types::Y16 );
    static_assert( TYPE_ID( hub::format::Z16 ) == hub::Types::Z16 );
    static_assert( TYPE_ID( hub::format::Position ) == hub::Types::POSITION );
    static_assert( TYPE_ID( hub::format::Orientation ) == hub::Types::ORIENTATION );
    static_assert( TYPE_ID( hub::format::Dof6 ) == hub::Types::DOF6 );
    static_assert( TYPE_ID( hub::format::XYZ32F ) == hub::Types::XYZ32F );
    static_assert( TYPE_ID( hub::format::Vec3 ) == hub::Types::VEC3 );
    static_assert( TYPE_ID( hub::format::Vec4 ) == hub::Types::VEC4 );
    // static_assert( (hub::TypeId_t)(hub::Types::Format_Count + 1) == hub::Types::Count );

    static_assert( TYPE_ID( User0 ) == hub::Types::User0 );
    static_assert( TYPE_ID( User1 ) == hub::Types::User1 );

    TEST_END()
}
