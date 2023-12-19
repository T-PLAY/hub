#pragma once

// #include "book/BookImpl.hpp"
#include "book/BookZppBits.hpp"

namespace hub {
namespace io {

//template <Size_t NumberOfCharacter>
//using Book = book::BookZppBits<>;

//template <Size_t NumberOfCharacter = 100>
//using Book = book::BookImpl<NumberOfCharacter>;
// #ifdef HUB_USE_ZPP_BITS
using Book = book::BookZppBits<>;
// #else
// using Book = book::BookImpl<>;
// #endif

} // namespace io
} // namespace hub
