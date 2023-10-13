#pragma once

#include <cassert>
#include <iostream>
#include <list>
// #include <cstdlib>
// #include <string>
#include <cstring>
#include <memory>

#include "core/Macros.hpp"
// #include "data/Measure.hpp"

// #include "data/Measure.hpp"
//  user friendly useless includes
// #include "data/Dof6.hpp"
// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"
// #include "data/UserData.hpp"

#include "Measure.hpp"
// #include "core/Tuple.hpp"

namespace hub {
namespace sensor {

//class Acquisition
//{
//  public:
//    // io
//    constexpr Size_t ioGetSize() const { return 2 * sizeof( long long ) + m_measures.nByte(); }
//    const Data_t* ioGetData() const {
//        assert( m_data != nullptr );
//        return m_data;
//    }
//    void ioSetData( const Data_t* data, Size_t size ) {
//        memcpy( (Data_t*)m_data, data, size );
//    }
//    static constexpr std::string ioTypeName() { return "Acquisition"; }
//    // end io

//    template <class Measures>
//    Acquisition( Measures&& measures ) :
//        m_measures( std::forward<Measures>( measures ) ),
//        m_dataContainer( ioGetSize() ),
//        m_data { m_dataContainer.data() },
//        m_start( &( (long long*)m_data )[0] ),
//        m_end( &( (long long*)m_data )[1] ) {

//        std::cout << "[AcquisitionT] AcquisitionT()" << std::endl;

//        int iData = 0;

////        for ( auto& measure : m_measures ) {
//        for (int i = 0; i < m_measures.nMeasure(); ++i) {
//            auto & measure = m_measures.at(i);
//            measure.setData( &m_data[2 * sizeof( long long ) + iData] );
//            iData += measure.nByte();
//        }

//#ifdef DEBUG
////        for (int i = 0; i <getSize(); ++i) {
////            assert(m_data[i] == 0);
////        }
//#endif
//    };

//    constexpr long long getStart() const {
//        assert( m_start != nullptr );
//        return *m_start;
//    }
//    void setStart( long long start ) {
//        assert( m_start != nullptr );
//        *m_start = start;
//    }
//    constexpr long long getEnd() const {
//        assert( m_end != nullptr );
//        return *m_end;
//    }
//    void setEnd( long long end ) {
//        assert( m_end != nullptr );
//        *m_end = end;
//    }

//    constexpr Measures& getMeasures() { return m_measures; }

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );

//    Measures m_measures;
//    std::vector<Data_t> m_dataContainer;
//    Data_t* m_data   = nullptr;
//    long long* m_start = nullptr;
//    long long* m_end   = nullptr;
//};

///////////////////////////////// TEMPLATE ///////////////////////////////////////////////////

template <typename Measures>
class AcquisitionT
{
  public:
    // io
    static constexpr Size_t ioGetSize() { return 2 * sizeof( long long ) + Measures::nByte(); }
    const Data_t* ioGetData() const {
        assert( m_data != nullptr );
        return m_data;
    }
    void ioSetData( const Data_t* data, Size_t size ) {
        memcpy( (Data_t*)m_data, data, size );
    }
    static constexpr std::string ioTypeName() { return "AcquisitionT"; }
    // end io

    using Array = std::array<Data_t, ioGetSize()>;

    AcquisitionT() :
        m_dataContainer { 0 },
        m_data { m_dataContainer.data() },
        m_start( &( (long long*)m_data )[0] ),
        m_end( &( (long long*)m_data )[1] ) {

        std::cout << "[AcquisitionT] AcquisitionT()" << std::endl;

        int iData = 0;
        m_measures.for_each( [&]( auto& measure ) {
            measure.setData( &m_data[2 * sizeof( long long ) + iData] );
            iData += measure.nByte();
        } );

#ifdef DEBUG
        for ( int i = 0; i < ioGetSize(); ++i ) {
            //            m_array.at(i) == 0;
            assert( m_data[i] == 0 );
        }
#endif
    };

    constexpr bool operator==( const AcquisitionT& acq ) const {
        static_assert( ioGetSize() == acq.ioGetSize() );
        return !std::memcmp( ioGetData(), acq.ioGetData(), ioGetSize() );
    }

    constexpr Measures& getMeasures() { return m_measures; }

    constexpr long long getStart() const {
        assert( m_start != nullptr );
        return *m_start;
    }
    void setStart( long long start ) {
        assert( m_start != nullptr );
        *m_start = start;
    }
    constexpr long long getEnd() const {
        assert( m_end != nullptr );
        return *m_end;
    }
    void setEnd( long long end ) {
        assert( m_end != nullptr );
        *m_end = end;
    }

    template <typename MeasuresT>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const AcquisitionT<MeasuresT>& acq );

    //  private:
    Measures m_measures;
    Array m_dataContainer;
    Data_t* m_data   = nullptr;
    long long* m_start = nullptr;
    long long* m_end   = nullptr;
    //    Data_t * m_data;
    //    std::array<Data_t, getSize()> m_array;
};

