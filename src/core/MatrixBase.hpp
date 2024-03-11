/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2024/01/04

#pragma once

#include <algorithm>
#include <type_traits>

#include "Buffer.hpp"
#include "Macros.hpp"
#include "Traits.hpp"

namespace hub {

#if CPLUSPLUSVERSION >= 20
template <class T>
concept isMatrix = requires {
    T::matrix;
};

template <class... Ts>
concept areMatrices = requires {
    requires( isMatrix<Ts> && ... );
};

#else

///
/// \brief is_matrix_t
///
template <typename T>
using is_matrix_t = decltype( T::matrix );
template <typename T, typename = std::void_t<>>
struct is_matrix : std::false_type {};
template <typename T>
struct is_matrix<T, std::void_t<is_matrix_t<T>>> : std::true_type {};
template <typename T>
static constexpr bool is_matrix_v = is_matrix<T>::value;

template <typename T>
static constexpr bool isMatrix = is_matrix<T>::value;

///
/// \brief areMatrices_
/// \return
///
template <class T, class... Ts>
constexpr bool areMatrices_() {
    if constexpr ( sizeof...( Ts ) > 0 ) { return isMatrix<T> && areMatrices_<Ts...>(); }
    else {
        return isMatrix<T>;
    }
}

///
/// \brief areMatrices
///
template <class... Ts>
constexpr bool areMatrices = areMatrices_<Ts...>();

#endif

static_assert( !isMatrix<int> );

////     virtual constexpr Size_t size() = 0;

} // namespace hub
