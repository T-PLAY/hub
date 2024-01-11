/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/01
	
#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
// #include <sstream>
#include <string>
#include <vector>

#include "Macros.hpp"
#include "Traits.hpp"
#include "Serializer.hpp"

#include "Anyable.hpp"


// #include "core/Vector.hpp"
//  #include "std_any.hpp"

// #if CPP_VERSION <= 17
// bool Any::operator!=( const Any& any ) const {
//     return !( *this == any );
// }
// #endif

// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"

// #define HUB_USE_BOOST
// #ifdef HUB_USE_BOOST
// #    include <boost/type_index.hpp>
// #endif

// #define HUB_DEBUG_ANY


namespace hub {

///
/// \brief The Any class
/// represents any value.
///
class SRC_API Any
{
  public:
    ///
    /// \brief The AnyType enum
    /// allows to unify the data according to the different architecture (32, 64 bits).
    ///
    //    enum class Type { NONE = 0, INT, DOUBLE, STRING, CONST_CHAR_PTR, MAT4, MESH, COUNT };

    Any() {
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any()" << std::endl;
#endif

        // Anyable::s_anyables.end() );
        assert( Anyable::s_anyables.find( "void" ) != Anyable::s_anyables.end() );
        // m_anyHelper = std::make_unique<Anyable::AnyHelper>(
        m_anyHelper = std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( "void" ) );
    }

    Any( const Any& any ) :
        m_any( any.m_any )
    //        m_anyHelper{std::make_unique<Anyable::AnyHelper>(any.m_anyHelper)}
    {
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any(const Any&)" << std::endl;
#endif
        // m_any = any.m_any;
        // m_anyHelper = std::make_unique<Anyable::AnyHelper>(any.m_anyHelper.get());
        // m_anyHelper = std::make_unique<Anyable::AnyHelper>(*any.m_anyHelper.get());
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            // Anyable::s_anyables.at( any.type().hash_code() ) );
            Anyable::s_anyables.at( any.typeName() ) );
    }

    // Any( Any&& any ) { std::cout << "[Any] Any(Any&&)" << std::endl; }
    Any( Any&& ) = default;

    // Any& operator=( const Any& any ) = default;
    Any& operator=( const Any& any ) {
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any& operator=(const Any&)" << std::endl;
#endif
        m_any       = any.m_any;
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            // Anyable::s_anyables.at( any.type().hash_code() ) );
            Anyable::s_anyables.at( any.typeName() ) );
        return *this;
    }

    // Any& operator=( Any& any ) {
    //     std::cout << "[Any] Any& operator=(Any&)" << std::endl;
    //     m_any = any.m_any;
    //     m_anyHelper = std::make_unique<Anyable::AnyHelper>(
    //     // Anyable::s_anyables.at( any.type().hash_code() ) );
    //     Anyable::s_anyables.at( any.typeName() ) );
    //     return *this;
    // }
    // Any& operator=(Any& ) = default;

    Any& operator=( Any&& ) = default;
    //     std::cout << "[Any] Any& operator=(Any&&)" << std::endl;
    //     m_any = std::move(any.m_any);
    //     m_anyHelper = std::make_unique<Anyable::AnyHelper>(
    //     // Anyable::s_anyables.at( any.type().hash_code() ) );
    //     Anyable::s_anyables.at( any.typeName() ) );
    //     return *this;
    // }

    template <class T,
              typename = typename std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>, Any>>>
    Any( T&& t ) : m_any( std::forward<T>( t ) ) {
        static_assert( !std::is_same_v<std::remove_cvref_t<T>, Any> );
        // std::cout << "[Any] Any(T&&) " << std::endl << std::flush;
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any(T&&) " << TYPE_NAME( t ) << std::endl;
#endif

        if ( Anyable::s_anyables.find( TYPE_NAME( t ) ) == Anyable::s_anyables.end() ) {
            Anyable::registerTypes<std::remove_cvref_t<T>>();
            //            std::cerr << HEADER << "'" << TYPE_NAME(T) << "' is not supported by
            //            hub::Any class, please do hub::Anyable::registerType<" << TYPE_NAME(T) <<
            //            ">() before using it." << std::endl; exit(1);
        }
        // );
        assert( Anyable::s_anyables.find( TYPE_NAME( t ) ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( TYPE_NAME( t ) ) );
    }

    template <class T,
              typename = typename std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>, Any>>>
    Any( const T* t ) : m_any( t ) {
        static_assert( !std::is_same_v<std::remove_cvref_t<T>, Any> );
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any(const T*)" << std::endl;
#endif
        if ( Anyable::s_anyables.find( TYPE_NAME( t ) ) == Anyable::s_anyables.end() ) {
            //            Anyable::registerTypes<const T*>();
            Anyable::registerTypes<const T*>();
            //            std::cerr << HEADER << "'" << TYPE_NAME(const T*) << "' is not supported
            //            by hub::Any class, please do hub::Anyable::registerType<" << TYPE_NAME(T)
            //            << ">() before using it." << std::endl; exit(1);
        }
        assert( Anyable::s_anyables.find( TYPE_NAME( t ) ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( TYPE_NAME( t ) ) );
    };

    void write( Serializer& serializer ) const {
        assert( m_anyHelper != nullptr );
        // const auto& hashCode = m_any.type().hash_code();
        const auto& type_name = typeName();
        serializer.write( type_name );
        // serializer.write( hashCode );

        m_anyHelper->write( serializer, m_any );
    }

    //    template <class Input>
    void read( Serializer& serializer ) {
        // decltype( m_any.type_name().hash_code() ) hashCode;
        decltype( typeName() ) type_name;
        serializer.read( type_name );
        // serializer.read( hashCode );
        // assert( Anyable::s_anyables.find( hashCode ) != Anyable::s_anyables.end() );
        assert( Anyable::s_anyables.find( type_name ) != Anyable::s_anyables.end() );
        // m_anyHelper = std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( hashCode ) );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( type_name ) );

        m_anyHelper->read( serializer, m_any );
    }


    bool hasValue() const {
#if CPP_VERSION < 17
        return !m_any.empty();
#else
        return m_any.has_value();
#endif
    }

    template <class T>
    bool is() const {
        return m_any.type() == typeid( T );
    }

    template <class T>
    const T& get() const {
        return std::any_cast<const T&>( m_any );
    }

    std::string typeName() const {
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->getTypeName != nullptr );
        return m_anyHelper->getTypeName();
        // return TYPE_NAME(m_any.type());
    }

    std::string valueAsString() const {
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->getValueStr != nullptr );
        return m_anyHelper->getValueStr( m_any );
        // return TYPE_NAME(m_any.type());
    }

    auto toString() const {
        std::string str;
        assert( m_anyHelper != nullptr );
        // assert( m_anyHelper->getTypeName != nullptr );
        // assert( m_anyHelper->getValueStr != nullptr );
        str += "*'" + typeName() + "' : " + valueAsString() + "*";
        return str;
    }

    bool operator==( const Any& other ) const {
        // assert( m_any.type() == other.m_any.type() );
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->compare != nullptr );
        return m_any.type() == other.m_any.type() && m_anyHelper->compare( m_any, other.m_any );
    }
