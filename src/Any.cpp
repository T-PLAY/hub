#include "Any.hpp"

#include <cassert>
#include <map>
#include <typeinfo>
#include <vector>
#include <iostream>

#include "data/Mat4.hpp"
#include "data/Mesh.hpp"
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

    case Any::Type::MESH: {
        m_mesh = new data::Mesh( *any.m_mesh );
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

    case Any::Type::MESH: {
        m_mesh = any.m_mesh;
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
    m_type     = Type::MAT4;
    m_hasValue = true;
}

Any::Any(const data::Mesh &value)
    : m_mesh(new data::Mesh(value))
{
    assert( m_type == Type::NONE );
    assert( !m_hasValue );
    m_type     = Type::MESH;
    m_hasValue = true;

}

// Any::Any( const std::vector<float>& value ) {
// }
// Any::Any( unsigned int value ) {
// }
// Any::Any( const float* value ) {
// }
// Any::Any( const double* value ) {
// }

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
    assert(m_mat4 != nullptr);
    return *m_mat4;
}

const data::Mesh &Any::getMesh() const
{
    assert( m_type == Type::MESH );
    assert(m_mesh != nullptr);
    return *m_mesh;
}
// const std::vector<float>& Any::getStdVectorFloat() const {
// }
// const unsigned int& Any::getUnsignedInt() const {
// }
// const float* const Any::getConstFloatPtr() const {
// }
// const double* const Any::getConstDoublePtr() const {
// }

// std::string Any::anyValue2string( const std::any& any ) {
std::string anyValue2string( const Any& any ) {

    if ( any.has_value() ) {

        switch ( any.type() ) {
        case Any::Type::INT: {
            const auto& val = any.getInt();
            return std::to_string( val );
        } break;

        case Any::Type::DOUBLE: {
            const auto& val = any.getDouble();
            return std::to_string( val );
        } break;

        case Any::Type::STRING: {
            const auto& val = "'" + any.getStdString() + "'";
            return val;
        } break;

        case Any::Type::CONST_CHAR_PTR: {
            const auto& val = any.getConstCharPtr();
            return "'" + std::string( val ) + "'";
        } break;

        case Any::Type::MAT4: {
            const auto& val = any.getMat4();
            return val.to_string();
        } break;

        case Any::Type::MESH: {
            const auto& val = any.getMesh();
            return val.to_string();
        } break;

        default:
            assert(false);
        }
    }

    return "";

    // #ifdef WIN32
    // #else
    // #endif

    // #ifdef WIN32
    // #else
    // #endif

    // #ifdef WIN32
    // #else
    // #endif
}

static const std::string s_anyType2string[static_cast<int>( Any::Type::COUNT )] = {
    "none",
    "int",
    "double",
    "string",
    "cst_char_ptr",
    "mat4",
    "mesh",
};

const std::string& Any::typeName() const {
    return s_anyType2string[static_cast<int>( m_type )];
}

//// const std::string& Any::anyType2string( const std::any& any ) {
// const std::string& anyType2string( const Any& any ) {

//}

std::ostream& operator<<( std::ostream& os, const Any::Type& type ) {
    os << s_anyType2string[(int)type];
    return os;
}

std::string Any::to_string() const {
    std::string ret = "'" + Any::typeName() + "' = " + anyValue2string( *this );
    return ret;
}

bool Any::operator==( const Any& any ) const {
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
            return *m_mat4 == *any.m_mat4;
        } break;

        case Any::Type::MESH: {
            return *m_mesh == *any.m_mesh;
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

// #ifdef WIN32
// #else
// #endif

// #ifdef WIN32
// #else
// #endif
