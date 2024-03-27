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

#include "core/Traits.hpp"
#include <array>
#include <iostream>

template <typename T, std::size_t LL, std::size_t RL>
constexpr std::array<T, LL + RL> join( std::array<T, LL> rhs, std::array<T, RL> lhs ) {
    std::array<T, LL + RL> ar;

    auto current = std::copy( rhs.begin(), rhs.end(), ar.begin() );
    std::copy( lhs.begin(), lhs.end(), current );

    return ar;
}
