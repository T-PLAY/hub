#pragma once

#include <type_traits>

#include "Macros.hpp"
#if defined( HUB_DEBUG_INPUT ) || defined( HUB_DEBUG_OUTPUT )
#    include <mutex>
#endif
#include "Traits.hpp"

namespace hub {

#if defined( HUB_DEBUG_INPUT ) || defined( HUB_DEBUG_OUTPUT )

extern std::mutex s_mtxIoPrint; // shared mutex over threads
// static std::mutex s_mtxIoPrint;

#    undef DEBUG_MSG
#    define DEBUG_MSG( str )               \
        s_mtxIoPrint.lock();               \
        do {                               \
            std::cout << str << std::endl; \
        } while ( false );                 \
        s_mtxIoPrint.unlock();

#endif

class ios
{
  public:
    virtual void close()        = 0;
    virtual bool isOpen() const = 0;
};

#if CPP_VERSION >= 20

template <class T>
concept isPacket = requires( T t ) {
    t.data();
    t.size();
}; // uses by acquisition

#else

template <typename T>
using has_data_t = decltype( std::declval<T>().data() );
template <typename T, typename = std::void_t<>>
struct has_data : std::false_type {};
template <typename T>
struct has_data<T, std::void_t<has_data_t<T>>> : std::true_type {};
template <typename T>
static constexpr bool has_data_v = has_data<T>::value;

///////////

template <typename T>
using has_size_t = decltype( std::declval<T>().size() );
template <typename T, typename = std::void_t<>>
struct has_size : std::false_type {};
template <typename T>
struct has_size<T, std::void_t<has_size_t<T>>> : std::true_type {};
template <typename T>
static constexpr bool has_size_v = has_size<T>::value;

template <class T>
constexpr bool isPacket = has_data_v<T> && has_size_v<T>;

#endif

template <typename T>
using packable_t = decltype( T::packable );
template <typename T, typename = std::void_t<>>
struct packable : std::false_type {};
template <typename T>
struct packable<T, std::void_t<packable_t<T>>> : std::true_type {};
template <typename T>
static constexpr bool packable_v =
    packable<T>::value || std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T> || isPacket<T>;

} // namespace hub
