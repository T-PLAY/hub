

// #include <string>

#include <queue>
#include <string>

#include <core/Vector.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>
// #include <core/Tuple.hpp>

//#define USE_BOOST
//#include <boost/type_index.hpp>
//#define HEADER_OUTPUT_MSG \
//    "\033[" << std::to_string( 31 + (long)this % 7 ) << "m<< [Output:" << this << "]\033[0m "
//#define HEADER_INPUT_MSG \
//    "\033[" << std::to_string( 31 + (long)this % 7 ) << "m>> [ Input:" << this << "]\033[0m "

//        std::cout << "[InputOutput] write " << std::endl;
// #    ifdef USE_BOOST
//    std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " ("
//              << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" <<
//              std::endl;
// #    else
//    std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " '" << t << "'"
//    << std::endl;
// #    endif

// using namespace hub::sensor;

#include <io/input/Input.hpp>
#include <io/output/Output.hpp>

class InputOutput : public hub::Input, public hub::Output
{
  public:
    using hub::Input::read;
    using hub::Output::write;

    InputOutput() = default;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( !m_datas.empty() );
        auto vector = m_datas.front();
        m_datas.pop();
        assert( vector.size() == size );
        std::cout << HEADER_INPUT_MSG "read(Data_t*, Size_t) : data = " << vector << std::endl;

        memcpy( data, vector.data(), size );
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        std::vector<hub::Data_t> vector( data, data + size );
        std::cout << HEADER_OUTPUT_MSG "write(T) : data = " << vector << std::endl;
        m_datas.push( vector );
    }

    void close() override {
    };

    bool isOpen() const override {
        return true;
    };

    bool isEnd() const override {
        return m_datas.empty();
    }

    std::queue<std::vector<hub::Data_t>> m_datas;
};

#include <core/Tuple.hpp>


