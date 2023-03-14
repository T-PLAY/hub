
#pragma once

//#include <any>
#include <cassert>
#include <string>
#include <vector>

// #include "data/Mat4.hpp"
#include <memory>

namespace hub {

namespace data {
class Mat4;
}

///
/// \brief The Any class
/// represents any value.
///
class Any
{
  public:
    ///
    /// \brief The AnyType enum
    /// allows to unify the data according to the different architecture (32, 64 bits).
    ///
    enum class Type { NONE = 0, INT, DOUBLE, STRING, CONST_CHAR_PTR, MAT4, COUNT };

    ///
    /// \brief Any
    /// \param any
    ///
    Any( const Any& any );

    ///
    /// \brief operator =
    /// \param any
    /// \return
    ///
    Any& operator=( const Any& any );

    Any() = default;

    /* cppcheck-suppress noExplicitConstructor */
    ///
    /// \brief Any
    /// \param value
    ///
    Any( int value );

    /* cppcheck-suppress noExplicitConstructor */
    ///
    /// \brief Any
    /// \param value
    ///
    Any( double value );

    /* cppcheck-suppress noExplicitConstructor */
    ///
    /// \brief Any
    /// \param value
    ///
    Any( const std::string& value );

    /* cppcheck-suppress noExplicitConstructor */
    ///
    /// \brief Any
    /// \param value
    ///
    Any( const char* value );

    /* cppcheck-suppress noExplicitConstructor */
    ///
    /// \brief Any
    /// \param value
    ///
    Any( const data::Mat4& value );

    ///
    /// \brief getInt
    /// \return
    ///
    const int& getInt() const;

    ///
    /// \brief getDouble
    /// \return
    ///
    const double& getDouble() const;

    ///
    /// \brief getStdString
    /// \return
    ///
    const std::string& getStdString() const;

    ///
    /// \brief getConstCharPtr
    /// \return
    ///
    const char* const getConstCharPtr() const;

    ///
    /// \brief getMat4
    /// \return
    ///
    const data::Mat4& getMat4() const;

    ///
    /// \brief has_value
    /// \return
    ///
    const bool& has_value() const;

    ///
    /// \brief type
    /// \return
    ///
    Type type() const;

    ///
    /// \brief typeName
    /// \return
    ///
    const std::string& typeName() const;

    ///
    /// \brief to_string
    /// \return
    ///
    std::string to_string() const;

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
    friend std::ostream& operator<<( std::ostream& os, const Type& type );

    ///
    /// \brief operator <<
    /// \param os
    /// \param any
    /// \return
    ///
    friend std::ostream& operator<<( std::ostream& os, const Any& any );

  private:
    Type m_type     = Type::NONE;
    bool m_hasValue = false;

    int m_int                    = 0;
    double m_double              = 0.0;
    std::string m_std_string     = "";
    const char* m_const_char_ptr = nullptr;
    hub::data::Mat4* m_mat4      = nullptr;

    // T m_value;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const bool& Any::has_value() const {
    return m_hasValue;
}

inline Any::Type Any::type() const {
    return m_type;
}

// inline std::string Any::typeName() const {
//}

} // namespace hub
