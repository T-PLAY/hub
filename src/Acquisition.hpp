#pragma once

#include <cassert>
#include <iostream>
#include <list>

#include "Macros.hpp"
#include "SensorSpec.hpp"

namespace hub {

// enum class Measurement { NONE, IMAGE, DOF6, COUNT };

class SRC_API Measure
{

  public:
    Measure( const unsigned char* const data, uint64_t size );
    Measure( unsigned char* data, uint64_t size );
    Measure( Measure&& measure );
    Measure( const Measure& )            = delete;
    Measure& operator=( const Measure& ) = delete;
    Measure& operator=( Measure&& )      = delete;

    ~Measure();

    Measure clone() const;

    bool interpolable() const;
    static Measure slerp( const Measure& left, const Measure& right, double t );

    const unsigned char* const m_data;
    const uint64_t m_size; // compatibility 32/64 bits

    bool operator==( const Measure& measure ) const;
    bool operator!=( const Measure& measure ) const;
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measure& measure );

    const hub::SensorSpec::Resolution &getResolution() const;

protected:
    bool m_ownData = false;
    bool m_isMoved = false;
    hub::SensorSpec::Resolution m_resolution;

  private:
    friend class InputSensor;
};
using Measures = std::vector<Measure>;

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

// measure_tag<Dof6> = SensorSpec::
// template <typename T>
// constexpr Measurement measure_format_tag = Measurement::NONE;
// template <>
// static constexpr Measurement measure_format_tag<Dof6> = Measurement::DOF6;
// template <>
// static constexpr Measurement measure_format_tag<Image> = Measurement::IMAGE;
// template <> SensorSpec::Format measure_version_tag<Image> = SensorSpec::Format::;

///
/// \brief The Acquisition class
/// represents the data with the start and the end of the acquire process.
///
class SRC_API Acquisition
{
  public:
    ///
    /// \brief Acquisition
    /// \param start
    /// timestamp in microseconds
    /// \param end
    /// timestamp in microseconds
    /// \param data
    /// \param size
    ///
    Acquisition( long long start, long long end );
    ~Acquisition();

    Acquisition( const Acquisition& acq ) = delete;
    Acquisition( Acquisition&& acq )      = default;

    Acquisition& operator=( const Acquisition& acq ) = delete;
    Acquisition& operator=( Acquisition&& acq )      = delete;

    bool operator==( const Acquisition& acq ) const;
    bool operator!=( const Acquisition& acq ) const;

    Acquisition& operator<<( Measure&& measure );
    Acquisition& operator<<( const Measures& measure );

    void addMeasure( unsigned char * data, uint64_t size);
//    template<typename... Values>
//    void add(Values...);

    bool interpolable() const;
    static Acquisition slerp( const Acquisition& left, const Acquisition& right, double t );

  public:
    Acquisition clone() const;

  public:
    const long long m_start; // microseconds
    const long long m_end;   // microseconds
  private:
    Measures m_measures;
    size_t m_size = 0;

  public:
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );
    size_t getSize() const;
    const Measures& getMeasures() const;

    friend class InputSensor;
};

// template <>
// inline const Dof6& Acquisition::get<Dof6>() const {
// }

// template <typename Arg, typename... Args>
// Acquisition::Acquisition( long long start, long long end, Args... args ) :

/// std::move(measure)));

//}

} // namespace hub
