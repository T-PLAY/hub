/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/28
	
#pragma once

#include <chrono>

#include "core/Matrix.hpp"
#include "core/matrix/MatrixTs.hpp"

#include "core/Format.hpp"
#include "core/Resolution.hpp"

// #include "Format.hpp"

namespace hub {
namespace sensor {

using Clock = long long;
static_assert( sizeof( Clock ) == 8 );

static inline Clock getClock() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch() )
        .count();
}

class SRC_API Acquisition : public Matrix
{
  public:
    static struct {
    } packable;

    Acquisition() = default;

    Acquisition( Acquisition&& )      = default;
    Acquisition( const Acquisition& ) = delete;

    Acquisition& operator=( Acquisition&& )      = default;
    Acquisition& operator=( const Acquisition& ) = delete;

    Acquisition clone() const {
        Acquisition acq;
        acq |= *this;
        return acq;
    }

    const Matrix& getResolution() const {
        if ( m_resolution.size() == 0 ) {
            for ( size_t i = 2; i < nType(); ++i ) {
                m_resolution.push_back( m_nodes.at( i ) );
            }
        }
        return m_resolution;
    }
    Clock& start() {
        auto offset = getOffset<Clock>(0);
        auto offset2 = getOffset<Clock>(1);
        assert(getOffset<Clock>(0) == 0);
        return get<Clock&>();
    }
    Clock& end() {
        assert(getOffset<Clock>(1) == sizeof(Clock));
        return get<Clock&, 1>();
    }
    auto clocks() { return std::tuple<Clock&, Clock&> { start(), end() }; }

    Clock getStart() const { return *get<const Clock*>(); }
    Clock getEnd() const { return *get<const Clock*, 1>(); }

    Acquisition& operator<<( const Acquisition& other ) {
        size_t sizeBeforeAdd = m_size;
        for ( size_t i = 2; i < other.nType(); ++i ) {
            push_back( other.m_nodes.at( i ) );
        }
        if ( !other.m_vector.empty() ) {
            m_vector.resize( sizeBeforeAdd + other.m_vector.size() - 2 * sizeof( Clock ) );
            std::copy( other.m_vector.begin() + 2 * sizeof( Clock ),
                       other.m_vector.end(),
                       m_vector.begin() + sizeBeforeAdd );
        }
        return *this;
    }

    Acquisition operator<<( const Acquisition& other ) const {
        Acquisition matrix = this->clone();
        matrix << other;
        return matrix;
    }

    bool operator<( const Acquisition& right ) const {
        if ( getStart() == right.getStart() ) return getEnd() < right.getEnd();
        return getStart() < right.getStart();
    }

  private:
    mutable Matrix m_resolution;
};

SRC_API Acquisition make_acquisition( const Matrix& resolution );

///////////////////////////////////// TEMPLATE ////////////////////////////////

template <class Resolution>
class AcquisitionT : public MatrixTs<Clock, Clock, Resolution>
{
  public:
    using Matrix = MatrixTs<Clock, Clock, Resolution>;
    using Matrix::get;

    Clock& start() { return Matrix::template get<Clock&>(); }
    Clock getStart() const { return Matrix::template get<Clock>(); }

    Clock& end() { return Matrix::template get<Clock&, 1>(); }
    Clock getEnd() const { return Matrix::template get<Clock, 1>(); }

    auto clocks() { return std::tuple<Clock&, Clock&> { start(), end() }; }

    // template <class Type>
    // Type get() {
    //     return Matrix::template get<Type>();
    // }
};

//// #include <iostream>
//// #include <list>
////// #include <cstdlib>
////// #include <string>
//// #include <cstring>
//// #include <memory>
//// #include <vector>
// #include <array>

////#include "core/traits/Array.hpp"
// #include "core/Macros.hpp"
////// #include "data/Measure.hpp"

//// #include "data/Measure.hpp"
////// #include "data/Measure.hpp"
//////  user friendly useless includes
////// #include "data/Dof6.hpp"
////// #include "data/Mat4.hpp"
////// #include "data/Mesh.hpp"
////// #include "data/UserData.hpp"

// #include "Measure.hpp"
////// #include "core/Tuple.hpp"
/////
///**
// *        +---------------+
// *        |  Acquisition  |
// *        |---------------|
// *        | Clock : start |  ->  Fixed size packet
// *        | Clock : end   |
// *        | Data  : data  |
// *        +---------------+
// */

//// #include "Resolution.hpp"

