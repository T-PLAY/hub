#include "io/Output.hpp"

#include "Any.hpp"
#include "Info.hpp"

namespace hub {
namespace io {

Output::~Output() {}

void Output::write( const std::string& str ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(std::string) : '" << str << "'" << std::endl;
#endif

    int strLen = static_cast<int>( str.size() );
    assert( sizeof( int ) == 4 );
    write( strLen );

    if ( strLen > 0 ) {
        const char* data = str.data();
        write( reinterpret_cast<const unsigned char*>( data ), strLen );
    }
}

void Output::write( const SensorSpec& sensorSpec ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(SensorSpec)" << std::endl;
#endif

    char magicNumber[80] = { 0 };
    constexpr char joker = ' ';
    memset( magicNumber, joker, 79 );
#ifdef WIN32
    sprintf_s( magicNumber,
               "%c%c%c %d.%d.%d",
               'H',
               'U',
               'B',
               hub::s_versionMajor,
               hub::s_versionMinor,
               hub::s_versionPatch );
#else
    sprintf( magicNumber,
             "%c%c%c %d.%d.%d",
             'H',
             'U',
             'B',
             hub::s_versionMajor,
             hub::s_versionMinor,
             hub::s_versionPatch );
#endif
    assert( strlen( magicNumber ) < 79 );
    magicNumber[strlen( magicNumber )] = joker;
    write( (unsigned char*)magicNumber, 80 );

    write( sensorSpec.getSensorName() );
    write( sensorSpec.getResolutions() );
    write( sensorSpec.getMetaData() );
}

void Output::write( const data::Measure& measure ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Measure)" << std::endl;
#endif

    assert( measure.m_size != 0 );
    assert( measure.m_data != nullptr );

    write( measure.m_size );
    write( measure.m_data, measure.m_size );
    write( measure.m_resolution );
}

void Output::write( const Acquisition& acq ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Acquisition)" << std::endl;
#endif

    assert( acq.m_start <= acq.m_end );
    assert( !acq.m_measures.empty() );
    assert( acq.m_size > 0 );

    write( acq.m_start );
    write( acq.m_end );
    write( acq.m_measures );
    write( acq.m_size );
}

void Output::write( const char* str ) {
    assert( str != nullptr );
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(const char*)" << std::endl;
#endif

    int strLen = static_cast<int>( strlen( str ) );
    write( strLen );

    if ( strLen > 0 ) { write( reinterpret_cast<const unsigned char*>( str ), strLen ); }
}

void Output::write( const Any& any ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
#    ifdef WIN32
    std::cout << "[Output] write(std::any) : '" << any << "'" << std::endl;
#    else
    std::cout << "[Output] write(std::any) : '" << any << "'" << std::endl;
#    endif
#endif

    assert( any.has_value() );
    const auto& anyType = any.type();
    write( anyType );
    switch ( anyType ) {

    case Any::Type::INT: {
        const auto& val = any.getInt();
        write( val );
        break;
    }

    case Any::Type::DOUBLE: {
        const auto& val = any.getDouble();
        write( val );
        break;
    }

    case Any::Type::STRING: {
        const auto& val = any.getStdString();
        write( val );
        break;
    }

    case Any::Type::CONST_CHAR_PTR: {
        const auto& val = any.getConstCharPtr();
        write( val );
        break;
    }

    case Any::Type::MAT4: {
        const auto& val = any.getMat4();
        write( reinterpret_cast<const unsigned char*>( val.getData() ), 64 );
        break;
    }

    case Any::Type::MESH: {
        const data::Measure& measure = any.getMesh();
        write( measure );
        break;
    }

#ifndef COVERAGE
    default:
        std::cerr << "non supported type : '" << any.type() << "'" << std::endl;
        assert( false );
#endif
    }
}

} // namespace io
} // namespace hub
