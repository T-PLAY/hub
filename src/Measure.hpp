#pragma once

#include <iostream>
#include <cassert>
#include <cstring>

#include "Macros.hpp"
#include "Resolution.hpp"

namespace hub {


class SRC_API Measure
{

  public:
    template <class ResolutionT = Resolution>
    Measure( const unsigned char* const data, uint64_t size, ResolutionT && resolution );
    template <class ResolutionT = Resolution>
    Measure( unsigned char* data, uint64_t size, ResolutionT && resolution );

    Measure( Measure&& measure );
    Measure( const Measure& )            = delete;
    Measure& operator=( const Measure& ) = delete;
    Measure& operator=( Measure&& )      = delete;

    ~Measure();

    Measure clone() const;

    bool isInterpolable() const;
    static Measure slerp( const Measure& left, const Measure& right, double t );

    const unsigned char* const m_data;
    const uint64_t m_size; // compatibility 32/64 bits

    bool operator==( const Measure& measure ) const;
    bool operator!=( const Measure& measure ) const;
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measure& measure );

    const inline Resolution &getResolution() const;

protected:
    bool m_ownData = false;
    bool m_isMoved = false;
    Resolution m_resolution;

  private:
    friend class InputSensor;
};
using Measures = std::vector<Measure>;

template <class ResolutionT>
Measure::Measure( const unsigned char* const data,
                  uint64_t size,
                  ResolutionT&& resolution ) :
    m_data( new unsigned char[size] ),
    m_size( size ),
    m_ownData( true ),
    m_resolution( std::move( resolution ) ) {
//    m_resolution( std::forward<Resolution>( resolution ) ) {
//    m_resolution( std::forward( resolution ) ) {
//    m_resolution( std::forward<ResolutionT>( resolution ) ) {

    static_assert(std::is_same<std::decay_t<Resolution>,std::decay_t<ResolutionT>>::value,
                     "must be the same as Resolution");
//    static_assert(std::is_base_of<Resolution, ResolutionT>::value, "not Resolution");
//    static_assert(std::is_same<Resolution, ResolutionT>::value, "not Resolution");
//    static_assert(std::is_same<ResolutionT, Resolution>::value, "not Resolution");

    assert( data != nullptr );
    memcpy( (unsigned char*)m_data, data, m_size );
    assert( m_size > 0 );
    assert( m_data != nullptr );
    assert(size == computeAcquisitionSize(m_resolution));
}

template <class ResolutionT>
Measure::Measure( unsigned char* data, uint64_t size, ResolutionT&& resolution ) :
    m_data( data ),
    m_size( size ),
    m_resolution( std::move( resolution ) )
//    m_resolution( std::forward( resolution ) )
//    m_resolution( std::forward<ResolutionT>( resolution ) )
//    m_ownData( false )
{
//    static_assert(std::is_base_of<hub::Resolution, ResolutionT>::value, "not Resolution");
    static_assert(std::is_same<std::decay_t<Resolution>,std::decay_t<ResolutionT>>::value,
                     "must be the same as Resolution");

    //    std::cout << "[Measure] steal data pointer of size : " << size << std::endl;
    assert( data != nullptr );
    assert( m_size > 0 );
    assert( m_data != nullptr );
    assert(size == computeAcquisitionSize(m_resolution));
}

const inline Resolution& Measure::getResolution() const {
    return m_resolution;
}

class SRC_API Dof6 : public Measure
{
  public:
    Dof6( const Measure& measure );
    Dof6( float x  = 0.0,
          float y  = 0.0,
          float z  = 0.0,
          float w0 = 1.0,
          float w1 = 0.0,
          float w2 = 0.0,
          float w3 = 0.0 );
    // private:
    const float m_x = 0.0, m_y = 0.0, m_z = 0.0;                // vec3
    const float m_w0 = 1.0, m_w1 = 0.0, m_w2 = 0.0, m_w3 = 0.0; // quat : w, x, y, z

    static Dof6 slerp( const Dof6& left, const Dof6& right, long long t );

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Dof6& dof6 );

  private:
};

class Mat4 : public Measure
{
  public:
    Mat4( const float* array );
};

// class Image : public Measure
//{

//};

} // namespace hub
