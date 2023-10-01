
#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
//#include <any>
#include "std_any.hpp"

#include "Macros.hpp"

#include "data/Mat4.hpp"
#include "data/Mesh.hpp"

//#define USE_BOOST
#ifdef USE_BOOST
#    include <boost/type_index.hpp>
#endif

namespace hub {



//namespace data {
//class Mat4;
//class Mesh;
//} // namespace data

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
    enum class Type { NONE = 0, INT, DOUBLE, STRING, CONST_CHAR_PTR, MAT4, MESH, COUNT };

    template <typename T>
    Any(T && t);

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

    ///
    /// \brief type
    /// \return
    ///
    Type type() const;

    const std::type_info & typeId() const;

    ///
    /// \brief getValueAsString
    /// \return
    ///
    std::string getValueAsString() const;

    ///
    /// \brief operator ==
    /// \param any
    /// \return
    ///
    bool operator==( const Any& any ) const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param type
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Type& type );

    ///
    /// \brief operator <<
    /// \param os
    /// \param any
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Any& any );

    template <class T>
    const T & get() const;

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
    const std::string& typeName() const;


  private:
    Type m_type     = Type::NONE;
//    bool m_hasValue = false;
    std::any m_any;
};

template<class T>
bool Any::is() const
{
    return m_any.type() == typeid(T);
}

template<typename T>
Any::Any(T &&t)
    : m_any(std::forward<T>(t))
{
    const auto & anyType = m_any.type();
    if (anyType == typeid(int)) {
        m_type = Type::INT;
    }
    else if (anyType == typeid(double)) {
        m_type = Type::DOUBLE;
    }
    else if (anyType == typeid(std::string)) {
        m_type = Type::STRING;
    }
    else if (anyType == typeid(const char *)) {
        m_type = Type::CONST_CHAR_PTR;
    }
    else if (anyType == typeid(data::Mat4)) {
        m_type = Type::MAT4;
    }
    else if (anyType == typeid(data::Mesh)) {
        m_type = Type::MESH;
    }
    else {
        assert(false);
    }
}

template<class T>
const T &Any::get() const
{
    return std::any_cast<const T&>(m_any);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Any::hasValue() const {
    return ! m_any.empty();
//    return m_hasValue;
}

inline Any::Type Any::type() const {
    return m_type;
}

} // namespace hub

//template<typename T>
//inline std::string Any::to_string() const
//{
//    std::stringstream sstream;

////     std::string ret;
////     if ( p == nullptr ) {
////        sstream << "nullptr";
////     }
////     else {
////        sstream << "'" << typeName() << "' = " << get<T>();
//#    ifdef USE_BOOST
//    sstream  << "'" << typeid( T ).name() << " ("
//            << boost::typeindex::type_id<T>().pretty_name() << ")' = " << get<T>();
////        sstream << "'" << typeid(T).name() << "' = " << get<T>();
//#    else
//        sstream << "'" << typeid(T).name() << "' = " << get<T>();
//#    endif
////         boost::typeindex::typeid(*p).pretty_name() << ") '" << t << "'" << std::endl;
////     }
//     return sstream.str();

//}
