#pragma once

#include <cassert>
#include <iostream>
#include <list>

#include "Macros.hpp"

// user friendly useless includes
#include "data/Dof6.hpp"
#include "data/Mat4.hpp"
#include "data/Measure.hpp"
#include "data/Mesh.hpp"
#include "data/UserData.hpp"

namespace hub {

///
/// \brief The Acquisition class
/// represents the data with the start and the end of the acquire process.
///
class SRC_API Acquisition
{
  public:
    Acquisition() = default;

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

    ///
    /// \brief operator =
    /// \param acq
    /// \return
    ///
    Acquisition& operator=( Acquisition&& acq );

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
    Acquisition& operator<<( data::Measure&& measure );

    ///
    /// \brief operator <<
    /// add new measure to current acquisition
    /// \param measure
    /// [in] measure to add
    /// \return
    /// modified acquisition with new measure.
    ///
    Acquisition& operator<<( const data::Measures& measure );

    ///
    /// \brief operator >>
    /// \param acq
    ///
    void operator>>( Acquisition& acq );

    ///
    /// \brief emplaceMeasure
    /// \param args
    ///
    template <class... Args>
    void emplaceMeasure( Args&&... args );

    ///
    /// \brief pushBack
    /// \param measure
    ///
    void pushBack( data::Measure&& measure );

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
    /// \brief hasFixedSize
    /// \return
    /// true if the acquisition size is constant,
    /// false otherwise.
    ///
    bool hasFixedSize() const;

    bool operator<( const Acquisition& right ) const;

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
    const data::Measures& getMeasures() const;

    ///
    /// \brief getStart
    /// \return
    ///
    long long getStart() const;

    ///
    /// \brief getEnd
    /// \return
    ///
    long long getEnd() const;

    ///
    /// \brief isEmpty
    /// \return
    ///
    bool isEmpty() const;

  private:
    long long m_start = 0; // microseconds
    long long m_end   = 0; // microseconds
    data::Measures m_measures;
    uint64_t m_size = 0;

    friend class io::Input;
    friend class io::Output;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline long long Acquisition::getStart() const {
    return m_start;
}

inline long long Acquisition::getEnd() const {
    return m_end;
}

inline bool Acquisition::isEmpty() const {
    return m_measures.empty();
}

template <class... Args>
void Acquisition::emplaceMeasure( Args&&... args ) {
#if ( __cplusplus >= 201703L )
    static_assert( 3 <= sizeof...( args ) && sizeof...( args ) <= 4 );
#endif
    m_measures.emplace_back( std::forward<Args>( args )... );
    m_size += m_measures.back().getSize();
}

inline bool Acquisition::operator<( const Acquisition& right ) const {
    return m_start < right.m_start;
}

} // namespace hub
