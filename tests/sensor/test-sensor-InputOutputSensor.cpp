#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

// #include <initializer_list>
// #include <span>
// #include <vector>

//#include "core/test_core_common.hpp"
#include "sensor/test_sensor_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "InputOutputSensor test" ) {

    hub::io::Archive archive;
    test::sensor::inputOutputSensorBench(archive, archive);
    return;



    //        assert(UserResolution() == inputSensor.getSpec().resolution);

    //        static_assert( UserResolution::nByte() == 640 * 480 * sizeof( UserType ) );
    //        //    using UserResolution2 = ResolutionT<UserType2, 100>;
    //        static_assert( UserResolution::hasType<UserType>() );
    //        static_assert( !UserResolution::hasType<int>() );
    //        std::cout << "resolution: " << UserResolution() << std::endl;

    //        using UserResolution2 = ResolutionT<UserType2>;
    //        static_assert( UserResolution2::nDim() == 1 );
    //        static_assert( UserResolution2::n() == 1 );
    //        std::cout << "resolution2: " << UserResolution2() << std::endl;

    //        using UserResolution3 = ResolutionT<int>;
    //        static_assert( UserResolution3::n() == 1 );
    //        //    static_assert(UserResolution3::isScalar;

    //        using UserResolutions = ResolutionsT<UserResolution, UserResolution2,
    //        UserResolution3>; static_assert( UserResolutions::nResolution == 3 );
    //        //    static_assert(UserResolutions::has<UserType>());

    //        std::cout << "resolutions: " << UserResolutions() << std::endl;
    //        static_assert( UserResolutions::hasType<UserType>() );
    //        static_assert( UserResolutions::hasType<UserType2>() );
    //        static_assert( UserResolutions::hasType<int>() );
    //        static_assert( !UserResolutions::hasType<void>() );
    //        static_assert( UserResolutions::hasType<UserType, UserType2>() );
    //        static_assert( UserResolutions::hasType<UserType, UserType2, int>() );
    //        static_assert( !UserResolutions::hasType<UserType, UserType2, int, double>() );

    //        //    constexpr auto res = UserResolutions::get<UserType>();
    //        static_assert( UserResolutions::nResolution == 3 );
    //    constexpr int nResolution = 2;
    //    for (constexpr int i = 0; i < nResolution; ++i) {
    //        UserResolutions::get<i>();
    //    }
    //    UserResolutions::for_each([](auto & measure) {
    //    });

    //    static_assert(std::is_same<UserResolutions::get<0>, UserResolution>());
    //    static_assert(std::is_same<UserResolutions::get<1>, UserResolution2>());

    //    constexpr std::tuple<int, int, int, char> tuple {1, 3, 2, 'a'};
    //    static_assert(std::get<1, int>(tuple) == 3);
    //    std::has

    //    using UserResolution2 = ResolutionT<

    //    OutputSensorT<UserResolution> outputSensor;
    //        AcquisitionT<UserResolutions> acq;
    //        //    static_assert( !acq.resolutions.hasType<int>() );
    //        //    static_assert( acq.resolutions.hasType<UserType>() );
    //        //    static_assert(acq.resolution.h)
    //        //    static_assert(acq.n)
    //        //    static_assert(Acquisition::has<UserType>());
    //        //    static_assert(Acquisition::has<double>());
    //        //    using Resolution_get = acq.resolution.get<UserType>();

    //        //    auto measure = acq.getMeasure<UserType>();
    //        static_assert( acq.hasType<UserType>() );
    //        static_assert( acq.hasType<UserType, int>() );
    //        //    static_assert(acq.)
    //        static_assert( acq.resolutions.nResolution == 3 );
    //        //    auto measure = acq.get<UserType>();
    //        auto measure = acq.getMeasure<0>();
    //        static_assert( std::is_same<decltype( measure.type() ), UserType>() );
    //        //    static_assert(measure.R)
    //        auto measure2 = acq.getMeasure<1>();
    //        //    measure.resolution.

    //        //    constexpr MeasureT<UserResolution> measure((hub::Data_t*)data, dataSize);
    //        //    constexpr auto span = std::span(data, data + dataSize);
    //        //    constexpr auto span = std::span(dataArray);
    //        //    constexpr MeasureT<UserResolution> measure((hub::Data_t*)data, dataSize);

    //        auto buffer = hub::Buffer<dataSize>( (const hub::Data_t*)data );
    //        //    auto buffer = hub::Buffer<dataSize>((hub::Data_t*)data);
    //        //    buffer.data[0] = 1;
    //        //    buffer.data = nullptr;
    //        //    buffer.size = 5;
    //        std::cout << "buffer: " << buffer << std::endl;

    //        std::cout << "measure: " << measure << std::endl;
    //        measure.setData( buffer );
    //        std::cout << "measure: " << measure << std::endl;

    //        const UserType2 userType2 { 5 };
    //        //    userType2.a = 5;
    //        //    measure2.setData(buffer);
    //        //    std::cout << hub::Buffer(userType2) << std::endl;
    //        //    auto buffer2 = hub::Buffer(userType2);

    //        //    std::cout << "buffer2: " << buffer2 << std::endl;

    //        std::cout << "measure2: " << measure2 << std::endl;
    //        //    measure2.setData(buffer2);
    //        //    measure2.setType( userType2 );

    //        std::cout << "measure2: " << measure2 << std::endl;

    //        //    auto buffer_get = measure.getData();
    //        //    std::cout << "buffer_get: " << buffer_get << std::endl;
    //        //    buffer_get.data[0] = 1;
    //        //    buffer_get.data = nullptr;
    //        //    buffer_get.size = 5;

    //        //    constexpr auto data2_get = measure2.getData<UserType2>();
    //        //    constexpr auto userType2_get = measure2.getData();
    //        //    static_assert( userType2 == userType2_get );

    //        constexpr int a         = 5;
    //        constexpr auto buffer_a = hub::Buffer( a );
    //        static_assert( buffer_a.data == a );

    //        constexpr int b         = 5;
    //        constexpr auto buffer_b = hub::Buffer( b );
    //        static_assert( buffer_a == buffer_b );

    //    {
    //        constexpr double double_ref = 5.0;
    //        constexpr float float_ref = 2.0f;
    //        using MatrixInt2 = MatrixXD<int, 2>;
    ////        static_assert(sizeof(MatrixInt2) == sizeof(int) * 2);
    //        MatrixInt2 matrix_ref {2, 1};
    //        std::cout << "matrix_ref: " << matrix_ref << std::endl;

    //        constexpr int dataSize = sizeof(int) * 2;
    //        Data_t * data_ptr = new Data_t[dataSize];
    //        memset(data_ptr, 55, dataSize);
    ////        std::array<int, 2> array {1, 2};
    ////        std::span<int, 2> span(array);
    ////        std::span<int, 2> span(data);
    //        std::span<Data_t, dataSize> span(data_ptr, data_ptr + dataSize);

    //        matrix_ref.setData(span);
    //        std::cout << "matrix_ref: " << matrix_ref << std::endl;

    //        int data_read[2] {0};
    //        std::span<Data_t, dataSize> span_read((unsigned char*)data_read, (unsigned
    //        char*)data_read + dataSize);

    //        matrix_ref.getData(span_read);
    ////        matrix_ref.data.at(100);
    //        std::cout << "span_read: " << span_read << std::endl;

    ////        int data2[4] { 1, 2, 3, 4};
    ////        std::span<int, 4> span2(data2);

    ////        return;
    ////        span = span2;

    ////        constexpr ArrayBuffer<MatrixInt2, double, float> buffer {matrix_ref, double_ref,
    /// float_ref};

    ////        static_assert(buffer.Size() == sizeof(MatrixInt2) + sizeof(double) + sizeof(float));
    ////        static_assert(buffer.get<MatrixInt2>() == matrix_ref);
    ////        static_assert(buffer.get<MatrixInt2>() != MatrixInt2{2, 2});
    //        struct Mat4 {
    //            float m44[16];
    //            static constexpr auto name() { return "Mat4"; };
    //        };

    //        struct RGB8 {
    //            unsigned char r;
    //            unsigned char g;
    //            unsigned char b;
    //            static constexpr auto name() { return "RGB8"; };
    //        };

    //        using Resolution1 = Matrix<Mat4>;
    //        static_assert(Resolution1::size() == 16 * 4);
    ////        using Resolution1 = Mat4;
    //        std::cout << "resolution1: " << Resolution1() << std::endl;
    //        using Resolution2 = MatrixXD<RGB8, 2>;
    //        static_assert(Resolution2::size() == 6);
    //        std::cout << "resolution2: " << Resolution2() << std::endl;
    ////        using ResolutionAcq = Matrix<Mat4, MatrixXD<RGB8, 2>>;
    //        using ResolutionAcq = Matrix<Resolution1, Resolution2>;
    //        static_assert(ResolutionAcq::size() == 16 * 4 + 3  * 2);
    //        std::cout << "resolutionAcq: " << ResolutionAcq() << std::endl;

    //        using Clock = long long;
    //        using MyMatrixAcq = Matrix<Clock, Clock, ResolutionAcq>;
    ////        using MyMatrixAcq = Matrix<Clock, Clock>;
    //        static_assert(MyMatrixAcq::size() == sizeof(Clock) * 2 + sizeof(ResolutionAcq));
    //        constexpr Clock clock_start = 5;
    //        constexpr Clock clock_end = 6;
    //        constexpr RGB8 rgb8{1, 2, 3};
    //        constexpr RGB8 rgb8s[2] {{1, 2, 3}, {4, 5, 6}};
    //        constexpr Mat4 mat4 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    //        constexpr Resolution1 resolution1 {mat4};
    ////        constexpr ResolutionAcq resolution{mat4, rgb8s};

    //        MyMatrixAcq acq;
    //        static_assert(acq.size() == MyMatrixAcq::size());

    //        static_assert(acq.hasType<Clock>());
    //        static_assert(acq.hasType<Clock, ResolutionAcq>());
    //        static_assert(! acq.hasType<Clock, ResolutionAcq, int>());

    //        std::cout << "acq: " << acq << std::endl;

    //        auto & clockStartRead = acq.getData<0>();
    //        clockStartRead = 5;
    //        auto & clockEndRead = acq.getData<1>();
    //        clockEndRead = 6;
    //        auto & measure = acq.getData<2>();
    ////        measure.

    //        std::cout << "acq: " << acq << std::endl;

    ////        constexpr
    ////        constexpr MatrixInt2 matrixInt2 {5, 3};

    ////        constexpr MyMatrixAcq matrix_acq {clock_start, clock_end, matrixInt2};
    ////        static_assert(sizeof(matrix_acq) == sizeof(long long) * 2 + sizeof(MatrixInt2));
    ////        std::cout << "matrix_acq: " << matrix_acq << std::endl;

    ////        static_assert(matrix_acq.get<0>() == clock_start);
    ////        static_assert(matrix_acq.get<1>() == clock_end);
    ////        static_assert(matrix_acq.get<2>() == matrixInt2);

    ////        static_assert(matrix_acq.get<MatrixInt2>() == matrixInt2);
    ////        static_assert(matrix_acq.get<0, Clock>() == matrixInt2);

    //        return;
    ////        std::array<int, 10> array {1};
    ////        std::span<int> span{array};
    ////        std::cout << "span: " << span << std::endl;

    ////        std::array<int, 10> array2 {2};
    ////        std::span<int> span2{array2};
    ////        std::cout << "span2: " << span2 << std::endl;
    ////        span2 = span;
    ////        std::cout << "span2: " << span2 << std::endl;

    ////        static_assert(buffer.get<int>() == 5);
    ////        static_assert(buffer.get<0>() == 5);

    ////        std::cout << "arrayBuffer: " << buffer << std::endl;

    ////        using Resolution = ResolutionT<int, 2>;
    ////        static_assert(Resolution::nByte() == 8);
    ////        static_assert(std::is_same<Resolution::Type, int>());

    ////        constexpr auto measure = MeasureT<int>(5);
    ////        std::cout << "measure: " << measure << std::endl;

    //        return;
    ////        using Resolution2 = ResolutionT<double>;
    ////        using Resolutions = ResolutionsT<Resolution, Resolution2>;
    ////        static_assert(std::is_same<Resolutions::Types, double>());

    ////        constexpr ArrayBuffer<int> buffer_int{5};
    ////        constexpr ArrayBuffer<int> buffer_int{5};

    ////        using Measure = MeasureT<Resolution>;
    ////        constexpr auto measure = MeasureT<Resolution, ArrayBuffer<int>>(buffer_int);
    ////        constexpr auto measure2 = MeasureT<Resolution2>(true);
    ////        constexpr auto measure2 = MeasureT<Resolution>(buffer_int);

    ////        constexpr ArrayBuffer<Resolution> buffer2;

    //        constexpr auto start = 1;
    //        constexpr auto end = 2;
    //        constexpr auto value_int = 5;
    ////        constexpr AcquisitionT<Resolution> acq{start, end};

    ////        static_assert(acq.getStart() == start);
    ////        static_assert(acq.getEnd() == end);

    ////        using Resolutions = ResolutionsT<ResolutionT<int>, ResolutionT<double>>;
    ////        static_assert(Resolutions::nByte() == 12);
    ////        const long long start = 5;
    ////        static constexpr auto start_list = {1, 2};
    ////        static constexpr auto start_list = std::initializer_list<hub::Data_t>{0, 1, 0, 0};
    ////        constexpr int value = reinterpret_cast<const int&>(&*start_list.begin());
    ////        const int * value = (const int*)start_list.begin();
    ////        constexpr int value = *(const int*)start_list.begin();

    ////        std::cout << "int: " << *value << std::endl;

    ////        static_assert(start_list.size() == 1);
    ////        constexpr std::array<hub::Data_t, 8> start{5};
    ////        constexpr std::array<hub::Data_t, 8> end{6};
    ////        constexpr auto array_acq = join(start, end);
    ////        const long long end = 6;
    ////        std::array<hub::Data_t, 8> array{start};
    ////        std::cout << "array: " << array << std::endl;

    ////        constexpr AcquisitionT<Resolutions> acq{array_acq};
    ////        static_assert(acq.nByte() == 28);
    ////        std::cout << "acq: " << acq << std::endl;

    ////        static_assert(acq.getStart() == 0);
    //    }
    //    return;
}

