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
    Measure( (unsigned char*)new float[7] { x, y, z, w0, w1, w2, w3 }, 28 )
//    m_x( x ),
//    m_y(),
//    m_z( z ),
//    m_w0( w0 ),
//    m_w1( w1 ),
//    m_w2( w2 ),
//    m_w3( w3 ) {
{
    m_ownData = true;
    assert( m_ownData == true );
    assert( m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
}

Dof6 Dof6::slerp( const Dof6& left,
//                  long long startLeft,
                  const Dof6& right,
//                  long long startRight,
                  long long t ) {
//    const Dof6& left  = ( startLeft > startRight ) ? ( pRight ) : ( pLeft );
//    const Dof6& right = ( startLeft > startRight ) ? ( pLeft ) : ( pRight );
//    if ( startLeft > startRight ) {
//        long long tmp = startLeft;
//        startLeft     = startRight;
//        startRight    = tmp;
//    }
//    assert( startLeft <= time && time <= startRight );
//    //    Dof6 ret;
//    double t = ( time - startLeft ) / ( startRight - startLeft );
//    assert( 0.0 <= t && t <= 1.0 );

    float x = ( 1.0 - t ) * left.m_x + t * right.m_x;
    float y = ( 1.0 - t ) * left.m_y + t * right.m_y;
    float z = ( 1.0 - t ) * left.m_z + t * right.m_z;

    //    return Dof6(x, y, z, w0, w1, w2, w3);
    float w0, w1, w2, w3;

    // source:
    // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
    //    quat slerp(quat qa, quat qb, double t) {
    // quaternion to return
    //        quat qm = new quat();
    // Calculate angle between them.
    double cosHalfTheta = left.m_w0 * right.m_w0 + left.m_w1 * right.m_w1 + left.m_w2 * right.m_w2 +
                          left.m_w3 * right.m_w3;
    // if left=right or left=-right then theta = 0 and we can return left
    if ( std::abs( cosHalfTheta ) >= 1.0 ) {
        w0 = left.m_w0;
        w1 = left.m_w1;
        w2 = left.m_w2;
        w3 = left.m_w3;
        //            return qm;
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
        //            return qm;
    }
    double ratioA = sin( ( 1 - t ) * halfTheta ) / sinHalfTheta;
    double ratioB = sin( t * halfTheta ) / sinHalfTheta;
    // calculate Quaternion.
    w0 = ( left.m_w0 * ratioA + right.m_w0 * ratioB );
    w1 = ( left.m_w1 * ratioA + right.m_w1 * ratioB );
    w2 = ( left.m_w2 * ratioA + right.m_w2 * ratioB );
    w3 = ( left.m_w3 * ratioA + right.m_w3 * ratioB );
    //        return qm;
    //    }
    return Dof6( x, y, z, w0, w1, w2, w3 );
}

// Dof6 Dof6::operator*(const Dof6 &dof6) const
//{
//     Dof6 ret;
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
////    delete m_data;
//    //    m_data = nullptr;
//}

// Measure::Measure( const unsigned char* const data, uint64_t size, bool floatData ) :
//     //    Measure( Measurement::IMAGE ),
//     m_data( new unsigned char[size] ),
//     //    m_data( data ),
//     m_size( size ) {
//     //    m_format( format ) {

//    assert( m_size > 0 );
//    assert( data != nullptr );
//    memcpy( (unsigned char*)m_data, data, m_size );

//    if ( floatData ) delete[] data;
//}

Measure::Measure( const unsigned char* const data, uint64_t size ) :
    m_data( new unsigned char[size] ), m_size( size ), m_ownData( true ) {
    memcpy( (unsigned char*)m_data, data, m_size );
    assert( m_size > 0 );
    assert( m_data != nullptr );
}

// Measure::Measure( unsigned char* data, uint64_t size ) : m_data( data ), m_size( size ) {
//     assert( m_size > 0 );
//     assert( m_data != nullptr );
// }

// Measure::Measure( Measure&& measure ) :
//     m_data( measure.m_data ), m_size( measure.m_size ), m_ownData( measure.m_ownData ) {
//     measure.m_ownData = false;
// }

// Measure::~Measure() {
//     if ( m_ownData ) { delete[] m_data; }
// }

Measure Measure::clone() const {
    //    unsigned char* data = new unsigned char[m_size];
    //    memcpy( data, m_data, m_size );
    //    Measure measure( m_data, m_size );
    //    measure.m_ownData = true;
    Measure ret ( m_data, m_size );
    ret.m_resolution = m_resolution;
    return ret;
}

bool Measure::interpolable() const {
    assert(m_resolution.second != SensorSpec::Format::NONE);
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
//        const Dof6& dof6Left  = left;
//        const Dof6& dof6Right = right;
        return Dof6::slerp(left, right, t);
    }
    default:
        assert( false );
        break;
    }
}

