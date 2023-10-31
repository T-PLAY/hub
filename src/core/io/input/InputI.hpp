#pragma once

#include "BasicInputI.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"

namespace hub {
namespace io {
namespace input {

//class InputI : public BasicInputI
//{
//  public:
//    virtual void read( Data_t* data, Size_t len ) = 0;

//    template <class T>
//    //    typename std::enable_if<!readable_v<T>>::type read( T& t ) {
//    //    typename std::enable_if<!serializable_v<T> && ! readable_v<T> && ! notReadable_v<T>>::type
//    //    read( T& t ) {
////    typename std::enable_if_t<packable_v<T>, void>
//    void read( T& ) {
//        assert(false);
//////#ifdef HUB_DEBUG_INPUT
//////        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ")" << std::endl;
//////        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") ..." << std::endl;
//////#endif
////        assert( isOpen() );
////        assert( !isEnd() );

////        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
////#ifdef HUB_DEBUG_INPUT
////        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
////#endif
//    }

//};

} // namespace input
} // namespace io
} // namespace hub
