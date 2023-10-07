#pragma once

#include <array>

template <typename T, std::size_t LL, std::size_t RL>
constexpr std::array<T, LL + RL> join( std::array<T, LL> rhs, std::array<T, RL> lhs ) {
    std::array<T, LL + RL> ar;

    auto current = std::copy( rhs.begin(), rhs.end(), ar.begin() );
    std::copy( lhs.begin(), lhs.end(), current );

    return ar;
}
