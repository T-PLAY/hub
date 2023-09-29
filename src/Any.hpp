
#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "Macros.hpp"

//#define USE_BOOST
#ifdef USE_BOOST
#    include <boost/type_index.hpp>
#endif

namespace hub {


class SRC_API Any
{
    using id = size_t;

    template<typename T>
    struct type { static void id() { } };

    template<typename T>
    static id type_id() { return reinterpret_cast<id>(&type<T>::id); }

    template<typename T>
    using decay = typename std::decay<T>::type;

    template<typename T>
    using none = typename std::enable_if<!std::is_same<Any, T>::value>::type;

    struct base
    {
        virtual ~base() { }
        virtual bool is(id) const = 0;
        virtual base *copy() const = 0;
    } *p = nullptr;

    template<typename T>
    struct data : base, std::tuple<T>
    {
        using std::tuple<T>::tuple;

        T       &get()      & { return std::get<0>(*this); }
        T const &get() const& { return std::get<0>(*this); }

        bool is(id i) const override { return i == type_id<T>(); }
        base *copy()  const override { return new data{get()}; }

    };

//    template<typename T>
//    T &stat() { return static_cast<data<T>&>(*p).get(); }

//    template<typename T>
//    T const &stat() const { return static_cast<data<T> const&>(*p).get(); }

    template<typename T>
    T &dyn() { return dynamic_cast<data<T>&>(*p).get(); }

    template<typename T>
    T const &dyn() const { return dynamic_cast<data<T> const&>(*p).get(); }

  public:
    Any() { }
    ~Any() { delete p; }

    Any(Any &&s)      : p{s.p} { s.p = nullptr; }
    Any(Any const &s) : p{s.p->copy()} { }

    template<typename T, typename U = decay<T>, typename = none<U>>
    Any(T &&x) : p{new data<U>{std::forward<T>(x)}} { }

    Any &operator=(Any s) { swap(*this, s); return *this; }

    friend void swap(Any &s, Any &r) { std::swap(s.p, r.p); }

//    void clear() { delete p; p = nullptr; }

    bool isEmpty() const { return p == nullptr; }

    template<typename T>
    bool is() const { return p ? p->is(type_id<T>()) : false; }

//    template<typename T> T      &&_()     && { return std::move(stat<T>()); }
//    template<typename T> T       &_()      & { return stat<T>(); }
//    template<typename T> T const &_() const& { return stat<T>(); }

//    template<typename T> T      &&cast()     && { return std::move(dyn<T>()); }
//    template<typename T> T       &cast()      & { return dyn<T>(); }
//    template<typename T> T const &cast() const& { return dyn<T>(); }

    template<typename T> T const &get() const& {
//        static_assert(is<T>(), "getting wrong type");
        return dyn<T>();
    }

//    template<typename T> operator T     &&()     && { return std::move(_<T>()); }
//    template<typename T> operator T      &()      & { return _<T>(); }
//    template<typename T> operator T const&() const& { return _<T>(); }

    template <typename T>
    std::string to_string() const;


//    id type_id() const {
//        return typeid(*p);
//    }
//  private:
//    std::string typeName() const;
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const Any& any );
};

template<typename T>
inline std::string Any::to_string() const
{
    std::stringstream sstream;

//     std::string ret;
//     if ( p == nullptr ) {
//        sstream << "nullptr";
//     }
//     else {
//        sstream << "'" << typeName() << "' = " << get<T>();
#    ifdef USE_BOOST
    sstream  << "'" << typeid( T ).name() << " ("
            << boost::typeindex::type_id<T>().pretty_name() << ")' = " << get<T>();
//        sstream << "'" << typeid(T).name() << "' = " << get<T>();
#    else
        sstream << "'" << typeid(T).name() << "' = " << get<T>();
#    endif
//         boost::typeindex::typeid(*p).pretty_name() << ") '" << t << "'" << std::endl;
//     }
     return sstream.str();

}

