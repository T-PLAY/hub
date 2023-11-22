#pragma once

// #include "io/input/InputBase.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

#include "core/Macros.hpp"
// #include "ios.hpp"
#include "InputBase.hpp"
// #include "serializer/SerializerI.hpp"
#include "Serializer.hpp"

namespace hub {

// using Input = io::input::InputImpl;
// using Input = io::input::InputZppBits;

// static_assert(std::is_base_of<io::input::InputBase, Input>::value, "Input is not base class of
// IntputI");

template <class SerializerT = Serializer>
// class Input : public ios
class InputT : public InputBase
{
  public:
    using InputBase::read;
//    using InputBase::clear;

    //    template <typename T>
    //    using readable_t = decltype( std::declval<T>().read( std::declval<Input&>() ) );
    //    template <typename T, typename = std::void_t<>>
    //    struct readable : std::false_type {};
    //    template <typename T>
    //    struct readable<T, std::void_t<readable_t<T>>> : std::true_type {};
    //    template <typename T>
    //    static constexpr bool readable_v = readable<T>::value;

    //  public:
    //    virtual bool isEnd() const                    = 0;
    //    virtual void clear()                          = 0;
    //    virtual void read( Data_t* data, Size_t len ) = 0;

    //  public:
    //    Input( SerializerT serializer = SerializerT() ) : m_serializer( std::move( serializer ) )
    //    {}

    virtual ~InputT() = default;

  public:
    //    template <class T>
    //    typename std::enable_if<!notReadable_v<T> && readable_v<T>>::type read( T& t ) {
    // #ifdef HUB_DEBUG_INPUT
    //        std::cout << "\t" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( T ) << ")
    //        ..."
    //                  << std::endl;
    // #endif
    //        assert( isOpen() );
    //        assert( !isEnd() );
    //        t.read( *this );
    // #ifdef HUB_DEBUG_INPUT
    //        std::cout << "\t" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( T )
    //                  << ") = " << t << std::endl;
    // #endif
    //    }

    template <class T>
    //    typename std::enable_if<!notReadable_v<T> && packable_v<T>>::type read( T& t ) {
    typename std::enable_if<packable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        if constexpr ( isPacket<T> ) { read( t.data(), t.size() ); }
        else { read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) ); }
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(packable: " << TYPE_NAME( T ) << ") = " << t
                  << std::endl;
#endif
    }

    //    template <class T>
    //    typename std::enable_if<!notReadable_v<T> && serializable_v<T>>::type read( T& t ) {
    //        assert( isOpen() );
    //        assert( !isEnd() );
    // #ifdef HUB_DEBUG_INPUT
    //        std::cout << "\t" << HEADER << "\033[1mread\033[0m(serial: " << TYPE_NAME( T ) << ")
    //        ..."
    //                  << std::endl;
    // #endif
    //        t.serialize( *this );
    // #ifdef HUB_DEBUG_INPUT
    //        std::cout << "\t" << HEADER << "\033[1mread\033[0m(serial: " << TYPE_NAME( T )
    //                  << ") = " << t << std::endl;
    // #endif
    //    }

    template <class T>
    //    typename std::enable_if<!notReadable_v<T> && !readable_v<T> && !serializable_v<T> &&
    //                            !packable_v<T>>::type
    //    read( T& t ) {
    typename std::enable_if<!packable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        //        m_serializer.read(*this, t);
        m_serializer.unpack( *this, t );

#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(raw: " << TYPE_NAME( T ) << ") = " << t << std::endl;
#endif
    }

    ////////////////////////////////////////////////////////////////////////////////

  public:
    //    template <class T>
    //        requires( packable_v<T> )
    //    void read( std::vector<T>& vector ) {
    //        Size_t nEl = static_cast<Size_t>( vector.size() );
    //        read( nEl );
    //        vector.resize( nEl );
    //        read( (Data_t*)vector.data(), nEl * sizeof( T ) );
    // #ifdef HUB_DEBUG_INPUT
    //        std::cout << "\t" << HEADER << "read(vector<" << TYPE_NAME( T ) << ">) = " << vector
    //        << std::endl;
    // #endif
    //    }

    //    template <class T>
    //        requires( !packable_v<T> )
    //    void read( std::vector<T>& vector ) {
    //        Size_t nEl = static_cast<Size_t>( vector.size() );
    //        read( nEl );
    //        assert( vector.size() == 0 );
    //        //        vector.resize(nEl);
    //        for ( int i = 0; i < nEl; ++i ) {
    //            //            read(vector.at(i));
    //            T t;
    //            read( t );
    //            vector.push_back( t );
    //        }
    // #ifdef HUB_DEBUG_INPUT
    //        std::cout << "\t" << HEADER << "read(vector<" << TYPE_NAME( T ) << ">) = " << vector
    //        << std::endl;
    // #endif
    //    }

    //    template <class T>
    //    void read( T& t) {
    //                assert( isOpen() );
    //                assert( !isEnd() );

    //        //        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
    //         #ifdef HUB_DEBUG_INPUT
    //                std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t <<
    //                std::endl;
    //         #endif
    //    }

    // #ifdef ARCH_X86
    //     void read( size_t size ) = delete; // non compatible format 32/64 bit
    // #endif

    //    void read( char* str ) {
    //        assert( str != nullptr );
    //        assert( isOpen() );
    //        assert( !isEnd() );

    // #ifdef HUB_DEBUG_INPUT
    //         std::cout << "\t" << HEADER << "read(char *)" << std::endl;
    // #endif

    //        int strLen = 0;
    //        assert( sizeof( int ) == 4 );
    //        read( strLen );

    //        if ( strLen == 0 ) { str[0] = 0; }
    //        else {
    //            read( reinterpret_cast<unsigned char*>( str ), strLen );

    //            str[strLen] = 0;
    //        }
    //    }

    //  private:
    //    template <class... Ts>
    //    void readAll_( Ts&... ts ) {
    //        assert( isOpen() );
    //        assert( !isEnd() );
    //        //        auto allSize = sizeofAll(ts...);
    //        //        read( m_serialBuff.data(), sizeofAll(ts...) );
    ////        Size_t size;
    ////        read(size);
    ////        Size_t size;
    ////        read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
    ////        assert(size < BuffSize);

    //////        read( m_serialBuff.data(), ( sizeof( Ts ) + ... ) );
    ////        read( m_serialBuff.data(), size);
    ////        zpp::bits::in input( m_serialBuff );
    ////        //        input( ts... ).or_throw();
    ////        input( ts... ).or_throw();
    //    }

    //    template <class... Ts>
    //    void operator()( Ts&... ts ) {
    //        assert( isOpen() );
    //        assert( !isEnd() );
    //        //        readAll_( ts... );
    //        m_serializer.unpack( *this, ts... );
    //    }

  private:
    SerializerT m_serializer;
};

using Input = InputT<Serializer>;

} // namespace hub
