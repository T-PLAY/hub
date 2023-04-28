#pragma once

#include <type_traits>


#if ( __cplusplus < 201703L )
    template<class...> struct is_one_of: std::false_type {};
    template<class T1, class T2> struct is_one_of<T1, T2>: std::is_same<T1, T2> {};
    template<class T1, class T2, class... Ts> struct is_one_of<T1, T2, Ts...>: std::conditional<std::is_same<T1, T2>::value, std::is_same<T1, T2>, is_one_of<T1, Ts...>>::type {};
#else
    template<class T1, class... Ts>
    constexpr bool is_one_of() noexcept {
    return (std::is_same<T1, Ts>{} || ...);
    }
#endif