bool Measure::operator==( const Measure& measure ) const {
    if ( m_size == measure.m_size ) { return memcmp( m_data, measure.m_data, m_size ) == 0 && m_resolution == measure.m_resolution; }
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
    //     Measure(measure.m_measurement),
    //    Measure( measure.m_format ),
    m_data( measure.m_data ),
    m_size( measure.m_size ),
    m_resolution(measure.m_resolution)
{
    //     m_format( measure.m_format ) {
    //    measure.m_data = nullptr;
    measure.m_isMoved = true;
}

Measure::~Measure() {
    //    std::cout << "[Measure] delete data : " << (uintptr_t)m_data << ", size : " << m_size
    //              << std::endl;

    //    assert( m_data != nullptr );

    if ( m_ownData && !m_isMoved ) { delete[] m_data; }
}

// Measure::Measure( Measurement measurement ) : m_measurement( measurement ) {}

// SensorSpec::Format Measure::getFormat()
//{
//     return m_format;
// }

//// SensorSpec::Format Dof6::getFormat()
////{
////     return m_format;
//// }

// Image::Image( const unsigned char* const data, size_t size, Format format ) :
//     Measure( Measurement::IMAGE ),
//     m_data( new unsigned char[size] ),
//     m_size( size ),
//     m_format( format ) {

//    assert( m_size > 0 );
//    assert( data != nullptr );
//    memcpy( (unsigned char*)m_data, data, m_size );
//}

// Image::Image( Image&& image ) :
//     Measure(image.m_measurement),
//     //    Measure( image.m_format ),
//     m_data( image.m_data ),
//     m_size( image.m_size ),
//     m_format( image.m_format ) {
//     image.m_isMoved = true;
// }

// Image::~Image() {
//     assert( m_data != nullptr );

//    if ( !m_isMoved ) { delete[] m_data; }
//}

// SensorSpec::Format Image::getFormat()
//{
//     return m_format;
// }

// Acquisition::Acquisition(long long start, long long end , hub::Acquisition::Args args) : m_start(
// start ), m_end( end ) {

//    assert( m_start <= m_end );
//}

// Acquisition::Acquisition( long long start,
//                           long long end,
//                           Measure&& measure,
//                           std::initializer_list<Measure> otherMeasures ) :
//     m_start( start ), m_end( end ) {
//     assert( m_start <= m_end );

//    assert( m_measures.find( measure.m_format ) == m_measures.end() );
//    m_measures.insert(
//        std::pair<SensorSpec::Format, const Measure>( measure.m_format, std::move( measure ) ) );

//    for ( auto&& otherMeasure : otherMeasures ) {
//        assert( m_measures.find( otherMeasure.m_format ) == m_measures.end() );
//        m_measures.insert( std::pair<SensorSpec::Format, const Measure>(
//            otherMeasure.m_format, std::move( otherMeasure ) ) );
//    }
//}

Acquisition::Acquisition( long long start, long long end ) :
    m_start( start ),
    m_end( end )
//    m_measures( std::move(measures) )
{
    assert( m_start <= m_end );
}

Acquisition::~Acquisition() {
    m_measures.clear();
}

// Acquisition::Acquisition( Acquisition&& acq ) noexcept :
//     m_start( acq.m_start ),
//     m_end( acq.m_end ),
//     m_measures(std::move(acq.m_measures)),
//     m_size(acq.m_size)
//     {
//     //    m_data( acq.m_data ), m_size( acq.m_size ) {
////    acq.m_isMoved = true;
//    assert(m_size == acq.m_size);
////    m_size = acq.m_size;
//}

bool Acquisition::operator==( const Acquisition& acq ) const {
    if ( m_start == acq.m_start && m_end == acq.m_end && m_size == acq.m_size &&
         m_measures.size() == acq.m_measures.size() ) {

        for ( int i = 0; i < m_measures.size(); ++i ) {
            if ( m_measures.at( i ) != acq.m_measures.at( i ) ) { return false; }
        }

        //        && m_size == acq.m_size ) {
        //        return memcmp( m_data, acq.m_data, m_size ) == 0;
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
    assert( 0.0 <= t <= 1.0 );
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
//     assert(left.getMeasures().size() == right.getMeasures().size());
//     assert(left.m_start <= time && time <= right.m_start);
//     for (int iMeasure = 0; iMeasure < left.getMeasures().size(); ++iMeasure) {
////        left.getMeasures().at(iMeasure).
//    }

//}

Acquisition& Acquisition::operator<<( const Measures& measures ) {
    for ( const auto& measure : measures ) {
        Acquisition::operator<<( measure.clone() );
    }
    return *this;
}

// Acquisition& Acquisition::operator<<( Measure&& measure ) {
//     assert( m_measures.find( measure.m_format ) == m_measures.end() );
//     //    m_measures[measure.m_format] = std::move(measure);
//     m_measures.insert(
//         std::pair<SensorSpec::Format, const Measure&>( measure.m_format, std::move( measure ) )
//         );
//     return *this;
// }

// const Measure& Acquisition::get( SensorSpec::Format format ) const {
//     assert( m_measures.find( format ) != m_measures.end() );
//     return m_measures.at( format );
// }

Acquisition Acquisition::clone() const {
    //    assert( m_data != nullptr );

    Acquisition acq( m_start, m_end );

    for ( const auto& measure : m_measures ) {
        acq << measure.clone();
    }

    return acq;

    //    return Acquisition( m_start, m_end, m_data, m_size );
    //    return Acquisition( m_start, m_end );
}

// const std::list<Measure> &Acquisition::getMeasures() const
const Measures& Acquisition::getMeasures() const {
    return m_measures;
}

// const std::vector<Measure> &Acquisition::getMeasures() const
//{
//     return m_measures;
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
    //        os << ", ptr = " << (uint64_t)acq.m_data;
    return os;
}

} // namespace hub
