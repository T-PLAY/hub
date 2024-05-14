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

#include "MatrixTs.hpp"
#include "MatrixXD.hpp"

namespace hub {

#if CPP_VERSION >= 20

template <class... Types>
requires( sizeof...( Types ) > 1 ) static inline constexpr auto MatrixT() {
    return MatrixTs<Types...>();
}

/////////////////////////////////////////////////////////

template <class Type, Size_t N>
static inline constexpr auto MatrixT() {
    return Vector<Type, N>();
}
template <class Type, Size_t N, Size_t N2>
static inline constexpr auto MatrixT() {
    return Image<Type, N, N2>();
}
template <class Type, Size_t N, Size_t N2, Size_t N3>
static inline constexpr auto MatrixT() {
    return Volume<Type, N, N2, N3>();
}
template <class Type, Size_t... Ns>
requires( sizeof...( Ns ) > 3 ) static inline constexpr auto MatrixT() {
    return MatrixXD<Type, Ns...>();
}

/////////////////////////////////////////////////////////

template <Size_t N, class Type>
static inline constexpr auto MatrixT() {
    return Vector<Type, N>();
}
template <Size_t N, class... Types>
requires( sizeof...( Types ) > 1 ) static inline constexpr auto MatrixT() {
    return Vector<MatrixTs<Types...>, N>();
}

template <Size_t N, Size_t N2, class Type>
static inline constexpr auto MatrixT() {
    return Image<Type, N, N2>();
}
template <Size_t N, Size_t N2, class... Types>
requires( sizeof...( Types ) > 1 ) static inline constexpr auto MatrixT() {
    return Image<MatrixTs<Types...>, N, N2>();
}

template <Size_t N, Size_t N2, Size_t N3, class Type>
static inline constexpr auto MatrixT() {
    return Volume<Type, N, N2, N3>();
}
template <Size_t N, Size_t N2, Size_t N3, class... Types>
requires( sizeof...( Types ) > 1 ) static inline constexpr auto MatrixT() {
    return Volume<MatrixTs<Types...>, N, N2, N3>();
}

template <Size_t N, Size_t N2, Size_t N3, Size_t N4, class Type>
static inline constexpr auto MatrixT() {
    return MatrixXD<Type, N, N2, N3, N4>();
}
template <Size_t N, Size_t N2, Size_t N3, Size_t N4, class... Types>
requires( sizeof...( Types ) > 1 ) static inline constexpr auto MatrixT() {
    return MatrixXD<MatrixTs<Types...>, N, N2, N3, N4>();
}

template <Size_t N, Size_t N2, Size_t N3, Size_t N4, Size_t N5, class Type>
static inline constexpr auto MatrixT() {
    return MatrixXD<Type, N, N2, N3, N4, N5>();
}
template <Size_t N, Size_t N2, Size_t N3, Size_t N4, Size_t N5, class... Types>
requires( sizeof...( Types ) > 1 ) static inline constexpr auto MatrixT() {
    return MatrixXD<MatrixTs<Types...>, N, N2, N3, N4, N5>();
}

#endif

} // namespace hub
