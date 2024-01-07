
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#define DEBUG_CHECK_TYPE
#include "test_core_io_common.hpp"

#include <core/Macros.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
// #include <core/io/InputOutput.hpp>
#include <core/Serializer.hpp>
// #include <core/Any.hpp>

// auto serialize(const Node &) -> zpp::bits::pb_protocol;



TEST_CASE( "Serializer test" ) {
    TEST_BEGIN()

    using namespace testCoreIoCommon;

    hub::io::Archive archive;

    static_assert( std::is_arithmetic_v<double> );
    static_assert( !std::is_arithmetic_v<std::string> );
    static_assert( std::is_array_v<double[]> );
    static_assert( hub::isPacket<std::string> );
    static_assert( hub::isPacket<std::vector<int>> );
    static_assert( !hub::packable_v<std::vector<int>> );
    static_assert( !hub::packable_v<std::string> );
    static_assert( !hub::packable_v<std::vector<SerializedClass>> );

    checkType<int>( archive, 5 );
    static_assert( std::is_arithmetic_v<int> );
    static_assert( !std::is_enum_v<int> );
    static_assert( hub::packable_v<int> );
    static_assert( !hub::Serializer::Readable_v<int> );
    static_assert( !hub::Serializer::Writable_v<int> );
    static_assert( !hub::Serializer::Serializable<int>() );

    checkType<double>( archive, 2.0 );
    static_assert( std::is_arithmetic_v<double> );
    static_assert( !std::is_enum_v<double> );
    static_assert( hub::packable_v<double> );
    static_assert( !hub::Serializer::Readable_v<double> );
    static_assert( !hub::Serializer::Writable_v<double> );
    static_assert( !hub::Serializer::Serializable<double>() );

    checkType<std::string>( archive, "hello" );
    static_assert( !std::is_arithmetic_v<std::string> );
    static_assert( !std::is_enum_v<std::string> );
    static_assert( !hub::packable_v<std::string> );
    static_assert( !hub::Serializer::Readable_v<std::string> );
    static_assert( !hub::Serializer::Writable_v<std::string> );
    static_assert( hub::Serializer::Serializable<std::string>() );

    checkType<UserEnum2>( archive, UserEnum2::B );
    static_assert( !std::is_arithmetic_v<UserEnum2> );
    static_assert( std::is_enum_v<UserEnum2> );
    static_assert( hub::packable_v<UserEnum2> );
    static_assert( !hub::Serializer::Readable_v<UserEnum2> );
    static_assert( !hub::Serializer::Writable_v<UserEnum2> );
    static_assert( !hub::Serializer::Serializable<UserEnum2>() );

    checkType<SerializedClass>( archive, SerializedClass { 2, true } );
    static_assert( !std::is_arithmetic_v<SerializedClass> );
    static_assert( !std::is_enum_v<SerializedClass> );
    static_assert( !hub::packable_v<SerializedClass> );
    static_assert( !hub::Serializer::Readable_v<SerializedClass> );
    static_assert( !hub::Serializer::Writable_v<SerializedClass> );
    static_assert( hub::Serializer::Serializable<SerializedClass>() );

    checkType<WritableReadableData>( archive, WritableReadableData { "gauthier", 30 } );
    static_assert( !std::is_arithmetic_v<WritableReadableData> );
    static_assert( !std::is_enum_v<WritableReadableData> );
    static_assert( !hub::packable_v<WritableReadableData> );
    static_assert( hub::Serializer::Readable_v<WritableReadableData> );
    static_assert( hub::Serializer::Writable_v<WritableReadableData> );
    static_assert( !hub::Serializer::Serializable<WritableReadableData>() );

    // hub::Any any;
    // int a = 5;
    // double d = 2.0;
    // archive.writeAll(a, d);
    // int a_read;
    // double d_read;
    // archive.readAll(a_read, d_read);
    // assert(a == a_read);
    // assert(d == d_read);

    TEST_END()
}

// class InputOutputStream : public hub::Output, public hub::Input
// {
//   public:
//     using hub::Input::read;
//     using hub::Output::write;

//     InputOutputStream( hub::Output& output ) : m_output( output ) {};

//     void write( const hub::Data_t* data, hub::Size_t size ) override {
//         if ( m_retained ) { m_retainedData.insert( m_retainedData.end(), data, data + size ); }
//         m_output.write( data, size );
//     }
//     void close() override {}
//     bool isOpen() const override { return true; }
//     void setRetain( bool retain ) override { m_retained = retain; }
//     void read( hub::Data_t* data, hub::Size_t size ) override {
//         assert( m_readPosition + size <= m_retainedData.size() );
//         std::copy( m_retainedData.data() + m_readPosition,
//                    m_retainedData.data() + m_readPosition + size,
//                    data );
//         m_readPosition += size;
//     }
//     bool isEnd() const override { return false; }
//     void clear() override {}

//     int m_readPosition = 0;
//     std::vector<hub::Data_t> m_retainedData;
//     bool m_retained = false;
//     hub::Output& m_output;
// };
