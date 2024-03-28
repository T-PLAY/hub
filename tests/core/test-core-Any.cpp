

#include "test_common.hpp"

#include <set>

#include <core/Any.hpp>
#include <core/Format.hpp>
#include <core/Serializer.hpp>
#include <core/io/Archive.hpp>

#define USE_ARCHIVE

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

#if CPP_VERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.a, self.b, self.c, self.ds, self.e );
    }
#endif

    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.a, self.b, self.c, self.ds, self.e );
    }

    auto toString() const {
        return hub::to_string( a, b, c, ds, e );
    }

    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

#ifdef USE_ARCHIVE
static hub::io::Archive s_archive;
#endif

template <class T>
void checkAny( const T& t, const T& t2 ) {
    const hub::Any any = t;
    CHECK( any.hasValue() );
    CHECK( any.get<T>() == t );
    CHECK( any.typeName() == TYPE_NAME( t ) );
    std::cout << "<" << TYPE_NAME( t ) << ">: " << any << std::endl;

    hub::Any any_compare;
    CHECK( !any_compare.hasValue() );
    CHECK( any != any_compare );

    any_compare = t;
    CHECK( any_compare.get<T>() == t );
    CHECK( any == any_compare );

    any_compare = t2;
    CHECK( any_compare.hasValue() );
    CHECK( any_compare.get<T>() == t2 );
    CHECK( any != any_compare );

    hub::Any any_clone { any };
    assert( any_clone == any );
    assert( any_clone != any_compare );
    const hub::Any any2_clone { any_compare };
    assert( any2_clone == any_compare );
    assert( any2_clone != any );

    hub::Any any_copy;
    any_copy = any;
    assert( any_copy == any );
    assert( any_copy != any_compare );
    hub::Any any2_copy;
    any2_copy = any_compare;
    assert( any2_copy == any_compare );
    assert( any2_copy != any );

    hub::Any any_moved = std::move( any_copy );
    assert( any_moved == any );
    assert( any_moved != any_compare );
    hub::Any any2_moved = std::move( any2_copy );
    assert( any2_moved == any_compare );
    assert( any2_moved != any );

    assert( ( std::is_same_v<T, int> == any.is<int>() ) );
    assert( ( std::is_same_v<T, bool> == any.is<bool>() ) );
    assert( ( std::is_same_v<T, double> == any.is<double>() ) );
    assert( ( std::is_same_v<T, const char*> == any.is<const char*>() ) );
    assert( ( std::is_same_v<T, std::string> == any.is<std::string>() ) );
    assert( ( std::is_same_v<T, Lambda> == any.is<Lambda>() ) );

    // CHECK( t3 == t );

#ifdef USE_ARCHIVE
    assert( s_archive.isEnd() );
    s_archive.write( any );
    s_archive.read( any_compare );
    CHECK( any == any_compare );
    assert( s_archive.isEnd() );
#endif
}

struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

TEST_CASE( "Any test" ) {
    TEST_BEGIN()

    using namespace hub::format;

    static_assert( hub::Serializer::Readable_v<hub::Any> );
    static_assert( hub::Serializer::Writable_v<hub::Any> );
    static_assert( !hub::Serializer::Serializable<hub::Any>() );
    static_assert( !hub::packable_v<hub::Any> );

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    const hub::Any any;
    assert( any.typeName() == "void" );
    assert( !any.hasValue() );
    std::cout << "any: " << any << std::endl;
    hub::Any any2;
    assert( !any2.hasValue() );
    assert( any == any2 );
    any2 = 1;
    assert( any2.hasValue() );
    std::cout << "any2: " << any2 << std::endl;

    checkAny<int>( 1, 2 );
    checkAny<double>( 1.0, 2.0 );
    checkAny<std::string>( "gauthier", "bouyjou" );
    checkAny<const char*>( "abc", "def" );
    checkAny<Lambda>( Lambda { 1, 2.0f, 3.0, { 1, 2 }, "hello" },
                      Lambda { 2, 3.0f, 4.0, { 3, 4 }, "hi" } );
    static_assert( hub::packable_v<hub::format::Mat4> );
    static_assert( !hub::Serializer::Serializable<hub::format::Mat4>() );
    static_assert( !hub::Serializer::Writable_v<hub::format::Mat4> );
    static_assert( !hub::Serializer::Readable_v<hub::format::Mat4> );
    checkAny<hub::format::Mat4>( hub::format::Mat4( 1.0 ), hub::format::Mat4( 2.0 ) );
    checkAny<std::set<std::string>>( { "hello", "gauthier" }, { "hello" } );
    checkAny<hub::format::Vec3>( hub::format::Vec3 { 0.0, 0.0, 0.0 }, hub::format::Vec3( 1.0f ) );
    checkAny<hub::format::Vec4>( hub::format::Vec4( 1.0 ), hub::format::Vec4( 2.0f ) );

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
    TEST_END()
}
