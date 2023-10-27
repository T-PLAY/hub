#include "InputImpl.hpp"

#include <cstring>

// #include "core/Any.hpp"
// #include "core/Info.hpp"

// #include "Measure.hpp"
// #include "InputMemory.hpp"

namespace hub {
namespace io {
namespace input {

/////////////////////////////////////////////////////////////////////////////

// size_t InputImpl::s_nInput = 0;

void InputImpl::read( std::string& str ) {
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
#ifdef HUB_DEBUG_INPUT
    std::cout << "\t" << HEADER <<  "read(" << TYPE_NAME( str ) << ") : '" << str << "'" << std::endl;
#endif
}

void InputImpl::read( char* str ) {
    assert( str != nullptr );
    assert( isOpen() );
    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
    std::cout << "\t" << HEADER << "read(" << TYPE_NAME( str ) << ")" << std::endl;
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

// void InputImpl::clear()
//{
//     while ( !isEnd() ) {
////	ts.emplace_back( get<T>() );
//    }
//}

// todo acq
// void InputImpl::read( sensor::SensorSpec& sensorSpec ) {
//    assert( isOpen() );
//    assert( !isEnd() );

////    uint64_t packetSize;
////    read(packetSize);

////    std::vector<char> buff(packetSize);
////    read((unsigned char*)buff.data(), packetSize);

////    input::InputMemory<decltype(buff)> memory(buff);

//    char magicNumber[80] = { 0 };
////    memory.read( reinterpret_cast<unsigned char*>(magicNumber), 80 );
//    read( reinterpret_cast<unsigned char*>(magicNumber), 80 );
//    int versionMajor;
//    int versionMinor;
//    int versionPatch;
//    char h;
//    char u;
//    char b;
// #ifdef WIN32
//    sscanf_s( magicNumber,
//              "%c%c%c %d.%d.%d",
//              &h,
//              1,
//              &u,
//              1,
//              &b,
//              1,
//              &versionMajor,
//              &versionMinor,
//              &versionPatch );
// #else
//    sscanf(
//        magicNumber, "%c%c%c %d.%d.%d", &h, &u, &b, &versionMajor, &versionMinor, &versionPatch );
// #endif
//    assert( h == 'H' );
//    assert( u == 'U' );
//    assert( b == 'B' );
//    assert( versionMajor <= s_versionMajor );
//    assert( versionMinor <= s_versionMinor );
//    assert( versionPatch <= s_versionPatch );
////#ifdef DEBUG
////    std::cout <<  "[InputImpl] read(magic number) : '" << magicNumber << "'" << std::endl;
////#endif

//    std::string sensorName;
//    sensor::Resolutions resolutions;
//    sensor::SensorSpec::MetaData metaData;
////    memory.read( sensorName );
//    read( sensorName );
////    memory.read( resolutions );
//    read( resolutions );
////    memory.read( metaData );
//    read( metaData );

////    assert(memory.isEnd());

//    sensorSpec =
//        sensor::SensorSpec( std::move( sensorName ), std::move( resolutions ), std::move( metaData
//        ) );

//    assert( !sensorSpec.isEnd() );
//}

// void InputImpl::read( Measure& measure ) {
//     assert( isOpen() );
//     assert( !isEnd() );

//    assert( measure.m_data == nullptr );
//    read( measure.m_size );
//    measure.m_data = new unsigned char[measure.m_size];
//    read( measure.m_data, measure.m_size );
//    read( measure.m_resolution );
//    measure.m_ownData = true;

//    assert( measure.m_size > 0 );
//    assert( measure.m_data != nullptr );
//    assert( !measure.m_resolution.first.empty() );
//    assert( measure.m_resolution.second != sensor::Format::NONE );
//}

// void InputImpl::read( sensor::Acquisition& acq ) {
//     assert( isOpen() );
//     assert( !isEnd() );

////    uint64_t packetSize;
////    read(packetSize);

////    std::vector<char> buff(packetSize);
////    read((unsigned char*)buff.data(), packetSize);

////    input::InputMemory<decltype(buff)> memory(buff);

//    read( acq.m_start );
//    read( acq.m_end );
//    read( acq.m_measures );
//    read( acq.m_size );

////    assert(memory.isEnd());

//    assert( acq.m_start <= acq.m_end );
//    assert( !acq.m_measures.empty() );
//    assert( acq.m_size > 0 );
//}

// void InputImpl::read( Any& any ) {
//     assert( isOpen() );
//     assert( !isEnd() );

// #ifdef HUB_DEBUG_INPUT
//     std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME(any) << ")" << std::endl;
// #endif

////    assert( !any.isEnd() );
//    assert( ! any.hasValue() );

//    Any::Type anyType;
//    read( anyType );

//    switch ( anyType ) {

//    case Any::Type::INT: {
//        int val;
//        read( val );
////        any = Any( val );
//        any = val;
//    } break;

//    case Any::Type::DOUBLE: {
//        double val;
//        read( val );
////        any = Any( val );
//        any = val;
//    } break;

//    case Any::Type::STRING: {
//        std::string val;
//        read( val );
////        any = Any( val );
//        any = val;
//    } break;

//    case Any::Type::CONST_CHAR_PTR: {
//        assert( sizeof( char ) == 1 );
//        char* buff = new char[256];
//        memset( buff, 0, 256 );
//        read( buff );
//        int len = static_cast<int>( strlen( buff ) );

//        const int buffSize = len + 1;
//        char* str          = new char[buffSize];
//        memcpy( str, buff, len );
//        str[len] = 0;
//        delete[] buff;

////        any = Any( str );
////        any = str;
//        any = (const char*)str;

//    } break;

//    // todo any
////    case Any::Type::MAT4: {
////        float buff[16];
////        read( reinterpret_cast<unsigned char*>( buff ), 64 );

////        data::Mat4 mat4( buff );
//////        any = Any( mat4 );
////        any = mat4;

////    } break;

////    case Any::Type::MESH: {
////        Measure measure;
////        read( measure );

//////        any = Any( data::Mesh( measure ) );
////        any = data::Mesh(measure);

////    } break;

// #ifndef COVERAGE
//     default:
//         assert( false );
// #endif
//     }
//     assert( any.hasValue() );
// }

/////////////////////////////////////////////////////////////////////////////

} // namespace input
} // namespace io
} // namespace hub
