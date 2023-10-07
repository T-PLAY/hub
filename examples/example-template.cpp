
// #include "example-wip.hpp"
#include <array>
#include <iostream>

#include <cassert>
// #include <stdlib.h>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

template <typename T, std::size_t LL, std::size_t RL>
constexpr std::array<T, LL + RL> join( std::array<T, LL> rhs, std::array<T, RL> lhs ) {
    std::array<T, LL + RL> ar;

    auto current = std::copy( rhs.begin(), rhs.end(), ar.begin() );
    std::copy( lhs.begin(), lhs.end(), current );

    return ar;
}

enum class Format : uint64_t {
    NONE = 0,
    RGB8 = 0b1,
    Y8   = 0b1 << 1,
    COUNT,
};

// static_assert(sizeof(long double) == 16);
static_assert( static_cast<int>( Format::RGB8 ) == 1 );
static_assert( static_cast<int>( Format::Y8 ) == 2 );

constexpr uint64_t FormatImg = ( (uint64_t)Format::RGB8 | (uint64_t)Format::Y8 );

constexpr bool formatIsImg( Format format ) {
    return FormatImg && (uint64_t)format;
}

static constexpr int s_format2nByte[static_cast<int>( Format::COUNT )] = {
    0,
    3,
    1,
};

template <Format format, int... Dims>
class Measure
{
  public:
    static constexpr int size() {
        //        return m_size;
        int p = s_format2nByte[static_cast<int>( format )];
        for ( auto n : { Dims... } )
            p *= n;
        return p;
    }
    using Array = std::array<unsigned char, size()>;

    //    template <typename T>
    //    constexpr Measure(T&&  t) : m_array{std::forward<T>(t)} {
    //    }

    template <typename T>
    constexpr Measure( T&& t ) :
        m_array { std::forward<T>( t ) } //        ,m_array{std::forward<T>(t)}
    {
        //        std::get<0>(m_measures).m_data = m_array.data();
    }

    //    template <typename ...T>
    //    constexpr Measure(T&&... t)
    //        : m_array {std::forward<T>(t)...}
    ////        ,m_array{std::forward<T>(t)}
    //    {
    ////        std::get<0>(m_measures).m_data = m_array.data();
    //    }

    constexpr bool operator==( const Measure& measure ) const { return m_array == measure.m_array; }

    //    void setData(unsigned char * data, int size) {
    //        assert(m_data != nullptr);
    //        assert(getSize() == size);
    //        m_data = data;
    //    }

    Array m_array;
    //    unsigned char * m_data = nullptr;
};

template <typename... Measure>
class Measures
{
  public:
    static constexpr int size() {
        int size = 0;
        int _[]  = { ( size = size + Measure::size() )... };
        return size;
    }
    using Array = std::array<unsigned char, size()>;

    template <typename... T>
    constexpr Measures( T&&... t ) :
        m_measures { std::forward<T>( t )... } //        ,m_array{std::forward<T>(t)}
    {
        //        std::get<0>(m_measures).m_data = m_array.data();
    }

    //    template <typename T>
    //    constexpr Measures(T&& t)
    //        : m_measures {std::forward<T>(t)}
    ////        ,m_array{std::forward<T>(t)}
    //    {
    ////        std::get<0>(m_measures).m_data = m_array.data();
    //    }

    constexpr bool operator==( const Measures& measures ) const {
        return m_measures == measures.m_measures;
    }

    template <typename T>
    constexpr auto get() const {
        return std::get<T>( m_measures );
    }

    template <int id>
    constexpr auto get() const {
        return std::get<id>( m_measures );
    }

    //    std::array<Measure, 2> m_measures;
    std::tuple<Measure...> m_measures;
    //    Array m_array;
};
// template <typename ...T>
// inline constexpr auto make_measures(T&&... t) {
//     return Measures<T...>(std::forward<Measures<T...>>(t...));
////    return Acquisition<Measures>(start, end, measures);
//}

// template <typename Measure>
////template <typename T, Format format, int... Dims>
// class Measure {
//   public:

//    constexpr int getSize() {
//        return Measure::getSize();
//    }

