#include "Any.hpp"

#include <cstring>
#include <iostream>
#include <map>
#include <typeinfo>
#include <vector>

// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"
#include "Anyable.hpp"

namespace hub {

std::string Any::getValueStr() const {
    // todo
    //    std::stringstream sstream;
    //    double a;
    //    std::vector<AnyHelper> anyables;
    //    AnyHelper anyable = make_anyable<int>();
    //    AnyHelper<double>();
    //    AnyHelper anyable = AnyHelper<int>();
    //    anyable.setup<int>();
    //    AnyHelper<int>();

//    const auto hashCode = m_any.type().hash_code();
//    assert( s_anyHelpers.find( hashCode ) != s_anyHelpers.end() );

//    const auto& anyable = s_anyHelpers.at( m_any.type().hash_code() );
//    assert( anyable.getValueStr != nullptr );
    assert(m_anyHelper != nullptr);
    assert(m_anyHelper->getValueStr != nullptr);

    return m_anyHelper->getValueStr( m_any );

    //    if ( type() == typeid( int ) ) { sstream << get<int>(); }
    //            sstream << *this;
    //    return sstream.str();
    //    return "";
    //     return m_any2valueStr( m_any );
}

#if CPLUSPLUS_VERSION <= 17
bool Any::operator!=( const Any& any ) const {
    return !( *this == any );
}
#endif

std::ostream& operator<<( std::ostream& os, const Any& any ) {
//    if ( any.hasValue() ) {
        //        if ( any.type() == typeid( int ) ) {
        //            os << "*'" << TYPE_NAME( int ) << "' : " << any.get<int>() << "*";
        //        }
//        const auto hashCode = any.type().hash_code();
//        assert( Anyable::s_anyables.find( hashCode ) != Anyable::s_anyHelpers.end() );

//        const auto& anyable = Anyable::s_anyHelpers.at( hashCode );
//        assert( anyable.getTypeName != nullptr );
//        assert( anyable.getValueStr != nullptr );
        //        os << "*'" << anyable.getTypeName() << "' : ";
    assert(any.m_anyHelper != nullptr);
    assert(any.m_anyHelper->getTypeName != nullptr);
    assert(any.m_anyHelper->getValueStr != nullptr);
        os << "*'" << any.m_anyHelper->getTypeName() << "' : " << any.m_anyHelper->getValueStr( any.m_any ) << "*";

        //        return anyable.compare( m_any, any.m_any );
//    }
//    else { os << "*'void' : nil*"; }
    return os;
}


bool Any::operator==( const Any& any ) const {
//    assert( hasValue() );
    //    assert( m_type == any.m_type );
    assert( m_any.type() == any.m_any.type() );
//    assert( hasValue() == any.hasValue() );
//    if (! hasValue()) {
//        return true;
//    }
    //    if ( m_type == any.m_type && hasValue() == any.m_hasValue() ) {
    //    if ( m_type == any.m_type) {

    //    if ( any.type() == typeid( int ) ) { return get<int>() == any.get<int>(); }
//    const auto hashCode = m_any.type().hash_code();
//    assert( s_anyHelpers.find( hashCode ) != s_anyHelpers.end() );

    assert(m_anyHelper != nullptr);
    assert(m_anyHelper->compare != nullptr);
//    const auto& anyable = s_anyHelpers.at( m_any.type().hash_code() );
//    assert( anyable.compare != nullptr );
//    return anyable.compare( m_any, any.m_any );
    return m_anyHelper->compare(m_any, any.m_any);
}

    // todo
    //    assert( m_anyCompare != nullptr );
    //    return m_anyCompare( m_any, any.m_any );
    //    return true;

    //    const auto& anyType = m_any.type();

    //    if ( anyType == typeid( int ) ) { return get<int>() == any.get<int>(); }
    //    else if ( anyType == typeid( double ) ) { return get<double>() == any.get<double>(); }
    //    else if ( anyType == typeid( std::string ) ) {
    //        return get<std::string>() == any.get<std::string>();
    //    }
    //    else if ( anyType == typeid( const char* ) ) {
    //        return !strcmp( get<const char*>(), any.get<const char*>() );
    //    }
    //    else {
    //        //        assert(false);
    //        return false;
    //    }