//    ////////////////////////// ResolutionT //////////////////////////////

//    using Resolution = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 640, 480>;
//    static_assert( Resolution::nResolution == 1 );
//    //    auto get = Resolution::get<0>();
//    //    static_assert(Resolution::get<0>() == Resolution());
//    //    static_assert(Resolution::getResolutionT<0>() == Resolution());

//    std::cout << "Resolution: " << Resolution() << std::endl;

//    ////////////////////////// MeasureT //////////////////////////////

//    constexpr auto dataSize = 640 * 480 * 3;

////    constexpr auto dataSize = 640 * 480 * 3 * 2;
//    unsigned char datas[640 * 480 * 3 * 2] {0};
////    std::span<hub::Data_t> datasSpan(datas, datas + dataSize);

////    hub::sensor::MeasureT<Resolution> measureT(datas);
////    auto measureT = hub::sensor::MeasureT<Resolution>(datas);
////    auto buffer = hub::Buffer(datas);
////    auto buffer = hub::Buffer<dataSize>(datas);
////    static_assert(buffer.size == dataSize);
//    auto datasBuff = hub::Buffer(datas, dataSize);
//    auto datasBuff2 = hub::Buffer((const hub::Data_t*)datas, dataSize);
//    assert(datasBuff == datasBuff2);

