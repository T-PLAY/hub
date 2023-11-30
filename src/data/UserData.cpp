#include "UserData.hpp"

// #include "io/Memory.hpp"
//#include "io/input/InputMemory.hpp"

namespace hub {
namespace data {

const std::string& UserData::getName() const {
    return m_name;
}

const Any& UserData::getValue() const {
    return m_value;
}

// UserData::UserData( const Measure& measure ) :
//     Measure( measure.getData(), measure.getSize(), sensor::Resolution { { 1 }, sensor::Format::USER_DATA } ) {
//     assert( m_data != nullptr );
//     assert( m_size > 0 );

//     std::vector<char> buff;
//     buff.insert( buff.begin(), m_data, m_data + m_size );

// //    io::Memory<decltype( buff )> memory( buff );
//     input::InputMemory<decltype( buff )> memory( buff );

//     memory.read( m_name );
//     memory.read( m_value );
// }

UserData::UserData( const std::string& name, const Any& value ) :
    // Measure( (unsigned char*)nullptr, 0, sensor::Resolution { { 1 }, sensor::Format::USER_DATA } ),
    m_name( name ),
    m_value( value ) {

    std::vector<char> buff;

    // output::OutputMemory<decltype( buff )> memory( buff );

    // memory.write( m_name );
    // memory.write( m_value );

    // assert( !buff.empty() );

    // m_size = buff.size();
    // m_data = new unsigned char[m_size];
    // memcpy( m_data, buff.data(), m_size );
}

std::ostream& operator<<( std::ostream& os, const UserData& userData ) {
    // os << "[\"" << userData.getName() << "\", '" << userData.getValue() << "']";
    return os;
}

} // namespace data
} // namespace hub
