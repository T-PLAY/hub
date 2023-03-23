#include "io/Output.hpp"

// #include <any>
// #include <cstring>
// #include <iostream>
// #include <typeindex>
// #include <typeinfo>

// #include "Acquisition.hpp"
// #include "SensorSpec.hpp"
#include "Any.hpp"

namespace hub {
namespace io {

void Output::write( const std::string& str ) {
    if (! isOpen())
        throw std::runtime_error("[Output] closed, unable to write");
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(std::string) : '" << str << "'" << std::endl;
#endif

    int strLen = static_cast<int>( str.size() );
    write( strLen );

    if ( strLen > 0 ) {
        const char* data = str.data();
        write( reinterpret_cast<const unsigned char*>( data ), strLen );
    }
}

void Output::write( const SensorSpec& sensorSpec ) {
    if (! isOpen())
        throw std::runtime_error("[Output] closed, unable to write");
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(SensorSpec)" << std::endl;
#endif

    write( sensorSpec.getSensorName() );
    write( sensorSpec.getResolutions() );
    write( sensorSpec.getMetaData() );
}

void Output::write( const data::Measure& measure ) {
    if (! isOpen())
        throw std::runtime_error("[Output] closed, unable to write");
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Measure)" << std::endl;
#endif

    assert( measure.getSize() != 0 );
    assert( measure.getData() != nullptr );

    write( measure.getSize() );
    write( measure.getData(), measure.getSize() );
}

// void Output::write( const data::UserData& userData ) {
//}

void Output::write( const Acquisition& acq ) {
    if (! isOpen())
        throw std::runtime_error("[Output] closed, unable to write");
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Acquisition)" << std::endl;
#endif

    write( acq.getStart() );
    write( acq.getEnd() );
    const auto& measures = acq.getMeasures();
    assert( measures.size() > 0 );
    write( measures );
}

void Output::write( const char* str ) {
    if (! isOpen())
        throw std::runtime_error("[Output] closed, unable to write");
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(const char*)" << std::endl;
#endif

    int strLen = static_cast<int>( strlen( str ) );
    write( strLen );

    if ( strLen > 0 ) { write( reinterpret_cast<const unsigned char*>( str ), strLen ); }
}

// Output::~Output()
//{
// }

// void Output::write( const std::any& any )  {
//Output::~Output()
//{
////    assert(Output::isOpen());
//    Output::close();
//}

void Output::write( const Any& any ) {
    if (! isOpen())
        throw std::runtime_error("[Output] closed, unable to write");
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
#    ifdef WIN32
    std::cout << "[Output] write(std::any) : '" << any.type().raw_name() << "'" << std::endl;
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

    default:
        std::cerr << "non supported type : '" << any.type() << "'" << std::endl;
        assert( false );
    }
}

/////////////////////////////////////////////////////////////////////////////

} // namespace io
} // namespace hub
