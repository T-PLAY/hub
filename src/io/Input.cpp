#include "io/Input.hpp"

#include "Any.hpp"
#include "data/Measure.hpp"

namespace hub {
namespace io {

/////////////////////////////////////////////////////////////////////////////

void Input::read( std::string& str ) {
    assert( isOpen() );

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str = ""; }
    else {
        const int buffSize = strLen + 1;
        unsigned char* tmp = new unsigned char[buffSize];
        read( tmp, strLen );
        tmp[strLen] = 0;
        str         = std::string( reinterpret_cast<char*>( tmp ) );
        delete[] tmp;
    }
#ifdef DEBUG_INPUT
    std::cout << "[Input] read(std::string) : '" << str << "'" << std::endl;
#endif
}

void Input::read( SensorSpec& sensorSpec ) {

    std::string sensorName;
    Resolutions resolutions;
    hub::SensorSpec::MetaData metaData;
    read( sensorName );
    read( resolutions );
    read( metaData );

    sensorSpec =
        SensorSpec( std::move( sensorName ), std::move( resolutions ), std::move( metaData ) );

    assert(! sensorSpec.isEmpty());

}

void Input::read(data::Measure &measure)
{
    assert(measure.m_data == nullptr);
    read(measure.m_size);
    measure.m_data = new unsigned char[measure.m_size];
    read(measure.m_data, measure.m_size);
    read(measure.m_resolution);
    measure.m_ownData = true;

    assert(measure.m_size != 0);
    assert(measure.m_data != nullptr);
}

void Input::read(Acquisition &acq)
{
    read(acq.m_start);
    read(acq.m_end);
    read(acq.m_measures);
    read(acq.m_size);

    assert(acq.m_start <= acq.m_end);
    assert(!acq.m_measures.empty());
    assert(acq.m_size > 0);
}

//Acquisition Input::operator>>(Input &input) {

//#ifdef DEBUG
//    assert( !isEnd() );
//    assert( !input.isEnd() );
//#endif

//    Acquisition masterAcq;
//    read(masterAcq);

//    auto & lastAcqs = input.m_lastAcqs;

//    assert( lastAcqs.size() < 20 );

//    Acquisition acq2;

//    if ( lastAcqs.empty() ) {
//        input.read(acq2);
//        lastAcqs.push_back( std::move(acq2) );
//    }

//    while ( masterAcq.getStart() < lastAcqs.front().getStart() ) {
//        read(masterAcq);
//        std::cout << "[InputSensor] operator>>(InputSensor&) shift masterAcq : " << masterAcq
//                  << std::endl;
//    }

//    while ( lastAcqs.back().getStart() < masterAcq.getStart() ) {
//        input.read(acq2);
//        lastAcqs.push_back( std::move(acq2) );
//    }

//    if ( lastAcqs.back().getStart() == masterAcq.getStart() ) {
//        masterAcq << lastAcqs.back().getMeasures();
//        return masterAcq;
//    }

//    while ( lastAcqs.size() > 2 ) {
//        lastAcqs.pop_front();
//    }

//    const auto& left  = lastAcqs.front();
//    const auto& right = lastAcqs.back();

//    assert( left.getStart() <= masterAcq.getStart() );
//    assert( masterAcq.getStart() <= right.getStart() );


//    const auto& closestAcq = ( std::abs( left.getStart() - masterAcq.getStart() ) >
//                               std::abs( right.getStart() - masterAcq.getStart() ) )
//                                 ? ( right )
//                                 : ( left );


//    masterAcq << closestAcq.getMeasures();
//    return masterAcq;

//    //    const auto dist = std::abs( closestAcq.m_start - masterAcq.m_start );

//    //    // assert(minDist < 20'000); // 20 ms
//    //    // if too far then abort synchronize
//    //    // consider constant period of acquistion rate
//    //    auto maxDist = ( right.m_start - left.m_start ) / 2;

//    //    // find acceptable corresponding acquisition if interpolation is not possible
//    //    if ( !left.isInterpolable() ) {
//    //        const auto& lastMasterAcq = getLastAcq( syncViewerName );
//    //        if ( lastMasterAcq != nullptr ) {
//    //            maxDist =
//    //                std::min( dist, std::abs( lastMasterAcq->m_start() - masterAcq.m_start() ) );
//    //        }
//    //    }

//    //        return ret;
//    //        return *this;
//}

void Input::read( char* str ) {
    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[Input] read(char*)" << std::endl;
#endif

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str[0] = 0; }
    else {
        read( reinterpret_cast<unsigned char*>( str ), strLen );

        str[strLen] = 0;
    }
}


void Input::read( Any& any ) {
    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[Input] read(std::any)" << std::endl;
#endif

    assert( !any.has_value() );

    Any::Type anyType;
    read( anyType );
    switch ( anyType ) {

    case Any::Type::INT: {
        int val;
        read( val );
        any = Any( val );
    } break;

    case Any::Type::DOUBLE: {
        double val;
        read( val );
        any = Any( val );
    } break;

    case Any::Type::STRING: {
        std::string val;
        read( val );
        any = Any( val );
    } break;

    case Any::Type::CONST_CHAR_PTR: {
        assert( sizeof( char ) == 1 );
        char* buff = new char[256];
        memset( buff, 0, 256 );
        read( buff );
        int len = static_cast<int>( strlen( buff ) );

        const int buffSize = len + 1;
        char* str          = new char[buffSize];
        memcpy( str, buff, len );
        str[len] = 0;
        delete[] buff;

        any = Any( str );

    } break;

    case Any::Type::MAT4: {
        float buff[16];
        read( reinterpret_cast<unsigned char*>( buff ), 64 );

        data::Mat4 mat4( buff );
        any = Any( mat4 );

    } break;

    case Any::Type::MESH: {
        data::Measure measure;
        read(measure);

        any = Any( data::Mesh(measure) );

    } break;


    default:
        assert( false );
    }
    assert( any.has_value() );
}


/////////////////////////////////////////////////////////////////////////////

} // namespace io
} // namespace hub
