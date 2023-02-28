#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "Macros.hpp"
#include "Resolution.hpp"

namespace hub {

///
/// \brief The Measure class
///
class SRC_API Measure
{
  public:
    ///
    /// \brief Measure
    /// is used when the data sensor is volatile
    /// \param data
    /// we want to temporaly copy before share through the communication bus.
    /// \param size
    /// of the data array in bytes.
    /// \param resolution
    /// needs to be compatible with the sensorSpec when you send this measure through the
    /// OutputSensor.
    ///
    template <class ResolutionT = Resolution>
    Measure( const unsigned char* const data, uint64_t size, ResolutionT&& resolution );

    ///
    /// \brief Measure
    /// is used when the data sensor is non-volatile
    /// \param data
    /// we want to use as pointer data before share through the communication bus.
    /// \param size
    /// of the data array in bytes.
    /// \param resolution
    /// needs to be compatible with the sensorSpec when you send this measure through the
    /// OutputSensor. \param stealData is on when you want the measure to be the owner of the
    /// external pointer data, that imply the deletion of data when measure is destruct.
    ///
    template <class ResolutionT = Resolution>
    Measure( unsigned char* data, uint64_t size, ResolutionT&& resolution, bool stealData = false );

    ///
    /// \brief Measure
    /// movable constructor.
    /// \param measure
    /// to move.
    ///
    Measure( Measure&& measure );
    Measure& operator=( Measure&& ) = delete;

    Measure( const Measure& )            = delete;
    Measure& operator=( const Measure& ) = delete;

    ~Measure();

  public:
    ///
    /// \brief clone
    /// make a copy of current measure.
    /// \return
    /// copy of current measure.
    ///
    Measure clone() const;

    /// \brief isInterpolable
    /// is used during synchronization with another acquisition stream to minimize
    /// the error distance between 2 differents acquisition of differents sensor stream.
    /// \return
    /// true if measure is interpolable
    /// false otherwise
    ///
    bool isInterpolable() const;

    ///
    /// \brief slerp
    /// is used during synchronization with another acquisition stream to minimize
    /// the error distance between 2 differents acquisition of differents sensor stream.
    /// \param left
    /// oldest measure
    /// \param right
    /// newest measure
    /// \param t
    /// to perform interpolation
    /// \return
    /// interpolate new created measure.
    ///
    static Measure slerp( const Measure& left, const Measure& right, double t );

    ///
    /// \brief operator ==
    /// \param measure
    /// \return
    ///
    bool operator==( const Measure& measure ) const;

    ///
    /// \brief operator !=
    /// \param measure
    /// \return
    ///
    bool operator!=( const Measure& measure ) const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param measure
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measure& measure );

  public:
    ///
    /// \brief m_data
    ///
    const unsigned char* const m_data;

    ///
    /// \brief m_size
    ///
    const uint64_t m_size; // compatibility 32/64 bits

  public: // getters
    ///
    /// \brief getResolution
    /// is used to check compatible resolution with sensor specification of OutputSensor.
    /// \return
    /// internal resolution.
    ///
    const inline Resolution& getResolution() const;

  private:
    bool m_ownData = false;
    bool m_isMoved = false;
    Resolution m_resolution;

    friend class InputSensor;
};
using Measures = std::vector<Measure>;

template <class ResolutionT>
Measure::Measure( const unsigned char* const data, uint64_t size, ResolutionT&& resolution ) :
    m_data( new unsigned char[size] ),
    m_size( size ),
    m_ownData( true ),
    m_resolution( std::move( resolution ) ) {
    //    m_resolution( std::forward<Resolution>( resolution ) ) {
    //    m_resolution( std::forward( resolution ) ) {
    //    m_resolution( std::forward<ResolutionT>( resolution ) ) {

    static_assert( std::is_same<std::decay_t<Resolution>, std::decay_t<ResolutionT>>::value,
                   "must be the same as Resolution" );
    //    static_assert(std::is_base_of<Resolution, ResolutionT>::value, "not Resolution");
    //    static_assert(std::is_same<Resolution, ResolutionT>::value, "not Resolution");
    //    static_assert(std::is_same<ResolutionT, Resolution>::value, "not Resolution");

    assert( data != nullptr );
    memcpy( const_cast<unsigned char*>( m_data ), data, m_size );
    assert( m_size > 0 );
    assert( m_data != nullptr );
    assert( size == computeAcquisitionSize( m_resolution ) );
}

