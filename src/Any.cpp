#include "Any.hpp"

#include <cassert>
#include <map>
#include <typeinfo>
#include <vector>

#include "data/Mat4.hpp"
// using namespace Any;

namespace hub {
// namespace any {

Any::Any( const Any& any ) : m_type( any.m_type ), m_hasValue( any.m_hasValue ) {
    assert( any.m_hasValue );

    switch ( m_type ) {
    case Any::Type::INT: {
        m_int = any.m_int;
    } break;

    case Any::Type::DOUBLE: {
        m_double = any.m_double;
    } break;

    case Any::Type::STRING: {
        m_std_string = any.m_std_string;
    } break;

    case Any::Type::CONST_CHAR_PTR: {
        m_const_char_ptr = any.m_const_char_ptr;
    } break;

    case Any::Type::MAT4: {
        m_mat4 = new data::Mat4( *any.m_mat4 );
    } break;

    default:
        assert( false );
    }
}

Any& Any::operator=( const Any& any ) {
    m_type     = any.m_type;
    m_hasValue = any.m_hasValue;
    assert( any.m_hasValue );

    switch ( m_type ) {
    case Any::Type::INT: {
        m_int = any.m_int;
    } break;

    case Any::Type::DOUBLE: {
        m_double = any.m_double;
    } break;

    case Any::Type::STRING: {
        m_std_string = any.m_std_string;
    } break;

    case Any::Type::CONST_CHAR_PTR: {
        m_const_char_ptr = any.m_const_char_ptr;
    } break;

    case Any::Type::MAT4: {
        m_mat4 = any.m_mat4;
    } break;

    default:
        assert( false );
    }

    return *this;
}

Any::Any( int value ) {
    assert( m_type == Type::NONE );
    assert( !m_hasValue );
    m_type     = Type::INT;
    m_int      = value;
    m_hasValue = true;
}
Any::Any( double value ) {
    assert( m_type == Type::NONE );
    assert( !m_hasValue );
    m_type     = Type::DOUBLE;
    m_double   = value;
    m_hasValue = true;
}
Any::Any( const std::string& value ) {
    assert( m_type == Type::NONE );
    assert( !m_hasValue );
    m_type       = Type::STRING;
    m_std_string = value;
    m_hasValue   = true;
}
Any::Any( const char* value ) {
    assert( m_type == Type::NONE );
    assert( !m_hasValue );
    m_type           = Type::CONST_CHAR_PTR;
    m_const_char_ptr = value;
    m_hasValue       = true;
}
Any::Any( const data::Mat4& value ) : m_mat4( new data::Mat4( value ) ) {
    assert( m_type == Type::NONE );
    assert( !m_hasValue );
    m_type = Type::MAT4;
    //        m_mat4 = std::move(value);
    m_hasValue = true;
}

// Any::Any( const std::vector<float>& value ) {
//     assert(m_type == Type::NONE);
//     assert(! m_hasValue);
//     m_type             = Type::VECTOR_FLOAT;
//     m_std_vector_float = value;
//     m_hasValue         = true;
// }
// Any::Any( unsigned int value ) {
//     assert(m_type == Type::NONE);
//     assert(! m_hasValue);
//     m_type         = Type::UINT;
//     m_unsigned_int = value;
//     m_hasValue     = true;
// }
// Any::Any( const float* value ) {
//     assert(m_type == Type::NONE);
//     assert(! m_hasValue);
//     m_type            = Type::CONST_FLOAT_PTR;
//     m_const_float_ptr = value;
//     m_hasValue        = true;
// }
// Any::Any( const double* value ) {
//     assert(m_type == Type::NONE);
//     assert(! m_hasValue);
//     m_type             = Type::CONST_DOUBLE_PTR;
//     m_const_double_ptr = value;
//     m_hasValue         = true;
// }
//  static const std::map<size_t, Any::Type> s_hash2anyType = {
//      { typeid( int ).hash_code(), Any::Type::INT },
//      { typeid( double ).hash_code(), Any::Type::DOUBLE },
//      { typeid( std::string ).hash_code(), Any::Type::STRING },
//      { typeid( const char* ).hash_code(), Any::Type::CONST_CHAR_PTR },
//      { typeid( std::vector<float> ).hash_code(), Any::Type::VECTOR_FLOAT },
//      { typeid( unsigned int ).hash_code(), Any::Type::UINT },
//      { typeid( const float* ).hash_code(), Any::Type::CONST_FLOAT_PTR },
//  };

const int& Any::getInt() const {
    assert( m_type == Type::INT );
    return m_int;
}
const double& Any::getDouble() const {
    assert( m_type == Type::DOUBLE );
    return m_double;
}
const std::string& Any::getStdString() const {
    assert( m_type == Type::STRING );
    return m_std_string;
}
const char* const Any::getConstCharPtr() const {
    assert( m_type == Type::CONST_CHAR_PTR );
    return m_const_char_ptr;
}
const data::Mat4& Any::getMat4() const {
    assert( m_type == Type::MAT4 );
    return *m_mat4;
}
// const std::vector<float>& Any::getStdVectorFloat() const {
//     assert( m_type == Type::VECTOR_FLOAT );
//     return m_std_vector_float;
// }
// const unsigned int& Any::getUnsignedInt() const {
//     assert( m_type == Type::UINT );
//     return m_unsigned_int;
// }
// const float* const Any::getConstFloatPtr() const {
//     assert( m_type == Type::CONST_FLOAT_PTR );
//     return m_const_float_ptr;
// }
// const double* const Any::getConstDoublePtr() const {
//     assert( m_type == Type::CONST_DOUBLE_PTR );
//     return m_const_double_ptr;
// }

// std::string Any::anyValue2string( const std::any& any ) {
std::string anyValue2string( const Any& any ) {

    //    assert( any.has_value() );
    //    const auto& hashCode = any.type().hash_code();

    //    assert( s_hash2anyType.find( hashCode ) != s_hash2anyType.end() );
    //    Any::Type anyType = s_hash2anyType.at( hashCode );

    if ( any.has_value() ) {

        switch ( any.type() ) {
        case Any::Type::INT: {
            //        const int* val = std::any_cast<int>( &any );
            const auto& val = any.getInt();
            return std::to_string( val );
        } break;

        case Any::Type::DOUBLE: {
            //        const double* val = std::any_cast<double>( &any );
            const auto& val = any.getDouble();
            return std::to_string( val );
        } break;

        case Any::Type::STRING: {
            //        const std::string* val = std::any_cast<std::string>( &any );
            const auto& val = "'" + any.getStdString() + "'";
            return val;
            //        return std::string( *val );
        } break;

        case Any::Type::CONST_CHAR_PTR: {
            //        const char* val = *std::any_cast<const char*>( &any );
            const auto& val = any.getConstCharPtr();
            return "'" + std::string( val ) + "'";
        } break;

        case Any::Type::MAT4: {
            //        const float* val = *std::any_cast<const float*>( &any );
            const auto& val = any.getMat4();
            return val.to_string();
        } break;

            //    default:
            //        assert( false );
        }
    }

    return "";

    //    case Any::Type::UINT: {
    //        //        const unsigned int* val = std::any_cast<unsigned int>( &any );
    //        const auto& val = any.getUnsignedInt();
    //        return std::to_string( val );
    //    } break;

    //    case Any::Type::VECTOR_FLOAT: {
    //        //        const std::vector<float>* val = std::any_cast<std::vector<float>>( &any );
    //        const auto& val = any.getStdVectorFloat();
    //        std::string str = "";
    //        const int n     = 3;
    //        for ( int i = 0; i < n; ++i ) {
    //            for ( int j = 0; j < n; ++j ) {
    //                char buff[32];
    //                const int k = i * n + j;
    // #ifdef WIN32
    //                sprintf_s( buff, "%.1f", val->at( k ) );
    // #else
    //                sprintf( buff, "%.1f", val.at( k ) );
    // #endif
    //                str += buff;
    //                if ( j != 2 ) str += " ";
    //            }
    //            if ( i != 2 ) str += "  ";
    //        }
    //        return str;
    //    } break;

    //    case Any::Type::CONST_FLOAT_PTR: {
    //        //        const float* val = *std::any_cast<const float*>( &any );
    //        const auto& val = any.getConstFloatPtr();
    //        std::string str = "";
    //        for ( int i = 0; i < 4; ++i ) {
    //            for ( int j = 0; j < 4; ++j ) {
    //                char buff[32];
    // #ifdef WIN32
    //                sprintf_s( buff, "%.0f ", val[4 * i + j] );
    // #else
    //                sprintf( buff, "%.0f ", val[4 * i + j] );
    // #endif
    //                str += buff;
    //            }
    //            if ( i != 3 ) str += " ";
    //        }
    //        return str;
    //    } break;

    //    case Any::Type::CONST_DOUBLE_PTR: {
    //        //        const float* val = *std::any_cast<const float*>( &any );
    //        const auto& val = any.getConstDoublePtr();
    //        std::string str = "";
    //        for ( int i = 0; i < 4; ++i ) {
    //            for ( int j = 0; j < 4; ++j ) {
    //                char buff[32];
    // #ifdef WIN32
    //                sprintf_s( buff, "%.0f ", val[4 * i + j] );
    // #else
    //                sprintf( buff, "%.0f ", val[4 * i + j] );
    // #endif
    //                str += buff;
    //            }
    //            if ( i != 3 ) str += " ";
    //        }
    //        return str;
    //    } break;
}

static const std::string s_anyType2string[static_cast<int>( Any::Type::COUNT )] = {
    "none",
    "int",
    "double",
    "string",
    "cst_char_ptr",
    "mat4",
    //    "vector_float",
    //    "uint",
    //    "cst_float_ptr",
    //    "cst_double_ptr",
};

const std::string& Any::typeName() const {
    return s_anyType2string[static_cast<int>( m_type )];
}

//// const std::string& Any::anyType2string( const std::any& any ) {
// const std::string& anyType2string( const Any& any ) {
//     //    assert( any.has_value() );
//     //    const auto& hashCode = any.type().hash_code();

//    //    assert( s_hash2anyType.find( hashCode ) != s_hash2anyType.end() );
//    //    Any::Type anyType = s_hash2anyType.at( hashCode );

//    //    return s_anyType2string[static_cast<int>( anyType )];
//    return s_anyType2string[static_cast<int>( any.type() )];
//    //    return nullptr;
//}

std::ostream& operator<<( std::ostream& os, const Any::Type& type ) {
    os << s_anyType2string[(int)type];
    return os;
}

std::string Any::to_string() const {
    std::string ret = typeName() + " : " + anyValue2string( *this );
    return ret;
}

bool Any::operator==( const Any& any ) const {
    //    return true;
    assert( m_hasValue );
    assert( m_type == any.m_type );
    assert( m_hasValue );
    assert( any.m_hasValue );
    if ( m_type == any.m_type && m_hasValue == any.m_hasValue ) {

        switch ( m_type ) {
        case Any::Type::INT: {
            return m_int == any.m_int;
        } break;

        case Any::Type::DOUBLE: {
            return m_double == any.m_double;
        } break;

        case Any::Type::STRING: {
            return m_std_string == any.m_std_string;
        } break;

        case Any::Type::CONST_CHAR_PTR: {
            return !strcmp( m_const_char_ptr, any.m_const_char_ptr );
        } break;

        case Any::Type::MAT4: {
            //            return true;
            return *m_mat4 == *any.m_mat4;
        } break;

        default:
            assert( false );
        }
    }

    return false;
}

std::ostream& operator<<( std::ostream& os, const Any& any ) {
    os << any.to_string();
    return os;
}

//} // any
} // namespace hub