int main() {


//    return 0;

    //    std::tuple t{42, 'a', 4.2}; // Another C++17 feature: class template argument deduction
    //    std::apply([](auto&&... args) {((std::cout << args << '\n'), ...);}, t);

    std::tuple<int, char> items { 1, 'a' };
    for_each( items, []( const auto& item ) { std::cout << item << "\n"; } );

    auto tup = std::make_tuple( 0, 'a', 3.14 );

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

    /////////////////////////////////////////////////////////////////////////////////////////////////
    //    //    using MeasureT1 = hub::sensor::MeasureT<UserFormat, 3, 2, 1>;

    //    //    hub::sensor::format::Z
    //    //    hub::sensor::SensorSpec sensorSpec;
    //    using MeasureT1 = hub::sensor::MeasureT<hub::sensor::format::Y8, 3, 2, 1>;
    //    //    static_assert(MeasureT1::getSize() == 6);
    //    //    static_assert(MeasureT1::nDim() == 3);
    //    //    using Output = int;
    //    //    MeasureT1 measure1{0, }
    //    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
    //    //        constexpr MeasureT1 measure1 { data1 };

    //    constexpr MeasureT1 measureT1;
    //    static_assert( MeasureT1::getFormat() == hub::sensor::format::Y8 );
    //    //    static_assert(MeasureT1::getFormat() == hub::sensor::format::Z16);
    //    static_assert( MeasureT1::nDim() == 3 );
    //    static_assert( MeasureT1::getDim( 0 ) == 3 );
    //    static_assert( MeasureT1::getDim( 1 ) == 2 );
    //    static_assert( MeasureT1::getDim( 2 ) == 1 );
    //    static_assert( MeasureT1::getDim( 3 ) == 0 );
    //    static_assert( MeasureT1::nByte() == 6 );
    //    //    static_assert( MeasureT1::getSize() == 6 );
    //    //    assert(MeasureT1::typeName() == "Y8");

    //    static_assert( hub::sensor::format::Y8.name() == "Y8" );

    //    std::cout << "typename: '" << MeasureT1::typeName() << "'" << std::endl;
    //    std::cout << "measureT1: " << measureT1 << std::endl;

    //    static constexpr hub::sensor::Format userFormat { 6, "userFormat", false };
    //    using UserMeasureT = hub::sensor::MeasureT<userFormat, 1>;
    //    UserMeasureT userMeasure;

    //    std::cout << "userMeasure: " << userMeasure << std::endl;

    //    static_assert( MeasureT1::nDim() == 3 );
    //    static_assert( MeasureT1::getDim( 0 ) == 3 );
    //    static_assert( MeasureT1::getDim( 1 ) == 2 );
    //    static_assert( MeasureT1::getDim( 2 ) == 1 );
    //    static_assert( MeasureT1::getDim( 3 ) == 0 );
    //    static_assert( MeasureT1::getDim( 4 ) == 0 );
    //    static_assert( MeasureT1::getDim( 0 ) == 3 );

    //    //    std::cout << "measureT1: '" << measureT1 << "'" << std::endl;

    //    //    MeasureT1 measure1_noConst;
    //    //    static_assert( measure1_noConst.getDim( 0 ) == 3 );

    //    //    std::cout << "measure1_noConst: '" << measure1_noConst << "'" << std::endl;

    //    using MeasureT2 = hub::sensor::MeasureT<hub::sensor::format::RGB8, 1>;
    //    //    using Output = int;
    //    //    MeasureT1 measureT1{0, }
    //    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };
    //    //    constexpr MeasureT2 measure2 { data2 };
    //    //    MeasureT2 measure2_noConst { data2 };

    //    constexpr std::array<unsigned char, 9> data12 { 1, 2, 3, 4, 5, 6, 1, 2, 3 };

    //    //    std::cout << "measure2: '" << measure2 << "'" << std::endl;

    //    using MeasuresT12 = hub::sensor::MeasuresT<MeasureT1, MeasureT2>;
    //    //    constexpr MeasuresT12 measures12 { measureT1, measure2 };
    //    //    MeasuresT12 measures12_noConst;

    //    //    auto fun = [](const auto & measure) {
    //    //    };
    //    //    for_each(measures12_noConst.getTuple(), [](const auto& measure) {
    //    //        std::cout << measure << "\n";
    //    //    });

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
    //    static_assert( MeasuresT12::nByte() == 9 );
    //    //    static_assert(measureT1.nMeasure() == 2);
    //    //    static_assert(MeasuresT12::nMeasure() == 2);
    //    //    static_assert(MeasuresT12::size() == 9);
    //    static_assert( MeasuresT12::nMeasure() == 2 );
    //    //    static_assert( measures12.nMeasure() == 2 );
    //    //    static_assert( measures12_noConst.nMeasure() == 2 );
    //    //    static_assert(measures12.nMeasure() == 2);
    //    //    static_assert(measures12_noConst.nMeasure() == 2);
    //    //    std::cout << MeasuresT12::nMeasure() << std::endl;
    //    //    static_assert(measures12.nMeasure() == 2);
    //    //    static_assert(measures12.nMeasure() == 2);

    //    //    //    using OutputSensor2  = OutputSensor<MeasureT2>;
    //    //    //    using OutputSensor12 = OutputSensor<MeasuresT12>;

    //    //    //    using Output1 = Output<MeasureT1>;
    //    //    //    constexpr Output1 output1;
    //    //    //    constexpr OutputSensor1 outputSensor1(output1);
    //    //    //    hub::output::Fil

    //    using AcquisitionT12 = hub::sensor::AcquisitionT<MeasuresT12>;

    //    AcquisitionT12 acqT12_noConst;
    //    std::cout << "acq12: " << acqT12_noConst << std::endl;

    //    acqT12_noConst.setStart( 1 );
    //    acqT12_noConst.setEnd( 2 );

    //    std::cout << "acq12: " << acqT12_noConst << std::endl;

    //    auto& measureT1_noConst = acqT12_noConst.getMeasures().get<0>();
    //    measureT1_noConst.setData( data1.data(), data1.size() );

    //    auto& measureT2_noConst = acqT12_noConst.getMeasures().get<1>();
    //    measureT2_noConst.setData( data2.data(), data2.size() );

    //    auto& measures12_noConst = acqT12_noConst.getMeasures();
    //    measures12_noConst.for_each(
    //        []( const auto& measure ) { std::cout << measure.typeName() << std::endl; } );
    //    std::cout << "measure12: " << measures12_noConst << std::endl;

    //    std::cout << "acq12: " << acqT12_noConst << std::endl;

    //    ////////////////////////////////////

    //    hub::sensor::SensorSpec sensorSpecT;
    //    InputOutput inputOutput;
    //    using OutputSensorT12 = hub::sensor::OutputSensorT<InputOutput, MeasuresT12>;
    //    OutputSensorT12 outputSensorT12 { inputOutput, sensorSpecT };
    //    //    //    auto outputSensor = make_outputSensor(output, sensorSpecT);

    //    for ( int iAcq = 0; iAcq < 2; ++iAcq ) {
    //        acqT12_noConst.setStart( iAcq );
    //        acqT12_noConst.setEnd( iAcq + 1 );
    //        measureT1_noConst.setData( data1.data(), data1.size() );
    //        measureT2_noConst.setData( data2.data(), data2.size() );
    //        outputSensorT12 << acqT12_noConst;
    //    }

    //    ///////////////////////////////////////////////////////
    //    //    ///

    //    using InputSensorT12 = hub::sensor::InputSensorT<InputOutput, MeasuresT12>;
    //    InputSensorT12 inputSensorT12 { inputOutput };
    //    //    //    auto inputSensorT12 = make_inputSensor(input);
    //    AcquisitionT12 acq12_read;
    //    //    std::cout << "acqread: " << acq12_read << std::endl;

    //    int iAcq = 0;
    //    while ( !inputSensorT12.getInput().isEnd() ) {
    //        inputSensorT12 >> acq12_read;

    //        assert( acq12_read.getStart() == iAcq );
    //        assert( acq12_read.getEnd() == iAcq + 1 );

    //        auto& measures = acq12_read.getMeasures();
    //        int iMeasure   = 0;

    //        auto& measureT1_read = acq12_read.getMeasures().get<0>();
    //        assert( measureT1_noConst == measureT1_read );
    //        auto& measureT2_read = acq12_read.getMeasures().get<1>();
    //        assert( measureT2_noConst == measureT2_read );
    //        //        auto & measure3_read = acq12_read.getMeasures().get<2>();

    //        //        assert(measure1_noConst == measure2_noConst);
    //        measures.for_each( [&]( const auto& measure ) {
    //            std::cout << "measure " << iMeasure << " : " << measure << std::endl;
    //            //            if (iMeasure == 0) {
    //            //                assert(measure == measure1_noConst);
    //            //            }
    //            //            else {
    //            //                assert(measure == measure2_noConst);
    //            //            }
    //            ++iMeasure;
    //        } );

    //        ++iAcq;
    //    }

    ///////////////

    //    InputOutput inputOutput;
    //    assert( inputOutput.isEnd() );
    std::cout << "--------------------------- no template part" << std::endl;

    //    constexpr std::vector<int> vec = {1, 2, 3};

    //    using MeasureT1 = hub::sensor::MeasureT<hub::sensor::format::Y8, 3, 2, 1>;
    //    hub::sensor::Measure measure = hub::sensor::Measure(hub::sensor::format::Y8, 3, 2, 1);
    const auto resolution1 = hub::sensor::Resolution { hub::sensor::format::Y8, 3, 2, 1 };
    //    assert(measure == measure1);
    std::cout << "resolution1: " << resolution1 << std::endl;
    assert( resolution1.nByte() == 6 );
    assert( resolution1.nDim() == 3 );
    assert( resolution1.getDim( 0 ) == 3 );
    assert( resolution1.getDim( 1 ) == 2 );
    assert( resolution1.getDim( 2 ) == 1 );
    assert( resolution1.getFormat() == hub::sensor::format::Y8 );
    //    assert(resolution1.getDim(3) == 1);

    //    //    using MeasureT2 = hub::sensor::MeasureT<hub::sensor::format::RGB8, 1>;
    const auto resolution2 = hub::sensor::Resolution { hub::sensor::format::RGB8, 1 };
    std::cout << "resolution2: " << resolution2 << std::endl;

    //    //    using Measures12 = hub::sensor::Measures<MeasureT1, MeasureT2>;
    hub::sensor::Resolutions resolutions { resolution1, resolution2 };
    std::cout << "resolutions: " << resolutions << std::endl;

    const auto resolutions2 = hub::sensor::Resolutions { { hub::sensor::format::RGB8, 640, 480 },
                                                         { hub::sensor::format::Y8, 640, 480 } };
    std::cout << "resolutions2: " << resolutions2 << std::endl;

    //    hub::sensor::SensorSpec sensorSpec;
    //    InputOutput inputOutput;
    //    using OutputSensorT12 = hub::sensor::OutputSensorT<InputOutput, MeasuresT12>;
    //    OutputSensorT12 outputSensorT12 { inputOutput, sensorSpec };

    const auto format = hub::sensor::format::RGB8;

    std::vector<int> dims {640, 480, 3};

    //        hub::sensor::OutputSensorT outputSensor12{inputOutput, measures12, sensorSpec};
    const auto sensorSpec = hub::sensor::SensorSpec { "sensorName", resolutions };
    std::cout << "sensorSpec: " << sensorSpec << std::endl;

    InputOutput inputOutput;

    assert(inputOutput.isEnd());

    static_assert(! hub::Input::readable<hub::sensor::Format>());
    static_assert(! hub::Output::writable<hub::sensor::Format>());

//    inputOutput << format;
    inputOutput.write(format);
    hub::sensor::Format format_read;
//    inputOutput >> format_read;
    inputOutput.read(format_read);
    assert(format == format_read);

    assert(inputOutput.isEnd());


//    inputOutput << dims;
    inputOutput.write(dims);
    std::vector<int> dims_read;
//    inputOutput >> dims_read;
    inputOutput.read(dims_read);
    assert(dims == dims_read);

    assert(inputOutput.isEnd());

//    inputOutput << resolution1;
    inputOutput.write(resolution1);
    hub::sensor::Resolution resolution1_read;
//    inputOutput >> resolution1_read;
    inputOutput.read(resolution1_read);
    assert(resolution1 == resolution1_read);

//    assert(inputOutput.isEnd());
    static_assert(hub::Input::readable<hub::sensor::Resolution>());
    static_assert(hub::Output::writable<hub::sensor::Resolution>());

    assert(inputOutput.isEnd());

    inputOutput.write(sensorSpec);
    hub::sensor::SensorSpec sensorSpec_read;
    inputOutput.read(sensorSpec_read);
    assert(sensorSpec == sensorSpec_read);


//    auto outputSensor = hub::sensor::OutputSensor{ inputOutput, sensorSpec };

    //    //    auto outputSensor = make_outputSensor(output, sensorSpec);
    //    auto inputSensor = hub::sensor::InputSensor<InputOutput>( inputOutput );

    //    for ( int iAcq = 0; iAcq < 2; ++iAcq ) {
    //        acq12.setStart( iAcq );
    //        acq12.setEnd( iAcq + 1 );
    //        measure1_acq.setData( data1.data(), data1.size() );
    //        measure2_acq.setData( data2.data(), data2.size() );
    //        outputSensor12 << acq12;
    //    }

    //    ///////////////////////////////////////////////////////
    //    //    ///

    //    std::cout << "measures12: " << measures12 << std::endl;

    //    //    using AcquisitionT12 = hub::sensor::Acquisition<MeasuresT12>;

    //    hub::sensor::Acquisition acq12 { measures12 };
    //    std::cout << "acq12: " << acq12 << std::endl;

    //    acq12.setStart( 1 );
    //    acq12.setEnd( 2 );

    //    std::cout << "acq12: " << acq12 << std::endl;

    //    auto& measure1_acq = acq12.getMeasures().at( 0 );
    //    measure1_acq.setData( data1.data(), data1.size() );

    //    auto& measure2_acq = acq12.getMeasures().at( 1 );
    //    measure2_acq.setData( data2.data(), data2.size() );

    //    auto& measures12_acq = acq12.getMeasures();
    //    //    measures12_acq.for_each(
    ////    for ( const auto& measure : measures12_acq ) {
    ////        std::cout << measure.typeName() << std::endl;
    ////    }
    //    std::cout << "measure12: " << measures12_acq << std::endl;

    //    std::cout << "acq12: " << acq12 << std::endl;

    //    /////////////////////////////////////////////////////////////////////

    //    hub::sensor::Format format_write = hub::sensor::format::Y8;
    //    assert( inputOutput.isEnd() );

    //    inputOutput.write( format_write );

    //    hub::sensor::Format format_read;
    //    inputOutput.read( format_read );

    //    //    format_read.m_isInterpolable = true;
    //    assert( format_read == format_write );

    //    ///////////////////////////////////////////////////////////////////////////////

    //    hub::sensor::Measure measure1_write { format_write, {3, 2, 1} };
    //    std::cout << "measure1_write: " << measure1_write << std::endl;

    ////    std::cout << "write measures12: " << measures12_acq << std::endl;
    //    inputOutput.write(measure1_write);
    //    hub::sensor::Measure measure1_read;
    //    std::cout << "measure1_read: " << measure1_read << std::endl;

    //    inputOutput.read(measure1_read);

    //    assert(measure1_write == measure1_read);

    //    ///////////

    //    hub::sensor::Measure measure2_write { hub::sensor::format::RGB8, {1, 1,
    //    0xffffffffffffffff} }; std::cout << "measure2_write: " << measure2_write << std::endl;
    //    inputOutput.write(measure2_write);

    //    hub::sensor::Measure measure2_read;
    //    inputOutput.read(measure2_read);
    //    std::cout << "measure2_read: " << measure2_read << std::endl;

    //    assert(measure2_write == measure2_read);

    //    ///////////////////////////////////////////////////////////////////////////////

    ////    hub::sensor::Measures measures12_write { }
    //    hub::sensor::Measures measures12_write { measure1_write, measure2_write };
    //    inputOutput.write(measures12_write);
    //    std::cout << "measures12_write: " << measures12_write << std::endl;

    //    hub::sensor::Measures measures12_read;
    //    inputOutput.read(measures12_read);
    //    std::cout << "measures12_read: " << measures12_read << std::endl;

    //    assert(measures12_write == measures12_read);

    //    hub::sensor::SensorSpec sensorSpec;
    //    InputOutput inputOutput;
    //    using OutputSensorT12 = hub::sensor::OutputSensorT<InputOutput, MeasuresT12>;
    //    OutputSensorT12 outputSensorT12 { inputOutput, sensorSpec };

    //    hub::sensor::OutputSensorT outputSensor12{inputOutput, measures12, sensorSpec};
    //    auto outputSensor12 = hub::sensor::OutputSensor<InputOutput>(inputOutput, measures12,
    //    sensorSpec);

    //    //    auto outputSensor = make_outputSensor(output, sensorSpec);

    //    for ( int iAcq = 0; iAcq < 2; ++iAcq ) {
    //        acq12.setStart( iAcq );
    //        acq12.setEnd( iAcq + 1 );
    //        measure1_acq.setData( data1.data(), data1.size() );
    //        measure2_acq.setData( data2.data(), data2.size() );
    //        outputSensor12 << acq12;
    //    }

    //    ///////////////////////////////////////////////////////
    //    //    ///

    //    using InputSensorT12 = hub::sensor::InputSensorT<InputOutput, MeasuresT12>;
    //    InputSensorT12 inputSensorT12 { inputOutput };
    //    //    //    auto inputSensorT12 = make_inputSensor(input);
    //    AcquisitionT12 acq12_read;
    //    //    std::cout << "acqread: " << acq12_read << std::endl;

    //    auto inputSensor12 = hub::sensor::InputSensor<InputOutput>(inputOutput);

    //    if (format == hub::sensor::format::DISPARITY32) {
    //        using Measure1_read = hub::sensor::MeasureT<hub::sensor::format::DISPARITY32, 3, 2,
    //        1>;
    //    }

    //    using Measure1_read = hub::sensor::MeasureT<format_read, 3, 2, 1>;
    //    using Measures12_read = hub::sensor::Measures<MeasureT1, Measure2>;

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
    //    ////    const hub::Output & output4 = hub::output::OutputStreamInterface("streamName",
    //    ipv4,
    //    /// port); // abstract class
    //    //    hub::sensor::OutputSensor outputSensor5(sensorSpec,
    //    hub::output::OutputFile("filePath"));
    //    //    std::vector<int> vs;
    //    // #if CPLUSPLUS_VERSION < 17
    //    //    hub::sensor::OutputSensor outputSensor6(sensorSpec,
    //    //    hub::output::OutputMemory<decltype(vs)>(vs));
    //    // #else
    //    //    hub::sensor::OutputSensor outputSensor6(sensorSpec, hub::output::OutputMemory(vs));
    // #endif
}

