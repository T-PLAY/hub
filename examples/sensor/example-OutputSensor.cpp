

// #include <string>

#include <queue>

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>
#include <core/Vector.hpp>
//#include <core/Tuple.hpp>

#define USE_BOOST
#include <boost/type_index.hpp>
#define HEADER_OUTPUT_MSG \
    "\033[" << std::to_string( 31 + (long)this % 7 ) << "m<< [Output:" << this << "]\033[0m "
#define HEADER_INPUT_MSG \
    "\033[" << std::to_string( 31 + (long)this % 7 ) << "m>> [ Input:" << this << "]\033[0m "

//        std::cout << "[InputOutput] write " << std::endl;
// #    ifdef USE_BOOST
//    std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " ("
//              << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" <<
//              std::endl;
// #    else
//    std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " '" << t << "'"
//    << std::endl;
// #    endif

//using namespace hub::sensor;

class InputOutput
{
  public:
    InputOutput( InputOutput&& inputOutput )                = delete;
    InputOutput operator=( InputOutput&& inputOutput )      = delete;
    InputOutput( const InputOutput& inputOutput )           = delete;
    InputOutput operator=( const InputOutput& inputOutput ) = delete;
    InputOutput()                                           = default;

    template <typename T>
    void write( const T& t ) {
        std::cout << HEADER_OUTPUT_MSG "write(T) : '" << t.typeName() << "' = (" << t << ")"
                  << std::endl;

        //        m_data.push_back(t.begin(), t.end(), t.size());
        //        m_data.insert(m_data.end(), )
        assert( t.getData() != nullptr );
        assert( t.getSize() != 0 );
        std::vector<hub::sensor::DataType> vector( t.getData(), t.getData() + t.getSize() );
        std::cout << HEADER_OUTPUT_MSG "write(T) : data = " << vector << std::endl;
        //        for ( int i = 0; i < vector.size(); ++i ) {
        //            std::cout << (int)vector.at( i ) << " ";
        //        }
        //        std::cout << ")" << std::endl;

        m_datas.push( vector );
    }

    template <class T>
    void read( T& t ) {

        assert( !m_datas.empty() );
        auto vector = m_datas.front();
        m_datas.pop();
        assert( vector.size() == t.getSize() );
        std::cout << HEADER_OUTPUT_MSG " read(T) : data = " << vector << std::endl;

        t.setData( vector.data(), vector.size() );

        std::cout << HEADER_INPUT_MSG " read(T) : '" << t.typeName() << "' = (" << t << ")"
                  << std::endl;
    }

    std::queue<std::vector<hub::sensor::DataType>> m_datas;
};

// class Input
//{
//   public:
//     template <typename T>
//     void read( T& t ) {}
// };

#include <core/Tuple.hpp>

