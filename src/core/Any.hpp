
#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Macros.hpp"
#include "Traits.hpp"

#include "Anyable.hpp"
#include "core/Vector.hpp"
// #include "std_any.hpp"


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

// class AnyHelper;
//  template <class T>
//  std::map<size_t, AnyHelper> makeMap() {
//      return std::map<size_t, AnyHelper> { makeAnyHelperRow<T>() };
//  }

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
        assert( Anyable::s_anyables.find( typeid( void ).hash_code() ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(Anyable::s_anyables.at( typeid( void ).hash_code() ));
    }

    template <class T>
    Any( T&& t ) : m_any( std::forward<T>( t ) ) {
        if ( Anyable::s_anyables.find( typeid( T ).hash_code() ) == Anyable::s_anyables.end() ) {
            Anyable::registerTypes<std::remove_cvref_t<T>>();
//            std::cerr << HEADER << "'" << TYPE_NAME(T) << "' is not supported by hub::Any class, please do hub::Anyable::registerType<" << TYPE_NAME(T) << ">() before using it." << std::endl;
//            exit(1);
        }
        assert( Anyable::s_anyables.find( typeid( T ).hash_code() ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(Anyable::s_anyables.at( typeid( T ).hash_code() ));


        //        const auto hashCode = m_any.type().hash_code();
        //        const auto hashCode = typeid(T).hash_code();
        //        std::cout << "hashCode of " << TYPE_NAME(T) << " = " << hashCode << std::endl;
        //        for (const auto & pair : s_anyHelpers) {
        //            std::cout << pair.first << " -> " << pair.second.getTypeName() << std::endl;
        //        }
        //        assert( s_anyHelpers.find( hashCode ) != s_anyHelpers.end() );
    }

    template <typename T>
    Any( const T* t ) : m_any( t ) {
        if ( Anyable::s_anyables.find( typeid( const T* ).hash_code() ) == Anyable::s_anyables.end() ) {
//            Anyable::registerTypes<const T*>();
            Anyable::registerTypes<const T*>();
//            std::cerr << HEADER << "'" << TYPE_NAME(const T*) << "' is not supported by hub::Any class, please do hub::Anyable::registerType<" << TYPE_NAME(T) << ">() before using it." << std::endl;
//            exit(1);
        }
        assert( Anyable::s_anyables.find( typeid( const T* ).hash_code() ) !=
                Anyable::s_anyables.end() );
//        m_anyHelper = &Anyable::s_anyables.at( typeid( const T* ).hash_code() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(Anyable::s_anyables.at( typeid( const T * ).hash_code() ));
        //        assert( s_anyHelpers.find( typeid( const T* ).hash_code() ) != s_anyHelpers.end()
        //        ); m_typeName = [](  ) {
        ////            return std::string("const ") + TYPE_NAME(T) + " *";
        //            return std::string(TYPE_NAME(T)) + "*";
    };

    template <class Output>
    void write(Output & output) const {
        const auto & hashCode = m_any.type().hash_code();
        output.write(hashCode);
        m_anyHelper->write(output, m_any);
//        auto & [data, size] = m_anyHelper->serialize(m_any);
//        output.write(data, size);
    }

    template <class Input>
    void read(Input & input) {
        decltype(m_any.type().hash_code()) hashCode;
        input.read(hashCode);
        assert( Anyable::s_anyables.find( hashCode ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>(Anyable::s_anyables.at( hashCode ));
        m_anyHelper->read(input, m_any);
    }

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

    ///
    /// \brief has_value
    /// \return
    ///
    bool hasValue() const;

    const std::type_info& type() const;

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
    bool operator!=( const Any& any ) const;
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
    //    std::string to_string() const;



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
    //    std::function<std::string( )> m_typeName;
    //    std::function<std::string( const std::any& )> m_any2valueStr;
    //    std::function<bool( const std::any&, const std::any& )> m_anyCompare;

    std::any m_any;
//    const Anyable::AnyHelper* m_anyHelper = nullptr;
    std::unique_ptr<Anyable::AnyHelper> m_anyHelper;
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

#if CPLUSPLUS_VERSION < 17
    return !m_any.empty();
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