////    return;
////    datasBuff.data = nullptr;
////    datasBuff.data[0] = 2;
//    assert(datasBuff == datasBuff2);

//    std::cout << "datasBuff: " << datasBuff << std::endl;
//    hub::sensor::MeasureT<Resolution> measureT(datasBuff);
////    hub::sensor::MeasureT<Resolution> measureT{datas, dataSize};
////    static_assert( measureT.getResolution() == Resolution() );

//    unsigned char data[dataSize] {0};
//    unsigned char * data2 = new unsigned char[dataSize];
//    auto buffer = hub::Buffer(data, dataSize);
//    auto buffer2 = hub::Buffer(data2, dataSize);
//    assert(buffer == buffer2);
////    assert(buffer == datasBuff);

////    std::array<unsigned char, dataSize> array;
////    decltype(array)::value_type;
////    auto type = typeid(std::span<int>::value_type()).hash_code();
////    measureT.setData(array);

//    std::memset(data2, 55, dataSize);
////    measureT.setData(5, nullptr);
////    measureT.setData<data, dataSize>();
////    hub::Buffer buffer2(data2, dataSize);
//    measureT.setData(buffer2);
////    measureT.setData(data, dataSize);
//    assert(std::memcmp(datas, data2, dataSize) == 0);
//    assert(datasBuff == buffer2);

