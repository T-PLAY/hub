/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2024/01/03

#pragma once

#include <memory>

#include <cstring>

#include "Macros.hpp"
#include "Traits.hpp"

namespace hub {

namespace Types {
enum Format : TypeId_t {
    MAT4 = Cpp_Count, // 3
    DENSITY, // 4
    DISTANCE, // 5
    RGB8, // 6
    RGBA8, // 7
    BGR8, // 8
    Y8, // 9
    Y16, // 10
    Z16, // 11
    POSITION, // 12
    ORIENTATION, // 13
    DOF6, // 14
    XYZ32F, // 15
    VEC3, // 16
    VEC4, // 17

    Format_Count // 18
};

static constexpr auto Count = Format_Count;
} // namespace Types

namespace format {

// static inline int nChannel(Types::Format format) {
static inline int nChannel(TypeId_t format) {
    switch (format) {
    case Types::Y8:
        return 1;
    case Types::Y16:
    case Types::Z16:
        return 2;
    case Types::RGB8:
    case Types::BGR8:
        return 3;
    case Types::RGBA8:
        return 4;
    default:
        return 1;
    }
}

/////////////////////////////////////// 1D ////////////////////////////////////////

// Transform matrix 4x4 of float.
///
/// \brief The Mat4 class
///
struct Mat4 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::MAT4;

    ///
    /// \brief packable
    ///
    static struct {
    } packable;

    // float data[16];
    ///
    /// \brief m_array
    ///
    std::array<float, 16> m_array;

    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Mat4"; };

    // clang-format off
    ///
    /// \brief Mat4
    /// \param a11
    /// \param a12
    /// \param a13
    /// \param a14
    /// \param a21
    /// \param a22
    /// \param a23
    /// \param a24
    /// \param a31
    /// \param a32
    /// \param a33
    /// \param a34
    /// \param a41
    /// \param a42
    /// \param a43
    /// \param a44
    ///
    constexpr Mat4(
        float a11 = 1.0, float a12 = 0.0, float a13 = 0.0, float a14 = 0.0,
        float a21 = 0.0, float a22 = 1.0, float a23 = 0.0, float a24 = 0.0,
        float a31 = 0.0, float a32 = 0.0, float a33 = 1.0, float a34 = 0.0,
        float a41 = 0.0, float a42 = 0.0, float a43 = 0.0, float a44 = 1.0 )
        : m_array{
                a11, a12, a13, a14,
                a21, a22, a23, a24,
                a31, a32, a33, a34,
                a41, a42, a43, a44
              }
    {
    }
    // clang-format on

    // constexpr Mat4( float value ) :
    //     m_array { value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value,
    //            value } {}

    ///
    /// \brief Mat4
    /// \param array
    ///
    Mat4( const float* array ) { memcpy( m_array.data(), array, 64 ); }

    ///
    /// \brief toString
    ///
    auto toString() const {
        float dataTmp[16];
        memcpy( dataTmp, m_array.data(), 64 );

        std::string str = "[";
        for ( int i = 0; i < 4; ++i ) {
            for ( int j = 0; j < 4; ++j ) {
                char buff[32];
#ifdef WIN32
                sprintf_s( buff, "%.0f ", dataTmp[4 * i + j] );
#else
#    ifdef OS_MACOS
                snprintf( buff, 32, "%.0f", dataTmp[4 * i + j] );
#    else
                sprintf( buff, "%.0f", dataTmp[4 * i + j] );
#    endif
#endif
                str += buff;
                if ( j != 3 ) str += " ";
            }
            if ( i != 3 ) str += ", ";
        }
        (void)dataTmp;
        return str + "]";
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Mat4& other ) const {
        return m_array == other.m_array;
        // return !memcmp( data, other.data, 64 );
    }

