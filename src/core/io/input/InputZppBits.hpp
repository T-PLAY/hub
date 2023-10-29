#pragma once

#include "BasicInputI.hpp"

#include "zpp_bits.h"

namespace hub {
namespace io {
namespace input {

// template <Size_t BuffSize = 1'000'000>
class InputZppBits : public BasicInputI
{
  public:
    //    template <typename T>
    //    using serializable_t = decltype( std::declval<T>().serialize(
    //    std::declval<InputZppBits&>() ) );

    //    template <typename T, typename = std::void_t<>>
    //    struct serializable : std::false_type {};

    //    template <typename T>
    //    struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

    //    template <typename T>
    //    static constexpr bool serializable_v = serializable<T>::value;

    static constexpr Size_t BuffSize = 1'000'000;

    //    using BuffSize = 1'000'000
    virtual void read( Data_t* data, Size_t len ) = 0;

    template <class T>
    typename std::enable_if<readable_v<T>>::type read( T& t ) {
        //    void read( T& t ) {
        // #ifdef HUB_DEBUG_INPUT
        //         std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ")" << std::endl;
        //         std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") ..." << std::endl;
        // #endif
        assert( isOpen() );
        assert( !isEnd() );

        //        t.serialize(*this);
        t.read( *this );

#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(readable: " << TYPE_NAME( t ) << ") = " << t
                  << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if<serializable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );

        t.serialize( *this );
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(serial: " << TYPE_NAME( t ) << ") = " << t
                  << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if<packable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );

        //        Size_t size;
        //        read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
        //        assert(size < BuffSize);
        read( m_serialBuff.data(), sizeof( T ) );
        //        read( m_serialBuff.data(), size );

        zpp::bits::in input( m_serialBuff );
        input( t ).or_throw();
        //        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );

#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(packable: " << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if<!readable_v<T> && !serializable_v<T> && !packable_v<T>>::type
    read( T& t ) {
        // #ifdef HUB_DEBUG_INPUT
        //         std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ")" << std::endl;
        //         std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") ..." << std::endl;
        // #endif
        assert( isOpen() );
        assert( !isEnd() );

        //        zpp::bits::out output(m_serialBuff);
        //        output(t).or_throw();
        //        output(t);
        //        assert(output.data().size() != 1'000'000);

        Size_t size;
        read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
        assert( size < BuffSize );
        //        read( m_serialBuff.data(), sizeof(T) );
        read( m_serialBuff.data(), size );

        zpp::bits::in input( m_serialBuff );
        input( t ).or_throw();
        //        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );

#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(raw: " << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
    }

    template <class T>
    void operator()( T& t ) {
        read( t );
    }

    template <class... Ts>
    void operator()( Ts&... ts ) {
        readAll( ts... );
    }

  private:
    template <class... Ts>
    void readAll( Ts&... ts ) {
        assert( isOpen() );
        assert( !isEnd() );
        //        auto allSize = sizeofAll(ts...);
        //        read( m_serialBuff.data(), sizeofAll(ts...) );
        read( m_serialBuff.data(), ( sizeof( Ts ) + ... ) );
        zpp::bits::in input( m_serialBuff );
        //        input( ts... ).or_throw();
        input( ts... ).or_throw();
    }

  private:
    std::array<unsigned char, BuffSize> m_serialBuff;
};

} // namespace input
} // namespace io
} // namespace hub