////    std::span span(data);
////    measureT.setData(span);

//    std::memset(data, 55, dataSize);
////    std::span span(data);
////    std::span<hub::Data_t> span2(data2, dataSize);
////    decltype(span)::value_type;

////    measureT.setData(span);
////    assert(std::memcmp(datas, data, dataSize) == 0);

////    measureT.setData(span2);
////    std::span<unsigned char> span2(data2, data2 + dataSize);

//    ////////////////////////// OutputSensorT //////////////////////////////

////    using OutputSensor = hub::sensor::OutputSensorT<Resolution>;
////    //    using Acquisition = OutputSensor::Acquisition;
////    OutputSensor outputSensor;
////    OutputSensor::Acquisition acq;

////    auto& measure = acq.getMeasure<0>();
////    measure.setData( nullptr, 10 );
//    //    auto & measure2 = acq.getMeasure<1>();

//    //    hub::sensor::
//}

////    const int ref_offset    = 5;
////    constexpr int ref_nAcqs = 10;

////    const int ref2_offset    = 0;
////    constexpr int ref_nAcqs2 = 10;

////    std::cout << "ref_acqs" << std::endl;
////    const hub::sensor::Resolution ref_resolution { hub::sensor::format::Y8, 1 };
////    const hub::sensor::Resolutions ref_resolutions { ref_resolution };
////    const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", ref_resolutions );
////    std::vector<hub::sensor::Acquisition> ref_acqs;
////    //    const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize(
/// ref_resolution /    //    ); /    const int ref_dataSize = ref_resolution.nByte(); /    unsigned
/// char* data    = new unsigned char[ref_dataSize]; /    for ( int iAcq = 0; iAcq < ref_nAcqs;
///++iAcq ) { /        for ( int i = 0; i < ref_dataSize; ++i ) { /            data[i] = ref_offset
///+ iAcq + 1; /        } /        //        ref_acqs.emplace_back( ref_offset + iAcq + 1,
/// ref_offset + iAcq + 1 ); /        ref_acqs.emplace_back( ref_resolutions ); /        auto& acq =
/// ref_acqs.back(); /        acq.setStart( ref_offset + iAcq + 1 ); /        acq.setEnd( ref_offset
///+ iAcq + 1 ); /        acq.getMeasures().at( 0 ).setData( data, ref_dataSize ); /        //
/// ref_acqs.back() << hub::Measure( /        //            reinterpret_cast<const unsigned char*>(
/// data ), ref_dataSize, ref_resolution /        //            ); /        std::cout << acq <<
/// std::endl; /    } /    delete[] data;

////    std::cout << std::endl;

////    //////////////////////

////    std::cout << "ref2_acqs" << std::endl;
////    const hub::sensor::Resolution ref_resolution2 { hub::sensor::format::DOF6, 1 };
////    const hub::sensor::Resolutions ref_resolutions2 { ref_resolution2 };
////    const hub::sensor::SensorSpec ref_sensorSpec2 { "sensorName2", ref_resolutions2 };
////    std::vector<hub::sensor::Acquisition> ref_acqs2;
////    //    const int ref_dataSize2 = hub::sensor::resolution::computeAcquisitionSize(
////    ref_resolution2
////    //    );
////    const int ref_dataSize2 = ref_resolution2.nByte();
////    unsigned char* data2    = new unsigned char[ref_dataSize2];
////    //    float* data2Float       = (float*)data2;
////    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
////        for ( int i = 0; i < ref_dataSize2; ++i ) {
////            data2[i] = ref2_offset + iAcq + 1;
////            //            data2Float[i] = ref2_offset + iAcq + 1;
////        }
////        //        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
////        ref_acqs2.emplace_back( ref_resolutions2 );
////        auto& acq = ref_acqs2.back();
////        acq.setStart( ref2_offset + iAcq + 1 );
////        acq.setEnd( ref2_offset + iAcq + 1 );
////        acq.getMeasures().at( 0 ).setData( data2, ref_dataSize2 );
////        //        ref_acqs2.back() << hub::Measure(
////        //            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2,
////        //            ref_resolution2 );
////        std::cout << acq << std::endl;
////    }
////    delete[] data2;

////    std::cout << std::endl;

////    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2
///);

////    static_assert( !hub::Input::readable_v<hub::sensor::Acquisition> );
////    static_assert( !hub::Input::readable_v<hub::sensor::Format> );
////    static_assert( !hub::Input::readable_v<hub::sensor::Measure> );
////    static_assert( !hub::Input::readable_v<hub::sensor::Measures> );
////    static_assert( !hub::Input::readable_v<hub::sensor::Resolution> );
////    static_assert( !hub::Input::readable_v<hub::sensor::Resolutions> );
////    static_assert( !hub::Input::readable_v<hub::sensor::SensorSpec> );

////    static_assert( !hub::Input::getable_v<hub::sensor::Acquisition> );
////    static_assert( !hub::Input::getable_v<hub::sensor::Format> );
////    static_assert( !hub::Input::getable_v<hub::sensor::Measure> );
////    static_assert( !hub::Input::getable_v<hub::sensor::Measures> );
////    static_assert( !hub::Input::getable_v<hub::sensor::Resolution> );
////    static_assert( !hub::Input::getable_v<hub::sensor::Resolutions> );
////    static_assert( !hub::Input::getable_v<hub::sensor::SensorSpec> );