//    InputOutput( InputOutput&& inputOutput )                = delete;
//    InputOutput operator=( InputOutput&& inputOutput )      = delete;
//    InputOutput( const InputOutput& inputOutput )           = delete;
//    InputOutput operator=( const InputOutput& inputOutput ) = delete;
//    InputOutput()                                           = default;

//    static void isInput() {};
//    static void isOutput() {};

//    void write( const unsigned char* data, size_t size ) {
//        std::vector<hub::Data_t> vector( data, data + size );
//        std::cout << HEADER_OUTPUT_MSG "write(T) : data = " << vector << std::endl;
//        m_datas.push( vector );
//    }

//    auto& operator<<( uint64_t value ) {
//        std::cout << HEADER_OUTPUT_MSG "write(int) : '" << value << "'" << std::endl;
//        write( reinterpret_cast<const unsigned char*>( &value ), sizeof( int ) );
//        return *this;
//    }

//    template <class T>
//    auto& operator<<( const std::vector<T>& vector ) {
//        //    assert( isOpen() );
//        assert(false);

//        // #ifdef DEBUG_OUTPUT
//        std::cout << HEADER_OUTPUT_MSG "write(std::vector)" << std::endl;
//        // #endif

////        uint64_t nbEl = static_cast<uint64_t>( vector.size() );
////        //    write( nbEl );
////        *this << nbEl;