#if CPP_VERSION < 20
    bool operator!=( const Any& other ) const { return !( *this == other ); }
#endif

    //    template <class Serial>
    //    void serialize(Serial & serial) {
    //        const auto & hashCode = m_any.type().hash_code();
    ////        serial(hashCode);
    //        serial(m_any.type().hash_code());
    //    }

  private:
    ///
    /// \brief typeName
    /// \return
    ///
    //    std::string anyValue2string() const;

  private:
    std::any m_any;
    //    decltype( m_any.type().hash_code() ) m_hashCode;
    std::unique_ptr<Anyable::AnyHelper> m_anyHelper;
};

} // namespace hub

//    static constexpr auto serialize( auto& archive, auto& self ) {
//        return archive(5);
//    }

//    static constexpr auto serialize( auto& archive, auto& self ) {
//        //        return archive( self.m_anyHelper);
//        using archive_type = std::remove_cvref_t<decltype( archive )>;

//        if constexpr ( archive_type::kind() == zpp::bits::kind::in ) {
//            // Input archive
//            const auto& hashCode = self.m_any.type().hash_code();
//            //			output.write( hashCode );
//            archive( hashCode );
//            return archive;
//        }
//        else if constexpr ( archive_type::kind() == zpp::bits::kind::out ) {
//            // Output archive
//            const auto& hashCode = self.m_any.type().hash_code();
//            //			output.write( hashCode );
//            archive( hashCode );
//            return archive;
//        }
////        return archive;
//    }

//        m_any2valueStr = []( const std::any& any ) {
//            const T* val = std::any_cast<const T*>( any );
//            std::stringstream sstream;
//            sstream << val;
//            return sstream.str();
//        };
//        m_anyCompare = []( const std::any& any, const std::any& any2 ) {
//            return !strcmp( std::any_cast<const T*>( any ), std::any_cast<const T*>( any2 ) );
//        };
//    }

//    template <typename T>
//    Any( T&& t ) : m_any( std::forward<T>( t ) ) {
//        m_typeName = [](  ) {
//            return TYPE_NAME(T);
//        };

//        m_any2valueStr = []( const std::any& any ) {
//            const T& val = std::any_cast<const T&>( any );
//            std::stringstream sstream;
//            sstream << val;
//            return sstream.str();
//        };
//        m_anyCompare = []( const std::any& any, const std::any& any2 ) {
//            return std::any_cast<const T&>( any ) == std::any_cast<const T&>( any2 );
//        };
//    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// const std::type_info& type() const { return m_any.type(); }

// std::string getValueStr() const {
//     assert( m_anyHelper != nullptr );
//     assert( m_anyHelper->getValueStr != nullptr );
//     return m_anyHelper->getValueStr( m_any );
// }

// #if CPP_VERSION <= 17
//     bool operator!=( const Any& any ) const;
// #endif

// template <typename T>
// Any::Any( T&& t ) :
//     m_any( std::forward<T>( t ) )
////    , m_type(m_any.type().hash_code())
//{

//    const auto& anyType = m_any.type();

//        //            const auto& val = any.get<int>();
//        //            return std::to_string( val );
//    }
//        //            const auto& val = any.get<double>();
//        //            return std::to_string( val );
//    }
//        //            const auto& val = "'" + any.get<std::string>() + "'";
//        //            return val;
//    }
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
//                      << std::endl;

//            std::any_cast<T>( any );
//            //        const T & val = std::any_cast<const T&>(any);
//            //        std::stringstream sstream;
//            //        sstream << val;
//            //        return sstream.str();
//        };
//    }

//    //    const auto & anyType = m_any.type();
//    //    m_type = std::make_unique<std::type_info>(new std::type_info(m_any.type()));

//    ////        m_type = Type::INT;
//    //    }
//    //        m_type = Type::DOUBLE;
//    //    }
//    //        m_type = Type::STRING;
//    //    }
//    //        m_type = Type::CONST_CHAR_PTR;
//    //    }
//    // todo any
//    //        m_type = Type::MAT4;
//    //    }
//    //        m_type = Type::MESH;
//    //    }
//    //    else {
//    //        assert(false);
//    //    }
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
//             << boost::typeindex::type_id<T>().pretty_name() << ")' = " << get<T>();
// #    else
// #    endif
////     }
//     return sstream.str();

//}
