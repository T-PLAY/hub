
#pragma once

#include <string>
#include <any>
#include <vector>
#include <cassert>

//#include "data/Mat4.hpp"
#include <memory>

namespace hub {

namespace data {
class Mat4;
}



class Any {
  public:
///
/// \brief The AnyType enum
/// allows to unify the data according to the different architecture (32, 64 bits).
///
enum class Type {
    NONE = 0,
    INT,
    DOUBLE,
    STRING,
    CONST_CHAR_PTR,
    VECTOR_FLOAT,
    UINT,
    CONST_FLOAT_PTR,
    CONST_DOUBLE_PTR,
    MAT4,
    COUNT
};
    ///
    /// \brief anyValue2string
    /// \param any
    /// \return
    ///
//    static std::string anyValue2string( const std::any& any );
    static std::string anyValue2string( const Any& any );

    ///
    /// \brief anyType2string
    /// \param any
    /// \return
    ///
//    static const std::string& anyType2string( const std::any& any );
    static const std::string& anyType2string( const Any& any );

    friend std::ostream& operator<<( std::ostream& os, const Type& type );


//    template <class T>
//    Any(T value) {

//    }
//    An(())
    Any() = default;

    Any(int value);
    Any(double value);
     Any(const std::string & value);
     Any(const char * value);
     Any(const std::vector<float> & value);
     Any(unsigned int value);
     Any(const float * value);
     Any(const double * value);
     Any(const data::Mat4 & value);


//    template <class T>
//    T get() const = delete;

//    template <>
     const int & getInt() const;
    const double & getDouble() const;
    const std::string & getStdString() const;
    const char * const getConstCharPtr() const;
    const std::vector<float> & getStdVectorFloat() const;
    const unsigned int & getUnsignedInt() const;
    const float * const getConstFloatPtr() const;
    const double * const getConstDoublePtr() const;
    const data::Mat4 & getMat4() const;

    const bool & has_value() const {
        return m_hasValue;
    }

    Type type() const {
        return m_type;
    }

    std::string typeName() const {
        return anyType2string(*this);
    }

  private:
    Type m_type = Type::NONE;
    bool m_hasValue = false;

    int m_int;
    double m_double;
    std::string m_std_string;
    const char * m_const_char_ptr;
    std::vector<float> m_std_vector_float;
    unsigned int m_unsigned_int;
    const float* m_const_float_ptr;
    const double* m_const_double_ptr;
    hub::data::Mat4 * m_mat4;



//T m_value;


};

    } // hub
