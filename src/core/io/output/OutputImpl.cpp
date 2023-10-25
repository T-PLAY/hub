#include "OutputImpl.hpp"

#include <cstring>
#include <iostream>

namespace hub {
namespace io {
namespace output {

// OutputImpl::~OutputImpl() {}

void OutputImpl::write( const std::string& str ) {
    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER << "write(" << TYPE_NAME( str ) << ") : '" << str << "'"
              << std::endl;
#endif

    int strLen = static_cast<int>( str.size() );
    assert( sizeof( int ) == 4 );
    write( strLen );

    if ( strLen > 0 ) {
        const char* data = str.data();
        write( reinterpret_cast<const unsigned char*>( data ), strLen );
    }
}

void OutputImpl::write( const char* str ) {
    assert( str != nullptr );
    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER << "write(" << TYPE_NAME( str ) << ")" << std::endl;
#endif

    uint32_t strLen = static_cast<int>( strlen( str ) );
    write( strLen );

    if ( strLen > 0 ) { write( reinterpret_cast<const unsigned char*>( str ), strLen ); }
}

// todo acq
// void OutputImpl::write( const sensor::SensorSpec& sensorSpec ) {
//    assert( isOpen() );

// #ifdef HUB_DEBUG_OUTPUT
////    std::cout << "[OutputImpl:" << this << "] write(SensorSpec)" << std::endl;
//    std::cout << HEADER_OUTPUT_MSG "write(SensorSpec)" << std::endl;
// #endif

//    std::vector<char> buff;
//    constexpr int maxBuffLen = 512;
//    buff.reserve(maxBuffLen);
////    output::OutputMemory<decltype(buff)> memory(buff);

//    char magicNumber[80] = { 0 };
//    constexpr char joker = ' ';
//    memset( magicNumber, joker, 79 );
// #ifdef WIN32
//    sprintf_s( magicNumber,
//               "%c%c%c %d.%d.%d",
//               'H',
//               'U',
//               'B',
//               s_versionMajor,
//               s_versionMinor,
//               s_versionPatch );
// #else
// #ifdef OS_MACOS
//    snprintf( magicNumber, 80,
//             "%c%c%c %d.%d.%d",
//             'H',
//             'U',
//             'B',
//             s_versionMajor,
//             s_versionMinor,
//             s_versionPatch );
// #else
//    sprintf( magicNumber,
//             "%c%c%c %d.%d.%d",
//             'H',
//             'U',
//             'B',
//             s_versionMajor,
//             s_versionMinor,
//             s_versionPatch );
// #endif
// #endif
//    assert( strlen( magicNumber ) < 79 );
//    magicNumber[strlen( magicNumber )] = joker;
////    memory.write( reinterpret_cast<unsigned char*>(magicNumber), 80 );
//    write( reinterpret_cast<unsigned char*>(magicNumber), 80 );

////    memory.write( sensorSpec.getSensorName() );
//    write( sensorSpec.getSensorName() );
////    memory.write( sensorSpec.getResolutions() );
//    write( sensorSpec.getResolutions() );
////    memory.write( sensorSpec.getMetaData() );
//    write( sensorSpec.getMetaData() );

////    uint64_t packetSize = buff.size();
//////    assert(packetSize < maxBuffLen);
////    write(packetSize);
////    write((unsigned char*)buff.data(), packetSize);
//}

// void OutputImpl::write( const Measure& measure ) {
//     assert( isOpen() );

// #ifdef HUB_DEBUG_OUTPUT
//     std::cout << HEADER_OUTPUT_MSG "write(Measure)" << std::endl;
// #endif

//    assert( measure.m_size != 0 );
//    assert( measure.m_data != nullptr );

//    write( measure.m_size );
//    write( measure.m_data, measure.m_size );
//    write( measure.m_resolution );
//}

// void OutputImpl::write( const sensor::Acquisition& acq ) {
//     assert( isOpen() );

// #ifdef HUB_DEBUG_OUTPUT
//     std::cout << HEADER_OUTPUT_MSG "write(Acquisition)" << std::endl;
// #endif

//    assert( acq.m_start <= acq.m_end );
//    assert( !acq.m_measures.empty() );
//    assert( acq.m_size > 0 );

////    std::vector<char> buff;
////    buff.reserve(acq.m_size);
////    output::OutputMemory<decltype(buff)> memory(buff);

//    write( acq.m_start );
//    write( acq.m_end );
//    write( acq.m_measures );
//    write( acq.m_size );

////    assert(acq.m_size == buff.size());
////    const uint64_t packetSize = buff.size();
////    write(packetSize);

////    write((unsigned char*)buff.data(), packetSize);
//}

// void OutputImpl::write(uint64_t size)
//{
// #ifdef HUB_DEBUG_OUTPUT
//     std::cout << HEADER_OUTPUT_MSG "write(uint64_t)" << std::endl;
// #endif
//     write((unsigned char*)&size, sizeof(uint64_t));
// }

// void OutputImpl::write( const Any& any ) {
//     assert( isOpen() );

// #ifdef HUB_DEBUG_OUTPUT
////#    ifdef WIN32
////    std::cout << HEADER_OUTPUT_MSG "write(std::any) : '" << any << "'" << std::endl;
////#    else
//    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(any) << ") : '" << any << "'" <<
//    std::endl;
////#    endif
// #endif

//    assert( any.hasValue() );

//    const auto& anyType = any.type();
//    write( anyType );

//    switch ( anyType ) {

//    case Any::Type::INT: {
//        const auto& val = any.get<int>();
//        write( val );
//        break;
//    }

//    case Any::Type::DOUBLE: {
//        const auto& val = any.get<double>();
//        write( val );
//        break;
//    }

//    case Any::Type::STRING: {
//        const auto& val = any.get<std::string>();
//        write( val );
//        break;
//    }

//    case Any::Type::CONST_CHAR_PTR: {
//        const auto& val = any.get<const char*>();
//        write( val );
//        break;
//    }

//    // todo any
////    case Any::Type::MAT4: {
////        const auto& val = any.get<data::Mat4>();
////        write( reinterpret_cast<const unsigned char*>( val.getData() ), 64 );
////        break;
////    }

////    case Any::Type::MESH: {
////        const Measure& measure = any.get<data::Mesh>();
////        write( measure );
////        break;
////    }

// #ifndef COVERAGE
//     default:
//         std::cerr << "non supported type : '" << any.type() << "'" << std::endl;
//         assert( false );
// #endif
//     }
// }

} // namespace output
} // namespace io
} // namespace hub
