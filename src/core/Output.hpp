#pragma once

// #include "io/output/OutputI.hpp"

// #include "io/output/OutputImpl.hpp"
// #include "io/output/OutputZppBits.hpp"

#include "core/Macros.hpp"
// #include "ioBase.hpp"
#include "OutputI.hpp"
#include "Serializer.hpp"

namespace hub {

//    using Output = output::OutputImpl;
// using Output = io::output::OutputZppBits;

// static_assert(std::is_base_of<io::output::OutputI, Output>::value, "Output is not base class of
// OutputI");

template <class SerializerT = Serializer>
// class Output : public ioBase
class OutputT : public OutputI
{
  public:
    using OutputI::write;


    //    template <typename T>
    //    using writable_t = decltype( std::declval<T>().write( std::declval<Output&>() ) );
    //    template <typename T, typename = std::void_t<>>
    //    struct writable : std::false_type {};
    //    template <typename T>
    //    struct writable<T, std::void_t<writable_t<T>>> : std::true_type {};
    //    template <typename T>
    //    static constexpr bool writable_v = writable<T>::value;

    //  public:
    //    virtual void write( const Data_t* data, Size_t len ) = 0;

  public:
    template <class T>
    typename std::enable_if<! notWritable_v<T> && writable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t ) << ") ..."
                  << std::endl;
#endif
        assert( isOpen() );
        t.write( *this );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t ) << ") = " << t
                  << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if<! notWritable_v<T> && packable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(packable: " << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        assert( isOpen() );
        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    }

    template <class T>
    typename std::enable_if<! notWritable_v<T> && serializable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "\033[1mwrite\033[0m(serializable: " << TYPE_NAME( t ) << ") ..."
                  << std::endl;
#endif
        assert( isOpen() );
        const_cast<T&>( t ).serialize( *this );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "\033[1mwrite\033[0m(serializable: " << TYPE_NAME( t ) << ") = " << t
                  << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if<! notWritable_v<T> && !writable_v<T> && !serializable_v<T> && !packable_v<T>>::type
    write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(raw: " << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        assert( isOpen() );
        //        m_serializer.write(*this, t);
        m_serializer.pack( *this, t );
    }

    template <class... Ts>
    void operator()( const Ts&... ts ) {
        assert( isOpen() );
        m_serializer.pack( *this, ts... );
    }

    void write( const char* str ) {
        assert( str != nullptr );
        assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(" << TYPE_NAME( str ) << ")" << std::endl;
#endif

        uint32_t strLen = static_cast<int>( std::strlen( str ) );
        write( strLen );

        if ( strLen > 0 ) { write( reinterpret_cast<const unsigned char*>( str ), strLen ); }
    }

    //  private:
    //    template <class... Ts>
    //    void writeAll_( const Ts&... ts ) {
    // #ifdef HUB_DEBUG_OUTPUT
    ////        std::cout << HEADER << "write(raw: " << TYPE_NAME( ts ) << ") = " << t << std::endl;
    // #endif
    //         assert( isOpen() );

    ////        zpp::bits::out output( m_serialBuff );
    ////        output( ts... ).or_throw();

    //////        assert( ( sizeof( Ts ) + ... ) == output.position() );

    ////                Size_t size = output.position();
    ////                assert(size < BuffSize);
    //////                write(size);
    ////                write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );

    ////        write( m_serialBuff.data(), output.position() );
    ////        //        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    //    }

  private:
    SerializerT m_serializer;
};

using Output = OutputT<>;

} // namespace hub
