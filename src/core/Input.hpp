#pragma once

#include "InputBase.hpp"
#include "Serializer.hpp"
#include "Macros.hpp"

// #define HUB_DEBUG_INPUT

namespace hub {

template <class SerializerT = Serializer>
class InputT : public InputBase
{
  public:
    using InputBase::read;

    virtual ~InputT() = default;

  public:
    template <class T>
    typename std::enable_if<packable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        if constexpr ( isPacket<T> ) { read( t.data(), t.size() ); }
        else { read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) ); }
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(packable: " << TYPE_NAME( T ) << ") = " << t );
#endif
    }

    template <class T>
        // requires (readable_v<T>)
    typename std::enable_if<!packable_v<T> && readable_v<T>>::type read( T& t ) {
    // void read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        // m_serializer.unpack( *this, t );
        t.read(*this);

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(readable: " << TYPE_NAME( T ) << ") = " << t );
#endif
    }

    template <class T>
    typename std::enable_if<!packable_v<T> && ! readable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        m_serializer.unpack( *this, t );

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(raw: " << TYPE_NAME( T ) << ") = " << t );
#endif
    }

    template <class T, class... Ts>
    void readAll(T& t, Ts&... ts) {
        m_serializer.unpack(*this, t, ts...);
        // read(t);
        // if constexpr (sizeof...(ts) > 0) {
            // readAll(ts...);
        // }
    }

  private:
    SerializerT m_serializer;
};

using Input = InputT<Serializer>;

} // namespace hub