////        for ( const T& el : vector ) {
////            //        write( el );
////            *this << el;
////        }
//        return *this;
//    }

//    auto& operator<<( int value ) {
//        std::cout << HEADER_OUTPUT_MSG "write(int) : '" << value << "'" << std::endl;
//        write( reinterpret_cast<const unsigned char*>( &value ), sizeof( int ) );
//        return *this;
//    }

//    auto& operator<<( const std::string& str ) {
//        //        assert( isOpen() );

//        // #ifdef DEBUG_OUTPUT
//        std::cout << HEADER_OUTPUT_MSG "write(std::string) : '" << str << "'" << std::endl;
//        // #endif
//        //         std::cout << HEADER_OUTPUT_MSG "write(T) : '" << t.ioTypeName() << "' = " << t
//        <<
//        //         std::endl;

//        int strLen = static_cast<int>( str.size() );
//        assert( sizeof( int ) == 4 );
//        //        write( strLen );
//        *this << strLen;

//        if ( strLen > 0 ) {
//            const char* data = str.data();
//            write( reinterpret_cast<const unsigned char*>( data ), strLen );
//        }
//        return *this;
//    }

//    /////////////////////////////////////////////////////////////////////

//    void read( unsigned char* data, size_t size ) {
//        //        std::vector<hub::Data_t> vector( data, data + size );
//        //        std::cout << HEADER_OUTPUT_MSG "write(T) : data = " << vector << std::endl;
//        //        m_datas.push( vector );
//        assert( !m_datas.empty() );
//        auto vector = m_datas.front();
//        m_datas.pop();
//        assert( vector.size() == size );
//        std::cout << HEADER_INPUT_MSG " read(T) : data = " << vector << std::endl;

