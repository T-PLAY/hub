#pragma once

#include <vector>
#include <iostream>

template <typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& vector ) {
    os << "[";
    for ( int i = 0; i < vector.size(); ++i ) {
        os << std::to_string( vector.at( i ) );
        if ( i != vector.size() - 1 ) os << " ";
    }
    os << "](" << vector.size() << ")";
    return os;
}

