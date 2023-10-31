#pragma once

#include "BasicOutputI.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"

namespace hub {
namespace io {
namespace output {

//class OutputI : public BasicOutputI
//{
//  public:
//    virtual void write( const Data_t* data, Size_t len ) = 0;

//    template <class T>
//    //    typename std::enable_if<!writable_v<T>>::type write( const T& t ) {
//    //    typename std::enable_if<!serializable_v<T> && !writable_v<T> && ! notWritable_v<T>>::type
//    //    write( const T& t ) {
////        typename std::enable_if_t<std::isww
////    typename std::enable_if_t<packable_v<T>, void>
//    void write( const T& t) {
////        assert(false);
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(" << TYPE_NAME( T ) << ") = " << t << std::endl;
//#endif
////        assert( isOpen() );
////        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
//    }
//};

} // namespace output
} // namespace io
} // namespace hub
