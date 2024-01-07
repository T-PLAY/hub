
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
// #include <catch2/catch_test_macros.hpp>
// #include <iostream>

#include <core/Any.hpp>
#include <core/Traits.hpp>
// #include <core/io/InputOutput.hpp>
#include <core/io/Archive.hpp>
#include <map>

// #ifndef HUB_NON_BUILD_SENSOR
// #include <sensor/Format.hpp>
// #endif

// #include <data/Mat4.hpp>
#include <core/Format.hpp>

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

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " << lambda.e;
        return os;
    }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

template <class T>
void checkAny( const T& t, const T& t2 ) {
#ifdef HUB_DEBUG_OUTPUT
    std::cout << "------------------ Any<" << TYPE_NAME( T() ) << "> ---------------------\n";
#endif
    const hub::Any any = t;
    CHECK(any.hasValue());
    CHECK(any.get<T>() == t);
    std::cout << "any: " << any << std::endl;
    // std::cout << any.valueAsString() << std::endl;

    hub::Any any_compare;
    CHECK(! any_compare.hasValue());
    CHECK( any != any_compare );

    any_compare = t2;
    CHECK(any_compare.hasValue());
    CHECK(any_compare.get<T>() == t2);
    CHECK( any != any_compare );

    any_compare = t;
    CHECK(any_compare.get<T>() == t);
    CHECK( any == any_compare );

    // std::cout << "any: " << any << std::endl;
    //    return;
    // std::cout << any.type().name() << std::endl;
    // assert( any.hasValue() );
    //    std::cout << any.to_string<int>() << std::endl;
    //    CHECK( any.typeId() == typeid(int));
    // if constexpr ( !std::is_same_v<T, int> ) { CHECK( !any.is<int>() ); }
    assert((std::is_same_v<T, int> == any.is<int>()));
    assert((std::is_same_v<T, bool> == any.is<bool>()));
    assert((std::is_same_v<T, double> == any.is<double>()));
    assert((std::is_same_v<T, const char*> == any.is<const char*>()));
    assert((std::is_same_v<T, std::string> == any.is<std::string>()));
    assert((std::is_same_v<T, Lambda> == any.is<Lambda>()));

    // const T t3 = any.get<T>();
    //    int a2 = any.get();
    // CHECK( t3 == t );

#ifdef USE_ARCHIVE
    archive.write( any );
    archive.read( any_compare );
    CHECK( any == any_compare );
#endif
    std::cout << std::endl;
}

// #define CHECK assert

#include <core/Serializer.hpp>

