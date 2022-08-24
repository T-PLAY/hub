#include "Acquisition.hpp"

#include <cassert>
#include <iomanip>

#include <cstring>

namespace hub {
Acquisition::Acquisition( long long start,
                          long long end,
                          const unsigned char* const data,
                          size_t size ) :
    m_start( start ),
    m_end( end ),
    m_data( new unsigned char[size] ),
    m_size( size ) {

    assert( data != nullptr );
    memcpy( (unsigned char*)m_data, data, m_size );
    assert( m_size > 0 );
    assert( m_start <= m_end );
}

Acquisition::~Acquisition() {
    assert( m_data != nullptr );

    if ( !m_isMoved ) { delete[] m_data; }
}

Acquisition::Acquisition( Acquisition&& acq ) noexcept :
    m_start( acq.m_start ),
    m_end( acq.m_end ),
    m_data( acq.m_data ),
    m_size( acq.m_size ) {
    acq.m_isMoved = true;
}

bool Acquisition::operator==( const Acquisition& acq ) const {
    if ( m_start == acq.m_start &&
         m_end == acq.m_end && m_size == acq.m_size ) {

        return memcmp( m_data, acq.m_data, m_size ) == 0;
    }
    return false;
}

bool Acquisition::operator!=(const Acquisition &acq) const
{
    return ! (*this == acq);
}

Acquisition Acquisition::clone() const {
    assert( m_data != nullptr );

    return Acquisition( m_start, m_end, m_data, m_size );
}

std::ostream& operator<<( std::ostream& os, const Acquisition& acq ) {
    os << "start:" << acq.m_start << ", end:" << acq.m_end ;
    os << ", data:[";
    for ( auto i = 0; i < std::min( (int)acq.m_size, 10 ); ++i ) {
        os << std::setw( 3 ) << (int)acq.m_data[i] << " ";
    }
    os << "], ";
    os << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps";
    os << ", ptr = " << (uint64_t)acq.m_data;
    return os;
}

} // namespace hub