    //        switch ( m_type ) {
    //        case Any::Type::INT: {
    //            return get<int>() == any.get<int>();
    ////            return m_int == any.m_int;
    //        } break;

    //        case Any::Type::DOUBLE: {
    //            return get<double>() == any.get<double>();
    ////            return m_double == any.m_double;
    //        } break;

    //        case Any::Type::STRING: {
    //            return get<std::string>() == any.get<std::string>();
    ////            return m_std_string == any.m_std_string;
    //        } break;

    //        case Any::Type::CONST_CHAR_PTR: {
    ////            return get<const char *>() == any.get<const char *>();
    //            return !strcmp( get<const char*>(), any.get<const char*>() );
    ////            return !strcmp( m_const_char_ptr, any.m_const_char_ptr );
    //        } break;

    //            // todo any
    ////        case Any::Type::MAT4: {
    ////            return get<data::Mat4>() == any.get<data::Mat4>();
    //////            return *m_mat4 == *any.m_mat4;
    ////        } break;

    ////        case Any::Type::MESH: {
    ////            return get<data::Mesh>() == any.get<data::Mesh>();
    //////            return *m_mesh == *any.m_mesh;
    ////        } break;

    //        default:
    //            assert( false );
    //        }
    ////    }

//    return false;
//}


// template <class T>

// s_anyables[0] = AnyHelper();

// Any::Any( const Any& any ) : m_type( any.m_type ), m_hasValue( any.m_hasValue ) {
//     assert( any.m_hasValue );

//    switch ( m_type ) {
//    case Any::Type::INT: {
//        m_int = any.m_int;
//    } break;

//    case Any::Type::DOUBLE: {
//        m_double = any.m_double;
//    } break;

//    case Any::Type::STRING: {
//        m_std_string = any.m_std_string;
//    } break;

//    case Any::Type::CONST_CHAR_PTR: {
//        m_const_char_ptr = any.m_const_char_ptr;
//    } break;

//    case Any::Type::MAT4: {
//        m_mat4 = new data::Mat4( *any.m_mat4 );
//    } break;

//    case Any::Type::MESH: {
//        m_mesh = new data::Mesh( *any.m_mesh );
//    } break;

//    default:
//        assert( false );
//    }
//}

// Any& Any::operator=( const Any& any ) {
//     m_type     = any.m_type;
//     m_hasValue = any.m_hasValue;
//     assert( any.m_hasValue );

//    switch ( m_type ) {
//    case Any::Type::INT: {
//        m_int = any.m_int;
//    } break;

//    case Any::Type::DOUBLE: {
//        m_double = any.m_double;
//    } break;

//    case Any::Type::STRING: {
//        m_std_string = any.m_std_string;
//    } break;

//    case Any::Type::CONST_CHAR_PTR: {
//        m_const_char_ptr = any.m_const_char_ptr;
//    } break;

//    case Any::Type::MAT4: {
//        m_mat4 = any.m_mat4;
//    } break;

//    case Any::Type::MESH: {
//        m_mesh = any.m_mesh;
//    } break;

//    default:
//        assert( false );
//    }

//    return *this;
//}

// Any::Any( int value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::INT;
//     m_int      = value;
//     m_hasValue = true;
// }
// Any::Any( double value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::DOUBLE;
//     m_double   = value;
//     m_hasValue = true;
// }
// Any::Any( const std::string& value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type       = Type::STRING;
//     m_std_string = value;
//     m_hasValue   = true;
// }
// Any::Any( const char* value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type           = Type::CONST_CHAR_PTR;
//     m_const_char_ptr = value;
//     m_hasValue       = true;
// }
// Any::Any( const data::Mat4& value ) : m_mat4( new data::Mat4( value ) ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::MAT4;
//     m_hasValue = true;
// }

// Any::Any( const data::Mesh& value ) : m_mesh( new data::Mesh( value ) ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::MESH;
//     m_hasValue = true;
// }

// const int& Any::get<int>() const {
//     assert( m_type == Type::INT );
//     return m_int;
// }
// const double& Any::get<double>() const {
//     assert( m_type == Type::DOUBLE );
//     return m_double;
// }
// const std::string& Any::getStdString() const {
//     assert( m_type == Type::STRING );
//     return m_std_string;
// }
// const char* Any::getConstCharPtr() const {
//     assert( m_type == Type::CONST_CHAR_PTR );
//     return m_const_char_ptr;
// }
// const data::Mat4& Any::getMat4() const {
//     assert( m_type == Type::MAT4 );
//     assert( m_mat4 != nullptr );
//     return *m_mat4;
// }

// const data::Mesh& Any::getMesh() const {
//     assert( m_type == Type::MESH );
//     assert( m_mesh != nullptr );
//     return *m_mesh;
// }

// std::string Any::anyValue2string() const {

//    const auto& any = *this;

//    if ( any.hasValue() ) {

//        return m_any2valueStr(m_any);
//    }
//    return "";

//        const auto& anyType = any.type();

////        return m_anyToString(m_any);

//        if ( anyType == typeid( int ) ) {
//            const auto& val = any.get<int>();
//            return std::to_string( val );
//        }
//        else if ( anyType == typeid( double ) ) {
//            const auto& val = any.get<double>();
//            return std::to_string( val );
//        }
//        else if ( anyType == typeid( std::string ) ) {
//            const auto& val = "'" + any.get<std::string>() + "'";
//            return val;
//        }
//        else if ( anyType == typeid( const char* ) ) {
//            const auto& val = any.get<const char*>();
//            return "'" + std::string( val ) + "'";
//        }
//        else {
//            return m_any2valueStr(m_any);
//            //        assert(false);
//            return "";
//        }

//        switch ( any.type() ) {
//        case Any::Type::INT: {
//            const auto& val = any.get<int>();
//            return std::to_string( val );
//        } break;

//        case Any::Type::DOUBLE: {
//            const auto& val = any.get<double>();
//            return std::to_string( val );
//        } break;

//        case Any::Type::STRING: {
//            const auto& val = "'" + any.get<std::string>() + "'";
//            return val;
//        } break;

//        case Any::Type::CONST_CHAR_PTR: {
//            const auto& val = any.get<const char*>();
//            return "'" + std::string( val ) + "'";
//        } break;

//            // todo any
////        case Any::Type::MAT4: {
////            const auto& val = any.get<data::Mat4>();
////            return val.to_string();
////        } break;

////        case Any::Type::MESH: {
////            const auto& val = any.get<data::Mesh>();
////            return val.to_string();
////        } break;

//        default:
//            assert( false );
//        }
//    }

//    return "";
//}

// static const std::string s_anyType2string[static_cast<int>( Any::Type::COUNT )] = {
//     "none",
//     "int",
//     "double",
//     "string",
//     "cst_char_ptr",
//     "mat4",
//     "mesh",
// };

// Any::Type Any::type() const
//{
//     return m_type;
// }

// const std::type_info & Any::typeId() const
//{
//     return m_any.type();
// }

// const std::string& Any::typeName() const {
//     return s_anyType2string[static_cast<int>( m_type )];
// }

// std::ostream& operator<<( std::ostream& os, const Any::Type& type ) {
//     os << s_anyType2string[(int)type];
//     return os;
// }

// std::string Any::to_string() const {
//     //    std::string ret = "'" + Any::typeName() + "' = " + anyValue2string( *this );

//    //    const auto& any = *this;

//    if ( hasValue() ) {
//        //    return m_any2valueStr(m_any);
////        return "*'" + std::string( m_any.type().name() ) + "' : " + m_any2valueStr( m_any ) +
///"*";

//        // todo
////        assert(m_typeName);
////        return "*'" + m_typeName() + "' : " + m_any2valueStr( m_any ) + "*";
//        if (m_any.type() == typeid(int)) {

//        }
//        return "*";

//        //    return ret;
//    }
//    else {
////        return "*'" + std::string( m_any.type().name() ) + "' : none*";
//        return "*'void' : none*";
//    }
//}

//void Any::printTable() {
    //        assert( s_anyables.find( hashCode ) != s_anyables.end() );
//}

//std::vector<std::string> Any::supportedTypes()
//{
//    std::vector<std::string> ret;
//    for ( const auto& pair : s_anyHelpers ) {
//        ret.push_back("<" + pair.second.getTypeName() + ">");
////        std::cout << pair.first << " -> " << pair.second.getTypeName() << std::endl;
//    }
//    return ret;
//}


} // namespace hub
