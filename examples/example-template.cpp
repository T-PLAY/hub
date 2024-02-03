
#include <array>
#include <iostream>

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

template <Format format, int... NDim>
class Measure
{
  public:
    static constexpr int size() {
        int p = s_format2nByte[static_cast<int>( format )];
        for ( auto n : { NDim... } )
            p *= n;
        return p;
    }
    using Array = std::array<unsigned char, size()>;

    template <typename T>
    constexpr Measure( T&& t ) :
        m_array { std::forward<T>( t ) } //        ,m_array{std::forward<T>(t)}
    {}

    ////        ,m_array{std::forward<T>(t)}
    ////        std::get<0>(m_measures).m_data = m_array.data();

    constexpr bool operator==( const Measure& measure ) const { return m_array == measure.m_array; }

    Array m_array;
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
    {}

    ////        ,m_array{std::forward<T>(t)}
    ////        std::get<0>(m_measures).m_data = m_array.data();

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

    std::tuple<Measure...> m_measures;
};
////    return Acquisition<Measures>(start, end, measures);

////template <typename T, Format format, int... NDim>

template <typename Measures>
class Acquisition
{
  public:
    template <typename T>
    constexpr Acquisition( long long start, long long end, T&& measures ) :
        m_start( start ), m_end( end ), m_measures( std::forward<T>( measures ) ) {}

    ////    template <long long Start, long long End>
    ////    constexpr Acquisition(long long start, long long end, T && measures)
    constexpr bool operator==( const Acquisition& acq ) const {
        return m_start == acq.m_start && m_end == acq.m_end && m_measures == acq.m_measures;
    }

    constexpr Measures getMeasures() const { return m_measures; }

    constexpr long long getStart() const { return m_start; }
    constexpr long long getEnd() const { return m_end; }

    ////        int size = 0;
    ////        int _[] = { (size = size + Measures::getSize()) };
    ////        return size;

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
    {}

    constexpr auto at( int idx ) const { return m_acqs.at( idx ); }

    constexpr auto size() const { return Size; }

    Array m_acqs;
};

// Measures & measures) {

template <typename Acquisitions1, typename Acquisitions2>
static constexpr auto sync( Acquisitions1& acqs1, Acquisitions2& acqs2 ) {
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

    auto acq1      = acqs1.at( 0 );
    using Measure1 = decltype( acq1.getMeasures() );

    auto acq2      = acqs2.at( 0 );
    using Measure2 = decltype( acq2.getMeasures() );

    using Measures12 = Measures<Measure1, Measure2>;
    Measures12 measures { acq1.getMeasures(), acq2.getMeasures() };

    using Acquisition12 = Acquisition<Measures12>;

    Acquisition12 acq {
        acq1.getStart(), acq1.getEnd(), Measures12 { acq1.getMeasures(), acq2.getMeasures() } };

    using Acquisitions12 = Acquisitions<Acquisition12, 5>;

    return acq;
}

class Output
{
  public:
};

class Input
{
  public:
    virtual bool isEnd() const = 0;

    template <typename Input>
    constexpr auto operator>>( const Input& input ) const {

        auto rightAcq  = input.m_data;
        using Measure2 = decltype( rightAcq.getMeasures() );
    }
};

class InputOutput : public Input, public Output
{};

template <typename... Data>
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

  private:
    std::tuple<Data...> m_datas;
    int m_id;
};

int main() {

    constexpr std::tuple<int, bool, std::string_view> tuple { 5, true, "ouoo" };
    static_assert( std::get<int>( tuple ) == 5 );
    static_assert( std::get<0>( tuple ) == 5 );

    constexpr std::array<int, 5> array { 0, 1, 2, 3, 4 };
    static_assert( array.at( 0 ) == 0 );
    static_assert( array[1] == 1 );

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

    constexpr Measures12 measures12 { measure1, measure2 };
    static_assert( measures12.size() == msrSize + msrSize2 );

    ////    constexpr auto measure11 = measures.get<Measure1>();
    static_assert( measures12.get<Measure1>().size() == msrSize );
    static_assert( measures12.get<0>().size() == msrSize );
    static_assert( measures12.get<0>() == measure1 );
    static_assert( measures12.get<Measure2>().size() == msrSize2 );
    static_assert( measures12.get<1>().size() == msrSize2 );
    static_assert( measures12.get<1>() == measure2 );

    ////    measure11.setData(data, msrSize);

    ////    static_assert(measuresArray.at(0) == 1);

    using Acquisition1 = Acquisition<Measure1>;
    constexpr Acquisition1 acq1_0 { 0, 0, measure1 };
    constexpr Acquisition1 acq1_1 { 1, 1, measure1 };
    constexpr Acquisition1 acq1_2 { 2, 2, measure1 };
    static_assert( acq1_0.m_start == 0 );
    static_assert( acq1_0.m_end == 0 );
    static_assert( acq1_0.m_measures == measure1 );

    constexpr Acquisition1 acq11 { 1, 2, measure1 };
    static_assert( acq1_0 != acq11 );

    using Acquisition2 = Acquisition<Measure2>;
    constexpr Acquisition2 acq2_0 { 0, 0, measure2 };
    constexpr Acquisition2 acq2_1 { 1, 1, measure2 };
    constexpr Acquisition2 acq2_2 { 2, 2, measure2 };
    static_assert( acq2_0.m_start == 0 );
    static_assert( acq2_0.m_end == 0 );
    static_assert( acq2_0.m_measures == measure2 );

    using Acquisition12 = Acquisition<Measures12>;
    constexpr Acquisition12 acq12 { 0, 0, measures12 };
    static_assert( acq12.m_start == 0 );
    static_assert( acq12.m_end == 0 );
    static_assert( acq12.m_measures == measures12 );

    constexpr auto measure11 = acq1_0.getMeasures();
    static_assert( measure11 == measure1 );
    constexpr auto measure22 = acq2_0.getMeasures();
    static_assert( measure22 == measure2 );
    constexpr Measures12 measures122 { acq1_0.getMeasures(), acq2_0.getMeasures() };
    static_assert( measures12 == measures122 );

    using InputData1 = InputData<int, Acquisition1, Acquisition1, Acquisition1, Acquisition2>;

    constexpr InputData1 inputData1 { 5, acq1_0, acq1_1, acq1_2, acq2_0 };

    constexpr auto acq11_0  = inputData1.get<0>();
    constexpr auto acq11_1  = inputData1.get<1>();
    constexpr auto acq11_2  = inputData1.get<2>();
    constexpr auto acq11_00 = inputData1.get<int>();
    constexpr auto acq22    = inputData1.get<Acquisition2>();

    static_assert( !inputData1.isEnd() );

    using Acquisitions1 = Acquisitions<Acquisition1, 3>;
    constexpr Acquisitions1 acqs1 { acq1_0, acq1_1, acq1_2 };

    using Acquisitions2 = Acquisitions<Acquisition2, 3>;
    constexpr Acquisitions2 acqs2 { acq2_0, acq2_1, acq2_2 };

    constexpr auto syncAcq = sync( acqs1, acqs2 );

    InputData1 inputData1_2 { 5, acq1_0, acq1_1, acq1_2, acq2_0 };
    const auto inputData = inputData1_2.get<int>();

    static_assert( FormatImg && (uint64_t)Format::RGB8 );
    static_assert( FormatImg && (uint64_t)Format::Y8 );
    static_assert( formatIsImg( Format::RGB8 ) );

    return 0;
}
