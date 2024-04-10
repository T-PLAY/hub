/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <sstream>

#include "Base.hpp"
#include "Format.hpp"
#include "Serializer.hpp"

#ifndef HUB_NON_BUILD_DATA
#    include "data/Mesh.hpp"
#endif


namespace hub {

///
/// \brief The Anyable class
/// able to register new user defined anyable class
///
class SRC_API Anyable
{

  private:
    class SRC_API AnyHelper
    {
      public:
        template <class T>
        void setup() {

            if constexpr ( std::is_same_v<T, void> ) {
                getTypeName = []() { return "void"; };
                getValueStr = []( const std::any& ) { return "nil"; };
                write       = []( Serializer&, const std::any& ) {};
                read        = []( Serializer&, std::any& ) {};
                compare     = []( const std::any& any, const std::any& any2 ) {
                    if ( any.type() == typeid( void ) || any2.type() == typeid( void ) ) {
                        return any.type() == any2.type();
                    }
                    return false;
                };
            }
            else {
                getTypeName = []() { return TYPE_NAME( T() ); };
                getValueStr = []( const std::any& any ) {
                    assert( typeid( T ) == any.type() );
                    const T& val = std::any_cast<const T&>( any );
                    std::stringstream sstream;
                    sstream << val;
                    return sstream.str();
                };
                if constexpr ( std::is_same_v<T, const char*> ) {
                    write = []( Serializer& serializer, const std::any& any ) {
                        const char* val = std::any_cast<const char*>( any );
                        serializer.write( val );
                    };
                    read = []( Serializer& serializer, std::any& any ) {
                        char* val =
                            new char[80]; // leak, please do not use char *, use std::string instead
                        serializer.read( val );
                        any = reinterpret_cast<const char*>(val);
                    };
                    compare = []( const std::any& any, const std::any& any2 ) {
                        return strcmp( std::any_cast<const char*>( any ),
                                       std::any_cast<const char*>( any2 ) ) == 0;
                    };
                }
                else {
                    write = []( Serializer& serializer, const std::any& any ) {
                        const T& val = std::any_cast<const T&>( any );
                        serializer.write( val );
                    };
                    read = []( Serializer& serializer, std::any& any ) {
                        T t;
                        serializer.read( t );
                        any = t;
                    };
                    compare = []( const std::any& any, const std::any& any2 ) {
                        if ( any.type() == typeid( void ) || any2.type() == typeid( void ) ) {
                            return any.type() == any2.type();
                        }
                        return std::any_cast<const T&>( any ) == std::any_cast<const T&>( any2 );
                    };
                }
            }
        }

        friend class Anyable;
        friend class Any;

      private:
        std::function<std::string()> getTypeName;
        std::function<std::string( const std::any& )> getValueStr;
        std::function<bool( const std::any&, const std::any& )> compare;
        std::function<void( Serializer&, const std::any& )> write;
        std::function<void( Serializer&, std::any& )> read;
    };

    template <class T>
    static AnyHelper make_anyHelper() {
        AnyHelper ret;
        ret.setup<T>();
        return ret;
    }

    template <class T>
    static std::pair<std::string, AnyHelper> makeAnyHelperRow() {
        if constexpr ( std::is_same_v<T, void> ) {
            return std::make_pair( "void", make_anyHelper<T>() );
        }
        else {
            return std::make_pair( TYPE_NAME( T() ), make_anyHelper<T>() );
        }
    }

    template <class... T>
    static std::map<std::string, AnyHelper> makeAnyHelperMap() {
        return { makeAnyHelperRow<T>()... };
    }

    /// todo change string to typeId
    static inline std::map<std::string, AnyHelper> s_anyables = makeAnyHelperMap<void,
                                                                                 int,
                                                                                 double,
                                                                                 bool,
                                                                                 std::string,
                                                                                 const char*,
                                                                                 format::Mat4,
                                                                                 format::Vec4,
                                                                                 format::Vec3
#ifndef HUB_NON_BUILD_DATA
                                                                                 , data::Mesh
#endif
                                                                                 >();

    template <class T>
    static void insertSupportedType() {
        assert( s_anyables.find( TYPE_NAME( T() ) ) == s_anyables.end() );
        s_anyables.insert( makeAnyHelperRow<T>() );
        assert( s_anyables.find( TYPE_NAME( T() ) ) != s_anyables.end() );
        std::cout << "[Anyable] added new supported type : " << TYPE_NAME( T() ) << std::endl;
    }
    template <class T, class... Ts>
    static void insertSupportedTypes() {
        insertSupportedType<T>();
        if constexpr ( sizeof...( Ts ) > 0 ) { insertSupportedTypes<Ts...>(); }
    }

  public:
    ///
    /// \brief supportedTypes
    /// \return
    ///
    static std::vector<std::string> supportedTypes() {
        std::vector<std::string> ret;
        for ( const auto& pair : s_anyables ) {
            assert( pair.first == pair.second.getTypeName() );
            ret.push_back( "<" + pair.second.getTypeName() + ">" );
        }
        return ret;
    }

    ///
    /// \brief registerTypes
    ///
    template <class... Ts>
    static void registerTypes() {
        insertSupportedTypes<Ts...>();
    }

    friend class Any;

}; // class Anyable

} // namespace hub