//        //            t.ioSetData( vector.data(), vector.size() );
//        memcpy( data, vector.data(), size );
//        //        read((hub::Data_t*)t.ioGetData(), t.ioGetSize());
//        //        t.unpack();

//        //            std::cout << HEADER_INPUT_MSG " read(T) : '" << t.ioTypeName() << "' = " <<
//        t
//        //            << std::endl;
//        //        }
//    }

//    auto& operator>>( int& value ) {
//        read( reinterpret_cast<unsigned char*>( &value ), sizeof( int ) );
//        std::cout << HEADER_OUTPUT_MSG "read(int&) : '" << value << "'" << std::endl;
//        return *this;
//    }

//    auto& operator>>( std::string& str ) {

//        //        assert( isOpen() );
//        //        assert( !isEnd() );

//        int strLen = 0;
//        //        read( strLen );
//        *this >> strLen;

//        if ( strLen == 0 ) { str = ""; }
//        else {
//            const int buffSize = strLen + 1;
//            unsigned char* tmp = new unsigned char[buffSize];
//            read( tmp, strLen );
//            tmp[strLen] = 0;
//            str         = std::string( reinterpret_cast<char*>( tmp ) );
//            delete[] tmp;
//        }
//        // #ifdef DEBUG_INPUT
//        std::cout << HEADER_INPUT_MSG "read(std::string) : '" << str << "'" << std::endl;
//        // #endif
//        return *this;
//    }
//    //    template <typename T>
//    //    void write( const T& t ) {
//    //        std::cout << HEADER_OUTPUT_MSG "write(T) : '" << t.ioTypeName() << "' = " << t <<
//    //        std::endl;
//    //        //                std::cout << HEADER_OUTPUT_MSG "write(T) : '" << t.typeName() <<
//    "'
//    //        = ( "
//    //        //                << t <<
//    //        //        " )"
//    //        //                  << std::endl;