// template <long long Start, long long End, typename Measures>
template <typename Measures>
class Acquisition
{
  public:
    template <typename T>
    //    template <long long Start, long long End>
    //    constexpr Acquisition(long long start, long long end, T && measures)
    constexpr Acquisition( long long start, long long end, T&& measures ) :
        m_start( start ), m_end( end ), m_measures( std::forward<T>( measures ) ) {}

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
        return m_start == acq.m_start && m_end == acq.m_end && m_measures == acq.m_measures;
    }

    constexpr Measures getMeasures() const { return m_measures; }

    constexpr long long getStart() const { return m_start; }
    constexpr long long getEnd() const { return m_end; }

    //    static constexpr int getSize() {
    ////        int size = 0;
    ////        int _[] = { (size = size + Measures::getSize()) };
    ////        return size;
    //        return Measures::getSize();
    //    }

    long long m_start;
    long long m_end;
    Measures m_measures;
};

/////////////////////////////////////////////////////////////

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

// template <typename Measures>
// inline constexpr Acquisition<Measures> makeAcquisition(long long start, long long end, const
// Measures & measures) {
//     return Acquisition<Measures>(start, end, measures);
// }

//};

// template <typename Input>
// constexpr auto sync( const Input& input ) const {
template <typename Acquisitions1, typename Acquisitions2>
static constexpr auto sync( Acquisitions1& acqs1, Acquisitions2& acqs2 ) {
    //        auto leftAcq   = m_data;
    //        auto leftAcq   = get();
    //    auto size = acqs.size();
    //    assert(size == 1);
    //    int iNearest = 0;
    for ( int i = 0; i < acqs1.size() - 1; ++i ) {
        auto acq1  = acqs1.at( i );
        auto acq12 = acqs1.at( i + 1 );
        assert( acq1.getStart() <= acq12.getStart() );
    }
    for ( int i = 0; i < acqs2.size() - 1; ++i ) {
        auto acq2  = acqs2.at( i );
        auto acq22 = acqs2.at( i + 1 );
        assert( acq2.getStart() <= acq22.getStart() );
    }

    //    auto nearestAcq = acqs.at(iNearest);
    //    return nearestAcq;
    auto acq1      = acqs1.at( 0 );
    using Measure1 = decltype( acq1.getMeasures() );

    //    return leftAcq;

    auto acq2      = acqs2.at( 0 );
    using Measure2 = decltype( acq2.getMeasures() );

    using Measures12 = Measures<Measure1, Measure2>;
    Measures12 measures { acq1.getMeasures(), acq2.getMeasures() };

    using Acquisition12 = Acquisition<Measures12>;

    Acquisition12 acq {
        acq1.getStart(), acq1.getEnd(), Measures12 { acq1.getMeasures(), acq2.getMeasures() } };

    using Acquisitions12 = Acquisitions<Acquisition12, 5>;

    //    Acquisition12 acqs12 {acq};
//    std::vector<Acquisition12> acqs12;
//    std::array<Acquisition12, acqs1.size()> acqs12;

//    for ( int i = 0; i < acqs1.size(); ++i ) {
//        auto acq1 = acqs1.at( i );
//        auto acq2 = acqs2.at( i );
//        Acquisition12 acq {
//            acq1.getStart(), acq1.getEnd(), Measures12 { acq1.getMeasures(), acq2.getMeasures() } };
//        acqs12.at(i) = acq;
//        //        iNearest = i;
//    }

    //    return 5;
        return acq;
//    return acqs12;

    //        if ( leftAcq.getStart() > rightAcq.getStart() ) {

    //            Acquisition12 acq { leftAcq.getStart(),
    //                                leftAcq.getEnd(),
    //                                Measures12 { leftAcq.getMeasures(), rightAcq.getMeasures() }
    //                                };
    //            return acq;
    //        }
    //        else {
    //            Acquisition12 acq { rightAcq.getStart(),
    //                                rightAcq.getEnd(),
    //                                Measures12 { leftAcq.getMeasures(), rightAcq.getMeasures() }
    //                                };
    //            return acq;
    //        }
}

// template <typename Measures>
class Output
{
  public:
    //    Acquisition<Measures> m_acq;
};

