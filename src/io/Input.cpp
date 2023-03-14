#include "io/Input.hpp"

//#include <any>
//#include <cstring>
//#include <iostream>
//#include <typeindex>
//#include <typeinfo>

//#include "Acquisition.hpp"
//#include "SensorSpec.hpp"
#include "Any.hpp"

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
}

// SensorSpec Input::getSensorSpec()  {

//#ifdef DEBUG_INPUT
//#endif

//}

Acquisition Input::getAcquisition( const SensorSpec& sensorSpec ) {
    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[Input] getAcquisition()" << std::endl;
#endif

    long long start, end;

    read( start );
    read( end );

    Acquisition acq { start, end };

    int nMeasures;
    read( nMeasures );
    assert( nMeasures > 0 );

    const auto& resolutions = sensorSpec.getResolutions();
    assert( resolutions.size() == nMeasures );

    for ( int iMeasure = 0; iMeasure < nMeasures; ++iMeasure ) {

        uint64_t size; // compatibility 32/64 bits
        read( size );
        unsigned char* data = new unsigned char[size];
        read( data, size );

        acq.emplaceMeasure( data, size, resolutions.at( iMeasure ), true );
    }

    assert( !acq.hasFixedSize() || sensorSpec.getAcquisitionSize() == acq.getSize() );

    return acq;
}

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

// void Input::read( std::any& any )  {
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

    default:
        assert( false );
    }
    assert( any.has_value() );
}

// void Input::read(data::UserData &userData)
//{

//}

/////////////////////////////////////////////////////////////////////////////

} // namespace io
} // namespace hub
