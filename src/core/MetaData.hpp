/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/14
	
#pragma once

#include "Any.hpp"

namespace hub {

using MetaData = std::map<std::string, Any>; // any -> C++17

// static std::string to_string( const std::pair<std::string, Any>& metaData ) {
//     const auto& name = metaData.first;
//     const auto& val  = metaData.second;
//     std::string str  = name + " : " + val.toString();
//     return str;
// }

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