// template <typename Data>
class Input
{
  public:
    //    template <typename T = Measures>
    //    template <Measures>

    //    template <typename T>
    //    constexpr Input( T&& t ) : m_data( std::forward<T>( t ) ) {
    //        //    constexpr Input(T && t) : m_data{t} {
    //    }

    //    template <typename Measures>
    //    constexpr Input(Acquisition<Measures> t) : m_data(std::forward<Acquisition<Measures>>(t))
    //    { constexpr Input(Acquisition<Measures> t) : m_data(t) {
    //    }
    //    constexpr Input(const T& t) : m_data(t) {
    //    constexpr Input(const Acquisition<Measures> & t) : m_data(t) {

    //    }
    //    template <typename ...T>
    //    constexpr Input(T&&... t)
    //        : m_data {std::forward<Acquisition<Measures>>(t)...}
    //        ,m_array{std::forward<T>(t)}
    //    {
    //        std::get<0>(m_measures).m_data = m_array.data();
    //    }

    virtual bool isEnd() const = 0;

    //    constexpr auto& get() const { return m_data; }
    //    template <typename T>
    //    constexpr auto get() const;

    template <typename Input>
    constexpr auto operator>>( const Input& input ) const {
        //        auto leftAcq   = m_data;
        //        auto leftAcq   = get();
        //        using Measure1 = decltype( leftAcq.getMeasures() );

        auto rightAcq  = input.m_data;
        using Measure2 = decltype( rightAcq.getMeasures() );

        //        using Measures12 = Measures<Measure1, Measure2>;
        //        Measures12 measures12 { leftAcq.getMeasures(), rightAcq.getMeasures() };

        //        using Acquisition12 = Acquisition<Measures12>;

        //        if ( leftAcq.getStart() > rightAcq.getStart() ) {

        //            Acquisition12 acq { leftAcq.getStart(),
        //                                leftAcq.getEnd(),
        //                                Measures12 { leftAcq.getMeasures(), rightAcq.getMeasures()
        //                                } };
        //            return acq;
        //        }
        //        else {
        //            Acquisition12 acq { rightAcq.getStart(),
        //                                rightAcq.getEnd(),
        //                                Measures12 { leftAcq.getMeasures(), rightAcq.getMeasures()
        //                                } };
        //            return acq;
        //        }
    }

    //    template <typename Measures2>
    //    template <typename Measures>
    //    constexpr auto operator >> (const Measures & input) const {
    //    constexpr auto operator >> (const T & input) const {

    //    Acquisition<Measures> m_data;
    //    AcquisitionT m_data;
    //    const Acquisition<Measures> & m_data;
    //    Data m_data;
    //    Measures m_data;
    //    std::unique_ptr<Acquisition<Measures>> m_acq;
};

class InputOutput : public Input, public Output
{};

template <typename... Data>
// template <typename Data, int size, typename Idxtype = int,
// typename = typename std::enable_if<std::is_integral<Idxtype>::value>::type>
class InputData : public Input
{
  public:
    template <typename... T>
    constexpr InputData( T&&... t ) : m_datas { std::forward<T>( t )... }, m_id( 0 ) {}

    template <typename T>
    constexpr auto get() const {
        return std::get<T>( m_datas );
    }

    template <int id>
    constexpr auto get() const {
        return std::get<id>( m_datas );
    }

    constexpr bool isEnd() const override { return m_id == std::tuple_size_v<decltype( m_datas )>; }

    //    constexpr bool pop() {

    //    }

    //    template <typename T>
//    auto get() -> decltype(std::get<0>(m_datas)) {
//    auto get() const {
//            return std::get<m_id>(m_datas);
//        }

  private:
    std::tuple<Data...> m_datas;
    //    std::array<Data, size> m_datas;
    int m_id;
    //    Idxtype m_id;
};

//    template <typename Input1, typename Input2>
//    static constexpr auto sync( Input1& input1, Input2& input2 ) {
//        auto leftAcq   = input1.m_data;
//        using Measure1 = decltype( leftAcq.getMeasures() );
//        //    Measure1 measure1 {leftAcq.getMeasures()};

