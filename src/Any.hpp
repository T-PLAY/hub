
#pragma once

#include <string>
#include <any>
#include <vector>
#include <cassert>

namespace hub {


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

    Any(int value) {
        m_type = Type::INT;
        m_int = value;
        m_hasValue = true;
    }
     Any(double value) {
        m_type = Type::DOUBLE;
        m_double = value;
        m_hasValue = true;
    }
     Any(const std::string & value) {
        m_type = Type::STRING;
        m_std_string = value;
        m_hasValue = true;
    }
     Any(const char * value) {
        m_type = Type::CONST_CHAR_PTR;
        m_const_char_ptr = value;
        m_hasValue = true;
    }
     Any(const std::vector<float> & value) {
        m_type = Type::VECTOR_FLOAT;
        m_std_vector_float = value;
        m_hasValue = true;
    }
     Any(unsigned int value) {
        m_type = Type::UINT;
        m_unsigned_int = value;
        m_hasValue = true;
    }
     Any(const float * value) {
        m_type = Type::CONST_FLOAT_PTR;
        m_const_float_ptr = value;
        m_hasValue = true;
    }
     Any(const double * value) {
        m_type = Type::CONST_DOUBLE_PTR;
        m_const_double_ptr = value;
        m_hasValue = true;
    }

//    template <class T>
//    T get() const = delete;

//    template <>
    const int & getInt() const {
        assert(m_type == Type::INT);
        return m_int;
    }
    const double & getDouble() const {
        assert(m_type == Type::DOUBLE);
        return m_double;
    }
    const std::string & getStdString() const {
        assert(m_type == Type::STRING);
        return m_std_string;
    }
    const char * const getConstCharPtr() const {
        assert(m_type == Type::CONST_CHAR_PTR);
        return m_const_char_ptr;
    }
    const std::vector<float> & getStdVectorFloat() const {
        assert(m_type == Type::VECTOR_FLOAT);
        return m_std_vector_float;
    }
    const unsigned int & getUnsignedInt() const {
        assert(m_type == Type::UINT);
        return m_unsigned_int;
    }
    const float * const getConstFloatPtr() const {
        assert(m_type == Type::CONST_FLOAT_PTR);
        return m_const_float_ptr;
    }
    const double * const getConstDoublePtr() const {
        assert(m_type == Type::CONST_DOUBLE_PTR);
        return m_const_double_ptr;
    }

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
    Type m_type;
    bool m_hasValue = false;

    int m_int;
    double m_double;
    std::string m_std_string;
    const char * m_const_char_ptr;
    std::vector<float> m_std_vector_float;
    unsigned int m_unsigned_int;
    const float* m_const_float_ptr;
    const double* m_const_double_ptr;



//T m_value;


};

    } // hub
