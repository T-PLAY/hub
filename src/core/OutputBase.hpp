/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09

#pragma once

#include <cassert>

#include "Macros.hpp"
#include "ios.hpp"

namespace hub {

class SRC_API OutputBase : public ios
{
  public:
    virtual void write( const Data_t* data, Size_t len ) = 0;
    virtual void setRetain( bool retain )                = 0;
};

template <typename T>
using writable_t = decltype( std::declval<T>().write( std::declval<OutputBase&>() ) );

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

} // namespace hub