//    switch ( anyType ) {
//    case Any::Type::INT: {
//        const int* val = std::any_cast<int>( &any );
//        return std::to_string( *val );
//    } break;

//    case Any::Type::DOUBLE: {
//        const double* val = std::any_cast<double>( &any );
//        return std::to_string( *val );
//    } break;

//    case Any::Type::STRING: {
//        const std::string* val = std::any_cast<std::string>( &any );
//        return std::string( *val );
//    } break;

//    case Any::Type::CONST_CHAR_PTR: {
//        const char* val = *std::any_cast<const char*>( &any );
//        return std::string( val );
//    } break;

//    case Any::Type::VECTOR_FLOAT: {
//        const std::vector<float>* val = std::any_cast<std::vector<float>>( &any );
//        std::string str               = "";
//        const int n                   = 3;
//        for ( int i = 0; i < n; ++i ) {
//            for ( int j = 0; j < n; ++j ) {
//                char buff[32];
//                const int k = i * n + j;
// #ifdef WIN32
//                sprintf_s( buff, "%.1f", val->at( k ) );
// #else
//                sprintf( buff, "%.1f", val->at( k ) );
// #endif
//                str += buff;
//                if ( j != 2 ) str += " ";
//            }
//            if ( i != 2 ) str += "  ";
//        }
//        return str;
//    } break;

//    case Any::Type::UINT: {
//        const unsigned int* val = std::any_cast<unsigned int>( &any );
//        return std::to_string( *val );
//    } break;

//    case Any::Type::CONST_FLOAT_PTR: {
//        const float* val = *std::any_cast<const float*>( &any );
//        std::string str  = "";
//        for ( int i = 0; i < 4; ++i ) {
//            for ( int j = 0; j < 4; ++j ) {
//                char buff[32];
// #ifdef WIN32
//                sprintf_s( buff, "%.0f ", val[4 * i + j] );
// #else
//                sprintf( buff, "%.0f ", val[4 * i + j] );
// #endif
//                str += buff;
//            }
//            if ( i != 3 ) str += " ";
//        }
//        return str;
//    } break;

//    default:
//        assert( false );
//    }