//        auto rightAcq  = input2.m_data;
//        using Measure2 = decltype( rightAcq.getMeasures() );
//        //    Measure2 measure2 {rightAcq.getMeasures()};
//        //    return measure1;

//        //    using Measure1 = Measure<Format::RGB8, 2, 1, 1>;
//        //    using Measure2 = Measure<Format::Y8, 3>;
//        //    Measure1 measure1 {leftAcq.getMeasures()};
//        //    Measure2 measure2 = rightAcq.getMeasures();

//        using Measures12 = Measures<Measure1, Measure2>;
//        Measures12 measures12 { leftAcq.getMeasures(), rightAcq.getMeasures() };

//        using Acquisition12 = Acquisition<Measures12>;
//        //    Acquisition12 acq {leftAcq.getStart(), leftAcq.getEnd(), measures12};
//        //    Acquisition12 acq {0, 0, measures12};
//        using Acquisition1 = decltype( leftAcq );
//        //    Acquisition1 acq {0, 0, measure1};
//        Acquisition12 acq { leftAcq.getStart(),
//                            rightAcq.getEnd(),
//                            Measures12 { leftAcq.getMeasures(), rightAcq.getMeasures() } };
//        //    Measures12 measures12;
//        //    return measures12;
//        //    Measures12 measures12 {measure1};
//        return acq;
//        //    return measures12;
//    }

// template <typename ...Resolution>
// template <typename Measures>
// class OutputSensor
//{
//  public:
//    template <typename T>
//    constexpr OutputSensor( T&& t ) : m_output( std::forward<T>( t ) ) {}

//    //    void operator <<(const Acquisition<Measures> &acq) const {
//    //    }

//    Output<Measures> m_output;

//    //    std::array<unsigned char, getSize()> m_array;
//    //    std::array<unsigned char, Measures::getSize()> m_array;
//};

// template <typename Measures>
// class InputSensor
//{
//   public:
//     template <typename T>
//     constexpr InputSensor( T&& t ) : m_input( std::forward<T>( t ) ) {}
//     //    constexpr InputSensor(const OutputSensor<Measures> & outputSensor)
//     //        : m_acq(outputSensor.m_acq)
//     //    {
//     ////        m_acq = outputSensor.m_acq;
//     //    }

//    Input<Measures> m_input;

//    //    void operator <<(const Acquisition<Measures> &acq) const {
//    //    }
//    //    void setAcq(const Acquisition<Measures> &acq) {
//    //        m_acq = acq;
//    //    }

//    //    const Acquisition<Measures> & m_acq;
//    //    std::array<unsigned char, getSize()> m_array;
//    //    std::array<unsigned char, Measures::getSize()> m_array;
//};

// using Resolutions = <Resolution...>;

// template < resolution>
// template <Format format, int... Dims>

#include <cassert>

