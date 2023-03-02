#pragma once

#include <cassert>
#include <iostream>
#include <list>

#include "Macros.hpp"
#include "Measure.hpp"

namespace hub {

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

    Acquisition( const Acquisition& acq ) = delete;
    Acquisition& operator=( const Acquisition& acq ) = delete;

    ///
    /// \param acq
    /// movable acquisition.
    ///
    Acquisition( Acquisition&& acq ) = default;
    Acquisition& operator=( Acquisition&& acq ) = delete;

    ~Acquisition();

  public:
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

    ///
    /// \brief operator <<
    /// add new measure to current acquisition
    /// \param measure
    /// [in] measure to add
    /// \return
    /// modified acquisition with new measure.
    ///
    Acquisition& operator<<( Measure&& measure );

    ///
    /// \brief operator <<
    /// add new measure to current acquisition
    /// \param measure
    /// [in] measure to add
    /// \return
    /// modified acquisition with new measure.
    ///
    Acquisition& operator<<( const Measures& measure );

    ///
    /// \brief emplaceMeasure
    /// \param args
    ///
    template <class... Args>
    void emplaceMeasure( Args&&... args );

    ///
    /// \brief isInterpolable
    /// is used during synchronization with another acquisition stream to minimize
    /// the error distance between 2 differents acquisition of differents sensor stream.
    /// \return
    /// true if acquisition is interpolable
    /// false otherwise
    ///
    bool isInterpolable() const;

    ///
    /// \brief slerp
    /// is used during synchronization with another acquisition stream to minimize
    /// the error distance between 2 differents acquisition of differents sensor stream.
    /// \param left
    /// oldest acquisition
    /// \param right
    /// newest acquisition
    /// \param t
    /// to perform interpolation
    /// \return
    /// interpolate new created acquisition.
    ///
    static Acquisition slerp( const Acquisition& left, const Acquisition& right, double t );

    ///
    /// \brief clone
    /// make a copy of current acquisition.
    /// \return
    /// copy of acquisition.
    ///
    Acquisition clone() const;

    ///
    /// \brief operator <<
    /// print acquisition information.
    /// \param os
    /// output to print.
    /// \param acq
    /// acquisition to print.
    /// \return
    /// std printed ostream.
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );

  public: // getters
    ///
    /// \brief getSize
    /// is used to serialize the acquisition into a constant buffer array (TCP communication).
    /// \return
    /// size of acquisition in bytes.
    ///
    size_t getSize() const;

    ///
    /// \brief getMeasures
    /// is used to process things for each measure separately.
    /// \return
    /// all measures acquired during the acquisition.
    ///
    const Measures& getMeasures() const;

  public:
    ///
    /// \brief m_start
    ///
    const long long m_start; // microseconds

    ///
    /// \brief m_end
    ///
    const long long m_end; // microseconds

  private:
    Measures m_measures;
    size_t m_size = 0;
    friend class InputSensor;
};

} // namespace hub

#include "Acquisition.inl"
