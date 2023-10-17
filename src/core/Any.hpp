
#pragma once

#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#if CPLUSPLUS_VERSION <= 14
    #include "std_any.hpp"
#else
    #include <any>
#endif

//#include "std_any.hpp"

#include "Macros.hpp"

// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"

//#define HUB_USE_BOOST
//#ifdef HUB_USE_BOOST
//#    include <boost/type_index.hpp>
//#endif

namespace hub {

// namespace data {
// class Mat4;
// class Mesh;
// } // namespace data

///
/// \brief The Any class
/// represents any value.
///
class SRC_API Any
{
  public:
    //    template <typename T>
    //    //     using readable_t = decltype( std::declval<T>().isReadable() );
    //    using anyable_t = decltype( std::declval<T>().anyable() );

    //    template <typename T, typename = std::void_t<>>
    //    struct anyable : std::false_type {};

    //    template <typename T>
    //    struct anyable<T, std::void_t<anyable_t<T>>> : std::true_type {};

    //    template <typename T>
    //    static constexpr bool anyable_v = anyable<T>::value;

    ///
    /// \brief The AnyType enum
    /// allows to unify the data according to the different architecture (32, 64 bits).
    ///
    //    enum class Type { NONE = 0, INT, DOUBLE, STRING, CONST_CHAR_PTR, MAT4, MESH, COUNT };

    template <typename T>
    Any( const T* t ) : m_any( t ) {
        m_typeName = [](  ) {
//            return std::string("const ") + TYPE_NAME(T) + " *";
            return std::string(TYPE_NAME(T)) + "*";
        };

        m_any2valueStr = []( const std::any& any ) {
            assert( typeid( const T* ) == any.type() );
            const T* val = std::any_cast<const T*>( any );
            std::stringstream sstream;
            sstream << val;
            return sstream.str();
        };
        m_anyCompare = []( const std::any& any, const std::any& any2 ) {
            return !strcmp( std::any_cast<const T*>( any ), std::any_cast<const T*>( any2 ) );
        };
    }

    template <typename T>
    Any( T&& t ) : m_any( std::forward<T>( t ) ) {
        m_typeName = [](  ) {
            return TYPE_NAME(T);
        };

        m_any2valueStr = []( const std::any& any ) {
            assert( typeid( T ) == any.type() );
            const T& val = std::any_cast<const T&>( any );
            std::stringstream sstream;
            sstream << val;
            return sstream.str();
        };
        m_anyCompare = []( const std::any& any, const std::any& any2 ) {
            return std::any_cast<const T&>( any ) == std::any_cast<const T&>( any2 );
        };
    }

    //    template <class T,
    //              typename =
    //              std::enable_if<anyable_v<T>>::type>
    //    Any( T&& t )
    //    : m_any( std::forward<T>( t ) )
    //    {
    //        };

    //    }

    //    template <class T,
    //              typename =
    //              std::enable_if<! anyable_v<T>>::type>
    //    Any( T&& t )
    //    : m_any( std::forward<T>( t ) )
    //    {

    //    }

    //    template <class T>
    //    std::enable_if<! anyable_v<T>>::type
    //    read( T&& t ) {

    //    }

    ///
    /// \brief Any
    /// \param any
    ///
    //    Any( const Any& any );

    ///
    /// \brief operator =
    /// \param any
    /// \return
    ///
    //    Any& operator=( const Any& any );

    Any() = default;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///
    /// \brief has_value
    /// \return
    ///
    bool hasValue() const;

    //    ///
    //    /// \brief type
    //    /// \return
    //    ///
    const std::type_info& type() const;

    //    const std::type_info & typeId() const;

    ///
    /// \brief getValueAsString
    /// \return
    ///
    std::string getValueStr() const;

    ///
    /// \brief operator ==
    /// \param any
    /// \return
    ///
    bool operator==( const Any& any ) const;

#if CPLUSPLUS_VERSION <= 17
    bool operator!=(const Any& any) const;
#endif

    ///
    /// \brief operator <<
    /// \param os
    /// \param type
    /// \return
    ///
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const Type& type );

    ///
    /// \brief operator <<
    /// \param os
    /// \param any
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Any& any );

    template <class T>
    const T& get() const;

    template <class T>
    bool is() const;

    ///
    /// \brief to_string
    /// \return
    ///
    std::string to_string() const;

  private:
    ///
    /// \brief typeName
    /// \return
    ///
    //    const std::string& typeName() const;