    ///
    /// \brief data
    /// \return
    ///
    const float * data() const {
        return m_array.data();
    }

#if CPP_VERSION <= 17
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        // archive( self.data );
        archive(self.m_array);
    }
#endif
};
static_assert( sizeof( Mat4 ) == 64 );

///
/// \brief The Vec3 class
///
struct Vec3 {
    ///
    /// \brief x
    ///
    float x;
    ///
    /// \brief y
    ///
    float y;
    ///
    /// \brief z
    ///
    float z;

    ///
    /// \brief Vec3
    /// \param value
    ///
    constexpr Vec3( float value = 0.0f ) : x { value }, y { value }, z { value } {};
    ///
    /// \brief Vec3
    /// \param x_
    /// \param y_
    /// \param z_
    ///
    constexpr Vec3( float x_, float y_, float z_ ) : x { x_ }, y { y_ }, z { z_ } {};
    ///
    /// \brief toString
    ///
    auto toString() const { return hub::to_string( x, y, z ); }
#if CPP_VERSION <= 17
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.x, self.y, self.z );
    }
#endif
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::VEC3;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Vec3"; };
    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Vec3& other ) const {
        return x == other.x && y == other.y && z == other.z;
    }
};
static_assert( sizeof( Vec3 ) == 12 );

///
/// \brief The Vec4 class
///
struct Vec4 {
    ///
    /// \brief x
    ///
    float x;
    ///
    /// \brief y
    ///
    float y;
    ///
    /// \brief z
    ///
    float z;
    ///
    /// \brief w
    ///
    float w;
    ///
    /// \brief Vec4
    /// \param value
    ///
    constexpr Vec4( float value = 0.0f ) : x { value }, y { value }, z { value }, w { value } {};
    ///
    /// \brief Vec4
    /// \param x_
    /// \param y_
    /// \param z_
    /// \param w_
    ///
    constexpr Vec4( float x_, float y_, float z_, float w_ ) :
        x { x_ }, y { y_ }, z { z_ }, w { w_ } {};
    ///
    /// \brief toString
    ///
    auto toString() const { return hub::to_string( x, y, z, w ); }
#if CPP_VERSION <= 17
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.x, self.y, self.z, self.w );
    }
#endif
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::VEC4;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Vec4"; };
    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Vec4& other ) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
};
static_assert( sizeof( Vec4 ) == 16 );

///
/// \brief The Density class
///
struct Density {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::DENSITY;
    // uint32_t value;
    ///
    /// \brief value
    ///
    float value;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Density"; };
};
static_assert( sizeof( Density ) == 4 );

///
/// \brief The Distance class
///
struct Distance {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::DISTANCE;
    ///
    /// \brief value
    ///
    float value;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Distance"; };
};
static_assert( sizeof( Distance ) == 4 );

/////////////////////////////////////// 2D ////////////////////////////////////////

///
/// \brief The RGB8 class
///
struct RGB8 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::RGB8;
    ///
    /// \brief RGB8
    ///
    RGB8()                   = default;
    ///
    /// \brief RGB8
    /// \param _r
    /// \param _g
    /// \param _b
    ///
    template <class T>
    constexpr RGB8( T _r, T _g, T _b ) :
        r { (unsigned char)_r }, g { (unsigned char)_g }, b { (unsigned char)_b } {};
    ///
    /// \brief r
    ///
    unsigned char r;
    ///
    /// \brief g
    ///
    unsigned char g;
    ///
    /// \brief b
    ///
    unsigned char b;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "RGB8"; };
};
static_assert( sizeof( RGB8 ) == 3 );

///
/// \brief The RGBA8 class
///
struct RGBA8 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::RGBA8;
    ///
    /// \brief r
    ///
    unsigned char r;
    ///
    /// \brief g
    ///
    unsigned char g;
    ///
    /// \brief b
    ///
    unsigned char b;
    ///
    /// \brief a
    ///
    unsigned char a;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "RGBA8"; };
};
static_assert( sizeof( RGBA8 ) == 4 );

