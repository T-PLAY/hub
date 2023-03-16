#include "Acquisition.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>

// #include "Resolution.hpp"

namespace hub {

//// SensorSpec::Format Dof6::getFormat()
////{
//// }

// Image::Image( const unsigned char* const data, size_t size, Format format ) :

//}

// Image::Image( Image&& image ) :
// }

// Image::~Image() {

//}

// SensorSpec::Format Image::getFormat()
//{
// }

// Acquisition::Acquisition(long long start, long long end , hub::Acquisition::Args args) : m_start(
// start ), m_end( end ) {

//}

// Acquisition::Acquisition( long long start,

//}

Acquisition::Acquisition( long long start, long long end ) : m_start( start ), m_end( end ) {
    assert( m_start <= m_end );
}

Acquisition::~Acquisition() {
    m_measures.clear();
}

// Acquisition::Acquisition( Acquisition&& acq ) noexcept :
//}

bool Acquisition::operator==( const Acquisition& acq ) const {
    if ( m_start == acq.m_start && m_end == acq.m_end && m_size == acq.m_size &&
         m_measures.size() == acq.m_measures.size() ) {

        for ( int i = 0; i < m_measures.size(); ++i ) {
            if ( m_measures.at( i ) != acq.m_measures.at( i ) ) { return false; }
        }

        return true;
    }
    return false;
}

bool Acquisition::operator!=( const Acquisition& acq ) const {
    return !( *this == acq );
}

Acquisition& Acquisition::operator<<( data::Measure&& measure ) {
    // void Acquisition::addMeasure( Measure&& measure ) {
    m_measures.push_back( std::move( measure ) );
    m_size += m_measures.back().m_size;
    return *this;
}

void Acquisition::pushBack( data::Measure&& measure ) {
    m_measures.push_back( std::move( measure ) );
    m_size += m_measures.back().m_size;
}

// template <class ResolutionT>
// void Acquisition::addMeasure(unsigned char* data, uint64_t size, ResolutionT &&resolution ) {
//}

// template<typename... Values>
// void Acquisition::add(Values... values)
//{
// }

bool Acquisition::isInterpolable() const {
    assert( !m_measures.empty() );
    return std::none_of( m_measures.crbegin(),
                         m_measures.crend(),
                         []( const data::Measure& measure ) { return !measure.isInterpolable(); } );
}

Acquisition Acquisition::slerp( const Acquisition& left, const Acquisition& right, double t ) {
    assert( !left.m_measures.empty() );
    assert( left.getMeasures().size() == right.getMeasures().size() );
    assert( left.isInterpolable() && right.isInterpolable() );
    assert( 0.0 <= t && t <= 1.0 );
    const long long start     = ( 1.0 - t ) * left.m_start + t * right.m_start;
    const long long end       = ( 1.0 - t ) * left.m_end + t * right.m_end;
    Acquisition ret           = Acquisition { start, end };
    const auto& leftMeasures  = left.getMeasures();
    const auto& rightMeasures = right.getMeasures();
    for ( int iMeasure = 0; iMeasure < leftMeasures.size(); ++iMeasure ) {
        const auto& leftMeasure  = leftMeasures.at( iMeasure );
        const auto& rightMeasure = rightMeasures.at( iMeasure );
        assert( leftMeasure.getResolution() == rightMeasure.getResolution() );
        ret << data::Measure::slerp( leftMeasure, rightMeasure, t );
    }
    return ret;
}

// Acquisition Acquisition::lerp(const Acquisition &left, const Acquisition &right, long long time)
//{

//}

Acquisition& Acquisition::operator<<( const data::Measures& measures ) {
    for ( const auto& measure : measures ) {
        emplaceMeasure( (const unsigned char*)measure.m_data, measure.m_size, measure.getResolution() );
//        emplaceMeasure( measure.m_data, measure.m_size, measure.getResolution() );
    }
    return *this;
}

// Acquisition& Acquisition::operator<<( Measure&& measure ) {
// }

// const Measure& Acquisition::get( SensorSpec::Format format ) const {
// }

Acquisition Acquisition::clone() const {
    assert( !m_measures.empty() );

    Acquisition acq { m_start, m_end };

    acq << m_measures;

    return acq;
}

// const std::list<Measure> &Acquisition::getMeasures() const
const data::Measures& Acquisition::getMeasures() const {
    assert( !m_measures.empty() );
    return m_measures;
}

bool Acquisition::hasFixedSize() const {
    for ( const auto& measure : m_measures ) {
        const auto& format = measure.getResolution().second;
        if ( !res::format2hasFixedSize( format ) ) return false;
    }
    return true;
}

// const std::vector<Measure> &Acquisition::getMeasures() const
//{
// }

size_t Acquisition::getSize() const {
    return m_size;
}

std::ostream& operator<<( std::ostream& os, const Acquisition& acq ) {
    os << "\tstart:" << ( acq.m_start / 1'000'000 ) % 1'000 << " "
       << ( acq.m_start / 1'000 ) % 1'000 << " " << acq.m_start % 1'000
       << ", end:" << ( acq.m_end / 1'000'000 ) % 1'000 << " " << ( acq.m_end / 1'000 ) % 1'000
       << " " << acq.m_end % 1'000;
    os << ", measures:[";
    for ( const auto& measure : acq.m_measures ) {
        os << "[" << measure << "], ";
    }
    os << "], ";
    os << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps.";
    return os;
}

} // namespace hub
