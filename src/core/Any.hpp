
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

#include "Anyable.hpp"

// #include "core/Vector.hpp"
//  #include "std_any.hpp"

// #if CPLUSPLUS_VERSION <= 17
// bool Any::operator!=( const Any& any ) const {
//     return !( *this == any );
// }
// #endif

#if CPLUSPLUS_VERSION <= 14
#    include "std_any.hpp"
#else
#    include <any>
#endif

// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"

// #define HUB_USE_BOOST
// #ifdef HUB_USE_BOOST
// #    include <boost/type_index.hpp>
// #endif

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
        assert( Anyable::s_anyables.find( typeid( void ).hash_code() ) !=
                Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( typeid( void ).hash_code() ) );
    }

    Any(Any && any) = default;

    template <class T>
    Any( T&& t ) : m_any( std::forward<T>( t ) ) {
        if ( Anyable::s_anyables.find( typeid( T ).hash_code() ) == Anyable::s_anyables.end() ) {
            Anyable::registerTypes<std::remove_cvref_t<T>>();
            //            std::cerr << HEADER << "'" << TYPE_NAME(T) << "' is not supported by
            //            hub::Any class, please do hub::Anyable::registerType<" << TYPE_NAME(T) <<
            //            ">() before using it." << std::endl; exit(1);
        }
        assert( Anyable::s_anyables.find( typeid( T ).hash_code() ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( typeid( T ).hash_code() ) );
    }

    template <typename T>
    Any( const T* t ) : m_any( t ) {
        if ( Anyable::s_anyables.find( typeid( const T* ).hash_code() ) ==
             Anyable::s_anyables.end() ) {
            //            Anyable::registerTypes<const T*>();
            Anyable::registerTypes<const T*>();
            //            std::cerr << HEADER << "'" << TYPE_NAME(const T*) << "' is not supported
            //            by hub::Any class, please do hub::Anyable::registerType<" << TYPE_NAME(T)
            //            << ">() before using it." << std::endl; exit(1);
        }
        assert( Anyable::s_anyables.find( typeid( const T* ).hash_code() ) !=
                Anyable::s_anyables.end() );
        //        m_anyHelper = &Anyable::s_anyables.at( typeid( const T* ).hash_code() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( typeid( const T* ).hash_code() ) );
        //        assert( s_anyHelpers.find( typeid( const T* ).hash_code() ) != s_anyHelpers.end()
        //        ); m_typeName = [](  ) {
        ////            return std::string("const ") + TYPE_NAME(T) + " *";
        //            return std::string(TYPE_NAME(T)) + "*";
    };

    Any( const Any& any ) :
        m_any( any.m_any )
    //        m_anyHelper{std::make_unique<Anyable::AnyHelper>(any.m_anyHelper)}
    {
        //        m_anyHelper = std::make_unique<Anyable::AnyHelper>(any.m_anyHelper.get());
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( any.type().hash_code() ) );
    }

    Any& operator=( const Any& any ) {
        m_any       = any.m_any;
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(
            Anyable::s_anyables.at( any.type().hash_code() ) );
        return *this;
    }

//    template <class Output>
    void write( Serializer& serializer ) const {
        const auto& hashCode = m_any.type().hash_code();
        serializer.write( hashCode );

        m_anyHelper->write( serializer, m_any );
    }

//    template <class Input>
    void read( Serializer& serializer ) {
        decltype( m_any.type().hash_code() ) hashCode;
        serializer.read( hashCode );
        assert( Anyable::s_anyables.find( hashCode ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( hashCode ) );

        m_anyHelper->read( serializer, m_any );
    }

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
    //            assert( typeid( const T* ) == any.type() );
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
    //            assert( typeid( T ) == any.type() );
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

    bool hasValue() const {
#if CPLUSPLUS_VERSION < 17
        return !m_any.empty();
#else
        return m_any.has_value();
#endif
    }

    const std::type_info& type() const { return m_any.type(); }

    std::string getValueStr() const {
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->getValueStr != nullptr );
        return m_anyHelper->getValueStr( m_any );
    }

#if CPLUSPLUS_VERSION <= 17
    bool operator!=( const Any& any ) const;
#endif

    template <class T>
    const T& get() const {
        return std::any_cast<const T&>( m_any );
    }

    template <class T>
    bool is() const {
        return m_any.type() == typeid( T );
    }

    std::string toString() const {
        std::string str;
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->getTypeName != nullptr );
        assert( m_anyHelper->getValueStr != nullptr );
        str += "*'" + m_anyHelper->getTypeName() + "' : " + m_anyHelper->getValueStr( m_any ) + "*";
        return str;
    }

    bool operator==( const Any& any ) const {
        assert( m_any.type() == any.m_any.type() );
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->compare != nullptr );
        return m_anyHelper->compare( m_any, any.m_any );
    }

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
    //    const std::string& typeName() const;
    //    std::string anyValue2string() const;

  private:
    std::any m_any;
//    decltype( m_any.type().hash_code() ) m_hashCode;
    std::unique_ptr<Anyable::AnyHelper> m_anyHelper;
};

} // namespace hub

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