int main() {

    constexpr std::tuple<int, bool, std::string_view> tuple { 5, true, "ouoo" };
    static_assert( std::get<int>( tuple ) == 5 );
    static_assert( std::get<0>( tuple ) == 5 );

    //    constexpr int buff[5] {0, 1, 2, 3, 4};
    //    constexpr std::array<int, 5> array {buff};
    constexpr std::array<int, 5> array { 0, 1, 2, 3, 4 };
    static_assert( array.at( 0 ) == 0 );
    static_assert( array[1] == 1 );

    //    constexpr std::array<int, 2> array2 = std::array<int, 2>(array.begin(), array.begin() +
    //    1);

    constexpr int msrWidth  = 2;
    constexpr int msrHeight = 1;
    constexpr int msrSize   = msrWidth * msrHeight * 3;
    static_assert( msrSize == 6 );

    using Measure1 = Measure<Format::RGB8, msrWidth, msrHeight, 1>;
    static_assert( Measure1::size() == msrSize );

    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
    constexpr Measure1 measure1 { data1 };
    static_assert( measure1.size() == msrSize );
    static_assert( measure1.m_array.at( 0 ) == 1 );
    static_assert( measure1.m_array.at( 5 ) == 6 );
    static_assert( measure1.m_array == data1 );

    /////////////////

    constexpr int msrSize2 = 3;
    using Measure2         = Measure<Format::Y8, 3>;
    static_assert( Measure2::size() == msrSize2 );

    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };
    constexpr Measure2 measure2 { data2 };
    static_assert( measure2.size() == msrSize2 );
    static_assert( measure2.m_array.at( 0 ) == 1 );
    static_assert( measure2.m_array.at( 2 ) == 3 );
    static_assert( measure2.m_array == data2 );

    //////////////////

    using Measures12 = Measures<Measure1, Measure2>;
    static_assert( Measures12::size() == msrSize + msrSize2 );

    constexpr auto data12 = join( data1, data2 );

    //    constexpr Measures12 measures {data12};
    constexpr Measures12 measures12 { measure1, measure2 };
    static_assert( measures12.size() == msrSize + msrSize2 );
    //    constexpr auto measure11 = measures12.get<Measure1>();
    //    static_assert(measure1 == measure11);
    //    static_assert(measure1 == measures12.get<Measure1>());

    //    constexpr Measures1 measures;
    ////    constexpr auto measure11 = measures.get<Measure1>();
    static_assert( measures12.get<Measure1>().size() == msrSize );
    static_assert( measures12.get<0>().size() == msrSize );
    static_assert( measures12.get<0>() == measure1 );
    static_assert( measures12.get<Measure2>().size() == msrSize2 );
    static_assert( measures12.get<1>().size() == msrSize2 );
    static_assert( measures12.get<1>() == measure2 );

    //    unsigned char data[msrSize] {1, 1, 2, 3, 4, 5};
    //    auto measure11 = measures.get<Measure1>();
    ////    measure11.setData(data, msrSize);

    //    constexpr auto measuresArray = measures.m_array;
    ////    static_assert(measuresArray.at(0) == 1);

    using Acquisition1 = Acquisition<Measure1>;
    constexpr Acquisition1 acq1_0 { 0, 0, measure1 };
    constexpr Acquisition1 acq1_1 { 1, 1, measure1 };
    constexpr Acquisition1 acq1_2 { 2, 2, measure1 };
    //    constexpr auto acq1_0 = makeAcquisition(0, 1, measure1);
    static_assert( acq1_0.m_start == 0 );
    static_assert( acq1_0.m_end == 0 );
    static_assert( acq1_0.m_measures == measure1 );

    constexpr Acquisition1 acq11 { 1, 2, measure1 };
    static_assert( acq1_0 != acq11 );

    using Acquisition2 = Acquisition<Measure2>;
    constexpr Acquisition2 acq2_0 { 0, 0, measure2 };
    constexpr Acquisition2 acq2_1 { 1, 1, measure2 };
    constexpr Acquisition2 acq2_2 { 2, 2, measure2 };
    //    constexpr auto acq1_0 = makeAcquisition(0, 1, measure1);
    static_assert( acq2_0.m_start == 0 );
    static_assert( acq2_0.m_end == 0 );
    static_assert( acq2_0.m_measures == measure2 );
    //    static_assert(acq1_0 != acq2_0);

    //    constexpr auto acq12 = makeAcquisition(0, 1, measures12);
    using Acquisition12 = Acquisition<Measures12>;
    constexpr Acquisition12 acq12 { 0, 0, measures12 };
    static_assert( acq12.m_start == 0 );
    static_assert( acq12.m_end == 0 );
    static_assert( acq12.m_measures == measures12 );

    constexpr auto measure11 = acq1_0.getMeasures();
    static_assert( measure11 == measure1 );
    constexpr auto measure22 = acq2_0.getMeasures();
    static_assert( measure22 == measure2 );
    //    constexpr Measures12 measures122 {measure11, measure22};
    constexpr Measures12 measures122 { acq1_0.getMeasures(), acq2_0.getMeasures() };
    //    constexpr auto measures1222 = make_measures(acq1_0.getMeasures(), acq2_0.getMeasures());
    static_assert( measures12 == measures122 );
    //    constexpr auto measures122 {acq1_0.getMeasures(), acq2_0.getMeasures()};

    //    constexpr Measures1 measures;

    //    using OutputSensor1  = OutputSensor<Measure1>;
    //    using OutputSensor2  = OutputSensor<Measure2>;
    //    using OutputSensor12 = OutputSensor<Measures12>;

    using InputData1 =
        InputData<int, Acquisition1, Acquisition1, Acquisition1, Acquisition2>;

    constexpr InputData1 inputData1 { 5, acq1_0, acq1_1, acq1_2, acq2_0 };

    constexpr auto acq11_0 = inputData1.get<0>();
    constexpr auto acq11_1 = inputData1.get<1>();
    constexpr auto acq11_2 = inputData1.get<2>();
    //    constexpr auto acq11_3 = inputData1.get<3>();
    constexpr auto acq11_00 = inputData1.get<int>();
    constexpr auto acq22    = inputData1.get<Acquisition2>();

    static_assert( !inputData1.isEnd() );

    //    auto acq = inputData1.get();

    //    auto acq = inputOutputData1_2.get();

    using Acquisitions1 = Acquisitions<Acquisition1, 3>;
    constexpr Acquisitions1 acqs1 { acq1_0, acq1_1, acq1_2 };

    using Acquisitions2 = Acquisitions<Acquisition2, 3>;
    constexpr Acquisitions2 acqs2 { acq2_0, acq2_1, acq2_2 };

    constexpr auto syncAcq = sync( acqs1, acqs2 );