// namespace hub {
// namespace sensor {

//// class Acquisition
////{
////   public:
////     constexpr Size_t nByte() const {
////         Size_t dataSize = 0;
////         for ( const auto& resolution : m_resolutions ) {
////             dataSize += resolution.nByte();
////         }
////         return 2 * sizeof( long long ) + dataSize;
////     }

////    template <class Serial>
////    void serialize( Serial& serial ) {
//////        serial( a, b, name, vints );
////    }
///
//     static constexpr auto nByte() { return 2 * sizeof( Clock ) + Resolution::nByte(); }
//     using Array = std::array<Data_t, nByte()>;
//     //    using resolution = Resolution;
//     static constexpr auto resolutions = Resolution();
//     //    template <class Type>
//     //    using has<Type> = Resolution::has<Type>;
//     //    using Resolution::nResolution;
//     //    static constexpr auto nResolution() { return Resolution::nResolution; }

//    //        template <class ResolutionT>
////    Array m_dataContainer;
////    ArrayBuffer<Clock, Clock, Resolution> m_data;
////    ArrayBuffer<Clock, Clock> m_data;
////    Data_t* m_data = nullptr;
////    Clock* m_start = nullptr;
////    Clock* m_end   = nullptr;

////    template <class... Arg>
////    template <class Start, class End>
////    constexpr AcquisitionT(Clock start, Clock end) : m_data{start, end}
////    {
//////        m_dataContainer{std::forward<Array>(array)} {
////    }

////    AcquisitionT()
//////        :
//////        m_dataContainer { 0 },
//////        m_data { m_dataContainer.data() + 2 * sizeof( long long ) },
//////        m_start( &( (long long*)m_data )[0] ),
//////        m_end( &( (long long*)m_data )[1] )
////    {

////        std::cout << "[AcquisitionT] AcquisitionT()" << std::endl;
////        //            int iData = 0;
////        //        int iMeasure = 0;
////        //        for ( auto& measure : m_measures ) {
////        //            measure.iMeasure = iMeasure;
////        //            ++iMeasure;
////        //        }
////        //            m_measures.for_each( [&]( auto& measure ) {
////        //                measure.setData( &m_data[2 * sizeof( long long ) + iData] );
////        //                iData += measure.nByte();
////        //            } );

////        // #ifdef DEBUG
////        //             for ( int i = 0; i < ioGetSize(); ++i ) {
////        //                 //            m_array.at(i) == 0;
////        //                 assert( m_data[i] == 0 );
////        //             }
////        // #endif
////    };

//    //    using has = Resolution::has<Type>;

//    //    template <class Type>
//    //    constexpr bool has() const {
//    //        return Resolution::has<Type>();
//    //    }
//    //    using has = Resolution::has;

//    template <class... Type>
//    constexpr auto hasType() const {
//        return Resolution::template hasType<Type...>();
//    }

//    //    template <class Type>
//    template <Size_t i>
//    constexpr auto getMeasure() const {
//        //        static_assert(Resolution::template has<Type>());
//        //        using Res = Resolution::template get<Type>;
//        return true;
////        using Res     = Resolution::template getResolution<i>;
////        return Res();
////        auto&& buffer = Buffer<Res::nByte()>( m_data );
////        return MeasureT<Res>( std::move( buffer ) );

//        //        return MeasureT<Res>(m_data, Res::nByte());
//        //        using res = Resolution::get<Type>();
//        //        Measure measure(m_data, 10);
//        //        return measure;
//        //        return Resolution::has<Type>();
//    }

//    //    template <int id>
//    //    auto& getMeasure() {
//    //        static_assert( 0 <= id && id < Resolution::nResolution );
//    ////        return m_measures.at( id );
//    //    }
//    template <class Resolution_>
//    SRC_API friend std::ostream& operator<<( std::ostream& os,
//                                             const AcquisitionT<Resolution_>& acq );

//    constexpr Clock getStart() const {
//        return m_data.get<0>();
//        //        return *(long long*)m_dataContainer.data();
//        //        assert( m_start != nullptr );
//        //        return *m_start;
//    }
////    void setStart( long long start ) {
////        assert( m_start != nullptr );
////        *m_start = start;
////    }
//    constexpr long long end() const {
//        return m_data.get<1>();
////        assert( m_end != nullptr );
////        return *m_end;
//    }
////    void setEnd( long long end ) {
////        assert( m_end != nullptr );
////        *m_end = end;
////    }

