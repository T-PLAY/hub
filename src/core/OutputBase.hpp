#pragma once

#include <cassert>

#include "core/Macros.hpp"
// #include "core/Traits.hpp"
#include "core/ios.hpp"

namespace hub {
// namespace io {
// namespace output {

class OutputBase : public ios
{
  public:
    virtual void write( const Data_t* data, Size_t len ) = 0;

    //    void write() {};

    //    template <class T>
    //    void write(const T & t) {
    // #ifdef HUB_DEBUG_OUTPUT
    //        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
    // #endif
    //        assert(false);
    //    }
};

template <typename T>
using writable_t = decltype( std::declval<T>().write( std::declval<OutputBase&>() ) );
//    using writable_t = decltype( write( std::declval<T>(), std::declval<OutputImpl&>() ) );

template <typename T, typename = std::void_t<>>
struct writable : std::false_type {};

template <typename T>
struct writable<T, std::void_t<writable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool writable_v = writable<T>::value;

////////////////////

template <typename T>
using notWritable_t = decltype( T::notWritable );

template <typename T, typename = std::void_t<>>
struct notWritable : std::false_type {};

template <typename T>
struct notWritable<T, std::void_t<notWritable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool notWritable_v = notWritable<T>::value;

//} // namespace output
//} // namespace io
} // namespace hub
