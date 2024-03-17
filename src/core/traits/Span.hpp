/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/09
 */

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
