
#pragma once

#include <algorithm>
#include <type_traits>

#include "core/Macros.hpp"
#include "core/Traits.hpp"
#include "core/Buffer.hpp"
//#include "core/Array.hpp"
//#include "core/Span.hpp"




namespace hub {

template <class T>
concept isMatrix = requires {T::Capacity; };

static_assert(! isMatrix<int>);

// class MatrixBase {
//   public:
////     virtual constexpr Size_t size() = 0;
// };

} // namespace hub
