
#pragma once

#include "matrix/MatrixTs.hpp"
#include "matrix/MatrixXD.hpp"

namespace hub {

enum class MatrixOptions {
    RowMajor = 0,
    ColumnMajor = 0x1,
    Dynamic = 0x2,
};

template <class... Types>
    requires( sizeof...( Types ) > 1 )
static inline constexpr auto Matrix() {
    return MatrixTs<Types...>();
}

/////////////////////////////////////////////////////////

template <class Type, Size_t N>
static inline constexpr auto Matrix() {
    return Vector<Type, N>();
}
template <class Type, Size_t N, Size_t N2>
static inline constexpr auto Matrix() {
    return Image<Type, N, N2>();
}
template <class Type, Size_t N, Size_t N2, Size_t N3>
static inline constexpr auto Matrix() {
    return Volume<Type, N, N2, N3>();
}
template <class Type, Size_t... Ns>
    requires( sizeof...( Ns ) > 3 )
static inline constexpr auto Matrix() {
    return MatrixXD<Type, Ns...>();
}

/////////////////////////////////////////////////////////

template <Size_t N, class Type>
static inline constexpr auto Matrix() {
    return Vector<Type, N>();
}
template <Size_t N, class... Types>
    requires( sizeof...( Types ) > 1 )
static inline constexpr auto Matrix() {
    return Vector<MatrixTs<Types...>, N>();
}

template <Size_t N, Size_t N2, class Type>
static inline constexpr auto Matrix() {
    return Image<Type, N, N2>();
}
template <Size_t N, Size_t N2, class... Types>
    requires( sizeof...( Types ) > 1 )
static inline constexpr auto Matrix() {
    return Image<MatrixTs<Types...>, N, N2>();
}

template <Size_t N, Size_t N2, Size_t N3, class Type>
static inline constexpr auto Matrix() {
    return Volume<Type, N, N2, N3>();
}
template <Size_t N, Size_t N2, Size_t N3, class... Types>
    requires( sizeof...( Types ) > 1 )
static inline constexpr auto Matrix() {
    return Volume<MatrixTs<Types...>, N, N2, N3>();
}

template <Size_t N, Size_t N2, Size_t N3, Size_t N4, class Type>
static inline constexpr auto Matrix() {
    return MatrixXD<Type, N, N2, N3, N4>();
}
template <Size_t N, Size_t N2, Size_t N3, Size_t N4, class... Types>
    requires( sizeof...( Types ) > 1 )
static inline constexpr auto Matrix() {
    return MatrixXD<MatrixTs<Types...>, N, N2, N3, N4>();
}

template <Size_t N, Size_t N2, Size_t N3, Size_t N4, Size_t N5, class Type>
static inline constexpr auto Matrix() {
    return MatrixXD<Type, N, N2, N3, N4, N5>();
}
template <Size_t N, Size_t N2, Size_t N3, Size_t N4, Size_t N5, class... Types>
    requires( sizeof...( Types ) > 1 )
static inline constexpr auto Matrix() {
    return MatrixXD<MatrixTs<Types...>, N, N2, N3, N4, N5>();
}

} // namespace hub
