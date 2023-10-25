#pragma once

#include "core/Macros.hpp"
#include "core/Traits.hpp"

namespace hub {
namespace io {
namespace output {

class OutputI
{
  public:
    virtual void write( const Data_t* data, Size_t len ) = 0;
    virtual void close()                                 = 0;
    virtual bool isOpen() const                          = 0;

    template <class T>
    //    typename std::enable_if<!writable_v<T>>::type write( const T& t ) {
    //    typename std::enable_if<!serializable_v<T> && !writable_v<T> && ! notWritable_v<T>>::type
    //    write( const T& t ) {
//        typename std::enable_if_t<std::isww
    typename std::enable_if_t<isPackable_v<T> || isSimpleType_v<T>, void>
    write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        assert( isOpen() );
        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    }

};

} // namespace output
} // namespace io
} // namespace hub
