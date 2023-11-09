#pragma once

#include <array>
#include <iostream>
//#include "Macros.hpp"
//#include "Traits.hpp"
#include <vector>

template <typename T, std::size_t LL, std::size_t RL>
constexpr std::array<T, LL + RL> join( std::array<T, LL> rhs, std::array<T, RL> lhs ) {
    std::array<T, LL + RL> ar;

    auto current = std::copy( rhs.begin(), rhs.end(), ar.begin() );
    std::copy( lhs.begin(), lhs.end(), current );

    return ar;
}

//template <class T, std::size_t N, typename Array = std::array<T, N>>
template <class T, class N, N n>
//template <class T>
//std::ostream& operator<<( std::ostream& os, Array tp ) {
std::ostream& operator<<( std::ostream& os, const std::array<T, n>& tp ) {
    std::vector<T> vector{tp.begin(), tp.end()};
    os << vector;
    return os;
//    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
}
