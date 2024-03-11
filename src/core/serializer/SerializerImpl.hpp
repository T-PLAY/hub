/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31

#pragma once

#include <array>
#include <cstring>
#include <exception>
#include <list>
#include <map>
#include <tuple>
#include <vector>

#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"
#include "core/ios.hpp"

#include "core/Macros.hpp"
#include "core/ios.hpp"

#include "SerializerI.hpp"

namespace hub {
namespace serializer {

template <typename T>
using serializable_t = decltype( std::declval<T>().serialize( std::declval<ios&>() ) );

template <typename T, typename = std::void_t<>>
struct serializable : std::false_type {};

template <typename T>
struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool serializable_v = serializable<T>::value;

///
/// \brief The SerializerImpl class
///
class SerializerImpl : public SerializerI
{
    static constexpr Size_t BuffSize = 1'000'000; // 1 Mo
    bool m_writing                   = false;
    bool m_reading                   = false;

  public:

    ///
    /// \brief pack
    /// \param output
    /// \param ts
    ///
    template <class Output, class... Ts>
    void pack( Output& output, const Ts&... ts ) {
        m_writing  = true;
        m_position = 0;
        writeAll( ts... );

        const Size_t size = m_position;
        assert( size < BuffSize );
        output.write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
        output.write( m_serialBuff.data(), size );
        m_writing = false;
    }

    ///
    /// \brief unpack
    /// \param input
    /// \param ts
    ///
    template <class Input, class... Ts>
    void unpack( Input& input, Ts&... ts ) {
        m_reading = true;
        Size_t size;
        input.read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
        m_position = 0;
        assert( size < BuffSize );
        input.read( m_serialBuff.data(), size );

        readAll( ts... );
        assert( m_position == size );
        m_reading = false;
    }

    ///
    /// \brief operator ()
    /// \param ts
    ///
    template <class... Ts>
    void operator()( Ts&... ts ) {
        if ( m_writing ) { writeAll( ts... ); }
        else {
            assert( m_reading );
            readAll( ts... );
        }
    }

    ///
    /// \brief write
    /// \param data
    /// \param size
    ///
    virtual void write( const Data_t* data, Size_t size ) {
#ifdef HUB_DEBUG_OUTPUT
        std::vector<Data_t> vector( data, data + size );
        std::cout << "<---" << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif

        memcpy( &m_serialBuff[m_position], data, size );
        m_position += size;
    }

    ///
    /// \brief writeAll
    /// \param t
    ///
    template <class T>
    void writeAll( const T& t ) {
        write( t );
    }

    ///
    /// \brief writeAll
    /// \param t
    /// \param ts
    ///
    template <class T, class... Ts>
    void writeAll( const T& t, const Ts&... ts ) {
        write( t );
        writeAll( ts... );
    }

    ///
    /// \brief read
    /// \param data
    /// \param size
    ///
    virtual void read( Data_t* data, Size_t size ) {
        memcpy( data, &m_serialBuff[m_position], size );
        m_position += size;

#ifdef HUB_DEBUG_INPUT
        std::vector<Data_t> vector( data, data + size );
        std::cout << "\t --->" << HEADER << "read(Data_t*, Size_t) : data = " << vector
                  << ", read size = " << size << std::endl;
#endif
    }

    ///
    /// \brief readAll
    /// \param t
    ///
    template <class T>
    void readAll( T& t ) {
        read( t );
    }

    ///
    /// \brief readAll
    /// \param t
    /// \param ts
    ///
    template <class T, class... Ts>
    void readAll( T& t, Ts&... ts ) {
        read( t );
        readAll( ts... );
    }

  private:
    using ByteView        = std::vector<Data_t>;
    ByteView m_serialBuff = std::vector<Data_t>( BuffSize );
    Size_t m_position     = 0;
};

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

} // namespace serializer
} // namespace hub
