#pragma once

// #include "core/Macros.hpp"
// #include "core/Traits.hpp"
#include <type_traits>

#include "core/Traits.hpp"
// #include <vector>

namespace hub {
// namespace io {

class ios
{
  public:
    virtual void close()        = 0;
    virtual bool isOpen() const = 0;
};

// template<typename>
// struct is_std_vector : std::false_type {};
// template<typename T, typename A>
// struct is_std_vector<std::vector<T,A>> : std::true_type {};

template <class T>
concept isPacket = requires (T t) { t.data(); t.size(); };

template <typename T>
using packable_t = decltype( T::packable );
template <typename T, typename = std::void_t<>>
struct packable : std::false_type {};
template <typename T>
struct packable<T, std::void_t<packable_t<T>>> : std::true_type {};
template <typename T>
// static constexpr bool packable_v = packable<T>::value || std::is_same_v<T, int> ||
// std::is_same_v<T, unsigned long>; static constexpr bool packable_v = packable<T>::value ||
// std::is_same_v<T, int> || std::is_same_v<T, unsigned long>; static constexpr bool packable_v =
// packable<T>::value || std::is_arithmetic_v<T> || std::is_array_v<T> || is_std_vector<T>::value;
// // double free corruption
static constexpr bool packable_v =
    packable<T>::value || std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T>;

// template <typename T>
// static constexpr bool isSimpleType_v = std::is_same<T, int>::value;

template <typename T>
using serializable_t = decltype( std::declval<T>().serialize( std::declval<ios&>() ) );
//using serializable_t = decltype( T::serialize );
// using serializable_t = decltype( std::declval<T&>().serialize() );

template <typename T, typename = std::void_t<>>
struct serializable : std::false_type {};

template <typename T>
struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

template <typename T>
// static constexpr bool serializable_v = serializable<std::remove_cvref_t<T>>::value;
static constexpr bool serializable_v = serializable<T>::value;

//} // namespace io
} // namespace hub