TEST_CASE( "Any test" ) {
    TEST_BEGIN()

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    //    hub::Anyable::registerTypes<Lambda>();
    //    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    checkAny<int>(1, 2);

    return ;

#ifdef USE_ARCHIVE
    hub::io::Archive archive;
#endif
    // std::any std_any;
    // assert(! std_any.has_value());

    hub::Any any;
    // assert(any.type() == typeid(void));
    assert( !any.hasValue() );
    hub::Any any_read;
    assert( !any_read.hasValue() );


#if CPP_VERSION >= 20
    static_assert( hub::Serializer::Readable_v<hub::Any> );
    static_assert( hub::Serializer::Writable_v<hub::Any> );
    static_assert( !hub::Serializer::Serializable<hub::Any>() );
#endif

    //     // void
    //     {
    // #ifdef HUB_DEBUG_OUTPUT
    //         std::cout << "\n------------------ " << TYPE_NAME( void ) << "
    //         ---------------------\n";
    // #endif
    //         archive.write( any );
    //         archive.read( any_read );
    //         assert(! any_read.hasValue());
    //         CHECK( any == any_read );
    //     }

    return;

    // int
    {
        any = 1;
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( int() ) << " ---------------------\n";
#endif
        any_read = 1;
        CHECK( any == any_read );
        any_read = 2;
        CHECK( any != any_read );
        std::cout << any << std::endl;
        //    return;
        // std::cout << any.type().name() << std::endl;
        std::cout << any.valueAsString() << std::endl;
        assert( any.hasValue() );
        //    std::cout << any.to_string<int>() << std::endl;
        CHECK( any.is<int>() );
        //    CHECK( any.typeId() == typeid(int));
        CHECK( !any.is<bool>() );
        CHECK( !any.is<double>() );
        CHECK( !any.is<const char*>() );
        CHECK( !any.is<std::string>() );
        CHECK( !any.is<Lambda>() );
        int a = any.get<int>();
        //    int a2 = any.get();
        CHECK( a == 1 );

#ifdef USE_ARCHIVE
        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
#endif
    }

    // double
    {
        any = 1.4;
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( double() ) << " ---------------------\n";
#endif
        any_read = 1.4;
        CHECK( any == any_read );
        any_read = 2.1;
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<double>() );
        CHECK( !any.is<int>() );
        double b = any.get<double>();
        //    auto b2 = any.get<int>();
        CHECK( b == 1.4 );

#ifdef USE_ARCHIVE
        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
#endif
    }

    // std::string
    {
        any = std::string( "hello" );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( std::string() )
                  << " ---------------------\n";
#endif
        any_read = std::string( "hello" );
        CHECK( any == any_read );
        any_read = std::string( "bye" );
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<std::string>() );
        std::string c = any.get<std::string>();
        CHECK( c == "hello" );

#ifdef USE_ARCHIVE
        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
#endif
    }

    // const char *
    {
        const char* str = "hello";
#ifdef HUB_DEBUG_OUTPUT
        // std::cout << "\n------------------ " << TYPE_NAME( const char * ) << "
        // ---------------------\n";
        std::cout << "\n------------------ " << TYPE_NAME( str ) << " ---------------------\n";
#endif
        any      = str;
        any_read = str;
        CHECK( any == any_read );
        any_read = "bye";
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<const char*>() );
        const char* d = any.get<const char*>();
        CHECK( !strcmp( d, "hello" ) );

#ifdef USE_ARCHIVE
        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
#endif
    }

    //    any = hub::format::Mat4();
    //    std::cout << any << std::endl;
    //    const auto& i = any.get<hub::format::Mat4>();
    //    CHECK( i == hub::format::Mat4() );

    {
        const Lambda lambda { 5, 0.0, 1.0f, { 0, 1, 2 }, "gauthier" };
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( lambda ) << " ---------------------\n";
#endif
        //    archive.write(lambda);
        //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
        any      = lambda;
        any_read = lambda;
        CHECK( any == any_read );
        any_read = Lambda { 2, 0.0, 1.0f, { 0 }, "bouyjou" };
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<Lambda>() );
        CHECK( any.get<Lambda>() == lambda );

#ifdef USE_ARCHIVE
        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
#endif
    }

#ifndef HUB_NON_BUILD_SENSOR
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

    {
        using Format = hub::format::Mat4;
        static_assert( hub::packable_v<Format> );
        // static_assert(! hub::isPacket<Format>);
        // static_assert(! hub::serializable_v<Format>);
        static_assert( !hub::serializer::SerializerZppBits::Serializable<Format>() );
        const Format mat4( 1.0f );
#    ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( lambda ) << " ---------------------\n";
#    endif
        //    archive.write(lambda);
        //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
        any      = mat4;
        any_read = mat4;
        CHECK( any == any_read );
        any_read = Format( 2.0f );
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<Format>() );
        CHECK( any.get<Format>() == mat4 );

        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
    }
#endif

    //    std::cout << "any type: " << any.type() << std::endl;
    //    std::cout << "any value: " << any.getValueAsString() << std::endl;
    //    std::cout << "any typeid: " << any.typeId().name() << std::endl;
    //    std::cout << any << std::endl;

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
    TEST_END()
}
