/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/28

#pragma once

#include <chrono>

#include "core/Matrix.hpp"
#include "core/matrix/MatrixTs.hpp"

#include "core/Format.hpp"
#include "core/Resolution.hpp"

namespace hub {
namespace sensor {

///
/// \brief Clock
///
using Clock = long long;
static_assert( sizeof( Clock ) == 8 );

static inline Clock getClock() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch() )
        .count();
}

///
/// \brief The Acquisition class
///
class SRC_API Acquisition : public Matrix
{
  public:
    using Matrix::getData;

    ///
    /// \brief packable
    ///
    static struct {
    } packable;

    Acquisition() = default;

    ///
    /// \brief Acquisition
    ///
    Acquisition( Acquisition&& )      = default;
    Acquisition( const Acquisition& ) = delete;

    ///
    /// \brief operator =
    /// \return
    ///
    Acquisition& operator=( Acquisition&& ) = default;
    Acquisition& operator=( const Acquisition& ) = delete;

    ///
    /// \brief clone
    /// \return
    ///
    Acquisition clone() const {
        Acquisition acq;
        acq |= *this;
        return acq;
    }

    ///
    /// \brief getResolution
    /// \return
    ///
    const Matrix& getResolution() const {
        if ( m_resolution.size() == 0 ) {
            for ( size_t i = 2; i < nType(); ++i ) {
                m_resolution.push_back( m_nodes.at( i ) );
            }
        }
        return m_resolution;
    }

    ///
    /// \brief start
    /// \return
    ///
    Clock& start() {
        auto offset  = getOffset<Clock>( 0 );
        auto offset2 = getOffset<Clock>( 1 );
        assert( getOffset<Clock>( 0 ) == 0 );
        return get<Clock&>();
    }

    ///
    /// \brief end
    /// \return
    ///
    Clock& end() {
        assert( getOffset<Clock>( 1 ) == sizeof( Clock ) );
        return get<Clock&, 1>();
    }

    ///
    /// \brief clocks
    ///
    auto clocks() { return std::tuple<Clock&, Clock&> { start(), end() }; }

    ///
    /// \brief getStart
    /// \return
    ///
    Clock getStart() const { return *get<const Clock*>(); }

    ///
    /// \brief getEnd
    /// \return
    ///
    Clock getEnd() const { return *get<const Clock*, 1>(); }

    ///
    /// \brief operator <<
    /// \param other
    /// \return
    ///
    Acquisition& operator<<( const Acquisition& other ) {
        size_t sizeBeforeAdd = m_size;
        for ( size_t i = 2; i < other.nType(); ++i ) {
            push_back( other.m_nodes.at( i ) );
        }
        if ( !other.m_vector.empty() ) {
            m_vector.resize( sizeBeforeAdd + other.m_vector.size() - 2 * sizeof( Clock ) );
            std::copy( other.m_vector.begin() + 2 * sizeof( Clock ),
                       other.m_vector.end(),
                       m_vector.begin() + sizeBeforeAdd );
        }
        return *this;
    }

    ///
    /// \brief operator <<
    /// \param other
    /// \return
    ///
    Acquisition operator<<( const Acquisition& other ) const {
        Acquisition matrix = this->clone();
        matrix << other;
        return matrix;
    }

    ///
    /// \brief operator <
    /// \param right
    /// \return
    ///
    bool operator<( const Acquisition& right ) const {
        if ( getStart() == right.getStart() ) return getEnd() < right.getEnd();
        return getStart() < right.getStart();
    }

  private:
    mutable Matrix m_resolution;
};

///
/// \brief make_acquisition
/// \param resolution
/// \return
///
SRC_API Acquisition make_acquisition( const Matrix& resolution );

///////////////////////////////////// TEMPLATE ////////////////////////////////

///
/// \brief The AcquisitionT class
///
template <class Resolution>
class AcquisitionT : public MatrixTs<Clock, Clock, Resolution>
{
  public:
    ///
    /// \brief Matrix
    ///
    using Matrix = MatrixTs<Clock, Clock, Resolution>;
    using Matrix::get;

    ///
    /// \brief start
    /// \return
    ///
    Clock& start() { return Matrix::template get<Clock&>(); }

    ///
    /// \brief getStart
    /// \return
    ///
    Clock getStart() const { return Matrix::template get<Clock>(); }

    ///
    /// \brief end
    /// \return
    ///
    Clock& end() { return Matrix::template get<Clock&, 1>(); }

    ///
    /// \brief getEnd
    /// \return
    ///
    Clock getEnd() const { return Matrix::template get<Clock, 1>(); }

    ///
    /// \brief clocks
    ///
    auto clocks() { return std::tuple<Clock&, Clock&> { start(), end() }; }
};

} // namespace sensor
} // namespace hub