// #include "core/Array.hpp"

// template <typename T, size_t N, typename Array = std::array<T, N>>
// std::ostream& operator<<( std::ostream& os, const Array& tp ) {
//     return os;
////    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
//}

template <typename MeasuresT>
std::ostream& operator<<( std::ostream& os, const AcquisitionT<MeasuresT>& acq ) {
    os << "(start:" << ( acq.getStart() / 1'000'000 ) % 1'000 << " "
       << ( acq.getStart() / 1'000 ) % 1'000 << " " << acq.getStart() % 1'000
       << ", end:" << ( acq.getEnd() / 1'000'000 ) % 1'000 << " "
       << ( acq.getEnd() / 1'000 ) % 1'000 << " " << acq.getEnd() % 1'000;
    os << ", measures:";
    const auto& measures = acq.m_measures;
    os << measures;
    os << ", data = ";
    os << "[";

    constexpr Size_t nMaxDataToShow = 30;
    const auto iMax                   = std::min( acq.ioGetSize(), nMaxDataToShow );
    for ( auto i = 0; i < iMax; ++i ) {
        //        os << std::setw( 3 ) << (int)array[i] << " ";
        os << std::to_string( acq.m_dataContainer.at( i ) );
        if ( i != iMax - 1 ) os << " ";
    }
    if ( acq.ioGetSize() > nMaxDataToShow ) {
        os << " ... " << std::to_string( acq.m_dataContainer.at( iMax - 1 ) );
    }
    os << "](" << acq.ioGetSize() << "))";

    return os;
}

// template <typename MeasuresT>
// std::ostream& operator<<( std::ostream& os, const AcquisitionT<MeasuresT>& acq ) {
//     os << "(start:" << ( acq.getStart() / 1'000'000 ) % 1'000 << " "
//        << ( acq.getStart() / 1'000 ) % 1'000 << " " << acq.getStart() % 1'000
//        << ", end:" << ( acq.getEnd() / 1'000'000 ) % 1'000 << " "
//        << ( acq.getEnd() / 1'000 ) % 1'000 << " " << acq.getEnd() % 1'000;
//     os << ", measures:";
//     const auto& measures = acq.m_measures;
//     os << measures;
//     os << ", data = ";
//     //    for ( int i = 0; i < acq.m_measures.size(); ++i ) {
//     //        const auto& measure = acq.m_measures.at( i );
//     //        acq.m_measures.get<0>( );
//     //        std::get<i>(acq.m_measures);

//    os << "[";
//    //        assert( data != nullptr );

//    constexpr Size_t nMaxDataToShow = 30;
//    const auto iMax                   = std::min( (Size_t)acq.m_array.size(), nMaxDataToShow );
//    for ( auto i = 0; i < iMax; ++i ) {
//        //        os << std::setw( 3 ) << (int)array[i] << " ";
//        os << std::to_string( acq.m_array[i] );
//        if ( i != iMax - 1 ) os << " ";
//    }
//    if ( acq.m_array.size() > nMaxDataToShow ) {
//        os << " ... " << std::to_string( acq.m_array[iMax - 1] );
//    }
//    os << "](" << acq.m_array.size() << "))";

//    //        acq.m_measures.
//    //        os << "[" << measure << "]";
//    //        if ( i != acq.m_measures.size() - 1 ) { os << ", "; }
//    //    }
//    //    os << ", " << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps.";
//    return os;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// template <typename AcquisitionT, int Size>
// class Acquisitions
//{
//   public:
//     using Array = std::array<AcquisitionT, Size>;
//     template <typename... T>
//     constexpr Acquisitions( T&&... t ) :
//         m_acqs { std::forward<T>( t )... } //        ,m_array{std::forward<T>(t)}
//     {
//         //        std::get<0>(m_measures).m_data = m_array.data();
//     }
//     //    template <typename T>
//     //    constexpr Acquisitions( T&& t ) :
//     //        m_array { std::forward<T>( t ) } //        ,m_array{std::forward<T>(t)}
//     //    {
//     //        //        std::get<0>(m_measures).m_data = m_array.data();
//     //    }

//    //    auto operator() {
//    //        return m_array;
//    //    }
//    constexpr auto at( int idx ) const { return m_acqs.at( idx ); }

//    constexpr auto size() const {
//        //        static_assert(Size == 3);
//        return Size;
//        //        return m_acqs.size();
//    }

//    Array m_acqs;
//};

/////
///// \brief The Acquisition class
///// represents the data with the start and the end of the acquire process.
/////
// class SRC_API Acquisition
//{
//   public:
//     Acquisition() = default;

//    ///
//    /// \brief Acquisition
//    /// \param start
//    /// timestamp in microseconds
//    /// \param end
//    /// timestamp in microseconds
//    ///
//    Acquisition( long long start, long long end );

//    Acquisition( const Acquisition& acq ) = delete;
//    Acquisition& operator=( const Acquisition& acq ) = delete;

//    ///
//    /// \param acq
//    /// movable acquisition.
//    ///
//    Acquisition( Acquisition&& acq ) = default;

//    ///
//    /// \brief operator =
//    /// \param acq
//    /// \return
//    ///
//    Acquisition& operator=( Acquisition&& acq );

//    ~Acquisition();

//  public:
//    ///
//    /// \brief operator ==
//    /// \param acq
//    /// \return
//    ///
//    bool operator==( const Acquisition& acq ) const;

//    ///
//    /// \brief operator !=
//    /// \param acq
//    /// \return
//    ///
//    bool operator!=( const Acquisition& acq ) const;

//    ///
//    /// \brief operator <<
//    /// add new measure to current acquisition
//    /// \param measure
//    /// [in] measure to add
//    /// \return
//    /// modified acquisition with new measure.
//    ///
//    Acquisition& operator<<( Measure&& measure );

//    ///
//    /// \brief operator <<
//    /// add new measure to current acquisition
//    /// \param measure
//    /// [in] measure to add
//    /// \return
//    /// modified acquisition with new measure.
//    ///
//    Acquisition& operator<<( const Measures& measure );

//    ///
//    /// \brief operator >>
//    /// \param acq
//    ///
//    void operator>>( Acquisition& acq );

//    ///
//    /// \brief emplaceMeasure
//    /// \param args
//    ///
//    template <class... Args>
//    void emplaceMeasure( Args&&... args );

//    ///
//    /// \brief pushBack
//    /// \param measure
//    ///
//    void pushBack( Measure&& measure );

//    ///
//    /// \brief isInterpolable
//    /// is used during synchronization with another acquisition stream to minimize
//    /// the error distance between 2 differents acquisition of differents sensor stream.
//    /// \return
//    /// true if acquisition is interpolable
//    /// false otherwise
//    ///
//    bool isInterpolable() const;

//    ///
//    /// \brief slerp
//    /// is used during synchronization with another acquisition stream to minimize
//    /// the error distance between 2 differents acquisition of differents sensor stream.
//    /// \param left
//    /// oldest acquisition
//    /// \param right
//    /// newest acquisition
//    /// \param t
//    /// to perform interpolation
//    /// \return
//    /// interpolate new created acquisition.
//    ///
//    static Acquisition slerp( const Acquisition& left, const Acquisition& right, double t );

//    ///
//    /// \brief clone
//    /// make a copy of current acquisition.
//    /// \return
//    /// copy of acquisition.
//    ///
//    Acquisition clone() const;

//    ///
//    /// \brief hasFixedSize
//    /// \return
//    /// true if the acquisition size is constant,
//    /// false otherwise.
//    ///
//    bool hasFixedSize() const;

//    ///
//    /// \brief operator <
//    /// \param right
//    /// \return
//    ///
//    bool operator<( const Acquisition& right ) const;

//    ///
//    /// \brief operator <<
//    /// print acquisition information.
//    /// \param os
//    /// output to print.
//    /// \param acq
//    /// acquisition to print.
//    /// \return
//    /// std printed ostream.
//    ///
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );

//  public: // getters
//    ///
//    /// \brief getSize
//    /// is used to serialize the acquisition into a constant buffer array (TCP communication).
//    /// \return
//    /// size of acquisition in bytes.
//    ///
//    size_t getSize() const;

//    ///
//    /// \brief getMeasures
//    /// is used to process things for each measure separately.
//    /// \return
//    /// all measures acquired during the acquisition.
//    ///
//    const Measures& getMeasures() const;

//    ///
//    /// \brief getStart
//    /// \return
//    ///
//    long long getStart() const;

//    ///
//    /// \brief getEnd
//    /// \return
//    ///
//    long long getEnd() const;

//    ///
//    /// \brief isEmpty
//    /// \return
//    ///
//    bool isEmpty() const;

//  private:
//    long long m_start = 0; // microseconds
//    long long m_end   = 0; // microseconds
//    Measures m_measures;
//    uint64_t m_size = 0;

//    friend class hub::Input;
//    friend class hub::Output;
//};

/////////////////////////////////////////////////// INLINE
///////////////////////////////////////////////////////////////////////////

// inline long long Acquisition::getStart() const {
//     return m_start;
// }

// inline long long Acquisition::getEnd() const {
//     return m_end;
// }

// inline bool Acquisition::isEmpty() const {
//     return m_measures.empty();
// }

// template <class... Args>
// void Acquisition::emplaceMeasure( Args&&... args ) {
// #if ( __cplusplus >= 201703L )
//     static_assert( 3 <= sizeof...( args ) && sizeof...( args ) <= 4 );
// #endif
//     m_measures.emplace_back( std::forward<Args>( args )... );
//     m_size += m_measures.back().getSize();
// }

// inline bool Acquisition::operator<( const Acquisition& right ) const {
//     if (m_start == right.m_start)
//         return m_end < right.m_end;
//     return m_start < right.m_start;
// }

} // namespace sensor
} // namespace hub