//namespace data {
//class Mat4;
//class Mesh;
//} // namespace data

/////
///// \brief The Any class
///// represents any value.
/////
//class SRC_API Any
//{
//  public:
//    ///
//    /// \brief The AnyType enum
//    /// allows to unify the data according to the different architecture (32, 64 bits).
//    ///
//    enum class Type { NONE = 0, INT, DOUBLE, STRING, CONST_CHAR_PTR, MAT4, MESH, COUNT };

//    ///
//    /// \brief Any
//    /// \param any
//    ///
//    Any( const Any& any );

//    ///
//    /// \brief operator =
//    /// \param any
//    /// \return
//    ///
//    Any& operator=( const Any& any );

//    Any() = default;

//    /* cppcheck-suppress noExplicitConstructor */
//    ///
//    /// \brief Any
//    /// \param value
//    ///
//    Any( int value );

//    /* cppcheck-suppress noExplicitConstructor */
//    ///
//    /// \brief Any
//    /// \param value
//    ///
//    Any( double value );

//    /* cppcheck-suppress noExplicitConstructor */
//    ///
//    /// \brief Any
//    /// \param value
//    ///
//    Any( const std::string& value );

//    /* cppcheck-suppress noExplicitConstructor */
//    ///
//    /// \brief Any
//    /// \param value
//    ///
//    Any( const char* value );

//    /* cppcheck-suppress noExplicitConstructor */
//    ///
//    /// \brief Any
//    /// \param value
//    ///
//    Any( const data::Mat4& value );

//    /* cppcheck-suppress noExplicitConstructor */
//    ///
//    /// \brief Any
//    /// \param value
//    ///
//    Any( const data::Mesh& value );

//    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    ///
//    /// \brief get<int>
//    /// \return
//    ///
//    const int& get<int>() const;

//    ///
//    /// \brief get<double>
//    /// \return
//    ///
//    const double& get<double>() const;

//    ///
//    /// \brief getStdString
//    /// \return
//    ///
//    const std::string& getStdString() const;

//    ///
//    /// \brief getConstCharPtr
//    /// \return
//    ///
//    const char* getConstCharPtr() const;

//    ///
//    /// \brief getMat4
//    /// \return
//    ///
//    const data::Mat4& getMat4() const;

//    ///
//    /// \brief getMesh
//    /// \return
//    ///
//    const data::Mesh& getMesh() const;

//    ///
//    /// \brief has_value
//    /// \return
//    ///
//    const bool& has_value() const;

//    ///
//    /// \brief type
//    /// \return
//    ///
//    Type type() const;

//    ///
//    /// \brief typeName
//    /// \return
//    ///
//    const std::string& typeName() const;

//    ///
//    /// \brief to_string
//    /// \return
//    ///
//    std::string to_string() const;


//    ///
//    /// \brief value_to_string
//    /// \return
//    ///
//    std::string value_to_string() const;

//    ///
//    /// \brief operator ==
//    /// \param any
//    /// \return
//    ///
//    bool operator==( const Any& any ) const;

//    ///
//    /// \brief operator <<
//    /// \param os
//    /// \param type
//    /// \return
//    ///
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Type& type );

//    ///
//    /// \brief operator <<
//    /// \param os
//    /// \param any
//    /// \return
//    ///
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Any& any );

//  private:
//    Type m_type     = Type::NONE;
//    bool m_hasValue = false;

//    int m_int                    = 0;
//    double m_double              = 0.0;
//    std::string m_std_string     = "";
//    const char* m_const_char_ptr = nullptr;
//    data::Mat4* m_mat4      = nullptr;
//    data::Mesh* m_mesh      = nullptr;
//    bool m_bool                  = false;
//};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//inline const bool& Any::has_value() const {
//    return m_hasValue;
//}

//inline Any::Type Any::type() const {
//    return m_type;
//}

} // namespace hub
