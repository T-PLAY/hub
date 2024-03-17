/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/12/14
 */

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
