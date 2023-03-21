#pragma once

#include <cassert>
#include <cstring>

#include "Macros.hpp"
#include "Resolution.hpp"


namespace hub {
namespace data {


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
//    template <class Resolution = hub::Resolution>
    template <class Resolution = Resolution>
//    template <class Resolution>
//    template <typename Resolution>
    Measure( const unsigned char* const data, uint64_t size, Resolution&& resolution );

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
//    template <class Resolution = Resolution>
    template <class Resolution>
    Measure( unsigned char* data, uint64_t size, Resolution&& resolution, bool stealData = false );

    ///
    /// \brief Measure
    /// movable constructor.
    /// \param measure
    /// to move.
    ///
    Measure( Measure&& measure );
    Measure& operator=( Measure&& ) = delete;

    Measure( const Measure& ) = delete;
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

  public: // getters
    ///
    /// \brief getResolution
    /// is used to check compatible resolution with sensor specification of OutputSensor.
    /// \return
    /// internal resolution.
    ///
    const inline Resolution& getResolution() const;

//    const unsigned char *getData() const;

//    bool ownData() const;

    ///
    /// \brief getData
    /// \return
    ///
    unsigned char *getData() const;

    ///
    /// \brief getSize
    /// \return
    ///
    uint64_t getSize() const;


  protected:
    ///
    /// \brief m_ownData
    /// indicated if this class is the owner of the data.
    ///
    bool m_ownData = false;

    ///
    /// \brief m_data
    ///
    unsigned char* m_data;

    ///
    /// \brief m_size
    ///
    uint64_t m_size; // compatibility 32/64 bits

  private:
    bool m_isMoved = false;
    Resolution m_resolution;

    friend class InputSensor;
};

using Measures = std::vector<Measure>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline unsigned char *Measure::getData() const
{
    return m_data;
}

inline uint64_t Measure::getSize() const
{
    return m_size;
}

template <class Resolution>
Measure::Measure( const unsigned char* const data, uint64_t size, Resolution&& resolution ) :
    m_data( new unsigned char[size] ),
    m_size( size ),
    m_ownData( true ),
    m_resolution( std::forward<Resolution>( resolution ) ) {

    static_assert( std::is_same<std::decay_t<Resolution>, std::decay_t<hub::Resolution>>::value,
                   "must be the same as Resolution" );

    assert( data != nullptr );
    memcpy( const_cast<unsigned char*>( m_data ), data, m_size );
    assert( m_size > 0 );
    assert( m_data != nullptr );
    assert( size == res::computeAcquisitionSize( m_resolution ) );
}

template <class Resolution>
Measure::Measure( unsigned char* data, uint64_t size, Resolution&& resolution, bool stealData ) :
    m_data( data ),
    m_size( size ),
    m_ownData( stealData ),
    m_resolution( std::forward<Resolution>( resolution ) ) {

    static_assert( std::is_same<std::decay_t<Resolution>, std::decay_t<hub::Resolution>>::value,
                   "must be the same as Resolution" );

    if ( !res::format2hasFixedSize( m_resolution.second ) ) return;
    assert( data != nullptr );
    assert( m_size > 0 );
    assert( m_data != nullptr );
    assert( !res::format2hasFixedSize( m_resolution.second ) ||
            size == res::computeAcquisitionSize( m_resolution ) );
}

const inline Resolution& Measure::getResolution() const {
    return m_resolution;
}

} // namespace data
} // namespace hub
