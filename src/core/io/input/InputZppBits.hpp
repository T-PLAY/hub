#pragma once

#include "BasicInputI.hpp"

#include "zpp_bits.h"

namespace hub {
namespace io {
namespace input {

//class InputZppBits : public InputI
class InputZppBits : public BasicInputI
{
  public:
//    using BasicInputI::read;

    virtual void read( Data_t* data, Size_t len ) = 0;

    template <class T>
    //    typename std::enable_if<!readable_v<T>>::type read( T& t ) {
    //    typename std::enable_if<!serializable_v<T> && ! readable_v<T> && ! notReadable_v<T>>::type
    //    read( T& t ) {
//    typename std::enable_if_t<isPackable_v<T> || isSimpleType_v<T>, void>
    void read( T& t ) {
//#ifdef HUB_DEBUG_INPUT
//        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ")" << std::endl;
//        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") ..." << std::endl;
//#endif
        assert( isOpen() );
        assert( !isEnd() );

//        zpp::bits::out output(m_serialBuff);
//        output(t).or_throw();
//        output(t);
//        assert(output.data().size() != 1'000'000);
        Size_t size;
        read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
        read(m_serialBuff.data(), size);

        zpp::bits::in input(m_serialBuff);
        input(t).or_throw();
//        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );

#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
    }

  private:
    std::array<unsigned char, 1'000'000> m_serialBuff;
};

} // namespace input
} // namespace io
} // namespace hub
