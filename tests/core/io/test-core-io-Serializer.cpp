
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/Macros.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
// #include <core/io/InputOutput.hpp>
#include <core/Serializer.hpp>
#include <core/Any.hpp>

// auto serialize(const Node &) -> zpp::bits::pb_protocol;

class WritableReadableData
{
  public:
    WritableReadableData( std::string name, int value ) : m_name { name }, m_value { value } {};
    WritableReadableData() = default;
    auto toString() const { return "name: '" + m_name + "'" + " " + std::to_string( m_value ); }

    void write( hub::Serializer& serializer ) const {
        std::cout << "[test][WritableReadableData] write(Serializer&) : " << *this << std::endl;
        serializer.writeAll( m_name, m_value );
    }
    void read( hub::Serializer& serializer ) {
        serializer.readAll( m_name, m_value );
        std::cout << "[test][WritableReadableData] read(Serializer&) : " << *this << std::endl;
    }
    bool operator==( const WritableReadableData& other ) const {
        return m_name == other.m_name && m_value == other.m_value;
    }

  private:
    std::string m_name;
    int m_value;
};

class SerializedClass
{
  public:
    SerializedClass( int a, bool b ) : m_a { a }, m_b { b } {};
    SerializedClass() = default;
    bool operator==( const SerializedClass& other ) const {
        return m_a == other.m_a && m_b == other.m_b;
    }
    auto toString() const { return hub::to_string( m_a, m_b ); }

    // friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_a, self.m_b );
    }

  private:
    int m_a;
    bool m_b;
};

enum class UserEnum2 { A, B, C };
auto toString( UserEnum2 ue ) -> std::string {
    switch ( ue ) {
    case UserEnum2::A:
        return "A";
        break;
    case UserEnum2::B:
        return "B";
        break;
    case UserEnum2::C:
        return "C";
        break;
    default:
        break;
    }
    return "None";
}

template <class T>
void checkType( hub::io::Archive& archive, const T& t ) {
    std::cout << "-------------------------------- " << TYPE_NAME( t ) << " ----------------------"
              << std::endl;
    assert( archive.isEnd() );

    archive.write( t );
    T t_read;
    archive.read( t_read );
    assert( t == t_read );

    assert( archive.isEnd() );
    std::cout << std::endl;
}

TEST_CASE( "Serializer test" ) {
    TEST_BEGIN()

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

    hub::Any any;
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
