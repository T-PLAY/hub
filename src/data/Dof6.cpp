#include "Dof6.hpp"

#include <cmath>
#include <iostream>


namespace hub {
namespace data {

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
    assert( m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
}

Dof6 Dof6::slerp( const Dof6& left, const Dof6& right, double t ) {

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


std::ostream& operator<<( std::ostream& os, const Dof6& dof6 ) {
    os << dof6.getResolution() << ", [";
    os << "x:" << dof6.m_x << " y:" << dof6.m_y << " z:" << dof6.m_z << "   w0:" << dof6.m_w0
       << " w1:" << dof6.m_w1 << " w2:" << dof6.m_w2 << " w3:" << dof6.m_w3;
    os << "]";
    return os;
}





} // namespace data
} // namespace hub
