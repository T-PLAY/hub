#pragma once

#include <cassert>

#include "core/Macros.hpp"
// #include "core/Traits.hpp"
#include "core/ioBase.hpp"

namespace hub {
// namespace io {
// namespace input {

// template <class T>
// class BusI
//{
//   public:
//     virtual void read(T & t) = 0;
//     virtual void write(const T & t) = 0;
// };

// template <class ReaderT>
class InputI : public ioBase
{
  public:
    virtual void read( Data_t* data, Size_t len ) = 0;
    virtual bool isEnd() const                    = 0;
    virtual void clear()                          = 0;

    //    void read();

    //    template <class T>
    //    void read( T& t ) {
    ////        BusI<T>::write(t);
    ////        ReaderT::read(t);
    // #ifdef HUB_DEBUG_INPUT
    //         std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
    // #endif
    ////        assert(false);
    //    }
    //    virtual void read() = 0;

    //  private:
};

template <typename T>
using readable_t = decltype( std::declval<T>().read( std::declval<InputI&>() ) );

template <typename T, typename = std::void_t<>>
struct readable : std::false_type {};

template <typename T>
struct readable<T, std::void_t<readable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool readable_v = readable<T>::value;

///////////////////////////////////////

template <typename T>
using notReadable_t = decltype( T::notReadable );

template <typename T, typename = std::void_t<>>
struct notReadable : std::false_type {};

template <typename T>
struct notReadable<T, std::void_t<notReadable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool notReadable_v = notReadable<T>::value;
//    static constexpr bool packable_v = ! std::is_constructible_v<T> ;

//} // namespace input
//} // namespace io
} // namespace hub
