/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/29
	
#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <sstream>

#include "Macros.hpp"
// #include "core/std_any.hpp"
// #include "core/Input.hpp"
// #include "core/Output.hpp"
// #include "core/io/input/InputBase.hpp"
// #include "core/io/output/OutputBase.hpp"
// #include "core/Input.hpp"
// #include "core/Output.hpp"
#include "Serializer.hpp"
// #include "core/serializer/SerializerI.hpp"

#if CPP_VERSION <= 14
#    include "traits/std_any.hpp"
#else
#    include <any>
#endif

namespace hub {

class SRC_API Anyable
{

  private:
    class SRC_API AnyHelper
    {
        //  private:
      public:
        //        AnyHelper() = delete;
        //        AnyHelper(AnyHelper &&) = delete;
        //        AnyHelper(const AnyHelper &) = delete;

        //    template <typename T>
        //    Anyable( const T* t )
        //    {
        //        getTypeName = [](  ) {
        ////            return std::string("const ") + TYPE_NAME(T) + " *";
        //            return std::string(TYPE_NAME(T)) + "*";
        //        };

        //        getValueStr = []( const std::any& any ) {
        //            const T* val = std::any_cast<const T*>( any );
        //            std::stringstream sstream;
        //            sstream << val;
        //            return sstream.str();
        //        };
        //        compare = []( const std::any& any, const std::any& any2 ) {
        //            return !strcmp( std::any_cast<const T*>( any ), std::any_cast<const T*>( any2
        //            ) );
        //        };
        //    }

        template <class T>
        void setup() {
            //            std::cout << "[Anyable] setup type : " << TYPE_NAME(T) << std::endl;

            // getTypeName = []() { return TYPE_NAME( T ); };

            // void
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
                // const char *
                if constexpr ( std::is_same_v<T, const char*> ) {
                    write = []( Serializer& serializer, const std::any& any ) {
                        const char* val = std::any_cast<const char*>( any );
                        serializer.write( val );
                    };
                    read = []( Serializer& serializer, std::any& any ) {
                        char* val =
                            new char[80]; // leak, please do not use char *, use std::string instead
                        serializer.read( val );
                        any = (const char*)val;
                    };
                    compare = []( const std::any& any, const std::any& any2 ) {
                        return strcmp( std::any_cast<const char*>( any ),
                                       std::any_cast<const char*>( any2 ) ) == 0;
                    };
                }
                // others
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

        // AnyHelper()
               // AnyHelper() = default;
               // AnyHelper(AnyHelper &&) = delete;
               // AnyHelper(const AnyHelper &) = default;
               // AnyHelper& operator=(const AnyHelper &) = default;

      private:
        std::function<std::string()> getTypeName;
        std::function<std::string( const std::any& )> getValueStr;
        std::function<bool( const std::any&, const std::any& )> compare;
        //        std::function<void( io::output::OutputBase& output, const std::any& )> write;
        //        std::function<void( Output&, const std::any& )> write;
        std::function<void( Serializer&, const std::any& )> write;
        std::function<void( Serializer&, std::any& )> read;
        //        std::function<void( Input&, std::any& )> read;
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

    // static std::map<size_t, AnyHelper> s_anyables = makeAnyHelperMap<double>();

    //    static std::map<size_t, AnyHelper> s_anyables;
    template <class... T>
    static std::map<std::string, AnyHelper> makeAnyHelperMap() {
        // AnyHelper voidHelper;
        return { makeAnyHelperRow<T>()... };
        //    return std::map<size_t, AnyHelper>{ makeRow<T>()};
    }

    // static std::map<size_t, AnyHelper> s_anyables;
    static inline std::map<std::string, AnyHelper> s_anyables = makeAnyHelperMap<void, int, double, std::string, const char*>();

    //    static std::vector<std::string> supportedTypes();

    template <class T>
    static void insertSupportedType() {
        assert( s_anyables.find( TYPE_NAME( T() ) ) == s_anyables.end() );
        s_anyables.insert( makeAnyHelperRow<T>() );
        assert( s_anyables.find( TYPE_NAME( T() ) ) != s_anyables.end() );
        // std::cout << "[Anyable] added new supported type : " << TYPE_NAME(T) << std::endl;
        std::cout << "[Anyable] added new supported type : " << TYPE_NAME( T() ) << std::endl;
    }
    template <class T, class... Ts>
    static void insertSupportedTypes() {
        insertSupportedType<T>();
        if constexpr ( sizeof...( Ts ) > 0 ) { insertSupportedTypes<Ts...>(); }
        //        s_anyables.insert( makeAnyHelperRow<T>() );
    }

  public:
    static std::vector<std::string> supportedTypes() {
        std::vector<std::string> ret;
        for ( const auto& pair : s_anyables ) {
            assert(pair.first == pair.second.getTypeName());
            ret.push_back( "<" + pair.second.getTypeName() + ">" );
            //        std::cout << pair.first << " -> " << pair.second.getTypeName() << std::endl;
        }
        return ret;
    }

    template <class... Ts>
    static void registerTypes() {
        insertSupportedTypes<Ts...>();
        //        insertSupportedType<Ts...>();
        //        if constexpr ( sizeof...( Ts ) > 0 ) { addSupportedType<Ts...>(); }
        //        s_anyables.insert( makeAnyHelperRow<T>() );
    }

    friend class Any;
    //    friend class std::ostream;

}; // class Anyable
//#pragma export(Anyable::s_anyables)

//std::map<std::string, Anyable::AnyHelper> SRC_API Anyable::s_anyables =
    // todo c++17
//    makeAnyHelperMap<void, int, double, std::string, const char*>();
    // makeAnyHelperMap<void, int, double, std::string, const char*>();

} // namespace hub
