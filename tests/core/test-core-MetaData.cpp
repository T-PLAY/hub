

#include "test_common.hpp"

#include <core/MetaData.hpp>
#include <core/io/Archive.hpp>

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

hub::MetaData s_metaData;
hub::MetaData s_metaData_read;
hub::io::Archive s_archive;

template <class T>
void checkMetaData( const T& t, const T& t2 ) {
    CHECK( s_metaData == s_metaData_read );
    s_metaData[TYPE_NAME( t )] = t;
    std::cout << "<" << TYPE_NAME( t ) << ">: " << s_metaData << std::endl;
    CHECK( s_metaData != s_metaData_read );
    s_metaData_read[TYPE_NAME( t )] = t;
    CHECK( s_metaData == s_metaData_read );
    s_metaData_read[TYPE_NAME( t )] = t2;
    CHECK( s_metaData != s_metaData_read );

    assert( s_archive.isEnd() );
    s_archive.write( s_metaData );
    s_archive.read( s_metaData_read );
    assert( s_archive.isEnd() );
    CHECK( s_metaData == s_metaData_read );
}

TEST_CASE( "MetaData test" ) {
    TEST_BEGIN()

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    assert( s_metaData == s_metaData_read );
    std::cout << s_metaData << std::endl;

    static_assert( !hub::Serializer::Serializable<hub::MetaData>() );
    static_assert( !hub::Serializer::Writable_v<hub::MetaData> );
    static_assert( !hub::Serializer::Readable_v<hub::MetaData> );
    static_assert( !hub::packable_v<hub::MetaData> );

    checkMetaData<int>( 1, 2 );
    checkMetaData<double>( 1.0, 2.0 );
    checkMetaData<std::string>( "gauthier", "bouyjou" );
    checkMetaData<const char*>( "abc", "def" );
    checkMetaData<Lambda>( Lambda { 1, 2.0f, 3.0, { 1, 2 }, "hello" },
                           Lambda { 2, 3.0f, 4.0, { 3, 4 }, "hi" } );
    using namespace hub::format;
    Mat4 mat4 { 1.0f };
    s_archive.write( mat4 );

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
    TEST_END()
}
