#include "Acquisition.hpp"

#include <cassert>
#include <iomanip>

#include <cmath>
#include <cstring>

namespace hub {

Dof6::Dof6( const Measure& measure ) : Measure( measure.m_data, measure.m_size ) {
    assert( measure.m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
    assert( m_data != nullptr );
}

Dof6::Dof6( float x, float y, float z, float w0, float w1, float w2, float w3 ) :
    Measure( (unsigned char*)new float[7] { x, y, z, w0, w1, w2, w3 }, 28 ) {
    m_ownData = true;
    assert( m_ownData == true );
    assert( m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
}

Dof6 Dof6::slerp( const Dof6& left, const Dof6& right, long long t ) {

    float x = ( 1.0 - t ) * left.m_x + t * right.m_x;
    float y = ( 1.0 - t ) * left.m_y + t * right.m_y;
    float z = ( 1.0 - t ) * left.m_z + t * right.m_z;

    float w0, w1, w2, w3;

    // source:
    // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
    // quaternion to return
    // Calculate angle between them.
    double cosHalfTheta = left.m_w0 * right.m_w0 + left.m_w1 * right.m_w1 + left.m_w2 * right.m_w2 +
                          left.m_w3 * right.m_w3;
    // if left=right or left=-right then theta = 0 and we can return left
    if ( std::abs( cosHalfTheta ) >= 1.0 ) {
        w0 = left.m_w0;
        w1 = left.m_w1;
        w2 = left.m_w2;
        w3 = left.m_w3;
        return Dof6( x, y, z, w0, w1, w2, w3 );
    }
    // Calculate temporary values.
    double halfTheta    = acos( cosHalfTheta );
    double sinHalfTheta = sqrt( 1.0 - cosHalfTheta * cosHalfTheta );
    // if theta = 180 degrees then result is not fully defined
    // we could rotate around any axis normal to left or right
    if ( fabs( sinHalfTheta ) < 0.001 ) { // fabs is floating point absolute
        w0 = ( left.m_w0 * 0.5 + right.m_w0 * 0.5 );
        w1 = ( left.m_w1 * 0.5 + right.m_w1 * 0.5 );
        w2 = ( left.m_w2 * 0.5 + right.m_w2 * 0.5 );
        w3 = ( left.m_w3 * 0.5 + right.m_w3 * 0.5 );
        return Dof6( x, y, z, w0, w1, w2, w3 );
    }
    double ratioA = sin( ( 1 - t ) * halfTheta ) / sinHalfTheta;
    double ratioB = sin( t * halfTheta ) / sinHalfTheta;
    // calculate Quaternion.
    w0 = ( left.m_w0 * ratioA + right.m_w0 * ratioB );
    w1 = ( left.m_w1 * ratioA + right.m_w1 * ratioB );
    w2 = ( left.m_w2 * ratioA + right.m_w2 * ratioB );
    w3 = ( left.m_w3 * ratioA + right.m_w3 * ratioB );
    return Dof6( x, y, z, w0, w1, w2, w3 );
}

// Dof6 Dof6::operator*(const Dof6 &dof6) const
//{
// }

std::ostream& operator<<( std::ostream& os, const Dof6& dof6 ) {
    os << "x:" << dof6.m_x << " y:" << dof6.m_y << " z:" << dof6.m_z << "   w0:" << dof6.m_w0
       << " w1:" << dof6.m_w1 << " w2:" << dof6.m_w2 << " w3:" << dof6.m_w3;
    return os;
}

Mat4::Mat4( const float* array ) : Measure( (unsigned char*)array, 64 ) {
    assert( m_size == 64 );
}

// Dof6::~Dof6() {
//}

// Measure::Measure( const unsigned char* const data, uint64_t size, bool floatData ) :

//}

Measure::Measure( const unsigned char* const data, uint64_t size ) :
    m_data( new unsigned char[size] ), m_size( size ), m_ownData( true ) {
    memcpy( (unsigned char*)m_data, data, m_size );
    assert( m_size > 0 );
    assert( m_data != nullptr );
}

// Measure::Measure( unsigned char* data, uint64_t size ) : m_data( data ), m_size( size ) {
// }

// Measure::Measure( Measure&& measure ) :
// }

// Measure::~Measure() {
// }

Measure Measure::clone() const {
    Measure ret( m_data, m_size );
    ret.m_resolution = m_resolution;
    return ret;
}

bool Measure::interpolable() const {
    assert( m_resolution.second != SensorSpec::Format::NONE );
    assert( !m_resolution.first.empty() );
    return SensorSpec::interpolable( m_resolution.second );
}

Measure Measure::slerp( const Measure& left, const Measure& right, double t ) {
    assert( left.m_size == right.m_size );
    assert( !left.m_resolution.first.empty() );
    assert( !right.m_resolution.first.empty() );
    assert( left.interpolable() );
    assert( right.interpolable() );
    assert( left.m_resolution == right.m_resolution );

    switch ( left.m_resolution.second ) {
    case SensorSpec::Format::DOF6: {
        return Dof6::slerp( left, right, t );
    }
    default:
        assert( false );
        break;
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

std::ostream& operator<<( std::ostream& os, const Measure& measure ) {
    if ( measure.m_size == 28 ) {
        float* dof6 = (float*)measure.m_data;
        os << "dof6:";
        for ( auto i = 0; i < 7; ++i ) {
            os << std::setw( 3 ) << dof6[i] << " ";
        }
    }
    else {
        for ( auto i = 0; i < std::min( (int)measure.m_size, 10 ); ++i ) {
            os << std::setw( 3 ) << (int)measure.m_data[i] << " ";
        }
    }
    return os;
}

Measure::Measure( Measure&& measure ) :
    m_data( measure.m_data ), m_size( measure.m_size ), m_resolution( measure.m_resolution ) {
    measure.m_isMoved = true;
}

Measure::~Measure() {

    if ( m_ownData && !m_isMoved ) { delete[] m_data; }
}

// Measure::Measure( Measurement measurement ) : m_measurement( measurement ) {}

// SensorSpec::Format Measure::getFormat()
//{
// }

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
    m_size += measure.m_size;
    m_measures.push_back( std::move( measure ) );
    return *this;
}

bool Acquisition::interpolable() const {
    for ( const auto& measure : m_measures ) {
        if ( !measure.interpolable() ) return false;
    }
    return true;
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
    }
    return ret;
}

// Acquisition Acquisition::lerp(const Acquisition &left, const Acquisition &right, long long time)
//{

//}

Acquisition& Acquisition::operator<<( const Measures& measures ) {
    for ( const auto& measure : measures ) {
        Acquisition::operator<<( measure.clone() );
    }
    return *this;
}

// Acquisition& Acquisition::operator<<( Measure&& measure ) {
// }

// const Measure& Acquisition::get( SensorSpec::Format format ) const {
// }

Acquisition Acquisition::clone() const {

    Acquisition acq( m_start, m_end );

    for ( const auto& measure : m_measures ) {
        acq << measure.clone();
    }

    return acq;
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
