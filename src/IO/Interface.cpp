#include "Interface.hpp"

#include <any>
#include <cstring>
#include <iostream>
#include <typeindex>
#include <typeinfo>

namespace hub {
namespace io {

///
/// \brief The AnyType enum
/// allows to unify the data according to the different architecture (32, 64 bits).
///
enum class AnyType {
    INT = 0,
    DOUBLE,
    STRING,
    CONST_CHAR_PTR,
    VECTOR_FLOAT,
    UINT,
    CONST_FLOAT_PTR,
    //        MAT3,
    //        FLOAT_ARRAY_9,
    COUNT
};
static const std::string s_anyType2string[static_cast<int>( AnyType::COUNT )] = {
    "int",
    "double",
    "string",
    "cst_char_ptr",
    "vector_float",
    "uint",
    "cst_float_ptr",
};
static const std::map<size_t, AnyType> s_hash2anyType = {
    { typeid( int ).hash_code(), AnyType::INT },
    { typeid( double ).hash_code(), AnyType::DOUBLE },
    { typeid( std::string ).hash_code(), AnyType::STRING },
    { typeid( const char* ).hash_code(), AnyType::CONST_CHAR_PTR },
    { typeid( std::vector<float> ).hash_code(), AnyType::VECTOR_FLOAT },
    { typeid( unsigned int ).hash_code(), AnyType::UINT },
    { typeid( const float* ).hash_code(), AnyType::CONST_FLOAT_PTR },
};
std::ostream& operator<<( std::ostream& os, const AnyType& type ) {
    os << s_anyType2string[(int)type];
    return os;
}

std::string Interface::anyValue2string( const std::any& any ) {
    assert( any.has_value() );
    const auto& hashCode = any.type().hash_code();

    assert( s_hash2anyType.find( hashCode ) != s_hash2anyType.end() );
    AnyType anyType = s_hash2anyType.at( hashCode );

    switch ( anyType ) {
    case AnyType::INT: {
        const int* val = std::any_cast<int>( &any );
        return std::to_string( *val );
    } break;

    case AnyType::DOUBLE: {
        const double* val = std::any_cast<double>( &any );
        return std::to_string( *val );
    } break;

    case AnyType::STRING: {
        const std::string* val = std::any_cast<std::string>( &any );
        return std::string( *val );
    } break;

    case AnyType::CONST_CHAR_PTR: {
        const char* val = *std::any_cast<const char*>( &any );
        return std::string( val );
    } break;

    case AnyType::VECTOR_FLOAT: {
        const std::vector<float>* val = std::any_cast<std::vector<float>>( &any );
        std::string str               = "";
        const int n                   = 3;
        for ( int i = 0; i < n; ++i ) {
            for ( int j = 0; j < n; ++j ) {
                char buff[32];
                const int k = i * n + j;
                // sprintf(buff, "%.1f", val->at(k));
                //                sprintf_s( buff, "%.1f", val->at( k ) );
#ifdef WIN32
                sprintf_s(buff, "%.1f", val->at( k ) );
#else
                sprintf( buff, "%.1f", val->at( k ) );
#endif
                str += buff;
                //            str += std::to_string(val->at(i));
                if ( j != 2 ) str += " ";
            }
            if ( i != 2 ) str += "  ";
        }
        return str;
    } break;

    case AnyType::UINT: {
        const unsigned int* val = std::any_cast<unsigned int>( &any );
        return std::to_string( *val );
    } break;

    case AnyType::CONST_FLOAT_PTR: {
        const float* val = *std::any_cast<const float*>( &any );
        std::string str  = "";
        for ( int i = 0; i < 4; ++i ) {
            for ( int j = 0; j < 4; ++j ) {
                char buff[32];
#ifdef WIN32
                sprintf_s(buff, "%.0f ", val[4 * i + j] );
#else
                sprintf( buff, "%.0f ", val[4 * i + j] );
#endif
                str += buff;
            }
                if ( i != 3 ) str += " ";
        }
        return str;
    } break;

    default:
//        auto name     = any.type().name();
//        auto raw_name = any.type().name();
        assert( false );
    }

    //    else if ( hashCode == typeid( hub::io::Interface::Mat3 ).hash_code() ) {
    //        const hub::io::Interface::Mat3* val = std::any_cast<hub::io::Interface::Mat3>( &any );
    //        std::string str                     = "";
    //        for ( int i = 0; i < 3; ++i ) {
    //            for ( int j = 0; j < 3; ++j ) {
    //                str += std::to_string( val->data[i * 3 + j] ) + " ";
    //            }
    //            str += "\n";
    //        }
    //        return str;
    //    }
        return "";
}

const std::string& Interface::anyType2string( const std::any& any ) {
    assert( any.has_value() );
    const auto& hashCode = any.type().hash_code();

    assert( s_hash2anyType.find( hashCode ) != s_hash2anyType.end() );
    AnyType anyType = s_hash2anyType.at( hashCode );

    return s_anyType2string[static_cast<int>( anyType )];
}

void Interface::write( const std::string& str ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(std::string) : '" << str << "' start" << std::endl;
#endif

    int strLen = static_cast<int>( str.size() );
    write( strLen );

    if ( strLen > 0 ) {
        const char* data = str.data();
        write( (const unsigned char*)data, strLen );
    }
}

void Interface::write( const SensorSpec& sensorSpec ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(SensorSpec) : start" << std::endl;
#endif

    write( sensorSpec.m_sensorName );
    write( sensorSpec.m_resolutions );
    //    write( sensorSpec.m_format );
    //    write( sensorSpec.m_dims );
    write( sensorSpec.m_metaData );
}

void Interface::write( const Measure& measure ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(Measure) : start" << std::endl;
#endif

    write( measure.m_size );
    write( measure.m_data, measure.m_size );
}

void Interface::write( const Acquisition& acq ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(Acquisition) : start" << std::endl;
#endif

    write( acq.m_start );
    write( acq.m_end );
    const auto & measures = acq.getMeasures();
    assert(measures.size() > 0);
    write( measures );
}

void Interface::write( const char* str ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(const char*) : start" << std::endl;
#endif

    int strLen = static_cast<int>( strlen( str ) );
    write( strLen );

    if ( strLen > 0 ) { write( (const unsigned char*)str, strLen ); }
}

// enum class ANY_TYPE {
// };

// hash of type info for 64 bit only, no conversion for 32 bits (same value in uint64_t)
// uint64_t typeInfoHash64( const char _DecoratedName[1] ) {
//    //#ifdef _WIN64
//    // static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
//    uint64_t const fnv_offset_basis = 14695981039346656037ULL;
//    uint64_t const fnv_prime        = 1099511628211ULL;
//    //#else
//    //    static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
//    //    size_t const fnv_offset_basis = 2166136261U;
//    //    size_t const fnv_prime = 16777619U;
//    //#endif

//    uint64_t value = fnv_offset_basis;
//    for ( char const* it = _DecoratedName + 1; *it != '\0'; ++it ) {
//        value ^= static_cast<uint64_t>( static_cast<unsigned char>( *it ) );
//        value *= fnv_prime;
//    }

//    //#ifdef _WIN64
//    static_assert( sizeof( uint64_t ) == 8, "This code is for 64-bit size_t." );
//    value ^= value >> 32;
//    //#else
//    //    static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
//    //#endif

//    return value;
//}

// Interface::~Interface()
//{
//     close();
// }

void Interface::write( const std::any& any ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(std::any) : start" << std::endl;
#    ifdef WIN32
    std::cout << "any raw name = '" << any.type().raw_name() << "'" << std::endl;
#    else
    std::cout << "any raw name = '" << any.type().name() << "'" << std::endl;
#    endif
#endif

    assert( any.has_value() );
    const auto& anyType = any.type();

    if ( anyType == typeid( int ) ) {
        write( AnyType::INT );
        const int* val = std::any_cast<int>( &any );
        assert( sizeof( int ) == 4 );
        write( *val );
    }
    else if ( anyType == typeid( double ) ) {
        write( AnyType::DOUBLE );
        const double* val = std::any_cast<double>( &any );
        assert( sizeof( double ) == 8 );
        write( *val );
    }
    else if ( anyType == typeid( std::string ) ) {
        write( AnyType::STRING );
        const std::string* val = std::any_cast<std::string>( &any );
        write( *val );
    }
    else if ( anyType == typeid( const char* ) ) {
        write( AnyType::CONST_CHAR_PTR );
        const char* val = *std::any_cast<const char*>( &any );
        assert( sizeof( char ) == 1 );
        write( val );
    }
    else if ( anyType == typeid( std::vector<float> ) ) {
        write( AnyType::VECTOR_FLOAT );
        auto* val = std::any_cast<std::vector<float>>( &any );
        assert( val->size() == 9 );
        write( *val );
    }
    else if ( anyType == typeid( unsigned int ) ) {
        write( AnyType::UINT );
        const unsigned int* val = std::any_cast<unsigned int>( &any );
        //        assert( sizeof( int ) == 4 );
        write( *val );
    }
    else if ( anyType == typeid( const float* ) ) {
        write( AnyType::CONST_FLOAT_PTR );
        const float* val = *std::any_cast<const float*>( &any );
        //        assert( sizeof( float ) == 1 );
        write( (unsigned char*)val, 64 );
    }
    //    else if ( anyType == typeid( Mat3 ) ) {
    //        write( AnyType::MAT3 );
    //        auto* val = std::any_cast<Mat3>( &any );
    //        //        assert(val->size() == 9);
    //        write( *val );
    //    }
    else {
//        auto name     = any.type().name();
//        auto raw_name = any.type().name();
        assert( false );
    }
}

// void Interface::write(const std::map<std::string, std::any> &map) const
//{

//}

void Interface::read( std::string& str ) const {

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str = ""; }
    else {
        const int buffSize = strLen + 1;
        unsigned char* tmp = new unsigned char[buffSize];
        read( tmp, strLen );
        tmp[strLen] = 0;
        str         = std::string( (char*)tmp );
        delete[] tmp;
    }
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::string) : '" << str << "'" << std::endl;
#endif
}

