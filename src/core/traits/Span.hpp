/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

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