////    static_assert( !hub::serializable_v<hub::sensor::Acquisition> );
////    static_assert( !hub::serializable_v<hub::sensor::Format> );
////    static_assert( !hub::serializable_v<hub::sensor::Measure> );
////    static_assert( !hub::serializable_v<hub::sensor::Measures> );
////    static_assert( hub::serializable_v<hub::sensor::Resolution> );
////    static_assert( !hub::serializable_v<hub::sensor::Resolutions> );
////    static_assert( !hub::serializable_v<hub::sensor::SensorSpec> );

////    //    static_assert(hub::Input::getable_v<hub::sensor::Resolution>);
////    //    static_assert(! isInput_v<hub::sensor::Resolution>);
////    //    static_assert(isInput_v<hub::Input>);
////    //    static_assert(isInput_v<InputOutput>);

////    {
////        InputOutput inputOutput;

////        int ref_int = 5;
////        inputOutput.write( ref_int );
//////        inputOutput.write( ref_int );
////        int read_int;
////        inputOutput.read( read_int );
////        assert( read_int == ref_int );
//////        const auto read_int2 = inputOutput.get<int>();
//////        assert( read_int2 == ref_int );

//////        inputOutput.write( 4.0 );
//////        auto d = inputOutput.get<double>();
//////        assert( d == 4.0 );

////        constexpr hub::sensor::Format format = hub::sensor::format::RGB8;
////        assert( format == hub::sensor::format::RGB8 );
////        inputOutput.write( format );
////        std::cout << "start reading" << std::endl;
////        hub::sensor::Format format_read;
////        inputOutput.read( format_read );
////        assert( format == format_read );

//////        inputOutput.write( format );
//////        auto format_read2 = inputOutput.get<hub::sensor::Format>();
//////        assert( format == format_read2 );

//////        constexpr auto dims2 = std::vector<int>{1, 2, 3};
//////        const hub::sensor::Resolution::NDim& nDim = { 1, 2, 3 };
//////        inputOutput.write( nDim );
//////        std::cout << "start reading" << std::endl;
//////        hub::sensor::Resolution::NDim dims_read;
//////        inputOutput.read( dims_read );
//////        assert( nDim == dims_read );

//////        inputOutput.write( nDim );
//////        auto dims_read2 = inputOutput.get<hub::sensor::Resolution::NDim>();
//////        assert( nDim == dims_read2 );

//////        static_assert(hub::Input::serializable_v<hub::sensor::Resolution>);
////        static_assert(! hub::serializable_v<hub::sensor::Resolution>);
////        static_assert(! hub::Output::writable_v<hub::sensor::Resolution>);
////        static_assert(! hub::Input::readable_v<hub::sensor::Resolution>);

////        constexpr auto dims0D = hub::sensor::resolution::NDim{};
////        static_assert(dims0D.n() == 0);
////        static_assert(dims0D.capacity() == 1);
////        static_assert(dims0D.ns[0] == 0);

////        constexpr auto dims1D = hub::sensor::resolution::NDim{640};
//////        static_assert(dims1D.n() == 640);
//////        dims1D.width();
//////        dims1D.height();
//////        dims1D.nx();
//////        static_assert(dims1D.height() == 480);
////        static_assert(dims1D.n() == 1);
//////        static_assert(dims1D.get<0>() == 640);
//////        static_assert(dims1D.get(1) == 480);
//////        static_assert(dims1D.n() == 640);
////        static_assert(dims1D.capacity() == 640);
//////        dims1D.width();

////        constexpr auto dims2D = hub::sensor::resolution::NDim{640, 480};
//////        static_assert(dims2D.width() == 640);
//////        static_assert(dims2D.height() == 480);
////        static_assert(dims2D.n() == 2);
////        static_assert(dims2D.ns[0] == 640);
//////        static_assert(dims2D.get<1>() == 480);
//////        static_assert(dims2D.get<2>() == 0);
//////        static_assert(dims2D.get<3>() == 0);
//////        static_assert(dims2D.get<4>() == 0);
//////        static_assert(dims2D.width() == 640);
//////        static_assert(dims2D.height() == 480);
////        static_assert(dims2D.capacity() == 640 * 480);
//////        dims2D.width();

////        static_assert((hub::Size_t)4294967295 == 0xffffffff);
////        static_assert((hub::Size_t)4294967296 == 0);
////        static_assert((hub::Size_t)4294967297 == 1);
////        constexpr auto dims3D = hub::sensor::resolution::NDim{640, 480, 1};
//////        static_assert(dims2D.width() == 640);
//////        static_assert(dims2D.height() == 480);
//////        static_assert(dims3D.size() == 3);
//////        static_assert(dims3D.get<0>() == 640);
////        static_assert(dims3D.ns[1] == 480);
//////        static_assert(dims3D.get<1>() == 480);
//////        static_assert(dims3D.get<2>() == 1);
//////        static_assert(dims3D.get<3>() == 0);
//////        static_assert(dims3D.get<4>() == 0);
//////        static_assert(dims3D.get<5>() == 0);
//////        static_assert(dims3D.nx() == 640);
//////        static_assert(dims3D.ny() == 480);
//////        static_assert(dims3D.nz() == 1);
//////        static_assert(dims3D.capacity() == 640 * 480);
//////        dims3D.width();

////        std::cout << "nDim:" << std::endl;

////        inputOutput.write(dims3D);
//////        hub::sensor::resolution::NDim dims_read;
//////        inputOutput.read(dims_read);
////        const auto dims_read = inputOutput.get<hub::sensor::resolution::NDim>();
////        assert(dims3D == dims_read);
//////        static_assert(dims_read == dims3D);