///
/// \brief The BGR8 class
///
struct BGR8 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::BGR8;
    ///
    /// \brief b
    ///
    unsigned char b;
    ///
    /// \brief g
    ///
    unsigned char g;
    ///
    /// \brief r
    ///
    unsigned char r;
    /// \brief name
    static constexpr auto name() { return "BGR8"; };
};
static_assert( sizeof( BGR8 ) == 3 );

///
/// \brief The Y8 class
///
struct Y8 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::Y8;
    ///
    /// \brief y
    ///
    unsigned char y;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Y8"; };
};
static_assert( sizeof( Y8 ) == 1 );

///
/// \brief The Y16 class
///
struct Y16 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::Y16;
    ///
    /// \brief y
    ///
    uint16_t y;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Y16"; };
};

///
/// \brief The Z16 class
///
struct Z16 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::Z16;
    ///
    /// \brief depth
    ///
    uint16_t depth; // meters
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Z16"; };
};
static_assert( sizeof( Z16 ) == 2 );

/////////////////////////////////////// 3D ////////////////////////////////////////

///
/// \brief The Position class
///
struct Position // Cartesian
{
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::POSITION;
    ///
    /// \brief x
    ///
    float x;
    ///
    /// \brief y
    ///
    float y;
    ///
    /// \brief z
    ///
    float z;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Position"; };
};

///
/// \brief The Orientation class
///
struct Orientation // Euler
{
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::ORIENTATION;
    ///
    /// \brief rx
    ///
    float rx;
    ///
    /// \brief ry
    ///
    float ry;
    ///
    /// \brief rz
    ///
    float rz;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Orientation"; };
};

// Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion
///
/// \brief The Dof6 class
///
struct Dof6 {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::DOF6;
    ///
    /// \brief x
    ///
    float x;
    ///
    /// \brief y
    ///
    float y;
    ///
    /// \brief z
    ///
    float z;
    ///
    /// \brief w0
    ///
    float w0; // w
    ///
    /// \brief w1
    ///
    float w1; // x
    ///
    /// \brief w2
    ///
    float w2; // y
    ///
    /// \brief w3
    ///
    float w3; // z
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Dof6"; };
    ///
    /// \brief Dof6
    /// \param _x
    /// \param _y
    /// \param _z
    /// \param _w0
    /// \param _w1
    /// \param _w2
    /// \param _w3
    ///
    constexpr Dof6( float _x  = 0.0,
                    float _y  = 0.0,
                    float _z  = 0.0,
                    float _w0 = 1.0,
                    float _w1 = 0.0,
                    float _w2 = 0.0,
                    float _w3 = 0.0 ) :
        x { _x }, y { _y }, z { _z }, w0 { _w0 }, w1 { _w1 }, w2 { _w2 }, w3 { _w3 } {}

    ///
    /// \brief Dof6
    /// \param array
    ///
    Dof6( const float* array ) { memcpy( this, array, 28 ); }

    ///
    /// \brief toString
    ///
    auto toString() const {
        return "x:" + std::to_string( x ) + " y:" + std::to_string( y ) +
               " z:" + std::to_string( z ) + " w0:" + std::to_string( w0 ) +
               " w1:" + std::to_string( w1 ) + " w2:" + std::to_string( w2 ) +
               " w3:" + std::to_string( w3 );
    }
};
static_assert( sizeof( Dof6 ) == 28 );

// Point consist of x, y and z position, rgb color and depth .

///
/// \brief The XYZ32F class
///
struct XYZ32F {
    ///
    /// \brief id
    ///
    static constexpr auto id = Types::Format::XYZ32F;
    ///
    /// \brief x
    ///
    float x;
    ///
    /// \brief y
    ///
    float y;
    ///
    /// \brief z
    ///
    float z;
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "XYZ32F"; };
};
static_assert( sizeof( XYZ32F ) == 12 );

} // namespace format

} // namespace hub
