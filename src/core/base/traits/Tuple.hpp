/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09

#pragma once

#include "Vector.hpp"

#include <iostream>
#include <tuple>

template <typename TupleT, std::size_t... Is>
std::ostream& printTupleImp( std::ostream& os, const TupleT& tp, std::index_sequence<Is...> ) {
    size_t index   = 0;
    auto printElem = [&index, &os]( const auto& x ) {
        if ( index++ > 0 ) os << ", ";
        os << TYPE_NAME( x ) << ":" << x;
    };

    os << "(";
    ( printElem( std::get<Is>( tp ) ), ... );
    os << ")";
    return os;
}