//////        hub::sensor::DimsT<640, 480, 1> dimsT;
////        using DimsT = hub::sensor::DimsT<640, 480, 1>;
////        hub::sensor::write(DimsT(), inputOutput);
//////        inputOutput.write(DimsT());

////        DimsT dimsT;
////        static_assert(dimsT.get<0>() == 640);

//////        inputOutput.write(dimsT);

//////        DimsT dimsT;
//////        inputOutput.read(dimsT);
////        const auto dimsT_read = inputOutput.get<hub::sensor::resolution::NDim>();
////        assert(dimsT == dimsT_read);

////        return;

////        hub::sensor::resolution::NDim dims_read2;
////        inputOutput.read(dims_read2);

////        using DimsT1 = hub::sensor::DimsT<5>;
////        static_assert(DimsT1::n() == 5);

////        inputOutput.write(DimsT1());

////        using DimsT2 = hub::sensor::DimsT<640, 480>;
////        static_assert(DimsT2::width() == 640);

////        inputOutput.write(DimsT2());

////        using DimsT3 = hub::sensor::DimsT<640, 480, 1>;
////        static_assert(DimsT3::nx() == 640);

////        inputOutput.write(DimsT3());

//////        if (dims_read2.n() == 1) {
//////            dims_read.ns
//////            using DimT_read = hub::sensor::DimsT<dims_read2.ns[0]>();

//////        }

//////        const auto dims_read2 = inputOutput.g

////        return;

////        constexpr hub::sensor::Resolution2 resolution2{hub::sensor::format::RGB8,
////        hub::sensor::nDim::Full_HD}; std::cout << resolution2 << std::endl;

//////        constexpr hub::sensor::Resolution2 resolution3{{3, "RGB", true}, {640, 480}};
////        constexpr hub::sensor::Resolution2 resolution3 = hub::sensor::resolution::RGB8_Full_HD;
////        std::cout << resolution3 << std::endl;

//////        dims_read.

//////        using DimsT1 = hub::sensor::DimsT<640, 480, 1>;
//////        static_assert(DimsT1::size() == 3);

//////        constexpr auto dimsT1 = dims1D.getDimsT();

//////            make_dimsT(dims1D);
//////        constexpr auto dimsT1D = make_dimsT(dims1D);
//////        constexpr auto dimsT2D = (dims1D.size() == 1) ?(1) :(2);
//////        hub::sensor::DimsT<1>();
//////        hub::sensor::DimsT<1, 2>();
//////        constexpr auto dimsT3D = (dims1D.size() == 1) ?(hub::sensor::DimsT<1>())
/////:(hub::sensor::DimsT<1, 2>());

//////        } else if (dims1D.size() == 2) {
//////        }
//////        static_assert(dimsT1D.size() == 1);

//////        constexpr std::tuple<int, int, int> tuple3{1, 2, 3};
//////        static_assert(std::get<0, int>(tuple3) == 1);
//////        static_assert(std::get<1, int>(tuple3) == 2);
//////        static_assert(std::get<2, int>(tuple3) == 3);

//////        InputOutputConstExpr<
//////        constexpr InputExpr inputExpr{0, 1, 2, 3, 4, true, dims1D, dims2D, dims3D, 5};
//////        static_assert(inputOutput.get<0, hub::sensor::resolution::NDim>() == dims3D);
//////        static_assert(inputExpr.get<int>() == 0);
//////        static_assert(inputExpr.get<bool>() == true);
//////        static_assert(inputExpr.get<1>() == dims1D);
//////        static_assert(inputExpr.get<2>() == dims2D);
//////        static_assert(inputExpr.get<3>() == dims3D);

//////        static_assert(inputExpr.get<0, hub::sensor::resolution::NDim>() == dims1D);
//////        static_assert(inputExpr.get<1, hub::sensor::resolution::NDim>() == dims2D);
//////        static_assert(inputExpr.get<2, hub::sensor::resolution::NDim>() == dims3D);

//////        constexpr int i = 0;
//////        for (constexpr i = 0; i < 5; ++i) {
//////            constexpr auto get0 = inputExpr.get<i>();
//////        }
//////        for (const auto & x : tuple3) {

//////        }
//////        constexpr auto get0 = inputExpr.get<0>();
//////        constexpr auto get1 = inputExpr.get<

////        return;

//////        constexpr auto nDim = hub::sensor::resolution::NDim{640};
//////        static_assert(nDim.size() == 1);
//////        static_assert(nDim.get(1) == 480);
//////        nDim.n();
//////        nDim.width();
//////        nDim.nx();

//////        constexpr hub::sensor::ResolutionT res {format, 640, 480};
//////        constexpr hub::sensor::ResolutionT<format, 1, 2, 3> resolution;

//////        constexpr hub::sensor::ResolutionT resolution {format, 1, 2, 3};
//////        static_assert(resolution.nDim.n() == 3);
//////        static_assert(resolution.format == format);
//////        std::cout << resolution.nByte() << std::endl;

//////        static_assert(resolution.nDim. == 3);
//////        hub::sensor::Resolution resolution { format, 1, 2, 3 };
//////        inputOutput.write( resolution );
//////        std::cout << "start reading" << std::endl;
//////        hub::sensor::Resolution resolution_read;
//////        hub::sensor::ResolutionT<format, 1, 2, 3> resolution_read;
//////        inputOutput.read( resolution_read );

//////        constexpr auto resolution_maked = hub::sensor::make_resolution(format, 1, 2);
//////        static_assert(resolution_maked == resolution);

