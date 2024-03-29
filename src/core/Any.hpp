/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/01

#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Base.hpp"
#include "Serializer.hpp"
#include "Anyable.hpp"

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

    Any() {
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any()" << std::endl;
#endif

        assert( Anyable::s_anyables.find( "void" ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( "void" ) );
    }

    ///
    /// \brief Any
    /// \param any
    ///
    Any( const Any& any ) :
        m_any( any.m_any ),
        m_anyHelper {
            std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( any.typeName() ) ) } {
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any(const Any&)" << std::endl;
#endif
    }

    ///
    /// \brief Any
    ///
    Any( Any&& ) = default;

    ///
    /// \brief operator =
    /// \param any
    /// \return
    ///
    Any& operator=( const Any& any ) {
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any& operator=(const Any&)" << std::endl;
#endif
        m_any = any.m_any;
        m_anyHelper =
            std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( any.typeName() ) );
        return *this;
    }

    ///
    /// \brief operator =
    /// \return
    ///
    Any& operator=( Any&& ) = default;

    ///
    /// \brief Any
    /// \param t
    ///
    template <class T,
              typename = typename std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>, Any>>>
    // cppcheck-suppress noExplicitConstructor
    Any( T&& t ) : m_any( std::forward<T>( t ) ) {
        static_assert( !std::is_same_v<std::remove_cvref_t<T>, Any> );
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any(T&&) " << TYPE_NAME( t ) << std::endl;
#endif
        const auto & type_name = TYPE_NAME(std::any_cast<T>(m_any));

        if ( Anyable::s_anyables.find( type_name ) == Anyable::s_anyables.end() ) {
            Anyable::registerTypes<std::remove_cvref_t<T>>();
        }
        assert( Anyable::s_anyables.find( type_name ) != Anyable::s_anyables.end() );
        m_anyHelper =
            std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( type_name ) );
    }

    ///
    /// \brief Any
    /// \param t
    ///
    template <class T,
              typename = typename std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>, Any>>>
    // cppcheck-suppress noExplicitConstructor
    Any( const T* t ) : m_any( t ) {
        static_assert( !std::is_same_v<std::remove_cvref_t<T>, Any> );
#ifdef HUB_DEBUG_ANY
        std::cout << "[Any] Any(const T*)" << std::endl;
#endif
        if ( Anyable::s_anyables.find( TYPE_NAME( t ) ) == Anyable::s_anyables.end() ) {
            Anyable::registerTypes<const T*>();
        }
        assert( Anyable::s_anyables.find( TYPE_NAME( t ) ) != Anyable::s_anyables.end() );
        m_anyHelper =
            std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( TYPE_NAME( t ) ) );
    };

    ///
    /// \brief write
    /// \param serializer
    ///
    void write( Serializer& serializer ) const {
        assert( m_anyHelper != nullptr );
        const auto& type_name = typeName();
        serializer.write( type_name );

        m_anyHelper->write( serializer, m_any );
    }

    ///
    /// \brief read
    /// \param serializer
    ///
    void read( Serializer& serializer ) {
        decltype( typeName() ) type_name;
        serializer.read( type_name );
        assert( Anyable::s_anyables.find( type_name ) != Anyable::s_anyables.end() );
        m_anyHelper = std::make_unique<Anyable::AnyHelper>( Anyable::s_anyables.at( type_name ) );

        m_anyHelper->read( serializer, m_any );
    }

    ///
    /// \brief hasValue
    /// \return
    ///
    bool hasValue() const {
#if CPP_VERSION < 17
        return !m_any.empty();
#else
        return m_any.has_value();
#endif
    }

    ///
    /// \brief is
    /// \return
    ///
    template <class T>
    bool is() const {
        return m_any.type() == typeid( T );
    }

    ///
    /// \brief get
    /// \return
    ///
    template <class T>
    const T& get() const {
        return std::any_cast<const T&>( m_any );
    }

    ///
    /// \brief typeName
    /// \return
    ///
    std::string typeName() const {
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->getTypeName != nullptr );
        return m_anyHelper->getTypeName();
    }

    ///
    /// \brief valueAsString
    /// \return
    ///
    std::string valueAsString() const {
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->getValueStr != nullptr );
        return m_anyHelper->getValueStr( m_any );
    }

    ///
    /// \brief toString
    /// \return
    ///
    auto toString() const {
        std::string str;
        assert( m_anyHelper != nullptr );
        str += "*'" + typeName() + "' : " + valueAsString() + "*";
        return str;
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Any& other ) const {
        assert( m_anyHelper != nullptr );
        assert( m_anyHelper->compare != nullptr );
        return m_any.type() == other.m_any.type() && m_anyHelper->compare( m_any, other.m_any );
    }

#if CPP_VERSION < 20
    ///
    /// \brief operator !=
    /// \param other
    /// \return
    ///
    bool operator!=( const Any& other ) const { return !( *this == other ); }
#endif

  private:
    std::any m_any;
    std::unique_ptr<Anyable::AnyHelper> m_anyHelper;
};

} // namespace hub
