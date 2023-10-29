#pragma once

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& vector ) {
    os << "[";
    //    auto nMax
    constexpr auto nMaxDataToShow = 40;
    const auto iMax               = std::min( (int)vector.size(), nMaxDataToShow );
    for ( auto i = 0; i < iMax; ++i ) {
        //    for ( int i = 0; i < vector.size(); ++i ) {
        os << std::to_string( vector.at( i ) );
        //        if ( i != vector.size() - 1 ) os << " ";
        if ( i != iMax - 1 ) os << " ";
    }
    if ( vector.size() > nMaxDataToShow ) { os << " ... " << std::to_string( vector[iMax - 1] ); }
    os << "](" << vector.size() << ")";
    return os;
}

template <typename T = std::string>
std::ostream& operator<<( std::ostream& os, const std::vector<std::string>& vector ) {
    os << "[";
    //    auto nMax
    constexpr auto nMaxDataToShow = 40;
    const auto iMax               = std::min( (int)vector.size(), nMaxDataToShow );
    for ( auto i = 0; i < iMax; ++i ) {
        //    for ( int i = 0; i < vector.size(); ++i ) {
        os << vector.at( i );
        //        if ( i != vector.size() - 1 ) os << " ";
        if ( i != iMax - 1 ) os << " ";
    }
    if ( vector.size() > nMaxDataToShow ) { os << " ... " << vector[iMax - 1]; }
    os << "](" << vector.size() << ")";
    return os;
}