//    //        //        m_data.push_back(t.begin(), t.end(), t.size());
//    //        //        m_data.insert(m_data.end(), )
//    //        assert( t.ioGetData() != nullptr );
//    //        assert( t.ioGetSize() != 0 );
//    //        std::vector<hub::Data_t> vector( t.ioGetData(), t.ioGetData() + t.ioGetSize() );
//    //        std::cout << HEADER_OUTPUT_MSG "write(T) : data = " << vector << std::endl;
//    //        //        for ( int i = 0; i < vector.size(); ++i ) {
//    //        //            std::cout << (int)vector.at( i ) << " ";
//    //        //        }
//    //        //        std::cout << ")" << std::endl;

//    //        m_datas.push( vector );
//    //    }

//    //    template <class T>
//    //    void read( T& t ) {

//    //        assert( !m_datas.empty() );
//    //        auto vector = m_datas.front();
//    //        m_datas.pop();
//    //        assert( vector.size() == t.ioGetSize() );
//    //        std::cout << HEADER_INPUT_MSG " read(T) : data = " << vector << std::endl;

//    //        t.ioSetData( vector.data(), vector.size() );
//    //        //        read((hub::Data_t*)t.ioGetData(), t.ioGetSize());
//    //        //        t.unpack();

//    //        std::cout << HEADER_INPUT_MSG " read(T) : '" << t.ioTypeName() << "' = " << t <<
//    //        std::endl;
//    //    }

