#include "AnyUtils.hpp"

#include <cassert>
#include <map>
#include <vector>

//using namespace Any;

namespace hub {
//namespace any {


static const std::map<size_t, Any::Type> s_hash2anyType = {
    { typeid( int ).hash_code(), Any::Type::INT },
    { typeid( double ).hash_code(), Any::Type::DOUBLE },
    { typeid( std::string ).hash_code(), Any::Type::STRING },
    { typeid( const char* ).hash_code(), Any::Type::CONST_CHAR_PTR },
    { typeid( std::vector<float> ).hash_code(), Any::Type::VECTOR_FLOAT },
    { typeid( unsigned int ).hash_code(), Any::Type::UINT },
    { typeid( const float* ).hash_code(), Any::Type::CONST_FLOAT_PTR },
};

std::string Any::anyValue2string( const std::any& any ) {
    assert( any.has_value() );
    const auto& hashCode = any.type().hash_code();

    assert( s_hash2anyType.find( hashCode ) != s_hash2anyType.end() );
    Any::Type anyType = s_hash2anyType.at( hashCode );

    switch ( anyType ) {
    case Any::Type::INT: {
        const int* val = std::any_cast<int>( &any );
        return std::to_string( *val );
    } break;

    case Any::Type::DOUBLE: {
        const double* val = std::any_cast<double>( &any );
        return std::to_string( *val );
    } break;

    case Any::Type::STRING: {
        const std::string* val = std::any_cast<std::string>( &any );
        return std::string( *val );
    } break;

    case Any::Type::CONST_CHAR_PTR: {
        const char* val = *std::any_cast<const char*>( &any );
        return std::string( val );
    } break;

    case Any::Type::VECTOR_FLOAT: {
        const std::vector<float>* val = std::any_cast<std::vector<float>>( &any );
        std::string str               = "";
        const int n                   = 3;
        for ( int i = 0; i < n; ++i ) {
            for ( int j = 0; j < n; ++j ) {
                char buff[32];
                const int k = i * n + j;
#ifdef WIN32
                sprintf_s( buff, "%.1f", val->at( k ) );
#else
                sprintf( buff, "%.1f", val->at( k ) );
#endif
                str += buff;
                if ( j != 2 ) str += " ";
            }
            if ( i != 2 ) str += "  ";
        }
        return str;
    } break;

    case Any::Type::UINT: {
        const unsigned int* val = std::any_cast<unsigned int>( &any );
        return std::to_string( *val );
    } break;

    case Any::Type::CONST_FLOAT_PTR: {
        const float* val = *std::any_cast<const float*>( &any );
        std::string str  = "";
        for ( int i = 0; i < 4; ++i ) {
            for ( int j = 0; j < 4; ++j ) {
                char buff[32];
#ifdef WIN32
                sprintf_s( buff, "%.0f ", val[4 * i + j] );
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
        assert( false );
    }

    return "";
}

static const std::string s_anyType2string[static_cast<int>( Any::Type::COUNT )] = {
    "int",
    "double",
    "string",
    "cst_char_ptr",
    "vector_float",
    "uint",
    "cst_float_ptr",
};

const std::string& Any::anyType2string( const std::any& any ) {
    assert( any.has_value() );
    const auto& hashCode = any.type().hash_code();

    assert( s_hash2anyType.find( hashCode ) != s_hash2anyType.end() );
    Any::Type anyType = s_hash2anyType.at( hashCode );

    return s_anyType2string[static_cast<int>( anyType )];
}

std::ostream& operator<<( std::ostream& os, const Any::Type& type ) {
    os << s_anyType2string[(int)type];
    return os;
}

//} // any
} // hub