//    static_assert( syncAcq == acq12 );

    //    auto cq1 = circularQueue<int, 5> {0, 1, 2, 3, 4};

    InputData1 inputData1_2 {5, acq1_0, acq1_1, acq1_2, acq2_0 };
    const auto inputData = inputData1_2.get<int>();
//    auto inputData = inputData1_2.get();

    //    constexpr auto int1 = cq1.front();
    //    static_assert(cq1.front() == 0);
    //    cq1.pop();

    //    while (! inputData1.isEnd()) {
    //        auto acq = inputData1.get();

    //    }

    //    using Output1 = Output<Measure1>;
    //    constexpr Output1 output1;
    //    constexpr OutputSensor1 outputSensor1(output1);
    //    static_assert(outputSensor1.m_acq == acq1_0);
    //    outputSensor1 << acq1_acq1_0;

    //    using Input1 = Input<Acquisition1>;
    //    constexpr Input1 input1 { acq1_0 };

    //    //    constexpr auto acq = input1.get();
    //    static_assert( input1.get() == acq1_0 );

    //    using Input2 = Input<Acquisition2>;
    //    constexpr Input2 input2( acq2 );

    //    //    static_assert(input2.get() == acq2);

    //    //    constexpr auto syncAcq = input1 >> input2;
    //    constexpr Acquisition12 syncAcq = input1 >> input2;
    //    //    constexpr Acquisition12 syncAcq = Input::sync( input1, input2 );
    //    //    static_assert(std::is_same<decltype(syncAcq), Acquisition12>::value);
    //    //    using Acquisition122 = decltype(syncAcq);

    //    //    static_assert(syncAcq == measures12);
    //    static_assert( syncAcq == acq12 );

    //    static_assert(typeid(Acquisition122) == typeid(Measures12));
    //    static_assert(std::is_same<Acquisition122, Measures12>::value);
    //    static_assert(std::is_same<Acquisition122, Acquisition12>::value);
    //    static_assert(syncAcq == measure1);
    //    static_assert(syncAcq == acq1_0);
    //    static_assert(syncAcq == acq2);

    //    using InputSensor1 = InputSensor<Measure1>;
    //    constexpr InputSensor1 inputSensor1(input1);

    //    constexpr OutputSensor2 outputSensor2(acq2);

    //    constexpr OutputSensor12 outputSensor12(acq12);
    //    static_assert(outputSensor12.m_acq == acq12);

    //    constexpr InputSensor1 inputSensor1(outputSensor1);
    //    static_assert(inputSensor1.m_acq == acq1_0);
    //    static_assert(inputSensor1.m_acq == acq2);

    static_assert( FormatImg && (uint64_t)Format::RGB8 );
    static_assert( FormatImg && (uint64_t)Format::Y8 );
    static_assert( formatIsImg( Format::RGB8 ) );

    return 0;
}
