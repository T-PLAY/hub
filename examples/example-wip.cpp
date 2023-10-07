

#include <cassert>
#include <array>

#include <sensor/Format.hpp>
#include <sensor/Measure.hpp>
#include <sensor/Acquisition.hpp>

using namespace hub::sensor;

int main() {

//    constexpr Format format = Format::BGR8;

//    static_assert(format::isImage(format));
//    static_assert(! format::isImage(Format::DOF6));

//    assert(format::to_string(Format::DOF6) == "6DOF");

//    constexpr std::tuple<int, bool, std::string_view> tuple { 5, true, "ouoo" };
//    static_assert( std::get<int>( tuple ) == 5 );
//    static_assert( std::get<0>( tuple ) == 5 );

//    //    constexpr int buff[5] {0, 1, 2, 3, 4};
//    //    constexpr std::array<int, 5> array {buff};
//    constexpr std::array<int, 5> array { 0, 1, 2, 3, 4 };
//    static_assert( array.at( 0 ) == 0 );
//    static_assert( array[1] == 1 );

//    //    constexpr std::array<int, 2> array2 = std::array<int, 2>(array.begin(), array.begin() +
//    //    1);

//    constexpr int msrWidth  = 2;
//    constexpr int msrHeight = 1;
//    constexpr int msrSize   = msrWidth * msrHeight * 3;
//    static_assert( msrSize == 6 );

//    using Measure1 = Measure<Format::RGB8, msrWidth, msrHeight, 1>;
//    static_assert( Measure1::getSize() == msrSize );

//    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
//    constexpr Measure1 measure1 { data1 };
//    static_assert( measure1.getSize() == msrSize );
////    static_assert( measure1.m_array.at( 0 ) == 1 );
////    static_assert( measure1.getData() == &data1 );
//    static_assert( measure1.at(0) == 1 );
////    static_assert( measure1.m_array.at( 5 ) == 6 );
//    static_assert( measure1.at( 5 ) == 6 );
////    static_assert( measure1.m_array == data1 );

//    /////////////////

//    constexpr int msrSize2 = 3;
//    using Measure2         = Measure<Format::Y8, 3>;
//    static_assert( Measure2::getSize() == msrSize2 );

//    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };
//    constexpr Measure2 measure2 { data2 };
//    static_assert( measure2.getSize() == msrSize2 );
////    static_assert( measure2.m_array.at( 0 ) == 1 );
//    static_assert( measure2.at( 0 ) == 1 );
////    static_assert( measure2.m_array.at( 2 ) == 3 );
//    static_assert( measure2.at( 2 ) == 3 );
////    static_assert( measure2.m_array == data2 );

////    constexpr auto measure3 = make_measure(data2);

//    //////////////////

//    using Measures12 = Measures<Measure1, Measure2>;
//    static_assert( Measures12::getSize() == msrSize + msrSize2 );

////    constexpr auto data12 = join( data1, data2 );

//    //    constexpr Measures12 measures {data12};
//    constexpr Measures12 measures12 { measure1, measure2 };
//    static_assert( measures12.getSize() == msrSize + msrSize2 );
//    //    constexpr auto measure11 = measures12.get<Measure1>();
//    //    static_assert(measure1 == measure11);
//    //    static_assert(measure1 == measures12.get<Measure1>());

//    //    constexpr Measures1 measures;
//    ////    constexpr auto measure11 = measures.get<Measure1>();
//    static_assert( measures12.get<Measure1>().getSize() == msrSize );
//    static_assert( measures12.get<0>().getSize() == msrSize );
//    static_assert( measures12.get<0>() == measure1 );
//    static_assert( measures12.get<Measure2>().getSize() == msrSize2 );
//    static_assert( measures12.get<1>().getSize() == msrSize2 );
//    static_assert( measures12.get<1>() == measure2 );

//    //    unsigned char data[msrSize] {1, 1, 2, 3, 4, 5};
//    //    auto measure11 = measures.get<Measure1>();
//    ////    measure11.setData(data, msrSize);

//    //    constexpr auto measuresArray = measures.m_array;
//    ////    static_assert(measuresArray.at(0) == 1);

//    using Acquisition1 = Acquisition<Measure1>;
//    constexpr Acquisition1 acq1_0 { 0, 0, measure1 };
//    constexpr Acquisition1 acq1_1 { 1, 1, measure1 };
//    constexpr Acquisition1 acq1_2 { 2, 2, measure1 };
//    //    constexpr auto acq1_0 = makeAcquisition(0, 1, measure1);
//    static_assert( acq1_0.getStart() == 0 );
//    static_assert( acq1_0.getEnd() == 0 );
//    static_assert( acq1_0.getMeasures() == measure1 );

//    static_assert( acq1_1.getStart() == 1 );
//    static_assert( acq1_1.getEnd() == 1 );

//    constexpr Acquisition1 acq11 { 1, 2, measure1 };
//    static_assert( acq1_0 != acq11 );

//    using Acquisition2 = Acquisition<Measure2>;
//    constexpr Acquisition2 acq2_0 { 0, 0, measure2 };
//    constexpr Acquisition2 acq2_1 { 1, 1, measure2 };
//    constexpr Acquisition2 acq2_2 { 2, 2, measure2 };
//    //    constexpr auto acq1_0 = makeAcquisition(0, 1, measure1);
//    static_assert( acq2_0.getStart() == 0 );
//    static_assert( acq2_0.getEnd() == 0 );
//    static_assert( acq2_0.getMeasures() == measure2 );
//    //    static_assert(acq1_0 != acq2_0);

//    //    constexpr auto acq12 = makeAcquisition(0, 1, measures12);
//    using Acquisition12 = Acquisition<Measures12>;
//    constexpr Acquisition12 acq12 { 0, 0, measures12 };
//    static_assert( acq12.getStart() == 0 );
//    static_assert( acq12.getEnd() == 0 );
//    static_assert( acq12.getMeasures() == measures12 );

//    constexpr auto measure11 = acq1_0.getMeasures();
//    static_assert( measure11 == measure1 );
//    constexpr auto measure22 = acq2_0.getMeasures();
//    static_assert( measure22 == measure2 );
//    //    constexpr Measures12 measures122 {measure11, measure22};
//    constexpr Measures12 measures122 { acq1_0.getMeasures(), acq2_0.getMeasures() };
//    //    constexpr auto measures1222 = make_measures(acq1_0.getMeasures(), acq2_0.getMeasures());
//    static_assert( measures12 == measures122 );
//    //    constexpr auto measures122 {acq1_0.getMeasures(), acq2_0.getMeasures()};

//    //    constexpr Measures1 measures;

//    //    using OutputSensor1  = OutputSensor<Measure1>;
//    //    using OutputSensor2  = OutputSensor<Measure2>;
//    //    using OutputSensor12 = OutputSensor<Measures12>;

////    using InputData1 =
////        InputData<int, Acquisition1, Acquisition1, Acquisition1, Acquisition2>;

////    constexpr InputData1 inputData1 { 5, acq1_0, acq1_1, acq1_2, acq2_0 };

////    constexpr auto acq11_0 = inputData1.get<0>();
////    constexpr auto acq11_1 = inputData1.get<1>();
////    constexpr auto acq11_2 = inputData1.get<2>();
////    //    constexpr auto acq11_3 = inputData1.get<3>();
////    constexpr auto acq11_00 = inputData1.get<int>();
////    constexpr auto acq22    = inputData1.get<Acquisition2>();

////    static_assert( !inputData1.isEnd() );

////    //    auto acq = inputData1.get();

////    //    auto acq = inputOutputData1_2.get();

////    using Acquisitions1 = Acquisitions<Acquisition1, 3>;
////    constexpr Acquisitions1 acqs1 { acq1_0, acq1_1, acq1_2 };

////    using Acquisitions2 = Acquisitions<Acquisition2, 3>;
////    constexpr Acquisitions2 acqs2 { acq2_0, acq2_1, acq2_2 };

////    constexpr auto syncAcq = sync( acqs1, acqs2 );
//////    static_assert( syncAcq == acq12 );

////    //    auto cq1 = circularQueue<int, 5> {0, 1, 2, 3, 4};

////    InputData1 inputData1_2 {5, acq1_0, acq1_1, acq1_2, acq2_0 };
////    const auto inputData = inputData1_2.get<int>();

    return 0;
}
