#pragma once

//#include "core/Macros.hpp"
//#include "core/Traits.hpp"
#include <type_traits>

namespace hub {
namespace io {



class iosBase
{
  public:
    virtual void close()                          = 0;
    virtual bool isOpen() const                   = 0;

};

template <typename T>
using packable_t = decltype( T::packable );

template <typename T, typename = std::void_t<>>
struct packable : std::false_type {};

template <typename T>
struct packable<T, std::void_t<packable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool packable_v = packable<T>::value || std::is_same_v<T, int> || std::is_same_v<T, unsigned long>;

//template <typename T>
//static constexpr bool isSimpleType_v = std::is_same<T, int>::value;

template <typename T>
using serializable_t = decltype( std::declval<T>().serialize( std::declval<iosBase&>() ) );
//using serializable_t = decltype( std::declval<T&>().serialize() );

template <typename T, typename = std::void_t<>>
struct serializable : std::false_type {};

template <typename T>
struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

template <typename T>
//static constexpr bool serializable_v = serializable<std::remove_cvref_t<T>>::value;
static constexpr bool serializable_v = serializable<T>::value;

} // namespace io
} // namespace hub
