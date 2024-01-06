
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "core/Traits.hpp"
#include "test_common.hpp"
#include <core/Macros.hpp>

#include <core/io/Archive.hpp>
// #include <core/io/InputOutput.hpp>
#include <core/Serializer.hpp>

// auto serialize(const Node &) -> zpp::bits::pb_protocol;

class SerializedData
{
  public:
    auto toString() const { return "name: '" + m_name + "'" + " " + std::to_string( m_value ); }

    void write( hub::Serializer& serializer ) const {
        std::cout << "[SerializedData] write(Serializer&) : " << *this << std::endl;
        serializer.write( m_name );
        // serializer.writeAll( m_name, m_value );
    }
    void read( hub::Serializer& serializer ) {
        serializer.read( m_name );
        // serializer.readAll( m_name, m_value );
        std::cout << "[SerializedData] read(Serializer&) : " << *this << std::endl;
    }
    bool operator==( const SerializedData& other ) const {
        return m_name == other.m_name && m_value == other.m_value;
    }

    std::string m_name;
    int m_value;
};

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

class UserClass {
  public:
    bool operator==(const UserClass & other) const {
        return a == other.a && b == other.b;
    }
    auto toString() const {
        return hub::to_string(a, b);
    }
  private:
    int a;
    bool b;
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
    assert( archive.isEnd() );

    archive.write( t );
    T t_read;
    archive.read( t_read );
    assert( t == t_read );

    assert( archive.isEnd() );
}

TEST_CASE( "Serializer test" ) {
    TEST_BEGIN()

    // hub::Serializer serializer;

    hub::io::Archive archive;

    // TYPE_NAME(UserEnum2());

    // packable
    static_assert(std::is_arithmetic_v<int>);
    static_assert(std::is_arithmetic_v<double>);
    static_assert(! std::is_arithmetic_v<std::string>);

    static_assert(std::is_array_v<double[]>);

    static_assert(std::is_enum_v<UserEnum2>);
    static_assert(! std::is_enum_v<int>);

    static_assert(hub::isPacket<std::string>);
    static_assert(hub::isPacket<std::vector<int>>);

    static_assert( hub::packable_v<int> );
    static_assert( hub::packable_v<double> );
    static_assert( ! hub::packable_v<UserClass> );
    static_assert( hub::packable_v<UserEnum2> );
    static_assert( hub::packable_v<std::vector<int>> );
    static_assert( hub::packable_v<std::string> );
    static_assert( hub::packable_v<std::vector<UserClass>> );

    // checkType<int>( archive, 5 );
    // checkType<double>( archive, 2.0 );
    // checkType<UserEnum2>( archive, UserEnum2::B );
    // checkType<UserClass>( archive, UserClass() );

    TEST_END()
}
