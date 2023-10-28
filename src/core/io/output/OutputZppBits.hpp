#pragma once

#include "BasicOutputI.hpp"

#include "zpp_bits.h"

namespace hub {
namespace io {
namespace output {

//template <Size_t BuffSize = 1'000'000>
class OutputZppBits : public BasicOutputI
{
  public:
    static constexpr Size_t BuffSize = 1'000'000;

    virtual void write( const Data_t* data, Size_t len ) = 0;

    template <class T>
//    typename std::enable_if_t<isPackable_v<T> || isSimpleType_v<T>, void>
    void write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        assert( isOpen() );

        zpp::bits::out output(m_serialBuff);
        output(t).or_throw();
//        input(t);
        Size_t size = output.position();
        assert(size < BuffSize);
        write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
        write( m_serialBuff.data(), output.position());

//        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    }

  private:
    std::array<unsigned char, BuffSize> m_serialBuff;
};

} // namespace output
} // namespace io
} // namespace hub
