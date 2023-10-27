#pragma once

#include "BasicOutputI.hpp"

#include "zpp_bits.h"

namespace hub {
namespace io {
namespace output {

class OutputZppBits : public BasicOutputI
{
  public:
//    using OutputI::write;

    virtual void write( const Data_t* data, Size_t len ) = 0;

    template <class T>
    //    typename std::enable_if<!writable_v<T>>::type write( const T& t ) {
    //    typename std::enable_if<!serializable_v<T> && !writable_v<T> && ! notWritable_v<T>>::type
    //    write( const T& t ) {
//        typename std::enable_if_t<std::isww
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
        write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
        write( m_serialBuff.data(), output.position());

//        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    }

  private:
    std::array<unsigned char, 1'000'000> m_serialBuff;
};

} // namespace output
} // namespace io
} // namespace hub
