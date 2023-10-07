#pragma once

#include <cassert>
#include <iostream>
#include <list>
//#include <cstdlib>
//#include <string>
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
//#include "core/Tuple.hpp"

namespace hub {
namespace sensor {

template <typename Measures>
class Acquisition
{
  public:
    static constexpr int getSize() {
//        return 2 * sizeof(long long) + Measures::getSize();
        return Measures::getSize();
    }
    using Array = std::array<DataType, getSize()>;

    const DataType * getData() const {
//        assert(m_array.size() == getSize());
//        auto * data = m_array.data();
//        return (DataType*)&m_start;
        return m_array.data();
//        memcpy(data, &m_start, sizeof(m_start));
//        memcpy(data + sizeof(m_start), &m_end, sizeof(m_end));
//        return m_array.data();
//        return nullptr;
    }
    void setData(const DataType * data, int size) {
        assert(size == getSize());
//        std::copy(std::begin(data), std::end(data), m_array);
//        std::copy(data, data + size, m_array);
//        memcpy(m_array.data(), data, size);
//        memcpy((DataType*)&m_start, data, size);
        memcpy((DataType*)m_array.data(), data, size);
//        m_start = *reinterpret_cast<decltype(m_start)*>(m_array.data());
//        m_end = *reinterpret_cast<decltype(m_end)*>(m_array.data() + sizeof(m_start));
    }
    constexpr std::string typeName() const {
//        return "Acquisition<" + Measures::typeName() + ">";
        return "Acquisition";
    }

//    template <typename T>
    //    template <long long Start, long long End>
    //    constexpr Acquisition(long long start, long long end, T && measures)
//    constexpr Acquisition( long long start, long long end, T&& measures ) :

//    template <typename ArrayT>
//    constexpr Acquisition( long long start, long long end, ArrayT && measures ) :
////        m_measures( std::forward<T>( measures ) ),
//        m_start( start ),
//        m_end( end ),
////        m_data(nullptr)
//        m_array{std::forward<ArrayT>(measures)},
//        m_measures{m_array.data(), m_array.data()}
//    {
////        unsigned char * data = m_array.data();
////        memcpy((void*)data, &start, sizeof(start));
////        memcpy(data, start, sizeof(start));
//    }

    Acquisition() {
//        assert(false);
//        static_assert(false);
//        for (int i = 0; i < m_measures.nMeasure(); ++i) {
//            auto & measure = m_measures.at(i);
//            auto & measure = std::get<i>(m_measures.getTuple());
//        auto & measure = std::get<0>(m_measures.getTuple());

//        }
    };

    //    template <typename T>
    //    template <typename ...T>
    ////    template <long long Start, long long End>
    ////    constexpr Acquisition(long long start, long long end, T && measures)
    //    constexpr Acquisition(long long start, long long end, T &&... measures)
    //        : m_start(start)
    //        , m_end(end)
    //        , m_measures(std::forward<T>(measures)...)
    //    {
    //    }
    //        template <typename Resolution>
    //        Measure<Resolution> getMeasure() {
    //            return Measure<Resolution>();
    //        }
    constexpr bool operator==( const Acquisition& acq ) const {
        return getStart() == acq.getStart() && getEnd() == acq.getEnd() && m_measures == acq.m_measures;
    }

    constexpr Measures getMeasures() const { return m_measures; }

    constexpr long long getStart() const {
        return m_start;
//        return m_array.data()[0];
//        long long start;
//        return start;
    }
    constexpr long long getEnd() const {
        return m_end;
//        return m_array.data()[0];
//        long long end;
//        return end;
    }

    template <typename T>
    auto & get() {
        return std::get<T>(m_measures.getTuple());
//        T t;
//        m_measures::get<T>();
//        return m_measures.get<T>();
//        return m_measures.get<T>();
//        return std::get<T>( m_measures );
    }

    template <int id>
    constexpr auto & get() {
//        return std::get<id>( m_tuple );
//        return std::get<id>( m_measures );
        return std::get<id>(m_measures.getTuple());
//        return Measures::get<id>();
    }

//    auto& at( int i ) {
////        return std::get<i>( m_measures );
//        return m_measures.at(i);
//    }

    auto getMeasure(int i) {
        return Measures::nMeasure();
//        m_measures.nDim();
    }


    //    static constexpr int getSize() {
    ////        int size = 0;
    ////        int _[] = { (size = size + Measures::getSize()) };
    ////        return size;
    //        return Measures::getSize();
    //    }

    template <typename MeasuresT>
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition<MeasuresT>& acq );

//  private:
    Measures m_measures;
    long long m_start;
    long long m_end;
//    DataType * m_data;
//    std::array<DataType, getSize()> m_array;
    Array m_array;
};


template <typename MeasuresT>
std::ostream& operator<<( std::ostream& os, const Acquisition<MeasuresT>& acq ) {
    os << "start:" << ( acq.getStart() / 1'000'000 ) % 1'000 << " "
       << ( acq.getStart() / 1'000 ) % 1'000 << " " << acq.getStart() % 1'000
       << ", end:" << ( acq.getEnd() / 1'000'000 ) % 1'000 << " " << ( acq.getEnd() / 1'000 ) % 1'000
       << " " << acq.getEnd() % 1'000;
    os << ", measures:";
    const auto & measures = acq.m_measures;
    os << measures;
//    for ( int i = 0; i < acq.m_measures.size(); ++i ) {
//        const auto& measure = acq.m_measures.at( i );
//        acq.m_measures.get<0>( );
//        std::get<i>(acq.m_measures);
//        acq.m_measures.
//        os << "[" << measure << "]";
//        if ( i != acq.m_measures.size() - 1 ) { os << ", "; }
//    }
//    os << ", " << 1'000'000.0 / ( acq.m_end - acq.m_start ) << " fps.";
    return os;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Acquisition, int Size>
class Acquisitions
{
  public:
    using Array = std::array<Acquisition, Size>;
    template <typename... T>
    constexpr Acquisitions( T&&... t ) :
        m_acqs { std::forward<T>( t )... } //        ,m_array{std::forward<T>(t)}
    {
        //        std::get<0>(m_measures).m_data = m_array.data();
    }
    //    template <typename T>
    //    constexpr Acquisitions( T&& t ) :
    //        m_array { std::forward<T>( t ) } //        ,m_array{std::forward<T>(t)}
    //    {
    //        //        std::get<0>(m_measures).m_data = m_array.data();
    //    }

    //    auto operator() {
    //        return m_array;
    //    }
    constexpr auto at( int idx ) const { return m_acqs.at( idx ); }

    constexpr auto size() const {
        //        static_assert(Size == 3);
        return Size;
        //        return m_acqs.size();
    }

    Array m_acqs;
};

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
