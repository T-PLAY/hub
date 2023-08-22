#include "Input.hpp"

#include "Any.hpp"
#include "Info.hpp"
#include "data/Measure.hpp"
#include "InputMemory.hpp"

namespace hub {
namespace io {

/////////////////////////////////////////////////////////////////////////////

void Input::read( std::string& str ) {
    assert( isOpen() );
    assert( !isEnd() );

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
    std::cout << HEADER_INPUT_MSG "read(std::string) : '" << str << "'" << std::endl;
#endif
}

void Input::read( SensorSpec& sensorSpec ) {
    assert( isOpen() );
    assert( !isEnd() );

    uint64_t packetSize;
    read(packetSize);

    std::vector<char> buff(packetSize);
    read((unsigned char*)buff.data(), packetSize);

    input::InputMemory<decltype(buff)> memory(buff);

    char magicNumber[80] = { 0 };
    memory.read( reinterpret_cast<unsigned char*>(magicNumber), 80 );
    int versionMajor;
    int versionMinor;
    int versionPatch;
    char h;
    char u;
    char b;
#ifdef WIN32
    sscanf_s( magicNumber,
              "%c%c%c %d.%d.%d",
              &h,
              1,
              &u,
              1,
              &b,
              1,
              &versionMajor,
              &versionMinor,
              &versionPatch );
#else
    sscanf(
        magicNumber, "%c%c%c %d.%d.%d", &h, &u, &b, &versionMajor, &versionMinor, &versionPatch );
#endif
    assert( h == 'H' );
    assert( u == 'U' );
    assert( b == 'B' );
    assert( versionMajor <= hub::s_versionMajor );
    assert( versionMinor <= hub::s_versionMinor );
    assert( versionPatch <= hub::s_versionPatch );
#ifdef DEBUG
    std::cout <<  "[Input] read(magic number) : '" << magicNumber << "'" << std::endl;
#endif

    std::string sensorName;
    Resolutions resolutions;
    hub::SensorSpec::MetaData metaData;
    memory.read( sensorName );
    memory.read( resolutions );
    memory.read( metaData );

    assert(memory.isEnd());

    sensorSpec =
        SensorSpec( std::move( sensorName ), std::move( resolutions ), std::move( metaData ) );

    assert( !sensorSpec.isEmpty() );
}

void Input::read( data::Measure& measure ) {
    assert( isOpen() );
    assert( !isEnd() );

    assert( measure.m_data == nullptr );
    read( measure.m_size );
    measure.m_data = new unsigned char[measure.m_size];
    read( measure.m_data, measure.m_size );
    read( measure.m_resolution );
    measure.m_ownData = true;

    assert( measure.m_size > 0 );
    assert( measure.m_data != nullptr );
    assert( !measure.m_resolution.first.empty() );
    assert( measure.m_resolution.second != Format::NONE );
}

void Input::read( Acquisition& acq ) {
    assert( isOpen() );
    assert( !isEnd() );


//    uint64_t packetSize;
//    read(packetSize);

//    std::vector<char> buff(packetSize);
//    read((unsigned char*)buff.data(), packetSize);

//    input::InputMemory<decltype(buff)> memory(buff);

    read( acq.m_start );
    read( acq.m_end );
    read( acq.m_measures );
    read( acq.m_size );

//    assert(memory.isEnd());

    assert( acq.m_start <= acq.m_end );
    assert( !acq.m_measures.empty() );
    assert( acq.m_size > 0 );
}

void Input::read( char* str ) {
    assert( str != nullptr );
    assert( isOpen() );
    assert( !isEnd() );

#ifdef DEBUG_INPUT
    std::cout << HEADER_INPUT_MSG "read(char*)" << std::endl;
#endif

    int strLen = 0;
    assert( sizeof( int ) == 4 );
    read( strLen );

    if ( strLen == 0 ) { str[0] = 0; }
    else {
        read( reinterpret_cast<unsigned char*>( str ), strLen );

        str[strLen] = 0;
    }
}

void Input::read( Any& any ) {
    assert( isOpen() );
    assert( !isEnd() );

#ifdef DEBUG_INPUT
    std::cout << HEADER_INPUT_MSG "read(std::any)" << std::endl;
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
        read( measure );

        any = Any( data::Mesh( measure ) );

    } break;
#ifndef COVERAGE
    default:
        assert( false );
#endif
    }
    assert( any.has_value() );
}

Acquisition Input::operator>>( Input& input ) {
    assert( isOpen() );
    assert( !isEnd() );
    assert( input.isOpen() );
    assert( !input.isEnd() );

    Input & leftInput = *this;
    Input & rightInput = input;

    Acquisition rightAcq;
    rightInput.read( rightAcq );
//    assert(rightAcq.getMeasures().front().getResolution().second == hub::Format::DOF6);

    auto& leftLastAcqs = leftInput.m_lastAcqs;

    assert( leftLastAcqs.size() < 20 );

    Acquisition leftAcq;

    if ( leftLastAcqs.empty() ) {
        leftInput.read( leftAcq );
        leftLastAcqs.push_back( std::move( leftAcq ) );
    }

    while ( rightAcq.getStart() < leftLastAcqs.front().getStart() ) {
        std::cout << "[InputSensor] operator>>(InputSensor&) shift t : " << rightAcq << std::endl;
        assert( !isEnd() );
        rightInput.read( rightAcq );
    }

    while ( leftLastAcqs.back().getStart() < rightAcq.getStart() && !leftInput.isEnd() ) {
        assert( !leftInput.isEnd() );
        leftInput.read( leftAcq );
        leftLastAcqs.push_back( std::move( leftAcq ) );
    }

    while ( leftLastAcqs.size() > 2 ) {
        leftLastAcqs.pop_front();
    }

    const auto& leftBeforeRightAcq  = leftLastAcqs.front();
    const auto& leftAfterRightAcq = leftLastAcqs.back();

    assert( leftInput.isEnd() || leftBeforeRightAcq.getStart() <= rightAcq.getStart() );
    assert( leftInput.isEnd() || rightAcq.getStart() <= leftAfterRightAcq.getStart() );

    const auto& closestAcq =
        ( std::abs( leftBeforeRightAcq.getStart() - rightAcq.getStart() ) > std::abs( leftAfterRightAcq.getStart() - rightAcq.getStart() ) )
            ? ( leftAfterRightAcq )
            : ( leftBeforeRightAcq );

    const auto & rightMeasures = rightAcq.getMeasures();
    const auto & closestMeasures = closestAcq.getMeasures();
//    rightMeasures.insert(rightMeasures.begin(), closestMeasures.begin(), closestMeasures.end());
//    rightAcq << closestAcq.getMeasures();
    hub::Acquisition acq(rightAcq.m_start, rightAcq.m_end);
    acq << closestMeasures;
    acq << rightMeasures;
    return acq;
//    return rightAcq;
}

/////////////////////////////////////////////////////////////////////////////

} // namespace io
} // namespace hub
