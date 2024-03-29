/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/09

#pragma once

#include <iostream>

namespace std {

#if CPP_VERSION >= 20
template <typename T, size_t Extent, typename R>
bool operator==( span<T, Extent> lhs, R const& rhs ) {
    return std::equal( begin( lhs ), end( lhs ), begin( rhs ), end( rhs ) );
}
#endif

} // namespace std
