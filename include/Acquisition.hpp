#pragma once

#include <cassert>
#include <iostream>
#include <list>

#include "Macros.hpp"
// #include "SensorSpec.hpp"
#include "Measure.hpp"

namespace hub {

// enum class Measurement { NONE, IMAGE, DOF6, COUNT };

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
    ///
    Acquisition( long long start, long long end );
    ~Acquisition();

    Acquisition( const Acquisition& acq ) = delete;

    ///
    /// \param acq
    ///
    Acquisition( Acquisition&& acq )      = default;


    Acquisition& operator=( const Acquisition& acq ) = delete;
    Acquisition& operator=( Acquisition&& acq ) = delete;

    ///
    /// \brief operator ==
    /// \param acq
    /// \return
    ///
    bool operator==( const Acquisition& acq ) const;

    ///
    /// \brief operator !=
    /// \param acq
    /// \return
    ///
    bool operator!=( const Acquisition& acq ) const;

    //    template <class Measure>
    ///
    /// \brief operator <<
    /// \param measure
    /// \return
    ///
    Acquisition& operator<<( Measure&& measure );

    //    void addMeasure( Measure&& measure );

    //    template <class... Args>
    //    Acquisition& operator<<( Args&&... args );

    ///
    /// \brief operator <<
    /// \param measure
    /// \return
    ///
    Acquisition& operator<<( const Measures& measure );

    ///
    /// \brief emplaceMeasure
    /// \param args
    ///
    template <class... Args>
    void emplaceMeasure( Args&&... args );

    //    template <class ResolutionT>
    //    void addMeasure(unsigned char * data, uint64_t size, ResolutionT &&resolution);

    //    template<typename... Values>
    //    void add(Values...);

    ///
    /// \brief isInterpolable
    /// \return
    ///
    bool isInterpolable() const;

    ///
    /// \brief slerp
    /// \param left
    /// \param right
    /// \param t
    /// \return
    ///
    static Acquisition slerp( const Acquisition& left, const Acquisition& right, double t );

  public:
    ///
    /// \brief clone
    /// \return
    ///
    Acquisition clone() const;

  public:

    ///
    /// \brief m_start
    ///
    const long long m_start; // microseconds

    ///
    /// \brief m_end
    ///
    const long long m_end;   // microseconds

  private:
    Measures m_measures;
    size_t m_size = 0;

  public:
    ///
    /// \brief operator <<
    /// \param os
    /// \param acq
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );

    ///
    /// \brief getSize
    /// \return
    ///
    size_t getSize() const;

    ///
    /// \brief getMeasures
    /// \return
    ///
    const Measures& getMeasures() const;

    friend class InputSensor;
};

// template <class... Args>
// Acquisition& Acquisition::operator<<( Args&&... args ) {
//     addMeasure(args...);

////    static_assert(sizeof...(args) == 3);
//////    static_assert(std::is_base_of<std::decay_t<hub::Measure>,std::decay_t<Args>>::value,
//////                     "must be the same as Measure");
//////    m_size += measure.m_size;
////    m_measures.emplace_back( std::forward<Args>( args )... );
////    m_size += m_measures.back().m_size;
////    m_measures.push_back( std::move( measure ) );

//    return *this;
//}

template <class... Args>
void Acquisition::emplaceMeasure( Args&&... args ) {
    static_assert( sizeof...( args ) == 3 );
    //    this << args;
    m_measures.emplace_back( std::forward<Args>( args )... );
    m_size += m_measures.back().m_size;
}

// template <class Measure>
// Acquisition& Acquisition::operator<<( Measure&& measure ) {
//     static_assert(std::is_base_of<std::decay_t<hub::Measure>,std::decay_t<Measure>>::value,
//                      "must be the same as Measure");
//     m_size += measure.m_size;
//     m_measures.emplace_back( std::forward<Measure>( measure ) );
////    m_measures.push_back( std::move( measure ) );
//    return *this;
//}

// template <class ResolutionT>
// void Acquisition::addMeasure(unsigned char* data, uint64_t size, ResolutionT &&resolution ) {
//     m_size += size;
//     static_assert(std::is_same<std::decay_t<Resolution>,std::decay_t<ResolutionT>>::value,
//                      "must be the same as Resolution");

//    //    m_measures.push_back( std::move( measure ) );
////    m_measures.emplace_back( data, size, std::move(resolution) );
//    m_measures.emplace_back( data, size, std::forward<ResolutionT>(resolution) );
//}

// template <>
// inline const Dof6& Acquisition::get<Dof6>() const {
// }

// template <typename Arg, typename... Args>
// Acquisition::Acquisition( long long start, long long end, Args... args ) :

/// std::move(measure)));

//}

} // namespace hub
