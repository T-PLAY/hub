#include "Acquisition.hpp"

#include <algorithm>
//#include <functional>
//#include <iterator>
#include <numeric>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>

namespace hub {
namespace sensor {

//Acquisition::Acquisition( long long start, long long end ) : m_start( start ), m_end( end ) {
//    assert( m_start <= m_end );
//}

//Acquisition& Acquisition::operator=( Acquisition&& acq ) {
//    m_measures.clear();

//    m_start    = acq.m_start;
//    m_end      = acq.m_end;
//    m_measures = std::move( acq.m_measures );
//    m_size     = acq.m_size;

//    assert( acq.m_measures.empty() );

//    return *this;
//}

//Acquisition::~Acquisition() {
//    m_measures.clear();
//}

//bool Acquisition::operator==( const Acquisition& acq ) const {
//    if ( m_start == acq.m_start && m_end == acq.m_end && m_size == acq.m_size &&
//         m_measures.size() == acq.m_measures.size() ) {

//        for ( size_t i = 0; i < m_measures.size(); ++i ) {
//            if ( m_measures.at( i ) != acq.m_measures.at( i ) ) { return false; }
//        }

//        return true;
//    }
//    return false;
//}

//bool Acquisition::operator!=( const Acquisition& acq ) const {
//    return !( *this == acq );
//}

//Acquisition& Acquisition::operator<<( Measure&& measure ) {
//    m_measures.push_back( std::move( measure ) );
//    m_size += m_measures.back().getSize();
//    return *this;
//}

//void Acquisition::operator>>( Acquisition& acq ) {

//    acq = std::move( *this );

//    assert( m_measures.empty() );
//}

//void Acquisition::pushBack( Measure&& measure ) {
//    m_measures.push_back( std::move( measure ) );
//    m_size += m_measures.back().getSize();
//}

//bool Acquisition::isInterpolable() const {
//    assert( !m_measures.empty() );
//    return std::none_of( m_measures.crbegin(),
//                         m_measures.crend(),
//                         []( const Measure& measure ) { return !measure.isInterpolable(); } );
//}

//Acquisition Acquisition::slerp( const Acquisition& left, const Acquisition& right, double t ) {
//    assert( !left.m_measures.empty() );
//    assert( left.getMeasures().size() == right.getMeasures().size() );
//    assert( left.isInterpolable() && right.isInterpolable() );
//    assert( 0.0 <= t && t <= 1.0 );
//    const long long start     = ( 1.0 - t ) * left.m_start + t * right.m_start;
//    const long long end       = ( 1.0 - t ) * left.m_end + t * right.m_end;
//    Acquisition ret           = Acquisition { start, end };
//    const auto& leftMeasures  = left.getMeasures();
//    const auto& rightMeasures = right.getMeasures();
//    for ( size_t iMeasure = 0; iMeasure < leftMeasures.size(); ++iMeasure ) {
//        const auto& leftMeasure  = leftMeasures.at( iMeasure );
//        const auto& rightMeasure = rightMeasures.at( iMeasure );
//        assert( leftMeasure.getResolution() == rightMeasure.getResolution() );
//        ret << Measure::slerp( leftMeasure, rightMeasure, t );
//    }
//    return ret;
//}

//Acquisition& Acquisition::operator<<( const Measures& measures ) {
//    for ( const auto& measure : measures ) {
//        emplaceMeasure( reinterpret_cast<const unsigned char*>( measure.getData() ),
//                        measure.getSize(),
//                        measure.getResolution() );
//    }
//    return *this;
//}

//Acquisition Acquisition::clone() const {
//    assert( !m_measures.empty() );

//    Acquisition acq { m_start, m_end };

//    acq << m_measures;

//    return acq;
//}

//const Measures& Acquisition::getMeasures() const {
//    assert( !m_measures.empty() );
//    return m_measures;
//}

//bool Acquisition::hasFixedSize() const {
//    return std::all_of(m_measures.cbegin(), m_measures.cend(), [](const Measure & measure) {
//        const auto& format = measure.getResolution().second;
//        return resolution::format2hasFixedSize(format);
//    });
////    for ( const auto& measure : m_measures ) {
////        const auto& format = measure.getResolution().second;
////        if ( !res::format2hasFixedSize( format ) ) return false;
////    }
////    return true;
//}

//size_t Acquisition::getSize() const {
//    return m_size;
//}

//std::ostream& operator<<( std::ostream& os, const Acquisition& acq ) {
//    os << "\tstart:" << ( acq.m_start / 1'000'000 ) % 1'000 << " "
//       << ( acq.m_start / 1'000 ) % 1'000 << " " << acq.m_start % 1'000
//       << ", end:" << ( acq.m_end / 1'000'000 ) % 1'000 << " " << ( acq.m_end / 1'000 ) % 1'000
//       << " " << acq.m_end % 1'000;
//    os << ", measures:[";
//    for ( int i = 0; i < acq.m_measures.size(); ++i ) {
//        const auto& measure = acq.m_measures.at( i );
//        os << "[" << measure << "]";
//        if ( i != acq.m_measures.size() - 1 ) { os << ", "; }
//    }
//    os << "], ";
//    os << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps.";
//    return os;
//}

} // namespace sensor
} // namespace hub