//  private:
//    //    std::array<Measure, nResolution()> m_measures;
//    //        std::array<Measure, nResolution()> m_measures;
//    //    Measures m_measures;
//    //    Data_t * m_data;
//    //    std::array<Data_t, getSize()> m_array;
//}; // end Acquisition

// template <class Resolution>
// SRC_API std::ostream& operator<<( std::ostream& os, const AcquisitionT<Resolution>& acq ) {
//     os << Resolution() << " ";
////    ::operator<<( os, acq.m_dataContainer );
//    return os;
//}

//////    template <class ResolutionsT>
////    Acquisition( const Resolutions& resolutions ) :
//////        m_resolutions( std::forward<ResolutionsT>( resolutions ) ),
////        m_resolutions( resolutions),
////        m_size { nByte() },
////        m_dataContainer( m_size ),
////        m_data { m_dataContainer.data() },
////        m_start( &( (long long*)m_data )[0] ),
////        m_end( &( (long long*)m_data )[1] ) {
//////        std::cout << "[Acquisition] Acquisition(ResolutionT&&)" << std::endl;
////        m_measures.reserve( m_resolutions.size() );

////        Size_t iData = 2 * sizeof( long long );
////        for ( auto i = 0; i < m_resolutions.size(); ++i ) {
////            const auto& resolution = m_resolutions.at( i );

////            m_measures.emplace_back( resolution, &m_data[iData] );

////            iData += resolution.nByte();
////        }
////        assert( iData == m_size );
////    };

////    //    Acquisition( const Acquisition & acq )
////    //        : m_resolutions{acq.m_resolutions}
////    //        , m_size(acq.m_size)
////    //    {
////    //        std::cout << "[Acquisition] Acquisition(const Acquisition&)" << std::endl;
////    //    }

////    Acquisition( const Acquisition& acq ) = delete;
////    Acquisition( Acquisition && acq ) = default;
////    Acquisition& operator=( const Acquisition& acq ) = delete;
////    Acquisition& operator=( Acquisition&& acq )      = delete;

////    Acquisition clone() const {
////        Acquisition acq { m_resolutions };
////        std::memcpy(acq.m_data, m_data, m_size);
////        return acq;
////    }

////    //    };

////    constexpr long long getStart() const {
////        assert( m_start != nullptr );
////        return *m_start;
////    }
////    void setStart( long long start ) {
////        assert( m_start != nullptr );
////        *m_start = start;
////    }
////    constexpr long long getEnd() const {
////        assert( m_end != nullptr );
////        return *m_end;
////    }
////    void setEnd( long long end ) {
////        assert( m_end != nullptr );
////        *m_end = end;
////    }

////    constexpr Measures& getMeasures() { return m_measures; }
////    constexpr const Measures& getMeasures() const { return m_measures; }

////    constexpr bool operator==( const Acquisition& acq ) const {
////        return m_resolutions == acq.m_resolutions && !std::memcmp( m_data, acq.m_data,
/// m_size ); /    }

////    const Resolutions& getResolutions() const;

////    bool operator<( const Acquisition& right ) const;

//////  private:
//////    template <typename Input>
//////    void read( Input& input ) {
//////        input.read( m_data, m_size );
//////    }

//////    void serialize() {

//////    }

//////    template <typename Input>
//////    static Acquisition create(Input & input) {
//////        Resolutions resolutions;
//////        input.read(resolutions);
////////        return Acquisition{m_resolutions};
//////    }

//////    template <typename Output>
//////    void write( Output& output ) const {
//////        output.write( m_data, m_size );
//////    }

////  private:
////    const Resolutions m_resolutions;
////    const Size_t m_size;
////    std::vector<Data_t> m_dataContainer;
////    Data_t* const m_data     = nullptr;
////    long long* const m_start = nullptr;
////    long long* const m_end   = nullptr;

////    Measures m_measures;
////};

//// inline const Resolutions& Acquisition::getResolutions() const {
////     return m_resolutions;
//// }

//// inline bool Acquisition::operator<( const Acquisition& right ) const {
////     if (m_start == right.m_start)
////         return m_end < right.m_end;
////     return m_start < right.m_start;
//// }

///////////////////////////////////// TEMPLATE
//////////////////////////////////////////////////////

