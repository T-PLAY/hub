//#include "Measure.hpp"

//#include <iomanip>

//#include "data/Dof6.hpp"
//#include "data/Mat4.hpp"
//#include "data/Mesh.hpp"
//#include "data/UserData.hpp"

//namespace hub {
////namespace data {

//Measure::~Measure() {

//    if ( m_ownData && !m_isMoved ) {
//        assert( m_data != nullptr );
//        delete[] m_data;
//    }
//}

//Measure::Measure( Measure&& measure ) :
//    m_data( measure.m_data ),
//    m_size( measure.m_size ),
//    m_ownData( measure.m_ownData ),
//    m_resolution( std::move( measure.m_resolution ) ) {

//    measure.m_isMoved = true;
//}

//Measure Measure::clone() const {
//    return { m_data, m_size, m_resolution };
//}

//bool Measure::isInterpolable() const {
//    assert( m_resolution.second != sensor::Format::NONE );
//    assert( !m_resolution.first.empty() );
//    return sensor::resolution::format2isInterpolable( m_resolution.second );
//}

//Measure Measure::slerp( const Measure& left, const Measure& right, double t ) {
//    assert( left.m_size == right.m_size );
//    assert( !left.m_resolution.first.empty() );
//    assert( !right.m_resolution.first.empty() );
//    assert( left.m_resolution == right.m_resolution );

//    return left.slerp(right, t);

//}

//Measure Measure::slerp(const Measure &right, double t) const
//{
////    assert(! isInterpolable());
////    return (t < 0.5) ?clone() :right.clone();

//    if ( isInterpolable() ) {
//        assert( right.isInterpolable() );

////        return clone();

//        switch ( m_resolution.second ) {
//        case sensor::Format::Dof6: {
//            return data::Dof6::slerp( data::Dof6( *this ), data::Dof6( right ), t );
//        }
//        default:
//            assert( false );
//            // do nothing
//        }
//    }
////    return left.clone(); // avoid non return warning
//    return (t < 0.5) ?clone() :right.clone();
////    return clone();
//}

//bool Measure::operator==( const Measure& measure ) const {
//    if ( m_size == measure.m_size ) {
//        return memcmp( m_data, measure.m_data, m_size ) == 0 &&
//               m_resolution == measure.m_resolution;
//    }
//    return false;
//}

//bool Measure::operator!=( const Measure& measure ) const {
//    return !( *this == measure );
//}

//// bool Measure::ownData() const
//// }

//// const unsigned char *Measure::getData() const
//// }

//std::ostream& operator<<( std::ostream& os, const Measure& measure ) {

//    const auto& format = measure.getResolution().second;
//    switch ( format ) {
//    case sensor::Format::Dof6: {
//        const data::Dof6 dof6( measure );
//        os << dof6;
//        break;
//    }
//    case sensor::Format::MAT4: {
//        const data::Mat4 mat4( measure );
//        os << mat4;
//        break;
//    }
//    case sensor::Format::USER_DATA: {
//        const data::UserData userData( measure );
//        os << userData;
//        break;
//    }

//    case sensor::Format::MESH: {
//        const data::Mesh mesh( measure );
//        os << mesh;
//        break;
//    }

//    default:
//        os << measure.getResolution() << ", [";
//        for ( auto i = 0; i < std::min( (int)measure.m_size, 10 ); ++i ) {
//            os << std::setw( 3 ) << (int)measure.m_data[i] << " ";
//        }
//        os << "]";
//        break;
//    }

//    return os;
//}

////} // namespace data
//} // namespace hub