int main() {
//    std::tuple t{42, 'a', 4.2}; // Another C++17 feature: class template argument deduction
//    std::apply([](auto&&... args) {((std::cout << args << '\n'), ...);}, t);

    std::tuple<int, char> items{1, 'a'};
    for_each(items, [](const auto& item) {
        std::cout << item << "\n";
    });

    auto tup = std::make_tuple(0, 'a', 3.14);

    std::cout << tup << std::endl;

//    print(tup);
//    template for (auto elem : tup)
//    return 0;

    //    return 0;
    constexpr int N       = 100;
    unsigned char data[N] = { 0 };
    std::vector<unsigned char> vector;
    vector.insert( vector.end(), data, data + N );

    //    std::cout << vector << std::endl;
    //    vector.em

    auto indexSeq = std::index_sequence<5> {};
    std::cout << indexSeq.size() << std::endl;

    std::tuple<int, bool, std::string> tuple { 5, true, "hello" };
    std::cout << tuple << std::endl;

    //    auto value = tuple_get(tuple, 2);
    //    std::cout << "value: " << value << std::endl;
    //    assert(value == 5);
//    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port         = 4042;


//    class UserFormat {
//      public:
////        enum class Format {
////            RGB8,
////            BGR8
////        };
//    };

//    constexpr UserFormat userFormat;

//    using Measure1 = hub::sensor::Measure<UserFormat, 3, 2, 1>;


//    hub::sensor::format::Z
    //    hub::sensor::SensorSpec sensorSpec;
    using Measure1 = hub::sensor::Measure<hub::sensor::format::Y8, 3, 2, 1>;
//    static_assert(Measure1::getSize() == 6);
//    static_assert(Measure1::nDim() == 3);
    //    using Output = int;
    //    Measure1 measure1{0, }
    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
//        constexpr Measure1 measure1 { data1 };

    constexpr Measure1 measure1;
    static_assert(Measure1::getFormat() == hub::sensor::format::Y8);
//    static_assert(Measure1::getFormat() == hub::sensor::format::Z16);
    static_assert(Measure1::nDim() == 3);
    static_assert(Measure1::getDim(0) == 3);
    static_assert(Measure1::getDim(1) == 2);
    static_assert(Measure1::getDim(2) == 1);
    static_assert(Measure1::getDim(3) == 0);
    static_assert(Measure1::getSize() == 6);
//    assert(Measure1::typeName() == "Y8");

    static_assert(hub::sensor::format::Y8.typeName() == "Y8");

    std::cout << "typename: '" << Measure1::typeName() << "'" << std::endl;
    std::cout << "measure1: " << measure1 << std::endl;

    static constexpr hub::sensor::Format userFormat{2, "userFormat", false};
    using UserMeasure = hub::sensor::Measure<userFormat, 10>;
    UserMeasure userMeasure;

    std::cout << "userMeasure: " << userMeasure << std::endl;

//    static_assert( Measure1::nDim() == 3 );
//    static_assert( Measure1::getDim( 0 ) == 3 );
//    static_assert( Measure1::getDim( 1 ) == 2 );
//    static_assert( Measure1::getDim( 2 ) == 1 );
//    static_assert( Measure1::getDim( 3 ) == 0 );
//    static_assert( Measure1::getDim( 4 ) == 0 );
//    static_assert( Measure1::getDim( 0 ) == 3 );

//    //    std::cout << "measure1: '" << measure1 << "'" << std::endl;

//    //    Measure1 measure1_noConst;
//    //    static_assert( measure1_noConst.getDim( 0 ) == 3 );

//    //    std::cout << "measure1_noConst: '" << measure1_noConst << "'" << std::endl;

//    using Measure2 = Measure<Format::RGB8, 1>;
//    //    using Output = int;
//    //    Measure1 measure1{0, }
//    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };
//    //    constexpr Measure2 measure2 { data2 };
//    //    Measure2 measure2_noConst { data2 };

//    constexpr std::array<unsigned char, 9> data12 { 1, 2, 3, 4, 5, 6, 1, 2, 3 };

//    //    std::cout << "measure2: '" << measure2 << "'" << std::endl;

//    using Measures12 = Measures<Measure1, Measure2>;
//    //    constexpr Measures12 measures12 { measure1, measure2 };
//    Measures12 measures12_noConst;

////    auto fun = [](const auto & measure) {
////    };
////    for_each(measures12_noConst.getTuple(), [](const auto& measure) {
////        std::cout << measure << "\n";
////    });

//    measures12_noConst.for_each([](const auto & measure) {
//        std::cout << measure.typeName() << std::endl;
//    });
//    std::cout << "measure12: " << measures12_noConst << std::endl;

//    //    measures12.at(0);
//    //    constexpr auto measure12 = measures12.get<0>();
//    //    constexpr auto measure12_2 = measures12.at(0);
//    //    measure12.m_array.at(0);
//    //    measure12.m_array.at(0) = 5;

//    //    std::cout << "measure12: '" << measures12 << "'" << std::endl;
//    //    std::cout << "measure" << measure1_noConst << std::endl;

//    //    auto measure12_noConst = measures12_noConst.get<0>();
//    //    measure12_noConst.m_array.at(0) = 5;

//    //    std::cout << "measure12_noConst: '" << measures12_noConst << "'" << std::endl;

//    //    template for (const auto & measure : measures12) {
//    //    }
//    //    for (int i = 0; i < measures12.)
//    static_assert( Measures12::getSize() == 9 );
//    //    static_assert(measure1.nMeasure() == 2);
//    //    static_assert(Measures12::nMeasure() == 2);
//    //    static_assert(Measures12::size() == 9);
//    static_assert( Measures12::nMeasure() == 2 );
//    //    static_assert( measures12.nMeasure() == 2 );
//    //    static_assert( measures12_noConst.nMeasure() == 2 );
//    //    static_assert(measures12.nMeasure() == 2);
//    //    static_assert(measures12_noConst.nMeasure() == 2);
//    //    std::cout << Measures12::nMeasure() << std::endl;
//    //    static_assert(measures12.nMeasure() == 2);
//    //    static_assert(measures12.nMeasure() == 2);

//    //    using OutputSensor2  = OutputSensor<Measure2>;
//    //    using OutputSensor12 = OutputSensor<Measures12>;

//    //    using Output1 = Output<Measure1>;
//    //    constexpr Output1 output1;
//    //    constexpr OutputSensor1 outputSensor1(output1);
//    //    hub::output::Fil

//    SensorSpec sensorSpec;
//    InputOutput inputOutput;

//    using OutputSensor1 = OutputSensor<InputOutput, Measures12>;
//    OutputSensor1 outputSensor { inputOutput, sensorSpec };
//    //    auto outputSensor = make_outputSensor(output, sensorSpec);

//    using Acquisition12 = Acquisition<Measures12>;

////    constexpr auto data12_2 = join( data1, data2 );
//    //    constexpr Acquisition12 acq12_0 { 0, 1, measures12 };
//    //    constexpr Acquisition12 acq12_1 { 1, 2, measures12 };
//    //    constexpr Acquisition12 acq12_1 { 1, 2, data12_2 };
//    //    constexpr Acquisition12 acq12_2 { 2, 3, measures12 };
//    //    static_assert( acq12_1.getSize() == 8 * 2 + 9 );

//    //    Acquisition12 acq12_1;
//    //    auto & measure1 = acq12_1.get<Measure1>();
//    //    assert(measure1.getSize() == Measure1::getSize());
//    //    auto & measure2 = acq12_1.get<Measure2>();
//    //    assert(measure2.getSize() == Measure2::getSize());

//    //    std::cout << "write acq: " << acq12_1 << std::endl;
//    //    outputSensor << acq12_1;

////    auto data12_noConst     = join( data1, data2 );
////    auto* data12_noConstPtr = data12_noConst.data();

//    //    Measure1 measure1_noConst(data12_noConstPtr);
//    //    std::cout << "measure1_noConst: " << measure1_noConst << std::endl;

//    //    Measures12 measure12_noConst{data12_noConst.data(), data12_noConst.size() };
//    Measures12 measure12_noConst;
//    //    std::cout << "measure12_noConst: " << measure12_noConst << std::endl;
//    //    measure12_noConst.at(0);

//    //    measure12_noConst.get<0>().setData(data12_noConst.data());
//    //    measure12_noConst.get<1>().setData(data12_noConst.data() + data1.size());
//    //    std::cout << "measure12_noConst: " << measure12_noConst << std::endl;
//    ////    Measures12 measure12_noConst{data1.data(), data2.data()};

//    ////    auto & measure1 = measure12_noConst.get<0>();
//    ////    auto & measure1 = measure12_noConst.getMeasure(0);

//    //    auto & measure1_noConst = measure12_noConst.get<0>();
//    //    auto & measure1_noConst2 = measure12_noConst.get<Measure1>();
//    //    assert(measure1_noConst == measure1_noConst2);
//    //    measure12_noConst.getMeasure(0);

//    //    unsigned char data1_2[6] = {0};
//    //    measure1_noConst.setData(data1_2, 6);
//    //    std::cout << "measure12_noConst: " << measure12_noConst << std::endl;

//    //    std::memset(data1_2, 1, 6);
//    //    measure1_noConst.setData(data1_2, 6);
//    //    std::cout << "measure12_noConst: " << measure12_noConst << std::endl;

//    //    unsigned char * data1_ptr = measure1_noConst.getData();
//    ////    data1_ptr = nullptr;
//    //    std::memset(data1_ptr, 2, 6);
//    //    std::cout << "measure12_noConst: " << measure12_noConst << std::endl;

//    //    OutputSensor<

//    //    Acquisition12 acq12;
//    //    std::cout << "acq12: " << acq12 << std::endl;

//    //    auto measure = acq12.getMeasure(0);

//    //    auto & measure1 = acq12.get<0>();
//    //    auto & measure2 = acq12.get<1>();
//    //    measure1.setData(data1.data(), data1.size());
//    //    measure2.setData(data2.data(), data2.size());

//    //    auto & measure3 = acq12.get<2>();
//    //    acq12.at(0);

//    //    Acquisition1 acq(0, 0, );
//    ///////////////////////////////////////////////////////
//    ///

//    //    InputOutput input;
//    using InputSensor1 = InputSensor<InputOutput, Measures12>;
//    InputSensor1 inputSensor { inputOutput };
//    //    auto inputSensor = make_inputSensor(input);

//    //    auto acq = inputSensor.get<Acquisition12>();
//    //    auto acq12_1read = inputSensor.getAcq();
//    //    auto acq = inputSensor.getAcq();

//    //    std::cout << "read acq: " << acq12_1read << std::endl;

//    //    assert(! acq.isEmpty());
//    //    assert( acq12_1read == acq12_1 );

//    //    std::cout << "end" <<std::endl;

//    ////    const hub::Output & output = hub::Output(); // abstract class
//    ////    const auto & outputSensor2 = hub::sensor::OutputSensor(sensorSpec,
//    /// hub::output::OutputStream("streamName", ipv4, port));
//    //    hub::sensor::OutputSensor outputSensor2(sensorSpec,
//    //    hub::output::OutputStream("streamName", ipv4, port)); hub::sensor::OutputSensor
//    //    outputSensor3(sensorSpec, hub::output::OutputStream("streamName", ipv4, port));
//    ////    const hub::Output & output4 = hub::output::OutputStreamInterface("streamName", ipv4,
//    /// port); // abstract class
//    //    hub::sensor::OutputSensor outputSensor5(sensorSpec, hub::output::OutputFile("filePath"));
//    //    std::vector<int> vs;
//    // #if CPLUSPLUS_VERSION < 17
//    //    hub::sensor::OutputSensor outputSensor6(sensorSpec,
//    //    hub::output::OutputMemory<decltype(vs)>(vs));
//    // #else
//    //    hub::sensor::OutputSensor outputSensor6(sensorSpec, hub::output::OutputMemory(vs));
    // #endif
}
