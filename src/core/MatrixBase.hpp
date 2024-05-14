/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <algorithm>
#include <type_traits>

#include "Base.hpp"
#include "Buffer.hpp"

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