////// template <typename Measures>
////// class AcquisitionT
//////{
//////   public:
//////     // io
//////     static constexpr Size_t ioGetSize() { return 2 * sizeof( long long ) +
/// Measures::nByte();
///}
//////     const Data_t* ioGetData() const {
//////         assert( m_data != nullptr );
//////         return m_data;
//////     }
//////     void ioSetData( const Data_t* data, Size_t size ) { memcpy( (Data_t*)m_data, data,
/// size
///);
/////} /     static constexpr std::string ioTypeName() { return "AcquisitionT"; } /     // end
/// io

//////    using Array = std::array<Data_t, ioGetSize()>;

//////    AcquisitionT() :
//////        m_dataContainer { 0 },
//////        m_data { m_dataContainer.data() },
//////        m_start( &( (long long*)m_data )[0] ),
//////        m_end( &( (long long*)m_data )[1] ) {

//////        std::cout << "[AcquisitionT] AcquisitionT()" << std::endl;

//////        int iData = 0;
//////        m_measures.for_each( [&]( auto& measure ) {
//////            measure.setData( &m_data[2 * sizeof( long long ) + iData] );
//////            iData += measure.nByte();
//////        } );

////// #ifdef DEBUG
//////         for ( int i = 0; i < ioGetSize(); ++i ) {
//////             //            m_array.at(i) == 0;
//////             assert( m_data[i] == 0 );
//////         }
////// #endif
//////     };

//////    constexpr bool operator==( const AcquisitionT& acq ) const {
//////        static_assert( ioGetSize() == acq.ioGetSize() );
//////        return !std::memcmp( ioGetData(), acq.ioGetData(), ioGetSize() );
//////    }

//////    constexpr Measures& getMeasures() { return m_measures; }

//////    constexpr long long getStart() const {
//////        assert( m_start != nullptr );
//////        return *m_start;
//////    }
//////    void setStart( long long start ) {
//////        assert( m_start != nullptr );
//////        *m_start = start;
//////    }
//////    constexpr long long getEnd() const {
//////        assert( m_end != nullptr );
//////        return *m_end;
//////    }
//////    void setEnd( long long end ) {
//////        assert( m_end != nullptr );
//////        *m_end = end;
//////    }

//////    template <typename MeasuresT>
//////    SRC_API friend std::ostream& operator<<( std::ostream& os, const
/// AcquisitionT<MeasuresT>&
///// acq /    );

//////    //  private:
//////    Measures m_measures;
//////    Array m_dataContainer;
//////    Data_t* m_data     = nullptr;
//////    long long* m_start = nullptr;
//////    long long* m_end   = nullptr;
//////    //    Data_t * m_data;
//////    //    std::array<Data_t, getSize()> m_array;
//////};

//////// #include "core/Array.hpp"

//////// template <typename T, size_t N, typename Array = std::array<T, N>>
//////// std::ostream& operator<<( std::ostream& os, const Array& tp ) {
////////     return os;
//////////    return printTupleImp( os, tp, std::make_index_sequence<TupSize> {} );
////////}

////// template <typename MeasuresT>
////// std::ostream& operator<<( std::ostream& os, const AcquisitionT<MeasuresT>& acq ) {
//////     os << "(start:" << ( acq.getStart() / 1'000'000 ) % 1'000 << " "
//////        << ( acq.getStart() / 1'000 ) % 1'000 << " " << acq.getStart() % 1'000
//////        << ", end:" << ( acq.getEnd() / 1'000'000 ) % 1'000 << " "
//////        << ( acq.getEnd() / 1'000 ) % 1'000 << " " << acq.getEnd() % 1'000;
//////     os << ", measures:";
//////     const auto& measures = acq.m_measures;
//////     os << measures;
//////     os << ", data = ";
//////     os << "[";

//////    constexpr Size_t nMaxDataToShow = 30;
//////    const auto iMax                 = std::min( acq.ioGetSize(), nMaxDataToShow );
//////    for ( auto i = 0; i < iMax; ++i ) {
//////        //        os << std::setw( 3 ) << (int)array[i] << " ";
//////        os << std::to_string( acq.m_dataContainer.at( i ) );
//////        if ( i != iMax - 1 ) os << " ";
//////    }
//////    if ( acq.ioGetSize() > nMaxDataToShow ) {
//////        os << " ... " << std::to_string( acq.m_dataContainer.at( iMax - 1 ) );
//////    }
//////    os << "](" << acq.ioGetSize() << "))";

//////    return os;
//////}

