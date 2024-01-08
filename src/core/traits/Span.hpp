#pragma once

//#include <array>
#include <iostream>

//#include "core/Macros.hpp"
//#include "core/Traits.hpp"
//#include "Vector.hpp"
//#include <vector>
//#include <span>


//template <class T, std::size_t N>
//// requires (! Printable<T>)
////typename std::enable_if_t<!Printable<T> || std::is_same_v<T, unsigned char>, std::ostream&>
//std::ostream & operator<<( std::ostream& os, const std::span<T, N>& span ) {
//    os << hub::to_string(span);
////    os << "[";
////    //    auto nMax
////    constexpr auto nMaxDataToShow = 40;
////    const auto iMax               = std::min( (int)span.size(), nMaxDataToShow );
////    for ( auto i = 0; i < iMax; ++i ) {
////        //    for ( int i = 0; i < span.size(); ++i ) {
////        os << std::to_string( span[ i ] );
////        //        if ( i != span.size() - 1 ) os << " ";
////        if ( i != iMax - 1 ) os << " ";
////    }
////    if ( span.size() > nMaxDataToShow ) { os << " ... " << std::to_string( span[span.size() - 1] ); }
////    os << "](" << PRETTY_BYTES(span.size()) << ")";
//    return os;
//}

namespace std {

#if CPP_VERSION >= 20
template <typename T, size_t Extent, typename R>
bool operator==(span<T, Extent> lhs,
                 R const& rhs) {
    // we don't have to bring in namespace std here since
    // we're already in namespace std. But qualified call
    // to std::equal to avoid ADL shenanigans
    return std::equal(begin(lhs), end(lhs),
                       begin(rhs), end(rhs));
}
#endif

//template <typename T, size_t Extent, typename R>
//bool operator==(R const& lhs,
//                 span<T, Extent> rhs) {
//    return rhs == lhs;
//}

}

//// template <typename T = std::string>
//template <typename T>
//// typename std::enable_if_t<std::is_same_v<T, std::string>, std::ostream&>
//// requires (Printable<T>)
//typename std::enable_if_t<Printable<T> && !std::is_same_v<T, unsigned char>, std::ostream&>
//operator<<( std::ostream& os, const std::vector<T>& vector ) {
//    os << "[";
//    //    auto nMax
//    constexpr auto nMaxDataToShow = 40;
//    const auto iMax               = std::min( (int)vector.size(), nMaxDataToShow );
//    for ( auto i = 0; i < iMax; ++i ) {
//        //    for ( int i = 0; i < vector.size(); ++i ) {
//        os << vector.at( i );
//        //        if ( i != vector.size() - 1 ) os << " ";
//        if ( i != iMax - 1 ) os << " ";
//    }
//    if ( vector.size() > nMaxDataToShow ) { os << " ... " << vector[iMax - 1]; }
//    os << "](" << vector.size() << ")";
//    return os;
//}

//template <typename T, std::size_t LL, std::size_t RL>
//constexpr std::array<T, LL + RL> join( std::array<T, LL> rhs, std::array<T, RL> lhs ) {
//    std::array<T, LL + RL> ar;

//    auto current = std::copy( rhs.begin(), rhs.end(), ar.begin() );
//    std::copy( lhs.begin(), lhs.end(), current );

//    return ar;
//}

////template <class T, std::size_t N, typename Array = std::array<T, N>>
//template <class Type, std::size_t Size>
////template <class T>
////std::ostream& operator<<( std::ostream& os, Array tp ) {
//std::ostream& operator<<( std::ostream& os, const std::span<Type, Size>& span ) {
//    std::vector<Type> vector{span.begin(), span.end()};
////    os << vector;
//    ::operator<<(os, vector);
//    return os;
////    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
//}

//#if CPP_VERSION >= 20 // concept
//template <class T>
//concept Printable = requires( T a ) { std::cout << a; };
//#else
//template <class T>
//static constexpr auto Printable = true;
//#endif
