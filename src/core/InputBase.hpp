/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09

#pragma once

#include <cassert>

#include "Macros.hpp"
#include "ios.hpp"

namespace hub {
class SRC_API InputBase : public ios
{
  public:
    virtual void read( Data_t* data, Size_t len ) = 0;
    virtual bool isEnd() const                    = 0;
    virtual void clear()                          = 0;
};

template <typename T>
using not_endable_t = decltype( T::not_endable );

template <typename T, typename = std::void_t<>>
struct not_endable : std::false_type {};

template <typename T>
struct not_endable<T, std::void_t<not_endable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool not_endable_v = not_endable<T>::value;

#if CPP_VERSION >= 20
template <class T>
concept Endable = !not_endable_v<T>;
#else
template <class T>
constexpr bool Endable = !not_endable_v<T>;
#endif

///////////////////////////////////////

template <typename T>
using readable_t = decltype( std::declval<T>().read( std::declval<InputBase&>() ) );

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

} // namespace hub
