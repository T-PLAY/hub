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
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(SensorSpec)" << std::endl;
#endif

    write( sensorSpec.getSensorName() );
    write( sensorSpec.getResolutions() );
    write( sensorSpec.getMetaData() );
}

void Output::write( const data::Measure& measure ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Measure)" << std::endl;
#endif

    //    const auto & format = measure.getResolution().second;
    assert( measure.m_size != 0 );
    assert( measure.m_data != nullptr );

    write( measure.m_size );
    write( measure.m_data, measure.m_size );
}

void Output::write( const data::UserData& userData ) {
    //    write(userData.m_data);
    //    write(userData.m_size);
    write( userData.getName() );
    write( userData.getValue() );
}

void Output::write( const Acquisition& acq ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Acquisition)" << std::endl;
#endif

    write( acq.m_start );
    write( acq.m_end );
    const auto& measures = acq.getMeasures();
    assert( measures.size() > 0 );
    //    if (measures.size() > 0) {
    write( measures );
    //    }
}

void Output::write( const char* str ) {
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
//     assert(! isOpen());
// }

// void Output::write( const std::any& any )  {
void Output::write( const Any& any ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
#    ifdef WIN32
    std::cout << "[Output] write(std::any) : '" << any.type().raw_name() << "'" << std::endl;
//    std::cout << "any raw name = '" << any.type().raw_name() << "'" << std::endl;
#    else
    //    std::cout << "any raw name = '" << any.type().name() << "'" << std::endl;
    std::cout << "[Output] write(std::any) : '" << any.type().name() << "'" << std::endl;
#    endif
#endif

    assert( any.has_value() );
    const auto& anyType = any.type();
    write(anyType);
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

    case Any::Type::VECTOR_FLOAT: {
        const auto& val = any.getStdVectorFloat();
        assert( val.size() == 9 );
        write( val );
        break;
    }

    case Any::Type::UINT: {
        const auto& val = any.getUnsignedInt();
        write( val );
        break;
    }

    case Any::Type::CONST_FLOAT_PTR: {
        const auto& val = any.getConstFloatPtr();
        write( reinterpret_cast<const unsigned char*>( val ), 64 );
        break;
    }

    case Any::Type::CONST_DOUBLE_PTR: {
        const auto& val = any.getConstDoublePtr();
        write( reinterpret_cast<const unsigned char*>( val ), 64 );
        break;
    }

    case Any::Type::MAT4: {
        const auto& val = any.getMat4();
        write( reinterpret_cast<const unsigned char*>( val.m_data ), 64 );
//        write(val);
        break;
    }

    default:
        std::cerr << "non supported type : '" << any.typeName() << "'" << std::endl;
        assert( false );
    }

    //    if ( anyType == typeid( int ) ) {
    //        write( hub::Any::Type::INT );
    //        const int* val = std::any_cast<int>( &any );
    //        assert( sizeof( int ) == 4 );
    //        write( *val );
    //    }
    //    else if ( anyType == typeid( double ) ) {
    //        write( hub::Any::Type::DOUBLE );
    //        const double* val = std::any_cast<double>( &any );
    //        assert( sizeof( double ) == 8 );
    //        write( *val );
    //    }
    //    else if ( anyType == typeid( std::string ) ) {
    //        write( hub::Any::Type::STRING );
    //        const std::string* val = std::any_cast<std::string>( &any );
    //        write( *val );
    //    }
    //    else if ( anyType == typeid( const char* ) ) {
    //        write( hub::Any::Type::CONST_CHAR_PTR );
    //        const char* val = *std::any_cast<const char*>( &any );
    //        assert( sizeof( char ) == 1 );
    //        write( val );
    //    }
    //    else if ( anyType == typeid( std::vector<float> ) ) {
    //        write( hub::Any::Type::VECTOR_FLOAT );
    //        auto* val = std::any_cast<std::vector<float>>( &any );
    //        assert( val->size() == 9 );
    //        write( *val );
    //    }
    //    else if ( anyType == typeid( unsigned int ) ) {
    //        write( hub::Any::Type::UINT );
    //        const unsigned int* val = std::any_cast<unsigned int>( &any );
    //        write( *val );
    //    }
    //    else if ( anyType == typeid( const float* ) ) {
    //        write( hub::Any::Type::CONST_FLOAT_PTR );
    //        const float* val = *std::any_cast<const float*>( &any );
    //        write( reinterpret_cast<const unsigned char*>( val ), 64 );
    //    }
    //    else {
    //        std::cerr << "non supported type : '" << anyType.name() << "'" << std::endl;
    //        assert( false );
    //    }
}

/////////////////////////////////////////////////////////////////////////////

} // namespace io
} // namespace hub
