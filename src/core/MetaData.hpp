/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*******************************************************************************/

#pragma once

#include "Any.hpp"

namespace hub {

///
/// \brief MetaData
///
using MetaData = std::map<std::string, Any>; // any -> C++17

static std::string to_string( const MetaData& metaData, bool expand ) {
    std::string str = "";
    if ( expand ) {
        bool first = true;
        for ( const auto& pair : metaData ) {
            if ( first )
                first = false;
            else
                str += "\n";
            str += hub::to_string( pair );
        }
    }
    else {
        str += "[";
        int i = 0;
        for ( const auto& pair : metaData ) {
            str += hub::to_string( pair );
            if ( i != metaData.size() - 1 ) str += ", ";
            ++i;
        }
        str += "]";
    }
    return str;
}

} // namespace hub
