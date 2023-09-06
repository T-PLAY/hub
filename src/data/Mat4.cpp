#include "Mat4.hpp"

#include <iostream>

namespace hub {
namespace data {

Mat4::Mat4( const Measure& measure ) :
    Measure( measure.getData(), measure.getSize(), Resolution { { 1 }, Format::MAT4 } ) {
    assert( measure.getSize() == 64 );
    assert( m_data != nullptr );
}

Mat4::Mat4( const Mat4& mat4 ) :
    Measure( reinterpret_cast<unsigned char*>( new float[16] ),
             64,
             Resolution { { 1 }, Format::MAT4 },
             true ) {
    memcpy( m_data, mat4.m_data, 64 );
}

Mat4::Mat4( float* array ) :
    Measure( reinterpret_cast<unsigned char*>( new float[16] ),
             64,
             Resolution { { 1 }, Format::MAT4 },
             true ) {
    memcpy( m_data, array, 64 );
    assert( m_size == 64 );
}

Mat4::Mat4( const float* array ) :
    Measure( reinterpret_cast<unsigned char*>( new float[16] ),
             64,
             Resolution { { 1 }, Format::MAT4 },
             true ) {
    memcpy( m_data, array, 64 );
    assert( m_size == 64 );
}

Mat4::Mat4( float r0c0,
            float r0c1,
            float r0c2,
            float r0c3,
            float r1c0,
            float r1c1,
            float r1c2,
            float r1c3,
            float r2c0,
            float r2c1,
            float r2c2,
            float r2c3,
            float r3c0,
            float r3c1,
            float r3c2,
            float r3c3 ) :
    Measure( reinterpret_cast<unsigned char*>( new float[16] ),
             64,
             Resolution { { 1 }, Format::MAT4 },
             true ) {

    float params[16] { r0c0,
                       r0c1,
                       r0c2,
                       r0c3,
                       r1c0,
                       r1c1,
                       r1c2,
                       r1c3,
                       r2c0,
                       r2c1,
                       r2c2,
                       r2c3,
                       r3c0,
                       r3c1,
                       r3c2,
                       r3c3 };
    memcpy( m_data, params, 64 );
    // row major

    assert( m_size == 64 );
}

std::string Mat4::to_string() const {
    float data[16];
    memcpy( data, m_data, 64 );

    std::string str = "[";
    for ( int i = 0; i < 4; ++i ) {
        for ( int j = 0; j < 4; ++j ) {
            char buff[32];
#ifdef WIN32
            sprintf_s( buff, "%.0f ", data[4 * i + j] );
#else
#ifdef OS_MACOS
            snprintf( buff, 32, "%.0f", data[4 * i + j] );
#else
            sprintf( buff, "%.0f", data[4 * i + j] );
#endif
#endif
            str += buff;
            if ( j != 3 ) str += " ";
        }
        if ( i != 3 ) str += ", ";
    }
    (void)data;
    return str + "]";
}

const float* Mat4::operator[]( int idx ) const {
    assert( 0 <= idx && idx < 4 );
    /* cppcheck-suppress invalidPointerCast */
    return &( reinterpret_cast<const float*>(m_data) )[idx * 4];
}

std::ostream& operator<<( std::ostream& os, const Mat4& mat4 ) {
    os << mat4.getResolution() << ", [" << mat4.to_string() << "]";
    return os;
}

} // namespace data
} // namespace hub
