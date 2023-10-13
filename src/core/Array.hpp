#pragma once

#include <array>
#include <iostream>

template <typename T, std::size_t LL, std::size_t RL>
constexpr std::array<T, LL + RL> join( std::array<T, LL> rhs, std::array<T, RL> lhs ) {
    std::array<T, LL + RL> ar;

    auto current = std::copy( rhs.begin(), rhs.end(), ar.begin() );
    std::copy( lhs.begin(), lhs.end(), current );

    return ar;
}

template <typename T, size_t N, typename Array = std::array<T, N>>
std::ostream& operator<<( std::ostream& os, const Array& tp ) {
    return os;
//    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
}
