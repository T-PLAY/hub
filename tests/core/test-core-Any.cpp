
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
// #include <catch2/catch_test_macros.hpp>
// #include <iostream>

#include <core/Any.hpp>
#include <core/io/Archive.hpp>
#include <core/Format.hpp>
#include <core/Serializer.hpp>

// #ifndef HUB_NON_BUILD_IO
// #include <io/output/OutputFile.hpp>
// #include <io/input/InputFile.hpp>
// #endif

// #ifndef HUB_NON_BUILD_SENSOR
// #include <sensor/Format.hpp>
// #endif

// #include <data/Mat4.hpp>

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

    // template <class Serial>
    // void serialize( Serial& serial ) {
    //     serial( a, b, c, ds, e );
    // }
    // friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.a, self.b, self.c, self.ds, self.e );
    }

    auto toString() const { return hub::to_string( a, b, c, ds, e ); }

    // void write( hub::Serializer& serializer ) const {
    //     std::cout << "[test][WritableReadableData] write(Serializer&) : " << *this << std::endl;
    //     serializer.writeAll( a, b, c, ds, e );
    // }
    // void read( hub::Serializer& serializer ) {
    //     serializer.readAll( a, b, c, ds, e );
    //     std::cout << "[test][WritableReadableData] read(Serializer&) : " << *this << std::endl;
    // }

    // friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
    //     os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " <<
    //     lambda.e; return os;
    // }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

#ifdef USE_ARCHIVE
static hub::io::Archive s_archive;
#endif

template <class T>
void checkAny( const T& t, const T& t2 ) {
// #ifdef HUB_DEBUG_OUTPUT
    // std::cout << "------------------ Any<" << TYPE_NAME( T() ) << "> ---------------------\n";
// #endif
    const hub::Any any = t;
    CHECK( any.hasValue() );
    CHECK( any.get<T>() == t );
    CHECK( any.typeName() == TYPE_NAME( t ) );
    std::cout << "<" << TYPE_NAME(t) << ">: " << any << std::endl;
    // std::cout << any.valueAsString() << std::endl;

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
    // std::cout << "any2: " << any_compare << std::endl;

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

    // std::cout << "any: " << any << std::endl;
    //    return;
    // std::cout << any.type().name() << std::endl;
    // assert( any.hasValue() );
    //    std::cout << any.to_string<int>() << std::endl;
    // if constexpr ( !std::is_same_v<T, int> ) { CHECK( !any.is<int>() ); }
    assert( ( std::is_same_v<T, int> == any.is<int>() ) );
    assert( ( std::is_same_v<T, bool> == any.is<bool>() ) );
    assert( ( std::is_same_v<T, double> == any.is<double>() ) );
    assert( ( std::is_same_v<T, const char*> == any.is<const char*>() ) );
    assert( ( std::is_same_v<T, std::string> == any.is<std::string>() ) );
    assert( ( std::is_same_v<T, Lambda> == any.is<Lambda>() ) );

    // const T t3 = any.get<T>();
    //    int a2 = any.get();
    // CHECK( t3 == t );

#ifdef USE_ARCHIVE
    assert( s_archive.isEnd() );
    s_archive.write( any );
    s_archive.read( any_compare );
    CHECK( any == any_compare );
    assert( s_archive.isEnd() );
#endif
    // std::cout << std::endl;
}

struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

// #include <core/Serializer.hpp>

TEST_CASE( "Any test" ) {
    TEST_BEGIN()

// #ifndef HUB_NON_BUILD_IO
// #endif
    using namespace hub::format;
    // hub::Anyable::registerTypes<Mat4>();
    // hub::Any any_wip;
    // any_wip = hub::format::Mat4{1.0};
    // return;

// #if CPP_VERSION >= 20
    static_assert( hub::Serializer::Readable_v<hub::Any> );
    static_assert( hub::Serializer::Writable_v<hub::Any> );
    static_assert( !hub::Serializer::Serializable<hub::Any>() );
    static_assert( !hub::packable_v<hub::Any> );
// #endif

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    //    hub::Anyable::registerTypes<Lambda>();
    //    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    // #ifdef USE_ARCHIVE
    //     hub::io::Archive archive;
    // #endif
    // std::any std_any;
    // assert(! std_any.has_value());

    // void
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
    static_assert(hub::packable_v<hub::format::Mat4>);
    static_assert(! hub::Serializer::Serializable<hub::format::Mat4>());
    static_assert(! hub::Serializer::Writable_v<hub::format::Mat4>);
    static_assert(! hub::Serializer::Readable_v<hub::format::Mat4>);
    checkAny<hub::format::Mat4>(hub::format::Mat4(1.0), hub::format::Mat4(2.0));

// #ifndef HUB_NON_BUILD_SENSOR
    //     {
    //         using Format = hub::format::BGR8;
    //         const Format bgr8(0.0f);
    // #ifdef HUB_DEBUG_OUTPUT
    //         std::cout << "\n------------------ " << TYPE_NAME( lambda ) << "
    //         ---------------------\n";
    // #endif
    //         //    archive.write(lambda);
    //         //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
    //         any      = bgr8;
    //         any_read = bgr8;
    //         CHECK( any == any_read );
    //         any_read = Format(0.0f);
    //         CHECK( any != any_read );
    //         std::cout << any << std::endl;
    //         CHECK( any.is<Format>() );
    //         // CHECK( any.get<Format>() == bgr8 );

    //         archive.write( any );
    //         archive.read( any_read );
    //         CHECK( any == any_read );
    //     }

//     {
//         using Format = hub::format::Mat4;
//         static_assert( hub::packable_v<Format> );
//         // static_assert(! hub::isPacket<Format>);
//         // static_assert(! hub::serializable_v<Format>);
//         // static_assert( !hub::serializer::SerializerZppBits::Serializable<Format>() );
//         const Format mat4( 1.0f );
// #    ifdef HUB_DEBUG_OUTPUT
//         std::cout << "\n------------------ " << TYPE_NAME( lambda ) << " ---------------------\n";
// #    endif
//         //    archive.write(lambda);
//         //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
//         any      = mat4;
//         any_read = mat4;
//         CHECK( any == any_read );
//         any_read = Format( 2.0f );
//         CHECK( any != any_read );
//         std::cout << any << std::endl;
//         CHECK( any.is<Format>() );
//         CHECK( any.get<Format>() == mat4 );

//         archive.write( any );
//         archive.read( any_read );
//         CHECK( any == any_read );
    // }
// #endif

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
    TEST_END()
}
