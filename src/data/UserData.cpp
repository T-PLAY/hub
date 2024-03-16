#include "UserData.hpp"

namespace hub {
namespace data {

const std::string& UserData::getName() const {
    return m_name;
}

const Any& UserData::getValue() const {
    return m_value;
}

UserData::UserData( const std::string& name, const Any& value ) :
    m_name( name ), m_value( value ) {}

std::ostream& operator<<( std::ostream& os, const UserData& userData ) {
    return os;
}

} // namespace data
} // namespace hub