template <class ResolutionT>
Measure::Measure( unsigned char* data, uint64_t size, ResolutionT&& resolution, bool stealData ) :
    m_data( data ),
    m_size( size ),
    m_ownData( stealData ),
    m_resolution( std::move( resolution ) )
//    m_resolution( std::forward( resolution ) )
//    m_resolution( std::forward<ResolutionT>( resolution ) )
//    m_ownData( false )
{
    //    static_assert(std::is_base_of<hub::Resolution, ResolutionT>::value, "not Resolution");
    static_assert( std::is_same<std::decay_t<Resolution>, std::decay_t<ResolutionT>>::value,
                   "must be the same as Resolution" );

    //    std::cout << "[Measure] steal data pointer of size : " << size << std::endl;
    assert( data != nullptr );
    assert( m_size > 0 );
    assert( m_data != nullptr );
    assert( size == computeAcquisitionSize( m_resolution ) );
}

const inline Resolution& Measure::getResolution() const {
    return m_resolution;
}

///
/// \brief The Dof6 class
/// represents a 6 degree of freedom measure that can be acquired with some tracking solution.
///
class SRC_API Dof6 : public Measure
{
  public:
    ///
    /// \brief Dof6
    /// copy constructor.
    /// \param measure
    /// to copy.
    ///
    Dof6( const Measure& measure );

    ///
    /// \brief Dof6
    /// corresponds to a position (x, y, z) and an orientation quaternion (w0, w1, w2, w3).
    /// \param x
    /// position
    /// \param y
    /// position
    /// \param z
    /// position
    /// \param w0
    /// orientation
    /// \param w1
    /// orientation
    /// \param w2
    /// orientation
    /// \param w3
    /// orientation
    ///
    explicit Dof6( float x  = 0.0,
          float y  = 0.0,
          float z  = 0.0,
          float w0 = 1.0,
          float w1 = 0.0,
          float w2 = 0.0,
          float w3 = 0.0 );

    ///
    /// \brief slerp
    /// \param left
    /// \param right
    /// \param t
    /// \return
    ///
    static Dof6 slerp( const Dof6& left, const Dof6& right, long long t );

    ///
    /// \brief operator <<
    /// \param os
    /// \param dof6
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Dof6& dof6 );

    ///
    /// \brief x
    /// \return
    ///
    inline float get_x() const;

    ///
    /// \brief y
    /// \return
    ///
    inline float get_y() const;

    ///
    /// \brief z
    /// \return
    ///
    inline float get_z() const;

    ///
    /// \brief w0
    /// \return
    ///
    inline float get_w0() const;

    ///
    /// \brief w1
    /// \return
    ///
    inline float get_w1() const;

    ///
    /// \brief w2
    /// \return
    ///
    inline float get_w2() const;

    ///
    /// \brief w3
    /// \return
    ///
    inline float get_w3() const;

  private:
    const float m_x = 0.0, m_y = 0.0, m_z = 0.0;                // vec3
    const float m_w0 = 1.0, m_w1 = 0.0, m_w2 = 0.0, m_w3 = 0.0; // quat : w, x, y, z
};

inline float Dof6::get_x() const {
    return m_x;
}

inline float Dof6::get_y() const {
    return m_y;
}

inline float Dof6::get_z() const {
    return m_z;
}

inline float Dof6::get_w0() const {
    return m_w0;
}

inline float Dof6::get_w1() const {
    return m_w1;
}

inline float Dof6::get_w2() const {
    return m_w2;
}

inline float Dof6::get_w3() const {
    return m_w3;
}


///
/// \brief The Mat4 class
/// represents a transform matrix (row-major).
///
class Mat4 : public Measure
{
  public:
    ///
    /// \brief Mat4
    /// is the 4x4 transformation matrix (row-major).
    /// \param array
    /// of 4x4 float (size = 4x4x4 = 64 bytes)
    ///
    explicit Mat4( const float* array );
};

} // namespace hub