//////        inputOutput.write( resolution );
//////        auto resolution_read2 = inputOutput.get<hub::sensor::Resolution>();
//////        assert( resolution == resolution_read2 );

////        return;

////        std::tuple<int, bool> tuple{2, true};
////        inputOutput.write(tuple);
////        std::tuple<int, bool> tuple2;
////        inputOutput.read(tuple2);
////        assert(tuple == tuple2);

////        //        return;

//////        const std::string sensorName = "sensorName";
//////        inputOutput.write( sensorName );
//////        inputOutput.write( sensorName );
//////        std::cout << "start reading" << std::endl;
//////        std::string sensorName_read;
//////        inputOutput.read( sensorName_read );
//////        assert( sensorName == sensorName_read );
//////        auto sensorName_read2 = inputOutput.get<std::string>();
//////        assert( sensorName == sensorName_read2 );

//////        inputOutput.write( ref_resolutions );
//////        std::cout << "start reading" << std::endl;
//////        hub::sensor::Resolutions resolutions_read;
//////        inputOutput.read( resolutions_read );
//////        assert( ref_resolutions == resolutions_read );

//////        inputOutput.write( ref_resolutions );
//////        auto resolutions_read2 = inputOutput.get<hub::sensor::Resolutions>();
//////        assert( ref_resolutions == resolutions_read2 );

//////        hub::sensor::SensorSpec::MetaData metaData;
//////        metaData["a"] = 5;
//////        inputOutput.write( metaData );
//////        std::cout << "start reading" << std::endl;
//////        hub::sensor::SensorSpec::MetaData metaData_read;
//////        inputOutput.read( metaData_read );
//////        assert( metaData == metaData_read );

//////        inputOutput.write( metaData );
//////        auto metaData_read2 = inputOutput.get<hub::sensor::SensorSpec::MetaData>();
//////        assert( metaData == metaData_read2 );

//////        inputOutput.write( ref_sensorSpec );
//////        hub::sensor::SensorSpec ref_sensorSpec_read =
/////inputOutput.get<hub::sensor::SensorSpec>(); /        //                inputOutput.read(
/////ref_sensorSpec_read ); /        assert( ref_sensorSpec == ref_sensorSpec_read );

////        //        //        InputOutput inputOutput2;

////        //        //            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
////        //        //            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

////        //        assert( inputOutput.isEnd() );
////        //        hub::sensor::OutputSensor outputSensor( inputOutput, ref_sensorSpec );
////        //        assert( !inputOutput.isEnd() );
////        //        assert( outputSensor.getSpec() == ref_sensorSpec );
////        //        //            hub::sensor::OutputSensor outputSensor2( inputOutput2,
////        //        ref_sensorSpec2 );
////        //        //            assert(outputSensor2.getSpec() == ref_sensorSpec2);

////        //        assert( !inputOutput.isEnd() );
////        //        hub::sensor::InputSensor inputSensor( inputOutput );
////        //        //        std::cout << "ref_sensorSpec: " << ref_sensorSpec << std::endl;
////        //        assert( inputSensor.getSpec() == ref_sensorSpec );
////        //        //        std::cout << "inputSensor.getSpec(): " << inputSensor.getSpec() <<
////        //        std::endl; assert( inputOutput.isEnd() );
////        //        //        assert( inputSensor.getSpec() == ref_sensorSpec );

////        //        for ( const auto& acq : ref_acqs ) {
////        //            outputSensor << acq;
////        //            std::cout << "write acq : " << acq << std::endl;
////        //        }

////        //        auto acq_read = inputSensor.acq();
////        //        while ( !inputOutput.isEnd() ) {
////        //            inputSensor >> acq_read;
////        //            std::cout << "read acq : " << acq_read << std::endl;
////        //        }

////        //            for ( const auto& acq2 : ref_acqs2 ) {
////        //                outputSensor2 << acq2;
////        //                std::cout << "write acq2 : " << acq2 << std::endl;
////        //            }

////        //        auto inputFile  = hub::input::InputFile( "filepath.txt" );
////        //        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );

////        //        hub::sensor::InputSensor inputSensor2( inputOutput2 );
////        //            assert(inputSensor2.getSpec() == ref_sensorSpec);

////        //        auto acq = inputOutput.get<hub::sensor::Acquisition>();

////        //        auto acq2_read = inputSensor2.acq();
////        //        while ( !inputOutput2.isEnd() ) {
////        //            inputSensor2 >> acq2_read;
////        //            std::cout << "read acq2 : " << acq2_read << std::endl;
////        //        }

////        //        hub::sensor::Acquisition acq;
////        //        std::cout << "sync acqs" << std::endl;
////        //        std::vector<hub::sensor::Acquisition> sync_acqs;
////        //        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() )
///{

////        //            inputSensor >> inputSensor2 >> acq;
////        //            std::cout << acq << std::endl;
////        //            sync_acqs.push_back( std::move( acq ) );
////        //        }

////        //        std::cout << std::endl;

////        //        assert( sync_acqs.size() == ref_sync_acqs.size() );
////        //        for ( int i = 0; i < sync_acqs.size(); ++i ) {
////        //            const auto& acq2 = sync_acqs.at( i );

////        //            CHECK( acq2 == ref_sync_acqs.at( i ) );
////        //        }
////    }

////    //    ///////////////////////////////// TESTING EACH INPUT IMPLEMENTS

////    //    std::cout << "----------------------------------------- Memory "
////    //                 "----------------------------------------"
////    //                 "---------------------"
////    //              << std::endl;

