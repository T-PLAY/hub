#include "Measure.hpp"

#include <iomanip>


#include "data/Dof6.hpp"

namespace hub {
namespace data {

Measure::~Measure() {

    if ( m_ownData && !m_isMoved ) {
        assert( m_data != nullptr );
        delete[] m_data;
    }
}



Measure::Measure( Measure&& measure ) :
    m_data( measure.m_data ),
    m_size( measure.m_size ),
    m_ownData( measure.m_ownData ),
    m_resolution( std::move( measure.m_resolution ) ) {

    measure.m_isMoved = true;
}






Measure Measure::clone() const {
    return { m_data, m_size, m_resolution };
}

bool Measure::isInterpolable() const {
    assert( m_resolution.second != Format::NONE );
    assert( !m_resolution.first.empty() );
    return format2isInterpolable( m_resolution.second );
}



Measure Measure::slerp( const Measure& left, const Measure& right, double t ) {
    assert( left.m_size == right.m_size );
    assert( !left.m_resolution.first.empty() );
    assert( !right.m_resolution.first.empty() );
    assert( left.m_resolution == right.m_resolution );

    if ( left.isInterpolable() ) {
        assert( right.isInterpolable() );

        switch ( left.m_resolution.second ) {
        case Format::DOF6: {
            return Dof6::slerp( Dof6( left ), Dof6( right ), t );
        }
        }
    }
    return left.clone(); // avoid non return warning
}

bool Measure::operator==( const Measure& measure ) const {
    if ( m_size == measure.m_size ) {
        return memcmp( m_data, measure.m_data, m_size ) == 0 &&
               m_resolution == measure.m_resolution;
    }
    return false;
}

bool Measure::operator!=( const Measure& measure ) const {
    return !( *this == measure );
}

bool Measure::ownData() const
{
    return m_ownData;
}

//const unsigned char *Measure::getData() const
//{
//    return m_data;
//}

std::ostream& operator<<( std::ostream& os, const Measure& measure ) {
    os << measure.getResolution() << ", [";

    for ( auto i = 0; i < std::min( (int)measure.m_size, 10 ); ++i ) {
        os << std::setw( 3 ) << (int)measure.m_data[i] << " ";
    }
    os << "]";
    return os;
}

} // namespace data
} // namespace hub
