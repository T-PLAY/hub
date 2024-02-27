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
    MAT4 = Cpp_Count,
    DENSITY,
    DISTANCE,
    RGB8,
    RGBA8,
    BGR8,
    Y8,
    Y16,
    Z16,
    POSITION,
    ORIENTATION,
    DOF6,
    XYZ32F,
    VEC3,
    VEC4,

    Format_Count
};

static constexpr auto Count = Format_Count;
} // namespace Types

namespace format {

/////////////////////////////////////// 1D ////////////////////////////////////////

// Transform matrix 4x4 of float.
struct Mat4 {
    static constexpr auto id = Types::Format::MAT4;
    static struct {
    } packable;
    float data[16];
    static constexpr auto name() { return "Mat4"; };

    constexpr Mat4( float value = 0.0f ) :
        data { value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value,
               value } {}

    Mat4( const float* array ) { memcpy( data, array, 64 ); }

    auto toString() const {
        float dataTmp[16];
        memcpy( dataTmp, data, 64 );

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
    bool operator==( const Mat4& other ) const { return !memcmp( data, other.data, 64 ); }
#if CPP_VERSION <= 17
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.data );
    }
#endif
};
static_assert( sizeof( Mat4 ) == 64 );

struct Vec3 {
    float x;
    float y;
    float z;
    constexpr Vec3( float value = 0.0f ) : x { value }, y { value }, z { value } {};
    constexpr Vec3( float x_, float y_, float z_ ) : x { x_ }, y { y_ }, z { z_ } {};
    auto toString() const { return hub::to_string( x, y, z ); }
#if CPP_VERSION <= 17
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.x, self.y, self.z );
    }
#endif
    static constexpr auto id = Types::Format::VEC3;
    static constexpr auto name() { return "Vec3"; };
    bool operator==( const Vec3& other ) const {
        return x == other.x && y == other.y && z == other.z;
    }
};
static_assert( sizeof( Vec3 ) == 12 );

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
    constexpr Vec4( float value = 0.0f ) : x { value }, y { value }, z { value }, w { value } {};
    constexpr Vec4( float x_, float y_, float z_, float w_ ) :
        x { x_ }, y { y_ }, z { z_ }, w { w_ } {};
    auto toString() const { return hub::to_string( x, y, z, w ); }
#if CPP_VERSION <= 17
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.x, self.y, self.z, self.w );
    }
#endif
    static constexpr auto id = Types::Format::VEC4;
    static constexpr auto name() { return "Vec4"; };
    bool operator==( const Vec4& other ) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
};
static_assert( sizeof( Vec4 ) == 16 );

struct Density {
    static constexpr auto id = Types::Format::DENSITY;
    // uint32_t value;
    float value;
    static constexpr auto name() { return "Density"; };
};
static_assert( sizeof( Density ) == 4 );

struct Distance {
    static constexpr auto id = Types::Format::DISTANCE;
    float value;
    static constexpr auto name() { return "Distance"; };
};
static_assert( sizeof( Distance ) == 4 );

/////////////////////////////////////// 2D ////////////////////////////////////////

struct RGB8 {
    static constexpr auto id = Types::Format::RGB8;
    RGB8()                   = default;
    template <class T>
    constexpr RGB8( T _r, T _g, T _b ) :
        r { (unsigned char)_r }, g { (unsigned char)_g }, b { (unsigned char)_b } {};
    unsigned char r;
    unsigned char g;
    unsigned char b;
    static constexpr auto name() { return "RGB8"; };
};
static_assert( sizeof( RGB8 ) == 3 );

struct RGBA8 {
    static constexpr auto id = Types::Format::RGBA8;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    static constexpr auto name() { return "RGBA8"; };
};
static_assert( sizeof( RGBA8 ) == 4 );

struct BGR8 {
    static constexpr auto id = Types::Format::BGR8;
    unsigned char b;
    unsigned char g;
    unsigned char r;
    static constexpr auto name() { return "BGR8"; };
};
static_assert( sizeof( BGR8 ) == 3 );

struct Y8 {
    static constexpr auto id = Types::Format::Y8;
    unsigned char y;
    static constexpr auto name() { return "Y8"; };
};
static_assert( sizeof( Y8 ) == 1 );

struct Y16 {
    static constexpr auto id = Types::Format::Y16;
    uint16_t y;
    static constexpr auto name() { return "Y16"; };
};

struct Z16 {
    static constexpr auto id = Types::Format::Z16;
    uint16_t depth; // meters
    static constexpr auto name() { return "Z16"; };
};
static_assert( sizeof( Z16 ) == 2 );

/////////////////////////////////////// 3D ////////////////////////////////////////

struct Position // Cartesian
{
    static constexpr auto id = Types::Format::POSITION;
    float x;
    float y;
    float z;
    static constexpr auto name() { return "Position"; };
};

struct Orientation // Euler
{
    static constexpr auto id = Types::Format::ORIENTATION;
    float rx;
    float ry;
    float rz;
    static constexpr auto name() { return "Orientation"; };
};

// Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion
struct Dof6 {
    static constexpr auto id = Types::Format::DOF6;
    float x                  ;
    float y                  ;
    float z                  ;
    float w0                 ; // w
    float w1                 ; // x
    float w2                 ; // y
    float w3                 ; // z
    static constexpr auto name() { return "Dof6"; };

    constexpr Dof6(float _x = 0.0, float _y = 0.0, float _z = 0.0, float _w0 = 1.0, float _w1 = 0.0, float _w2 = 0.0, float _w3 = 0.0)
        : x{_x}, y{_y}, z{_z}, w0{_w0}, w1{_w1}, w2{_w2}, w3{_w3}
    {
    }

    Dof6( const float* array ) { memcpy( this, array, 28 ); }

    auto toString() const {
        return "x:" + std::to_string( x ) + " y:" + std::to_string( y ) +
               " z:" + std::to_string( z ) + " w0:" + std::to_string( w0 ) +
               " w1:" + std::to_string( w1 ) + " w2:" + std::to_string( w2 ) +
               " w3:" + std::to_string( w3 );
    }
};
static_assert( sizeof( Dof6 ) == 28 );

// Point consist of x, y and z position, rgb color and depth .

struct XYZ32F {
    static constexpr auto id = Types::Format::XYZ32F;
    float x;
    float y;
    float z;
    static constexpr auto name() { return "XYZ32F"; };
};
static_assert( sizeof( XYZ32F ) == 12 );

} // namespace format

} // namespace hub