////// template <typename MeasuresT>
////// std::ostream& operator<<( std::ostream& os, const AcquisitionT<MeasuresT>& acq ) {
//////     os << "(start:" << ( acq.getStart() / 1'000'000 ) % 1'000 << " "
//////        << ( acq.getStart() / 1'000 ) % 1'000 << " " << acq.getStart() % 1'000
//////        << ", end:" << ( acq.getEnd() / 1'000'000 ) % 1'000 << " "
//////        << ( acq.getEnd() / 1'000 ) % 1'000 << " " << acq.getEnd() % 1'000;
//////     os << ", measures:";
//////     const auto& measures = acq.m_measures;
//////     os << measures;
//////     os << ", data = ";
//////     //    for ( int i = 0; i < acq.m_measures.size(); ++i ) {
//////     //        const auto& measure = acq.m_measures.at( i );
//////     //        acq.m_measures.get<0>( );
//////     //        std::get<i>(acq.m_measures);

//////    os << "[";
//////    //        assert( data != nullptr );

//////    constexpr Size_t nMaxDataToShow = 30;
//////    const auto iMax                   = std::min( (Size_t)acq.m_array.size(),
/// nMaxDataToShow
///);
//////    for ( auto i = 0; i < iMax; ++i ) {
//////        //        os << std::setw( 3 ) << (int)array[i] << " ";
//////        os << std::to_string( acq.m_array[i] );
//////        if ( i != iMax - 1 ) os << " ";
//////    }
//////    if ( acq.m_array.size() > nMaxDataToShow ) {
//////        os << " ... " << std::to_string( acq.m_array[iMax - 1] );
//////    }
//////    os << "](" << acq.m_array.size() << "))";

//////    //        acq.m_measures.
//////    //        os << "[" << measure << "]";
//////    //        if ( i != acq.m_measures.size() - 1 ) { os << ", "; }
//////    //    }
//////    //    os << ", " << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps.";
//////    return os;
//////}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////// template <typename AcquisitionT, int Size>
////// class Acquisitions
//////{
//////   public:
//////     using Array = std::array<AcquisitionT, Size>;
//////     template <typename... T>
//////     constexpr Acquisitions( T&&... t ) :
//////         m_acqs { std::forward<T>( t )... } //        ,m_array{std::forward<T>(t)}
//////     {
//////         //        std::get<0>(m_measures).m_data = m_array.data();
//////     }
//////     //    template <typename T>
//////     //    constexpr Acquisitions( T&& t ) :
//////     //        m_array { std::forward<T>( t ) } //        ,m_array{std::forward<T>(t)}
//////     //    {
//////     //        //        std::get<0>(m_measures).m_data = m_array.data();
//////     //    }

//////    //    auto operator() {
//////    //        return m_array;
//////    //    }
//////    constexpr auto at( int idx ) const { return m_acqs.at( idx ); }

//////    constexpr auto size() const {
//////        //        static_assert(Size == 3);
//////        return Size;
//////        //        return m_acqs.size();
//////    }

//////    Array m_acqs;
//////};

/////////
///////// \brief The Acquisition class
///////// represents the data with the start and the end of the acquire process.
/////////
////// class SRC_API Acquisition
//////{
//////   public:
//////     Acquisition() = default;

//////    ///
//////    /// \brief Acquisition
//////    /// \param start
//////    /// timestamp in microseconds
//////    /// \param end
//////    /// timestamp in microseconds
//////    ///
//////    Acquisition( long long start, long long end );

//////    Acquisition( const Acquisition& acq ) = delete;
//////    Acquisition& operator=( const Acquisition& acq ) = delete;

//////    ///
//////    /// \param acq
//////    /// movable acquisition.
//////    ///
//////    Acquisition( Acquisition&& acq ) = default;

//////    ///
//////    /// \brief operator =
//////    /// \param acq
//////    /// \return
//////    ///
//////    Acquisition& operator=( Acquisition&& acq );

//////    ~Acquisition();

//////  public:
//////    ///
//////    /// \brief operator ==
//////    /// \param acq
//////    /// \return
//////    ///
//////    bool operator==( const Acquisition& acq ) const;

//////    ///
//////    /// \brief operator !=
//////    /// \param acq
//////    /// \return
//////    ///
//////    bool operator!=( const Acquisition& acq ) const;

//////    ///
//////    /// \brief operator <<
//////    /// add new measure to current acquisition
//////    /// \param measure
//////    /// [in] measure to add
//////    /// \return
//////    /// modified acquisition with new measure.
//////    ///
//////    Acquisition& operator<<( Measure&& measure );

