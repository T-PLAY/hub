#pragma once

#include <cassert>

#include "core/Macros.hpp"
//#include "core/Traits.hpp"
#include "core/io/iosBase.hpp"

namespace hub {
namespace io {
namespace input {

//template <class T>
//class BusI
//{
//  public:
//    virtual void read(T & t) = 0;
//    virtual void write(const T & t) = 0;
//};

//template <class ReaderT>
class BasicInputI : public iosBase
{
  public:
//    virtual void read( Data_t* data, Size_t len ) = 0;
    virtual bool isEnd() const                  = 0;
    virtual void clear()                          = 0;

//    void read();

//    template <class T>
//    void read( T& t ) {
////        BusI<T>::write(t);
////        ReaderT::read(t);
//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
////        assert(false);
//    }
//    virtual void read() = 0;

//  private:
};

} // namespace input
} // namespace io
} // namespace hub
