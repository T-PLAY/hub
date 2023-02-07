#include "Acquisition.hpp"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <algorithm>

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

Acquisition& Acquisition::operator<<( Measure&& measure ) {
    // void Acquisition::addMeasure( Measure&& measure ) {
    //     emplaceMeasure(measure.m_data, measure.m_size, measure.getResolution());
    //     m_size += m_measures.back().m_size;
    m_size += measure.m_size;
    m_measures.push_back( std::move( measure ) );
    return *this;
}

// template <class ResolutionT>
// void Acquisition::addMeasure(unsigned char* data, uint64_t size, ResolutionT &&resolution ) {
//     m_size += size;
//     //    m_measures.push_back( std::move( measure ) );
////    m_measures.emplace_back( data, size, std::move(resolution) );
//    m_measures.emplace_back( data, size, std::forward<ResolutionT>(resolution) );
//}

// template<typename... Values>
// void Acquisition::add(Values... values)
//{
//     m_measures.emplace_back(values);
// }

bool Acquisition::isInterpolable() const {
//    for ( const auto& measure : m_measures ) {
//        if ( !measure.isInterpolable() ) return false;
//    }
//    return true;
    return std::none_of(m_measures.begin(), m_measures.end(), [](const Measure & measure) { return ! measure.isInterpolable(); } );
}

Acquisition Acquisition::slerp( const Acquisition& left, const Acquisition& right, double t ) {
    assert( left.getMeasures().size() == right.getMeasures().size() );
    assert( 0.0 <= t && t <= 1.0 );
    const long long start     = ( 1.0 - t ) * left.m_start + t * right.m_start;
    const long long end       = ( 1.0 - t ) * left.m_end + t * right.m_end;
    Acquisition ret           = Acquisition { start, end };
    const auto& leftMeasures  = left.getMeasures();
    const auto& rightMeasures = right.getMeasures();
    for ( int iMeasure = 0; iMeasure < leftMeasures.size(); ++iMeasure ) {
        ret << Measure::slerp( leftMeasures.at( iMeasure ), rightMeasures.at( iMeasure ), t );
        //        ret.addMeasure(Measure::slerp( leftMeasures.at( iMeasure ), rightMeasures.at(
        //        iMeasure ), t ));
    }
    return ret;
}

// Acquisition Acquisition::lerp(const Acquisition &left, const Acquisition &right, long long time)
//{

//}

Acquisition& Acquisition::operator<<( const Measures& measures ) {
    for ( const auto& measure : measures ) {
        //        Acquisition::operator<<( measure.clone() );
        emplaceMeasure( measure.m_data, measure.m_size, measure.getResolution() );
    }
    return *this;
}

// Acquisition& Acquisition::operator<<( Measure&& measure ) {
// }

// const Measure& Acquisition::get( SensorSpec::Format format ) const {
// }

Acquisition Acquisition::clone() const {

    Acquisition&& acq { m_start, m_end };

    acq << m_measures;
    //    for ( const auto& measure : m_measures ) {
    ////        acq << measure.clone();
    ////        acq.addMeasure(mea);
    //    }

    return std::move( acq );
}

// const std::list<Measure> &Acquisition::getMeasures() const
const Measures& Acquisition::getMeasures() const {
    return m_measures;
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
