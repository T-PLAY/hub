
#pragma once

#include <algorithm>
#include <type_traits>

#include "core/Buffer.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"
// #include "core/Array.hpp"
// #include "core/Span.hpp"

namespace hub {

template <class T>
// concept isMatrix = requires { T::Capacity; };
concept isMatrix = requires { T::matrix; };

template <class... Ts>
concept areMatrices = requires { requires (isMatrix<Ts> && ... ); };
// constexpr bool areMatrices = {isMatrix<Ts>} && ... ;
// concept areMatrices = std::all_of<isMatrix,

static_assert( !isMatrix<int> );

// class MatrixBase {
//   public:
////     virtual constexpr Size_t size() = 0;
// };

} // namespace hub
