/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/14

#pragma once

#include "InputBase.hpp"
#include "Macros.hpp"
#include "Serializer.hpp"

namespace hub {

///
/// \brief The InputT class
/// is an implement of InputBase
/// able to use custom serializer as a template parameter
///
template <class SerializerT = Serializer>
class SRC_API InputT : public InputBase
{
  public:
    using InputBase::read;

    virtual ~InputT() = default;

  public:
    ///
    /// \brief read
    /// \param t
    /// \return
    ///
    template <class T>
    typename std::enable_if<packable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        if constexpr ( isPacket<T> ) { read( t.data(), t.size() ); }
        else {
            read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
        }
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(packable: " << TYPE_NAME( t ) << ") = " << t );
#endif
    }

    ///
    /// \brief read
    /// \param t
    /// \return
    ///
    template <class T>
    typename std::enable_if<!packable_v<T> && readable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        t.read( *this );

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(readable: " << TYPE_NAME( t ) << ") = " << t );
#endif
    }

    ///
    /// \brief read
    /// \param t
    /// \return
    ///
    template <class T>
    typename std::enable_if<!packable_v<T> && !readable_v<T>>::type read( T& t ) {
        assert( isOpen() );
        assert( !isEnd() );
        m_serializer.unpack( *this, t );

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t" << HEADER << "read(serial: " << TYPE_NAME( t ) << ") = " << t );
#endif
    }

    ///
    /// \brief readAll
    /// \param t
    /// \param ts
    ///
    template <class T, class... Ts>
    void readAll( T& t, Ts&... ts ) {
        m_serializer.unpack( *this, t, ts... );
    }

  private:
    SerializerT m_serializer;
};

///
/// \brief Input
///
using Input = InputT<Serializer>;

} // namespace hub
