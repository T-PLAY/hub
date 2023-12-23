
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

// #include <core/io/Memory.hpp>
#include <core/MetaData.hpp>
#include <core/io/Archive.hpp>

#ifndef HUB_NON_BUILD_SENSOR
#    include <sensor/Format.hpp>
#endif

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.a, self.b, self.c, self.ds, self.e );
    }

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " << lambda.e;
        return os;
    }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
};

// #include <core/Serializer.hpp>

TEST_CASE( "MetaData test" ) {
    TEST_BEGIN()

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    hub::io::Archive archive;
    // std::any std_any;
    // assert( !std_any.has_value() );

    hub::MetaData metadata;
    // assert( metadata.type() == typeid( void ) );
    hub::MetaData metadata_read;
    assert( metadata == metadata_read );
    // assert( !metadata.hasValue() );
    // assert( !metadata_read.hasValue() );
    // assert( metadata.type() == typeid( void ) );
    //    std::cout << metadata.type().name() << std::endl;
    std::cout << metadata << std::endl;

    // static_assert( hub::Serializer::Readable_v<hub::MetaData> );
    // static_assert( hub::Serializer::Writable_v<hub::MetaData> );
    static_assert( !hub::Serializer::Serializable<hub::MetaData>() );
    // void
    {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( void ) << " ---------------------\n";
#endif
        archive.write( metadata );
        archive.read( metadata_read );
        // assert( !metadata_read.hasValue() );
        CHECK( metadata == metadata_read );
    }

    // int
    {
        metadata["int"] = 1;
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( int ) << " ---------------------\n";
#endif
        metadata_read["int"] = 1;
        CHECK( metadata == metadata_read );
        metadata_read["int"] = 2;
        CHECK( metadata != metadata_read );
        std::cout << metadata << std::endl;

        archive.write( metadata );
        archive.read( metadata_read );
        CHECK( metadata == metadata_read );
    }

    // double
    {
        metadata["double"] = 1.4;
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( double ) << " ---------------------\n";
#endif
        metadata_read["double"] = 1.4;
        CHECK( metadata == metadata_read );
        metadata_read["double"] = 2.1;
        CHECK( metadata != metadata_read );
        std::cout << metadata << std::endl;

        archive.write( metadata );
        archive.read( metadata_read );
        CHECK( metadata == metadata_read );
    }

    // std::string
    {
        metadata["string"] = std::string( "hello" );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( std::string )
                  << " ---------------------\n";
#endif
        metadata_read["string"] = std::string( "hello" );
        CHECK( metadata == metadata_read );
        metadata_read["string"] = std::string( "bye" );
        CHECK( metadata != metadata_read );
        std::cout << metadata << std::endl;

        archive.write( metadata );
        archive.read( metadata_read );
        CHECK( metadata == metadata_read );
    }

    // const char *
    {
        const char* str = "hello";
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( const char* )
                  << " ---------------------\n";
#endif
        metadata["char ptr"]      = str;
        metadata_read["char ptr"] = str;
        CHECK( metadata == metadata_read );
        metadata_read["char ptr"] = "bye";
        CHECK( metadata != metadata_read );
        std::cout << metadata << std::endl;

        archive.write( metadata );
        archive.read( metadata_read );
        CHECK( metadata == metadata_read );
    }

    {
        const Lambda lambda { 5, 0.0, 1.0f, { 0, 1, 2 }, "gauthier" };
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( Lambda ) << " ---------------------\n";
#endif
        metadata["lambda"]      = lambda;
        metadata_read["lambda"] = lambda;
        CHECK( metadata == metadata_read );
        metadata_read["lambda"] = Lambda { 2, 0.0, 1.0f, { 0 }, "bouyjou" };
        CHECK( metadata != metadata_read );
        std::cout << metadata << std::endl;

        archive.write( metadata );
        archive.read( metadata_read );
        CHECK( metadata == metadata_read );
    }

#ifndef HUB_NON_BUILD_SENSOR
    {
        using Format = hub::sensor::format::Mat4;
        static_assert( hub::packable_v<Format> );
        static_assert( !hub::serializer::SerializerZppBits::Serializable<Format>() );
        const Format mat4( 1.0f );
#    ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( Lambda ) << " ---------------------\n";
#    endif
        //    archive.write(lambda);
        //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
        metadata["mat4"]      = mat4;
        metadata_read["mat4"] = mat4;
        CHECK( metadata == metadata_read );
        metadata_read["mat4"] = Format( 2.0f );
        CHECK( metadata != metadata_read );
        std::cout << metadata << std::endl;

        archive.write( metadata );
        archive.read( metadata_read );
        CHECK( metadata == metadata_read );
    }
#endif

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
    TEST_END()
}
