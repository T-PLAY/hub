#pragma once

#include <cassert>
#include <iostream>
#include <list>

#include "Macros.hpp"
#include "SensorSpec.hpp"

namespace hub {

// enum class Measurement { NONE, IMAGE, DOF6, COUNT };

class Measure
{

  public:
    Measure( const unsigned char* const data, size_t size, bool floatData = false );
    Measure( Measure&& measure );
    //    Measure( Measurement measurement );
    Measure( const Measure& ) = delete;
    //    Measure( Measure&& )                 = default;
    Measure& operator=( const Measure& ) = delete;
    Measure& operator=( Measure&& )      = delete;

    ~Measure();

    Measure clone() const;

    //  private:
    //    Measurement m_measurement;
    //    virtual SensorSpec::Format getFormat() = 0;
    const unsigned char* const m_data;
    const size_t m_size;

    bool operator==( const Measure& measure ) const;
    bool operator!=( const Measure& measure ) const;
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Measure& measure );

  protected:
    //    bool m_ownData = false;
    bool m_isMoved = false;

  private:
    //    friend class Acquisition;
};
using Measures = std::vector<Measure>;

class Dof6 : public Measure
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
    ~Dof6();
    // private:
    const float m_x = 0.0, m_y = 0.0, m_z = 0.0;
    const float m_w0 = 1.0, m_w1 = 0.0, m_w2 = 0.0, m_w3 = 0.0;
    //    SensorSpec::Format getFormat() override;

  private:
    //    constexpr static SensorSpec::Format m_format = SensorSpec::Format::DOF6;
};

// class Image : public Measure
//{
//   public:
//     Image( const unsigned char* const data, size_t size, Format format );
//     Image( Image&& image );
//     ~Image();
//     // private:
//     const unsigned char* const m_data;
//     const size_t m_size;

////    Format m_format;

//    //        SensorSpec::Format getFormat() override;

//  private:
//    bool m_isMoved = false;
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
    //    template <typename Arg, typename... Args>
//    Acquisition( long long start, long long end, Measures && measures = {} );
    Acquisition( long long start, long long end );
    //    Acquisition( long long start,
    //                 long long end,
    //                 Measure&& measure,
    //                 std::initializer_list<Measure> otherMeasures = {} );
    //                 const unsigned char* const data,
    //                 size_t size );
    ~Acquisition();

    Acquisition( const Acquisition& acq ) = delete;
//    Acquisition( Acquisition&& acq ) noexcept;
    Acquisition( Acquisition&& acq ) = default;

    Acquisition& operator=( const Acquisition& acq ) = delete;
    Acquisition& operator=( Acquisition&& acq )      = delete;

    bool operator==( const Acquisition& acq ) const;
    bool operator!=( const Acquisition& acq ) const;

    Acquisition& operator<<( Measure&& measure );
//    Acquisition& operator<<( const Measures& measure );

    //        template <class T>
    //        const T& get() const {
    //            static_assert( std::is_base_of<Measure, T>::value, "not a base class" );
    //            constexpr Measurement measurement = measure_format_tag<T>;
    //            static_assert( measurement != Measurement::NONE );
    //            assert( m_measures.find( measurement ) != m_measures.end() );
    //            return dynamic_cast<const T&>( m_measures.at( measurement ) );
    //        };

    //        assert( false );
    //        //        static_assert( std::is_base_of<T, Measure>::value,
    //        //                       "not a base class" );
    //        //        T measure;
    //        //        Measure * m = &measure;
    //        //        SensorSpec::Format format2 = m->getFormat();
    //        ////        SensorSpec::Format format2 = SensorSpec::Format::DOF6;

    //        //        assert( m_measures.find( format2 ) != m_measures.end() );
    //        //        return dynamic_cast<const T&>(m_measures.at( format2 ));
    //    }

  public:
    Acquisition clone() const;

  public:
    const long long m_start; // microseconds
    const long long m_end;   // microseconds
                             //    std::map<Measurement, const Measure&> m_measures;
                             //    const unsigned char* const m_data;
  private:
    Measures m_measures;
    size_t m_size  = 0;
//    bool m_isMoved = false;

  public:
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );
    size_t getSize() const;
    //    const std::vector<Measure> &getMeasures() const;
    //    const std::list<Measure>& getMeasures() const;
    const Measures& getMeasures() const;
};

// template <>
// inline const Dof6& Acquisition::get<Dof6>() const {
//     assert( m_measures.find( SensorSpec::Format::DOF6 ) != m_measures.end() );
//     return dynamic_cast<const Dof6&>( m_measures.at( SensorSpec::Format::DOF6 ) );
// }

// template <typename Arg, typename... Args>
// Acquisition::Acquisition( long long start, long long end, Args... args ) :
//     m_start( start ), m_end( end ) {

//    for (auto arg : args) {
//    assert(m_measures.find(arg.m_format) == m_measures.end());

//    }
////    m_measures[measure.m_format] = std::move(measure);
////    m_measures.insert(std::pair<SensorSpec::Format, const Measure>(measure.m_format,
/// std::move(measure)));

//}

} // namespace hub
