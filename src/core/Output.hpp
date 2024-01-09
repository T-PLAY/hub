/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/14
	
#pragma once

#include "Macros.hpp"
#include "OutputBase.hpp"
#include "Serializer.hpp"

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
        DEBUG_MSG( HEADER << "write(packable: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        if constexpr ( isPacket<T> ) {
            // assert(false);
            write( t.data(), t.size() );
        }
        else { write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) ); }
    }

    template <class T>
    typename std::enable_if<!packable_v<T> && writable_v<T>>::type write( const T& t ) {
        // assert(false);
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(writable: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        const_cast<T&>( t ).write( *this );
    }

    template <class T>
    typename std::enable_if<!packable_v<T> && !writable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( HEADER << "write(serial: " << TYPE_NAME( t ) << ") = " << t );
#endif
        assert( isOpen() );
        m_serializer.pack( *this, t );
    }

    template <class T, class... Ts>
    void writeAll( const T& t, const Ts&... ts ) {
        m_serializer.pack( *this, t, ts... );
    }

  private:
    SerializerT m_serializer;
};

using Output = OutputT<>;

} // namespace hub
