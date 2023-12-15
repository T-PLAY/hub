#pragma once

#include "OutputBase.hpp"
#include "Serializer.hpp"
#include "core/Macros.hpp"

// #define HUB_DEBUG_OUTPUT

namespace hub {

template <class SerializerT = Serializer>
class OutputT : public OutputBase
{
  public:
    using OutputBase::write;

    virtual ~OutputT() = default;

    template <class T>
    typename std::enable_if<packable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(packable: " << TYPE_NAME( T ) << ") = " << t );
#endif
        assert( isOpen() );
        if constexpr ( isPacket<T> ) { write( t.data(), t.size() ); }
        else { write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) ); }
    }

    template <class T>
    typename std::enable_if<! packable_v<T> && writable_v<T>>::type
        // requires (writable_v<T>)
    write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(writable: " << TYPE_NAME( T ) << ") = " << t );
#endif
        assert( isOpen() );
        // m_serializer.pack( *this, t );
        const_cast<T&>(t).write(*this);
        // t.write(const_cast<Output*>(this));
        // t.write(*reinterpret_cast<T*>(t));
    }

    template <class T>
    typename std::enable_if<!packable_v<T> && ! writable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(raw: " << TYPE_NAME( T ) << ") = " << t );
#endif
        assert( isOpen() );
        m_serializer.pack( *this, t );
    }

    template <class T, class... Ts>
    void writeAll(const T& t, const Ts&... ts) {
        m_serializer.pack(*this, t, ts...);
        // write(t);
        // if constexpr (sizeof...(ts) > 0) {
            // writeAll(ts...);
        // }
    }

  private:
    SerializerT m_serializer;
};

using Output = OutputT<>;

} // namespace hub