////    //    {
////    //        std::vector<char> buff;
////    //        hub::output::OutputMemory<decltype( buff )> outputMemory( buff );
////    //        hub::input::InputMemory<decltype( buff )> inputMemory( buff );
////    //        std::vector<char> buff2;
////    //        hub::output::OutputMemory<decltype( buff2 )> outputMemory2( buff2 );
////    //        hub::input::InputMemory<decltype( buff2 )> inputMemory2( buff2 );

////    //        checkSynchronize( std::move( outputMemory ),
////    //                          ref_sensorSpec,
////    //                          ref_acqs,
////    //                          std::move( outputMemory2 ),
////    //                          ref_sensorSpec2,
////    //                          ref_acqs2,
////    //                          std::move( inputMemory ),
////    //                          std::move( inputMemory2 ),
////    //                          ref_sync_acqs );
////    //    }

////    //    std::cout
////    //        << "----------------------------------------- File
////    //        ----------------------------------------"
////    //           "---------------------"
////    //        << std::endl;

////    //    {
////    //        {
////    //            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
////    //            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

////    //            hub::sensor::OutputSensor outputSensor( ref_sensorSpec, std::move( outputFile
///) /    ); /    //            hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, std::move(
/// outputFile2 /    ) /    //            );

////    //            for ( const auto& acq : ref_acqs ) {
////    //                outputSensor << acq;
////    //            }
////    //            for ( const auto& acq2 : ref_acqs2 ) {
////    //                outputSensor2 << acq2;
////    //            }
////    //        }

////    //        auto inputFile  = hub::input::InputFile( "filepath.txt" );
////    //        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );
////    //        hub::sensor::InputSensor inputSensor( std::move( inputFile ) );
////    //        hub::sensor::InputSensor inputSensor2( std::move( inputFile2 ) );

////    //        hub::sensor::Acquisition acq;
////    //        std::cout << "sync acqs" << std::endl;
////    //        std::vector<hub::sensor::Acquisition> sync_acqs;
////    //        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

////    //            inputSensor >> inputSensor2 >> acq;
////    //            std::cout << acq << std::endl;
////    //            sync_acqs.push_back( std::move( acq ) );
////    //        }

////    //        std::cout << std::endl;

////    //        assert( sync_acqs.size() == ref_sync_acqs.size() );
////    //        for ( int i = 0; i < sync_acqs.size(); ++i ) {
////    //            const auto& acq2 = sync_acqs.at( i );

////    //            CHECK( acq2 == ref_sync_acqs.at( i ) );
////    //        }
////    //    }

////    //    std::cout << "----------------------------------------- Stream "
////    //                 "----------------------------------------"
////    //                 "---------------------"
////    //              << std::endl;

////    // #ifdef HUB_BUILD_SERVER
////    //     auto outputSensor  = hub::sensor::OutputSensor(ref_sensorSpec,
////    hub::output::OutputStream(
////    //     FILE_NAME )); auto outputSensor2 = hub::sensor::OutputSensor(ref_sensorSpec2,
////    //     hub::output::OutputStream( FILE_NAME + "2" )); auto inputSensor   =
////    //     hub::sensor::InputSensor(hub::input::InputStream( FILE_NAME )); auto inputSensor2  =
////    //     hub::sensor::InputSensor(hub::input::InputStream( FILE_NAME + "2" ));

////    //        checkSynchronize( outputSensor.getOutput() ,
////    //                          ref_acqs,
////    //                          outputSensor2.getOutput() ,
////    //                          ref_acqs2,
////    //                          ref_sync_acqs,
////    //                          false,
////    //                          inputSensor.getInput(),
////    //                          inputSensor2.getInput()
////    //                      );
////    // #else
////    //    {
////    //        auto outputStream  = hub::output::OutputStream( FILE_NAME );
////    //        auto outputStream2 = hub::output::OutputStream( FILE_NAME "2" );
////    //        auto inputStream   = hub::input::InputStream( FILE_NAME );
////    //        auto inputStream2  = hub::input::InputStream( FILE_NAME "2" );

////    //        checkSynchronize( std::move( outputStream ),
////    //                          ref_sensorSpec,
////    //                          ref_acqs,
////    //                          std::move( outputStream2 ),
////    //                          ref_sensorSpec2,
////    //                          ref_acqs2,
////    //                          std::move( inputStream ),
////    //                          std::move( inputStream2 ),
////    //                          ref_sync_acqs,
////    //                          true );
////    //    }
////    // #endif

////    ///////////////////////////////////////////////////////////////////////////////////////////
////}

//////    std::vector<char> buff;

//////    const auto resolution = hub::sensor::Resolution { hub::sensor::format::BGR8, 1 };
//////    const hub::sensor::SensorSpec sensorSpec( "sensorName", { resolution } );
//////    unsigned char data[3] { 1, 2, 3 };
//////    hub::sensor::OutputSensor outputSensor( sensorSpec, hub::io::Memory<decltype( buff )>(
/// buff
/////) /    );

//////    auto acq = std::move( hub::sensor::Acquisition { 0, 1 } << hub::Measure { data, 3,
/////resolution /    } ); outputSensor << acq; outputSensor << acq; outputSensor << acq;

//////    hub::sensor::InputSensor inputSensor { hub::io::Memory<decltype( buff )>( buff ) };

//////    hub::sensor::Acquisition acq2;
//////    inputSensor >> acq2;
//////    CHECK( acq == acq2 );

//////    const auto& input = inputSensor.getInput();
//////    CHECK( !input.isEnd() );

//////    auto acqs = inputSensor.getAllAcquisitions();
//////    CHECK( acqs.size() == 2 );
//////    CHECK( acqs[0] == acq );
//////    CHECK( acqs[1] == acq );
//////    CHECK( input.isEnd() );
