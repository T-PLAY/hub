
//#pragma once

#include "Measure.hpp"

//#include "core/Vector.hpp"
#include <string>


namespace hub {
namespace sensor {

const Resolution & Measure::getResolution() const
{
    return m_resolution;
}

//template <Format format, Size_t... Dims>
std::ostream& operator<<( std::ostream& os, const Measure& measure ) {

//    os << measure.name();
    os << measure.getResolution();
    os << " = ";

    const auto* data = measure.m_data;
    if ( data == nullptr ) {
        os << "nil";
    }
    else {
        os << "[";
        assert( data != nullptr );

        constexpr Size_t nMaxDataToShow = 10;
        const auto iMax              = std::min( measure.nByte(), nMaxDataToShow );
        for ( auto i = 0; i < iMax; ++i ) {
            //        os << std::setw( 3 ) << (int)array[i] << " ";
            os << std::to_string(data[i]);
            if ( i != iMax - 1 ) os << " ";
        }
        if ( measure.nByte() > nMaxDataToShow ) { os << " ... " << std::to_string(data[iMax - 1]); }
        os << "]";
    }

    return os;
}

////////////////////////////////////////////////////////////////////////////////


//template <typename T>
//std::ostream& operator<<( std::ostream& os, const std::vector<T>& vector ) {
//    os << "(";
//    for ( int i = 0; i < vector.size(); ++i ) {
//        os << vector.at( i );
//        if ( i != vector.size() - 1 ) os << ", ";
//    }
//    os << ")";
//    return os;
//}

std::ostream& operator<<( std::ostream& os, const Measures& measures ) {
//    os << measures.m_measures;
    os << "{";
    for (int i = 0; i < measures.size(); ++i) {
        os << measures.at(i);
        if ( i != measures.size() - 1 ) os << ", ";
    }
    os << "}";
    return os;
}


} // namespace sensor
} // namespace hub