void Interface::read( SensorSpec& sensorSpec ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(SensorSpec) : start" << std::endl;
#endif

    read( sensorSpec.m_sensorName );
    read( sensorSpec.m_resolutions );
    read( sensorSpec.m_metaData );

    sensorSpec.m_acquisitionSize = SensorSpec::computeAcquisitionSize( sensorSpec.m_resolutions );
}

Measure Interface::getMeasure() const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] getMeasure() : start" << std::endl;
#endif

    size_t size;
    read( size );
    unsigned char* data = new unsigned char[size];
    read( data, size );

    Measure measure( data, size );
    //    delete [] data;
    //    measure.m_ownData = true;
    return measure;
}

// void Interface::read(Measure &measure) const
//{
//     read(measure.m_data, measure.m_size);
// }

SensorSpec Interface::getSensorSpec() const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] getSensorSpec() : start" << std::endl;
#endif

    SensorSpec sensorSpec;
    read( sensorSpec );
    return sensorSpec;
}

Acquisition Interface::getAcquisition( int acquisitionSize ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] getAcquisition() : start" << std::endl;
#endif

    long long start, end;

    read( start );
    read( end );

    Acquisition acq( start, end );

    int nMeasures;
    read( nMeasures );
    assert(nMeasures > 0);

    for ( int iMeasure = 0; iMeasure < nMeasures; ++iMeasure ) {
        acq << Interface::getMeasure();
    }

    assert( acquisitionSize == acq.getSize() );

    return acq;
}

