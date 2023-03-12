
#pragma once

#include <string>
#include <any>

namespace hub {


class Any {
  public:

///
/// \brief The AnyType enum
/// allows to unify the data according to the different architecture (32, 64 bits).
///
enum class Type {
    INT = 0,
    DOUBLE,
    STRING,
    CONST_CHAR_PTR,
    VECTOR_FLOAT,
    UINT,
    CONST_FLOAT_PTR,
    COUNT
};

    ///
    /// \brief anyValue2string
    /// \param any
    /// \return
    ///
    static std::string anyValue2string( const std::any& any );

    ///
    /// \brief anyType2string
    /// \param any
    /// \return
    ///
    static const std::string& anyType2string( const std::any& any );

    friend std::ostream& operator<<( std::ostream& os, const Type& type );

};

    } // hub
