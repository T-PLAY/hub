#pragma once

#include "book/BookImpl.hpp"
#include "book/BookZppBits.hpp"

namespace hub {
namespace io {

//template <Size_t NumberOfCharacter>
//using Book = book::BookZppBits<>;

//template <Size_t NumberOfCharacter = 100>
//using Book = book::BookImpl<NumberOfCharacter>;
using Book = book::BookZppBits<>;

} // namespace io
} // namespace hub
