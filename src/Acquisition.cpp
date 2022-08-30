#include "Acquisition.hpp"

#include <cassert>
#include <iomanip>

#include <cstring>

namespace hub {

Dof6::Dof6( const Measure& measure ) : Measure( measure.m_data, measure.m_size ) {
    assert( measure.m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
    assert( m_data != nullptr );
}

Dof6::Dof6( float x, float y, float z, float w0, float w1, float w2, float w3 ) :
    Measure( (unsigned char*)new float[7] { x, y, z, w0, w1, w2, w3 }, 28, true )
//    m_x( x ),
//    m_y(),
//    m_z( z ),
//    m_w0( w0 ),
//    m_w1( w1 ),
//    m_w2( w2 ),
//    m_w3( w3 ) {
{
    assert( m_size == 28 );
    memcpy( (unsigned char*)&m_x, m_data, m_size );
}

std::ostream& operator<<( std::ostream& os, const Dof6& dof6 ) {
    os << "dof6";
    return os;
}

Mat4::Mat4(const float *array)
    : Measure((unsigned char*)array, 64)
{
    assert(m_size == 64);
}


// Dof6::~Dof6() {
////    delete m_data;
//    //    m_data = nullptr;
//}

Measure::Measure( const unsigned char* const data, size_t size, bool floatData ) :
    //    Measure( Measurement::IMAGE ),
    m_data( new unsigned char[size] ),
    //    m_data( data ),
    m_size( size ) {
    //    m_format( format ) {

    assert( m_size > 0 );
    assert( data != nullptr );
    memcpy( (unsigned char*)m_data, data, m_size );

    if ( floatData ) delete[] data;
}

// Measure::Measure( Measure&& measure ) :
//     m_data( measure.m_data ), m_size( measure.m_size ), m_ownData( measure.m_ownData ) {
//     measure.m_ownData = false;
// }

// Measure::~Measure() {
//     if ( m_ownData ) { delete[] m_data; }
// }

Measure Measure::clone() const {
    //    unsigned char* data = new unsigned char[m_size];
    //    memcpy( data, m_data, m_size );
    //    Measure measure( m_data, m_size );
    //    measure.m_ownData = true;
    return Measure( m_data, m_size );
}

bool Measure::operator==( const Measure& measure ) const {
    if ( m_size == measure.m_size ) { return memcmp( m_data, measure.m_data, m_size ) == 0; }
    return false;
}

bool Measure::operator!=( const Measure& measure ) const {
    return !( *this == measure );
}

std::ostream& operator<<( std::ostream& os, const Measure& measure ) {
    if ( measure.m_size == 28 ) {
        float* dof6 = (float*)measure.m_data;
        os << "dof6:";
        for ( auto i = 0; i < 7; ++i ) {
            os << std::setw( 3 ) << dof6[i] << " ";
        }
    }
    else {
        for ( auto i = 0; i < std::min( (int)measure.m_size, 10 ); ++i ) {
            os << std::setw( 3 ) << (int)measure.m_data[i] << " ";
        }
    }
    return os;
}

Measure::Measure( Measure&& measure ) :
    //     Measure(measure.m_measurement),
    //    Measure( measure.m_format ),
    m_data( measure.m_data ),
    m_size( measure.m_size ) {
    //     m_format( measure.m_format ) {
    //    measure.m_data = nullptr;
    measure.m_isMoved = true;
}

Measure::~Measure() {
    //    assert( m_data != nullptr );

    if ( !m_isMoved ) { delete[] m_data; }
}

// Measure::Measure( Measurement measurement ) : m_measurement( measurement ) {}

// SensorSpec::Format Measure::getFormat()
//{
//     return m_format;
// }

//// SensorSpec::Format Dof6::getFormat()
////{
////     return m_format;
//// }

// Image::Image( const unsigned char* const data, size_t size, Format format ) :
//     Measure( Measurement::IMAGE ),
//     m_data( new unsigned char[size] ),
//     m_size( size ),
//     m_format( format ) {

//    assert( m_size > 0 );
//    assert( data != nullptr );
//    memcpy( (unsigned char*)m_data, data, m_size );
//}

// Image::Image( Image&& image ) :
//     Measure(image.m_measurement),
//     //    Measure( image.m_format ),
//     m_data( image.m_data ),
//     m_size( image.m_size ),
//     m_format( image.m_format ) {
//     image.m_isMoved = true;
// }

// Image::~Image() {
//     assert( m_data != nullptr );

//    if ( !m_isMoved ) { delete[] m_data; }
//}

// SensorSpec::Format Image::getFormat()
//{
//     return m_format;
// }

// Acquisition::Acquisition(long long start, long long end , hub::Acquisition::Args args) : m_start(
// start ), m_end( end ) {

//    assert( m_start <= m_end );
//}

// Acquisition::Acquisition( long long start,
//                           long long end,
//                           Measure&& measure,
//                           std::initializer_list<Measure> otherMeasures ) :
//     m_start( start ), m_end( end ) {
//     assert( m_start <= m_end );

//    assert( m_measures.find( measure.m_format ) == m_measures.end() );
//    m_measures.insert(
//        std::pair<SensorSpec::Format, const Measure>( measure.m_format, std::move( measure ) ) );

//    for ( auto&& otherMeasure : otherMeasures ) {
//        assert( m_measures.find( otherMeasure.m_format ) == m_measures.end() );
//        m_measures.insert( std::pair<SensorSpec::Format, const Measure>(
//            otherMeasure.m_format, std::move( otherMeasure ) ) );
//    }
//}

Acquisition::Acquisition( long long start, long long end ) :
    m_start( start ),
    m_end( end )
//    m_measures( std::move(measures) )
{
    assert( m_start <= m_end );
}

Acquisition::~Acquisition() {}

// Acquisition::Acquisition( Acquisition&& acq ) noexcept :
//     m_start( acq.m_start ),
//     m_end( acq.m_end ),
//     m_measures(std::move(acq.m_measures)),
//     m_size(acq.m_size)
//     {
//     //    m_data( acq.m_data ), m_size( acq.m_size ) {
////    acq.m_isMoved = true;
//    assert(m_size == acq.m_size);
////    m_size = acq.m_size;
//}

bool Acquisition::operator==( const Acquisition& acq ) const {
    if ( m_start == acq.m_start && m_end == acq.m_end && m_size == acq.m_size &&
         m_measures.size() == acq.m_measures.size() ) {

        for ( int i = 0; i < m_measures.size(); ++i ) {
            if ( m_measures.at( i ) != acq.m_measures.at( i ) ) { return false; }
        }

        //        && m_size == acq.m_size ) {
        //        return memcmp( m_data, acq.m_data, m_size ) == 0;
        return true;
    }
    return false;
}

bool Acquisition::operator!=( const Acquisition& acq ) const {
    return !( *this == acq );
}

Acquisition& Acquisition::operator<<( Measure&& measure ) {
    m_size += measure.m_size;
    m_measures.push_back( std::move( measure ) );
    return *this;
}

// Acquisition& Acquisition::operator<<( const Measures& measures ) {
//     for (const auto & measure : measures) {
//         Acquisition::operator<<(measure.clone());
//     }
//     return *this;
// }

// Acquisition& Acquisition::operator<<( Measure&& measure ) {
//     assert( m_measures.find( measure.m_format ) == m_measures.end() );
//     //    m_measures[measure.m_format] = std::move(measure);
//     m_measures.insert(
//         std::pair<SensorSpec::Format, const Measure&>( measure.m_format, std::move( measure ) )
//         );
//     return *this;
// }

// const Measure& Acquisition::get( SensorSpec::Format format ) const {
//     assert( m_measures.find( format ) != m_measures.end() );
//     return m_measures.at( format );
// }

Acquisition Acquisition::clone() const {
    //    assert( m_data != nullptr );

    Acquisition acq( m_start, m_end );

    for ( const auto& measure : m_measures ) {
        acq << measure.clone();
    }

    return acq;

    //    return Acquisition( m_start, m_end, m_data, m_size );
    //    return Acquisition( m_start, m_end );
}

// const std::list<Measure> &Acquisition::getMeasures() const
const Measures& Acquisition::getMeasures() const {
    return m_measures;
}

// const std::vector<Measure> &Acquisition::getMeasures() const
//{
//     return m_measures;
// }

size_t Acquisition::getSize() const {
    return m_size;
}

std::ostream& operator<<( std::ostream& os, const Acquisition& acq ) {
    os << "start:" << acq.m_start << ", end:" << acq.m_end;
    os << ", measures:[";
    for ( const auto& measure : acq.m_measures ) {
        os << "[" << measure << "], ";
    }
    os << "], ";
    os << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps.";
    //        os << ", ptr = " << (uint64_t)acq.m_data;
    return os;
}

} // namespace hub
