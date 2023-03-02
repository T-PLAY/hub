#include "Measure.hpp"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>

namespace hub {

Dof6::Dof6( const Measure& measure ) :
    Measure( measure.m_data, measure.m_size, Resolution { { 1 }, Format::DOF6 } ) {
    assert( measure.m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
    assert( m_data != nullptr );
}

Dof6::Dof6( float x, float y, float z, float w0, float w1, float w2, float w3 ) :
    Measure( reinterpret_cast<unsigned char*>( new float[7] { x, y, z, w0, w1, w2, w3 } ),
             28,
             Resolution { { 1 }, Format::DOF6 } ) {
    //    m_ownData = true;
    //    assert( m_ownData == true );
    assert( m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
}

Dof6 Dof6::slerp(const Dof6& left, const Dof6& right, double t ) {

    const float x = ( 1.0 - t ) * left.m_x + t * right.m_x;
    const float y = ( 1.0 - t ) * left.m_y + t * right.m_y;
    const float z = ( 1.0 - t ) * left.m_z + t * right.m_z;

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

Mat4::Mat4( const float* array ) :
    Measure( reinterpret_cast<const unsigned char*>( array ),
             64,
             Resolution { { 1 }, Format::MAT4 } ) {
    assert( m_size == 64 );
}

// Dof6::~Dof6() {
//}

// Measure::Measure( const unsigned char* const data, uint64_t size, bool floatData ) :

//}

// Measure::Measure( Measurement measurement ) : m_measurement( measurement ) {}

// SensorSpec::Format Measure::getFormat()
//{
// }

Measure::~Measure() {

    //    if ( m_ownData && !m_isMoved ) {
    if ( m_ownData && !m_isMoved ) {
        //    std::cout << "[Measure] delete data pointer of size : " << m_size << std::endl;
        assert( m_data != nullptr );
        delete[] m_data;
        //        m_data = nullptr;
    }
}

// Measure::Measure( const unsigned char* const data,
//                   uint64_t size,
//                   Resolution&& resolution ) :
//     m_data( new unsigned char[size] ),
//     m_size( size ),
//     m_ownData( true ),
//     m_resolution( std::move( resolution ) ) {
//     assert( data != nullptr );
//     memcpy( (unsigned char*)m_data, data, m_size );
//     assert( m_size > 0 );
//     assert( m_data != nullptr );
//     assert(size == computeAcquisitionSize(resolution));
// }

// Measure::Measure( unsigned char* data, uint64_t size, Resolution&& resolution ) :
//     m_data( data ),
//     m_size( size ),
//     m_resolution( std::move( resolution ) )
////    m_ownData( false )
//{
//    //    std::cout << "[Measure] steal data pointer of size : " << size << std::endl;
//    assert( data != nullptr );
//    assert( m_size > 0 );
//    assert( m_data != nullptr );
//    assert(size == computeAcquisitionSize(resolution));
//}

Measure::Measure( Measure&& measure ) :
    m_data( measure.m_data ),
    m_size( measure.m_size ),
    m_ownData( measure.m_ownData ),
    m_resolution( std::move( measure.m_resolution ) ) {

    measure.m_isMoved = true;
    //    measure.m_data = nullptr;
}

// Measure::~Measure() {

//    //    if ( m_ownData && !m_isMoved ) {
//    if ( m_ownData && !m_isMoved ) {
//        //    std::cout << "[Measure] delete data pointer of size : " << m_size << std::endl;
//        assert( m_data != nullptr );
//        delete[] m_data;
//        //        m_data = nullptr;
//    }
//}

// Measure::Measure( unsigned char* data, uint64_t size ) : m_data( data ), m_size( size ) {
// }

// Measure::Measure( Measure&& measure ) :
// }

// Measure::~Measure() {
// }

Measure Measure::clone() const {
    //    Measure ret( m_data, m_size, m_resolution );
    ////    ret.m_resolution = m_resolution;
    //    return ret;
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
    assert( left.isInterpolable() );
    assert( right.isInterpolable() );
    assert( left.m_resolution == right.m_resolution );

    switch ( left.m_resolution.second ) {
    case Format::DOF6: {
        return Dof6::slerp( Dof6( left ), Dof6( right ), t );
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
    os << measure.getResolution() << ", [";

    if ( measure.m_resolution.second == Format::DOF6 ) {
        const hub::Dof6 dof6(measure);
        os << dof6;
        //        const float* const dof6 = reinterpret_cast<const float* const>(measure.m_data);
        //        os << "dof6:";
        //        for ( auto i = 0; i < 7; ++i ) {
        //            os << std::setw( 3 ) << dof6[i] << " ";
        //        }
    }
    else {
        for ( auto i = 0; i < std::min( (int)measure.m_size, 10 ); ++i ) {
            os << std::setw( 3 ) << (int)measure.m_data[i] << " ";
        }
    }
    os << "]";
    return os;
}

} // namespace hub