//    std::string anyValue2string() const;

  private:
    std::function<std::string( )> m_typeName;
    std::function<std::string( const std::any& )> m_any2valueStr;
    std::function<bool( const std::any&, const std::any& )> m_anyCompare;
//    std::function<auto( const std::any& )> m_getValue;
    //    Type m_type     = Type::NONE;
    //    std::unique_ptr<std::type_info&> m_type;
    //    siz_t m_type;
    //    bool m_hasValue = false;
    std::any m_any;
};

template <class T>
bool Any::is() const {
    return m_any.type() == typeid( T );
}

// template <typename T>
// Any::Any( T&& t ) :
//     m_any( std::forward<T>( t ) )
////    , m_type(m_any.type().hash_code())
//{

//    const auto& anyType = m_any.type();

//    if ( anyType == typeid( int ) ) {
//        //            const auto& val = any.get<int>();
//        //            return std::to_string( val );
//    }
//    else if ( anyType == typeid( double ) ) {
//        //            const auto& val = any.get<double>();
//        //            return std::to_string( val );
//    }
//    else if ( anyType == typeid( std::string ) ) {
//        //            const auto& val = "'" + any.get<std::string>() + "'";
//        //            return val;
//    }
//    else if ( anyType == typeid( const char* ) ) {
//        //            const auto& val = any.get<const char*>();
//        //            return "'" + std::string( val ) + "'";
//    }
//    else {
//        //            return m_any2valueStr(m_any);
//        //        assert(false);
//        //            return "";
//        m_anyValue2string = []( const std::any& any ) {
//            //        const auto & val = std::any_cast<const
//            //        const auto & val = std::any_cast<const T&>(any);
//            std::cout << "anyToString: " << typeid( T ).name() << " = " << any.type().name()
//                      << std::endl;
//            assert( typeid( T ) == any.type() );

//            std::any_cast<T>( any );
//            //        const T & val = std::any_cast<const T&>(any);
//            return std::string( "fuck" );
//            //        std::stringstream sstream;
//            //        sstream << val;
//            //        return sstream.str();
//        };
//    }

//    //    const auto & anyType = m_any.type();
//    //    m_type = std::make_unique<std::type_info>(new std::type_info(m_any.type()));
//    //    m_type = std::make_unique<std::type_info&>(typeid(T));

//    //    if (anyType == typeid(int)) {
//    ////        m_type = Type::INT;
//    //        m_type = std::make_unique<std::type_info>(typeid(int));
//    //    }
//    //    else if (anyType == typeid(double)) {
//    //        m_type = Type::DOUBLE;
//    //    }
//    //    else if (anyType == typeid(std::string)) {
//    //        m_type = Type::STRING;
//    //    }
//    //    else if (anyType == typeid(const char *)) {
//    //        m_type = Type::CONST_CHAR_PTR;
//    //    }
//    // todo any
//    //    else if (anyType == typeid(data::Mat4)) {
//    //        m_type = Type::MAT4;
//    //    }
//    //    else if (anyType == typeid(data::Mesh)) {
//    //        m_type = Type::MESH;
//    //    }
//    //    else {
//    //        assert(false);
//    //    }
//}

template <class T>
const T& Any::get() const {
    return std::any_cast<const T&>( m_any );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Any::hasValue() const {

#if CPLUSPLUS_VERSION <= 17
    return ! m_any.empty();
#else
    return m_any.has_value();
#endif
}

inline const std::type_info& Any::type() const {
    return m_any.type();
}

} // namespace hub

// template<typename T>
// inline std::string Any::to_string() const
//{
//     std::stringstream sstream;

////     std::string ret;
////     if ( p == nullptr ) {
////        sstream << "nullptr";
////     }
////     else {
////        sstream << "'" << typeName() << "' = " << get<T>();
// #    ifdef HUB_USE_BOOST
//     sstream  << "'" << typeid( T ).name() << " ("
//             << boost::typeindex::type_id<T>().pretty_name() << ")' = " << get<T>();
////        sstream << "'" << typeid(T).name() << "' = " << get<T>();
// #    else
//         sstream << "'" << typeid(T).name() << "' = " << get<T>();
// #    endif
////         boost::typeindex::typeid(*p).pretty_name() << ") '" << t << "'" << std::endl;
////     }
//     return sstream.str();

//}
