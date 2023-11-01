#pragma once

#include <thread>
#include <vector>

#include "core/io/BasicInputOutput.hpp"


#include "zpp_bits.h"

namespace hub {
namespace io {
namespace book {

// template <Size_t NumberOfCharacter = 1'000'000> // max size on stack 1Mo
//           class Characters         = std::array<unsigned char, NumberOfCharacter>>
// template <zpp::bits::concepts::byte_view ByteView>
template <Size_t NumberOfCharacter = 1'000'000, class Characters = std::array<unsigned char, NumberOfCharacter>>
class BookZppBits : public BasicInputOutput
{
  public:
    //    template <class Characters>
    BookZppBits( Characters& characters ) : m_characters( characters ) {}
    BookZppBits() : m_characters { *new Characters } {}

    void write( const Data_t* data, Size_t len ) override {
        zpp::bits::out out( m_characters );
        std::span span( data, len );
        //        zpp::bits::out(m_characters)( std::span(data, len) ).or_throw();
        out( span ).or_throw();
        //        assert( len < m_characters.size());
        //        memcpy(m_characters.data(), data, len);
    }

    constexpr void read( Data_t* data, Size_t len ) override {
        zpp::bits::in in( m_characters );
        std::span span( data, len );
        in( span ).or_throw();
        //        zpp::bits::in(m_characters)( std::span(data, len) ).or_throw();
        //        assert( len < m_characters.size());
        //        memcpy(data, m_characters.data(), len);
    }

    template <class T>
    void read( T& t ) {
        // #ifdef HUB_DEBUG_INPUT
        //         std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") ..." << std::endl;
        // #endif
        zpp::bits::in in( m_characters );
        in( t ).or_throw();
//        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
//        m_in( t ).or_throw();
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
    }

    template <class T>
    void write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        zpp::bits::out out( m_characters );
        out( t ).or_throw();
        //        m_out( t ).or_throw();
    }

    void close() override {};

    bool isOpen() const override { return true; }

    bool isEnd() const override {
        zpp::bits::in in( m_characters );
        zpp::bits::out out( m_characters );
        return in.position() == out.position();
    }

    void clear() override {
        //        m_in.reset( 0 );
        //        m_out.reset( 0 );
    }

  private:
    Characters& m_characters;
    //    std::array<unsigned char, NumberOfCharacter> m_characters;
};

} // namespace book
} // namespace io
} // namespace hub
