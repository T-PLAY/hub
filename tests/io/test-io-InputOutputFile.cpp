// #include <catch2/catch_test_macros.hpp>
#include "test_common.hpp"
// #include "sensor/test_sensor_common.hpp"

// #include <sensor/InputSensor.hpp>
// #include <sensor/OutputSensor.hpp>
//  #include <io/File.hpp>
#include <io/input/InputFile.hpp>
#include <io/output/OutputFile.hpp>

#if CPP_VERSION >= 17
#    include <filesystem>
#endif

#include <thread>

struct UserDefined {
    int a;
    bool b;
    double c;
    hub::Size_t dataSize() const { return sizeof( int ); }
    auto toString() const {
        return std::to_string( a ) + ":" + std::to_string( b ) + ":" + std::to_string( c );
    }
    bool operator==(const UserDefined & other) {
        return a == other.a && b == other.b && c == other.c;
    }
};

TEST_CASE( "File test" ) {
    TEST_BEGIN()

    const std::string filename = FILE_NAME + ".txt";

#if CPP_VERSION >= 17
    std::filesystem::remove( filename );
#endif

    // std::vector<unsigned char> vector( 1'000 );
    // std::generate( vector.begin(), vector.end(), rand );
    // std::cout << "data: " << vector << std::endl;

    // static_assert( hub::readable_v<hub::io::Header> );
    // static_assert( hub::writable_v<hub::io::Header> );
    const UserDefined userDefined { 1, true, 2.0 };
    const auto header_ref = hub::io::make_header( userDefined );

    const int a_ref       = 5;
    {
        hub::output::OutputFile outputFile( header_ref, filename );
        outputFile.write( a_ref );

        // outputFile.write( vector.data(), vector.size() );

        //        outputSensorBench(outputFile);
    }

    int a_read;
    {
        hub::input::InputFile inputFile( filename );
        assert( inputFile.getHeader() == header_ref );
        inputFile.read( a_read );

        assert( a_ref == a_read );

        hub::io::Memory memory(inputFile.getHeader().getUserDefined());
        UserDefined userDefined_read;
        memory.read(userDefined_read);
        assert(userDefined == userDefined_read);

        // std::vector<unsigned char> vector_read( 1'000 );
        // inputFile.read( vector_read.data(), vector.size() );
        // assert( vector == vector_read );

        //        inputSensorBench(inputFile);
        assert( inputFile.isEnd() );
    }

    //    std::vector<hub::sensor::Acquisition> acqs;
    //    constexpr int nAcqs = 100;
    //    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //        const unsigned char data[3] = {
    //            (unsigned char)iAcq, (unsigned char)( iAcq + 1 ), (unsigned char)( iAcq + 2 ) };
    //        acqs.push_back( hub::sensor::Acquisition( iAcq, iAcq ) );
    //        acqs.back() << hub::Measure( data, 3, { { 1 }, hub::sensor::Format::BGR8 } );
    //        CHECK( acqs.back().getSize() == 3 );
    //    }
    //    CHECK( acqs[0] != acqs[1] );

    //    std::cout << "outputStream start" << std::endl;
    //    INFO( "OutputStream" );
    //    {
    ////        hub::output::OutputFile outputFile(filename);

    //        hub::sensor::OutputSensor outputSensor(
    //            hub::sensor::SensorSpec { "sensorName", { { { 1 }, hub::sensor::Format::BGR8 } }
    //            },
    ////            hub::input::InputFile(
    ////                std::fstream( filename, std::ios::out | std::ios::binary | std::ios::trunc )
    ///) );
    //            hub::output::OutputFile(filename));

    //        const auto& sensorSpec = outputSensor.getSpec();
    //        CHECK( sensorSpec.getAcquisitionSize() == 3 );
    //        CHECK( sensorSpec.getSensorName() == "sensorName" );
    //        CHECK( sensorSpec.getResolutions().size() == 1 );
    //        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
    //        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
    //        CHECK( sensorSpec.getResolutions()[0].second == hub::sensor::Format::BGR8 );

    //        for ( const auto& acq : acqs ) {
    //            outputSensor << acq;
    //        }
    //        assert( outputSensor.getOutput().isOpen() );
    //        std::cout << "outputStream start end" << std::endl;
    //    }
    //    std::cout << "outputStream end ################################" << std::endl;

    //    std::cout << "inputStream start" << std::endl;
    //    INFO( "InputStream" );
    //    {
    ////        hub::input::InputFile inputFile(filename);
    ////        hub::sensor::InputSensor inputSensor(
    ////            hub::input::InputFile(filename));
    //        auto inputSensor = hub::sensor::InputSensor(hub::input::InputFile(filename));
    ////            hub::input::InputFile( std::fstream( filename, std::ios::in | std::ios::binary ) ) );
    //        const auto& input = inputSensor.getInput();

    //        const auto& sensorSpec = inputSensor.getSpec();
    //        CHECK( sensorSpec.getAcquisitionSize() == 3 );
    //        CHECK( sensorSpec.getSensorName() == "sensorName" );
    //        CHECK( sensorSpec.getResolutions().size() == 1 );
    //        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
    //        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
    //        CHECK( sensorSpec.getResolutions()[0].second == hub::sensor::Format::BGR8 );
    //        std::cout << "####### compare acqs" << std::endl;
    //        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //            hub::sensor::Acquisition acq;
    //            inputSensor >> acq;
    //            assert( acq == acqs[iAcq] );
    //            CHECK( acq == acqs[iAcq] );
    //        }
    //        CHECK( input.isEnd() );
    //    }
    //    std::cout << "inputStream end #################################" << std::endl;
    TEST_END()
}