//////    ///
//////    /// \brief operator <<
//////    /// add new measure to current acquisition
//////    /// \param measure
//////    /// [in] measure to add
//////    /// \return
//////    /// modified acquisition with new measure.
//////    ///
//////    Acquisition& operator<<( const Measures& measure );

//////    ///
//////    /// \brief operator >>
//////    /// \param acq
//////    ///
//////    void operator>>( Acquisition& acq );

//////    ///
//////    /// \brief emplaceMeasure
//////    /// \param args
//////    ///
//////    template <class... Args>
//////    void emplaceMeasure( Args&&... args );

//////    ///
//////    /// \brief pushBack
//////    /// \param measure
//////    ///
//////    void pushBack( Measure&& measure );

//////    ///
//////    /// \brief isInterpolable
//////    /// is used during synchronization with another acquisition stream to minimize
//////    /// the error distance between 2 differents acquisition of differents sensor stream.
//////    /// \return
//////    /// true if acquisition is interpolable
//////    /// false otherwise
//////    ///
//////    bool isInterpolable() const;

//////    ///
//////    /// \brief slerp
//////    /// is used during synchronization with another acquisition stream to minimize
//////    /// the error distance between 2 differents acquisition of differents sensor stream.
//////    /// \param left
//////    /// oldest acquisition
//////    /// \param right
//////    /// newest acquisition
//////    /// \param t
//////    /// to perform interpolation
//////    /// \return
//////    /// interpolate new created acquisition.
//////    ///
//////    static Acquisition slerp( const Acquisition& left, const Acquisition& right, double t
///);

//////    ///
//////    /// \brief clone
//////    /// make a copy of current acquisition.
//////    /// \return
//////    /// copy of acquisition.
//////    ///
//////    Acquisition clone() const;

//////    ///
//////    /// \brief hasFixedSize
//////    /// \return
//////    /// true if the acquisition size is constant,
//////    /// false otherwise.
//////    ///
//////    bool hasFixedSize() const;

//////    ///
//////    /// \brief operator <
//////    /// \param right
//////    /// \return
//////    ///
//////    bool operator<( const Acquisition& right ) const;

//////    ///
//////    /// \brief operator <<
//////    /// print acquisition information.
//////    /// \param os
//////    /// output to print.
//////    /// \param acq
//////    /// acquisition to print.
//////    /// \return
//////    /// std printed ostream.
//////    ///
//////    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );

//////  public: // getters
//////    ///
//////    /// \brief getSize
//////    /// is used to serialize the acquisition into a constant buffer array (TCP
/// communication).
//////    /// \return
//////    /// size of acquisition in bytes.
//////    ///
//////    size_t getSize() const;

//////    ///
//////    /// \brief getMeasures
//////    /// is used to process things for each measure separately.
//////    /// \return
//////    /// all measures acquired during the acquisition.
//////    ///
//////    const Measures& getMeasures() const;

//////    ///
//////    /// \brief getStart
//////    /// \return
//////    ///
//////    long long getStart() const;

//////    ///
//////    /// \brief getEnd
//////    /// \return
//////    ///
//////    long long getEnd() const;

//////    ///
//////    /// \brief isEmpty
//////    /// \return
//////    ///
//////    bool isEnd() const;

//////  private:
//////    long long m_start = 0; // microseconds
//////    long long m_end   = 0; // microseconds
//////    Measures m_measures;
//////    uint64_t m_size = 0;

//////    friend class hub::Input;
//////    friend class hub::Output;
//////};

/////////////////////////////////////////////////////// INLINE
///////////////////////////////////////////////////////////////////////////////

////// inline long long Acquisition::getStart() const {
//////     return m_start;
////// }

////// inline long long Acquisition::getEnd() const {
//////     return m_end;
////// }

////// inline bool Acquisition::isEnd() const {
//////     return m_measures.empty();
////// }

////// template <class... Args>
////// void Acquisition::emplaceMeasure( Args&&... args ) {
////// #if ( __cplusplus >= 201703L )
//////     static_assert( 3 <= sizeof...( args ) && sizeof...( args ) <= 4 );
////// #endif
//////     m_measures.emplace_back( std::forward<Args>( args )... );
//////     m_size += m_measures.back().getSize();
////// }

////// inline bool Acquisition::operator<( const Acquisition& right ) const {
//////     if (m_start == right.m_start)
//////         return m_end < right.m_end;
//////     return m_start < right.m_start;
////// }

} // namespace sensor
} // namespace hub