void Interface::read( char* str ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read char* : start" << std::endl;
#endif

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str[0] = 0; }
    else {
        read( (unsigned char*)str, strLen );

        str[strLen] = 0;
    }
}

void Interface::read( std::any& any ) const {
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::any) : start" << std::endl;
#endif

    AnyType type;
    read( type );

    switch ( type ) {
    case AnyType::INT: {
        assert( sizeof( int ) == 4 );
        int val;
        read( val );
        any = std::any_cast<int>( val );
    } break;

    case AnyType::DOUBLE: {
        double val;
        assert( sizeof( double ) == 8 );
        read( val );
        any = std::any_cast<double>( val );
    } break;

    case AnyType::STRING: {
        std::string val;
        read( val );
        any = std::any_cast<std::string>( val );
    } break;

    case AnyType::CONST_CHAR_PTR: {
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

        any = std::any_cast<const char*>( (const char*)str );

    } break;

    case AnyType::VECTOR_FLOAT: {
        std::vector<float> val;
        read( val );
        any = std::any_cast<std::vector<float>>( val );
    } break;

    case AnyType::UINT: {
        unsigned int val;
        read( val );
        any = std::any_cast<unsigned int>( val );
    } break;

    case AnyType::CONST_FLOAT_PTR: {
        float* buff = new float[16];
        read( (unsigned char*)buff, 64 );

        any = std::any_cast<const float*>( (const float*)buff );

    } break;

        //    case AnyType::MAT3: {
        //        Mat3 val;
        //        read( val );
        //        any = std::any_cast<Mat3>( val );
        //    } break;

    default:
//        auto name     = any.type().name();
//        auto raw_name = any.type().name();
        assert( false );
    }
    assert( any.has_value() );
}

} // namespace io
} // namespace hub
