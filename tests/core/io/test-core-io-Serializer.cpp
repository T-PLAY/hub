

#include "test_common.hpp"

#define DEBUG_CHECK_TYPE
#include "test_core_io_common.hpp"

//#include <core/Macros.hpp>
#include <core/Serializer.hpp>
#include <core/io/Archive.hpp>

class InputCustom : public hub::Input
{
  public:
    virtual void close() override {}
    virtual bool isOpen() const override { return true; }
    virtual void read( hub::Data_t* data, hub::Size_t len ) override {}
    virtual bool isEnd() const override { return false; }
    virtual void clear() override {}
};

TEST_CASE( "Serializer test" ) {
    TEST_BEGIN()

    using namespace testCoreIoCommon;

    // InputCustom input;

    hub::io::Archive archive;

    archive.writeAll( 1, 2.0, true );
    int a;
    double b;
    bool c;
    archive.readAll( a, b, c );
    CHECK( a == 1 );
    CHECK( b == 2.0 );
    CHECK( c );

    hub::io::Archive archive2 { std::move( archive ) };
    archive2.writeAll( 1, 2.0, true );
    archive2.readAll( a, b, c );
    CHECK( a == 1 );
    CHECK( b == 2.0 );
    CHECK( c );

    static_assert( std::is_arithmetic_v<double> );
    static_assert( !std::is_arithmetic_v<std::string> );
    static_assert( std::is_array_v<double[]> );
    static_assert( hub::isPacket<std::string> );
    static_assert( hub::isPacket<std::vector<int>> );
    static_assert( !hub::packable_v<std::vector<int>> );
    static_assert( !hub::packable_v<std::string> );
    static_assert( !hub::packable_v<std::vector<SerializedClass>> );

    checkType<int>( archive2, 5 );
    static_assert( std::is_arithmetic_v<int> );
    static_assert( !std::is_enum_v<int> );
    static_assert( hub::packable_v<int> );
    static_assert( !hub::Serializer::Readable_v<int> );
    static_assert( !hub::Serializer::Writable_v<int> );
    static_assert( !hub::Serializer::Serializable<int>() );

    checkType<double>( archive2, 2.0 );
    static_assert( std::is_arithmetic_v<double> );
    static_assert( !std::is_enum_v<double> );
    static_assert( hub::packable_v<double> );
    static_assert( !hub::Serializer::Readable_v<double> );
    static_assert( !hub::Serializer::Writable_v<double> );
    static_assert( !hub::Serializer::Serializable<double>() );

    checkType<std::string>( archive2, "hello" );
    static_assert( !std::is_arithmetic_v<std::string> );
    static_assert( !std::is_enum_v<std::string> );
    static_assert( !hub::packable_v<std::string> );
    static_assert( !hub::Serializer::Readable_v<std::string> );
    static_assert( !hub::Serializer::Writable_v<std::string> );
    static_assert( hub::Serializer::Serializable<std::string>() );

    checkType<UserEnum2>( archive2, UserEnum2::B );
    static_assert( !std::is_arithmetic_v<UserEnum2> );
    static_assert( std::is_enum_v<UserEnum2> );
    static_assert( hub::packable_v<UserEnum2> );
    static_assert( !hub::Serializer::Readable_v<UserEnum2> );
    static_assert( !hub::Serializer::Writable_v<UserEnum2> );
    static_assert( !hub::Serializer::Serializable<UserEnum2>() );

    checkType<SerializedClass>( archive2, SerializedClass { 2, true } );
    static_assert( !std::is_arithmetic_v<SerializedClass> );
    static_assert( !std::is_enum_v<SerializedClass> );
    static_assert( !hub::packable_v<SerializedClass> );
    static_assert( !hub::Serializer::Readable_v<SerializedClass> );
    static_assert( !hub::Serializer::Writable_v<SerializedClass> );
    static_assert( hub::Serializer::Serializable<SerializedClass>() );

    checkType<WritableReadableData>( archive2, WritableReadableData { "gauthier", 30 } );
    static_assert( !std::is_arithmetic_v<WritableReadableData> );
    static_assert( !std::is_enum_v<WritableReadableData> );
    static_assert( !hub::packable_v<WritableReadableData> );
    static_assert( hub::Serializer::Readable_v<WritableReadableData> );
    static_assert( hub::Serializer::Writable_v<WritableReadableData> );
    static_assert( !hub::Serializer::Serializable<WritableReadableData>() );

    TEST_END()
}