//    bool isEnd() const { return m_datas.empty(); }

//    //    void read( hub::Data_t* data, hub::Size_t size ) {
//    //        assert( !m_datas.empty() );
//    //        auto vector = m_datas.front();
//    //        m_datas.pop();
//    //        assert( vector.size() == size );
//    //        std::cout << HEADER_INPUT_MSG " read(T) : data = " << vector << std::endl;

//    ////        t.setData( vector.data(), vector.size() );
//    //        memcpy(data, vector.data(), size);

//    ////        std::cout << HEADER_INPUT_MSG " read(T) : '" << t.typeName() << "' = " << t <<
//    /// std::endl;
//    //    }

//    //    void read(uint64_t & size) {
//    //	    read( reinterpret_cast<unsigned char*>( &t ), sizeof( T ) );
//    //        std::memcpy(&size, )
//    //    }

//    //    template <class T>
//    //    void read( std::vector<T>& vector ) {

//    //        uint64_t nbEl;
//    //        read( nbEl );

//    //        vector.clear();
//    //        vector.reserve( nbEl );

//    //        for ( int i = 0; i < nbEl; ++i ) {
//    //            T el;
//    //            read( el );
//    //            vector.push_back( std::move( el ) );
//    //        }
//    //    }

//    std::queue<std::vector<hub::Data_t>> m_datas;
//};

// class Input
//{
//   public:
//     template <typename T>
//     void read( T& t ) {}
// };
