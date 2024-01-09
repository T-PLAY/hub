
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

// #include <core/io/Memory.hpp>
#include <core/MetaData.hpp>
#include <core/io/Archive.hpp>

// #ifndef HUB_NON_BUILD_SENSOR
// #    include <sensor/Format.hpp>
// #endif
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

    // friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.a, self.b, self.c, self.ds, self.e );
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

hub::MetaData s_metaData;
hub::MetaData s_metaData_read;
hub::io::Archive s_archive;

template <class T>
void checkMetaData( const T& t, const T& t2 ) {
    // int
    // {
    CHECK( s_metaData == s_metaData_read );
    s_metaData[TYPE_NAME( t )] = t;
    std::cout << "<" << TYPE_NAME( t ) << ">: " << s_metaData << std::endl;
    // #ifdef HUB_DEBUG_OUTPUT
    //     std::cout << "\n------------------ MetaData<" << TYPE_NAME( int() )
    //               << "> ---------------------\n";
    // #endif
    CHECK( s_metaData != s_metaData_read );
    s_metaData_read[TYPE_NAME( t )] = t;
    CHECK( s_metaData == s_metaData_read );
    s_metaData_read[TYPE_NAME( t )] = t2;
    CHECK( s_metaData != s_metaData_read );
    // std::cout << s_metaData << std::endl;

    assert( s_archive.isEnd() );
    s_archive.write( s_metaData );
    s_archive.read( s_metaData_read );
    assert( s_archive.isEnd() );
    CHECK( s_metaData == s_metaData_read );
    // }
}

TEST_CASE( "MetaData test" ) {
    TEST_BEGIN()

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    // std::any std_any;
    // assert( !std_any.has_value() );

    // hub::MetaData metadata;
    // hub::MetaData metadata_read;
    assert( s_metaData == s_metaData_read );
    std::cout << s_metaData << std::endl;
    // assert( !metadata.hasValue() );
    // assert( !metadata_read.hasValue() );
    //    std::cout << metadata.type().name() << std::endl;

    // static_assert( hub::Serializer::Readable_v<hub::MetaData> );
    // static_assert( hub::Serializer::Writable_v<hub::MetaData> );
    // todo c++17

    // #if CPP_VERSION >= 20
    static_assert( !hub::Serializer::Serializable<hub::MetaData>() );
    static_assert( !hub::Serializer::Writable_v<hub::MetaData> );
    static_assert( !hub::Serializer::Readable_v<hub::MetaData> );
    static_assert( !hub::packable_v<hub::MetaData> );
    // #endif

    checkMetaData<int>( 1, 2 );
    checkMetaData<double>( 1.0, 2.0 );
    checkMetaData<std::string>( "gauthier", "bouyjou" );
    checkMetaData<const char*>( "abc", "def" );
    checkMetaData<Lambda>( Lambda { 1, 2.0f, 3.0, { 1, 2 }, "hello" },
                           Lambda { 2, 3.0f, 4.0, { 3, 4 }, "hi" } );
    // checkMetaData<hub::format::Mat4>(hub::format::Mat4(1.0), hub::format::Mat4());

    // todo serialize
    // hub::io::Archive archive;

    //     // void
    //     {
    // #ifdef HUB_DEBUG_OUTPUT
    //         std::cout << "\n------------------ " << "MetaData<void>" << "
    //         ---------------------\n";
    // #endif
    //         archive.write( metadata );
    //         archive.read( metadata_read );
    //         // assert( !metadata_read.hasValue() );
    //         CHECK( metadata == metadata_read );
    //     }

// #ifndef HUB_NON_BUILD_SENSOR
//     {
//         using Format = hub::format::Mat4;
//         static_assert( hub::packable_v<Format> );
//         static_assert( !hub::serializer::SerializerZppBits::Serializable<Format>() );
//         const Format mat4( 1.0f );
// #    ifdef HUB_DEBUG_OUTPUT
//         std::cout << "\n------------------ " << TYPE_NAME( lambda ) << " ---------------------\n";
// #    endif
//         //    archive.write(lambda);
//         //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
//         metadata["mat4"]      = mat4;
//         metadata_read["mat4"] = mat4;
//         CHECK( metadata == metadata_read );
//         metadata_read["mat4"] = Format( 2.0f );
//         CHECK( metadata != metadata_read );
//         std::cout << metadata << std::endl;

//         archive.write( metadata );
//         archive.read( metadata_read );
//         CHECK( metadata == metadata_read );
//     }
// #endif

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
    TEST_END()
}
