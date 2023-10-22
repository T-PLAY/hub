#include "test_common.hpp"
// #include "sensor/test_sensor_utils.hpp"

#include <vector>

#include "core/test-core-common.hpp"
#include "sensor/test-sensor-common.hpp"

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

// #include <io/Memory.hpp>

TEST_CASE( "InputSensor test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    const int ref2_offset    = 0;
    constexpr int ref_nAcqs2 = 10;

    std::cout << "ref_acqs" << std::endl;
    const hub::sensor::Resolution ref_resolution { hub::sensor::format::Y8, 1 };
    const hub::sensor::Resolutions ref_resolutions { ref_resolution };
    const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", ref_resolutions );
    std::vector<hub::sensor::Acquisition> ref_acqs;
    //    const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize( ref_resolution
    //    );
    const int ref_dataSize = ref_resolution.nByte();
    unsigned char* data    = new unsigned char[ref_dataSize];
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = ref_offset + iAcq + 1;
        }
        //        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 1 );
        ref_acqs.emplace_back( ref_resolutions );
        auto& acq = ref_acqs.back();
        acq.setStart( ref_offset + iAcq + 1 );
        acq.setEnd( ref_offset + iAcq + 1 );
        acq.getMeasures().at( 0 ).setData( data, ref_dataSize );
        //        ref_acqs.back() << hub::Measure(
        //            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution
        //            );
        std::cout << acq << std::endl;
    }
    delete[] data;

    std::cout << std::endl;

    //////////////////////

    std::cout << "ref2_acqs" << std::endl;
    const hub::sensor::Resolution ref_resolution2 { hub::sensor::format::DOF6, 1 };
    const hub::sensor::Resolutions ref_resolutions2 { ref_resolution2 };
    const hub::sensor::SensorSpec ref_sensorSpec2 { "sensorName2", ref_resolutions2 };
    std::vector<hub::sensor::Acquisition> ref_acqs2;
    //    const int ref_dataSize2 = hub::sensor::resolution::computeAcquisitionSize( ref_resolution2
    //    );
    const int ref_dataSize2 = ref_resolution2.nByte();
    unsigned char* data2    = new unsigned char[ref_dataSize2];
    //    float* data2Float       = (float*)data2;
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize2; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
            //            data2Float[i] = ref2_offset + iAcq + 1;
        }
        //        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
        ref_acqs2.emplace_back( ref_resolutions2 );
        auto& acq = ref_acqs2.back();
        acq.setStart( ref2_offset + iAcq + 1 );
        acq.setEnd( ref2_offset + iAcq + 1 );
        acq.getMeasures().at( 0 ).setData( data2, ref_dataSize2 );
        //        ref_acqs2.back() << hub::Measure(
        //            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2,
        //            ref_resolution2 );
        std::cout << acq << std::endl;
    }
    delete[] data2;

    std::cout << std::endl;

    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );


    static_assert( !hub::Input::readable_v<hub::sensor::Acquisition> );
    static_assert( !hub::Input::readable_v<hub::sensor::Format> );
    static_assert( !hub::Input::readable_v<hub::sensor::Measure> );
    static_assert( !hub::Input::readable_v<hub::sensor::Measures> );
    static_assert( !hub::Input::readable_v<hub::sensor::Resolution> );
    static_assert( !hub::Input::readable_v<hub::sensor::Resolutions> );
    static_assert( hub::Input::readable_v<hub::sensor::SensorSpec> );

    static_assert( !hub::Input::getable_v<hub::sensor::Acquisition> );
    static_assert( !hub::Input::getable_v<hub::sensor::Format> );
    static_assert( !hub::Input::getable_v<hub::sensor::Measure> );
    static_assert( !hub::Input::getable_v<hub::sensor::Measures> );
    static_assert( !hub::Input::getable_v<hub::sensor::Resolution> );
    static_assert( !hub::Input::getable_v<hub::sensor::Resolutions> );
    static_assert( hub::Input::getable_v<hub::sensor::SensorSpec> );

    static_assert( !hub::serializable_v<hub::sensor::Acquisition> );
    static_assert( !hub::serializable_v<hub::sensor::Format> );
    static_assert( !hub::serializable_v<hub::sensor::Measure> );
    static_assert( !hub::serializable_v<hub::sensor::Measures> );
    static_assert( hub::serializable_v<hub::sensor::Resolution> );
    static_assert( !hub::serializable_v<hub::sensor::Resolutions> );
    static_assert( !hub::serializable_v<hub::sensor::SensorSpec> );

    //    static_assert(hub::Input::getable_v<hub::sensor::Resolution>);
    //    static_assert(! isInput_v<hub::sensor::Resolution>);
    //    static_assert(isInput_v<hub::Input>);
    //    static_assert(isInput_v<InputOutput>);

    {
        InputOutput inputOutput;

        int ref_int = 5;
        inputOutput.write( ref_int );
        inputOutput.write( ref_int );
        int read_int;
        inputOutput.read( read_int );
        assert( read_int == ref_int );
        const auto read_int2 = inputOutput.get<int>();
        assert( read_int2 == ref_int );

        inputOutput.write( 4.0 );
        auto d = inputOutput.get<double>();
        assert( d == 4.0 );

        constexpr hub::sensor::Format format = hub::sensor::format::RGB8;
        assert( format == hub::sensor::format::RGB8 );
        inputOutput.write( format );
        std::cout << "start reading" << std::endl;
        hub::sensor::Format format_read;
        inputOutput.read( format_read );
        assert( format == format_read );

        inputOutput.write( format );
        auto format_read2 = inputOutput.get<hub::sensor::Format>();
        assert( format == format_read2 );



//        constexpr auto dims2 = std::vector<int>{1, 2, 3};
//        const hub::sensor::Resolution::Dims& dims = { 1, 2, 3 };
//        inputOutput.write( dims );
//        std::cout << "start reading" << std::endl;
//        hub::sensor::Resolution::Dims dims_read;
//        inputOutput.read( dims_read );
//        assert( dims == dims_read );

//        inputOutput.write( dims );
//        auto dims_read2 = inputOutput.get<hub::sensor::Resolution::Dims>();
//        assert( dims == dims_read2 );

//        static_assert(hub::Input::serializable_v<hub::sensor::Resolution>);
        static_assert(hub::serializable_v<hub::sensor::Resolution>);
        static_assert(! hub::Output::writable_v<hub::sensor::Resolution>);
        static_assert(! hub::Input::readable_v<hub::sensor::Resolution>);
        constexpr hub::sensor::ResolutionT<format, 1, 2, 3> resolution;
        static_assert(resolution.nDim() == 3);
//        hub::sensor::Resolution resolution { format, 1, 2, 3 };
        inputOutput.write( resolution );
//        std::cout << "start reading" << std::endl;
//        hub::sensor::Resolution resolution_read;
        hub::sensor::ResolutionT<format, 1, 2, 3> resolution_read;
//        inputOutput.read( resolution_read );

//        constexpr auto resolution_maked = hub::sensor::make_resolution(format, 1, 2);
//        static_assert(resolution_maked == resolution);

//        inputOutput.write( resolution );
//        auto resolution_read2 = inputOutput.get<hub::sensor::Resolution>();
//        assert( resolution == resolution_read2 );

        return;

        std::tuple<int, bool> tuple{2, true};
        inputOutput.write(tuple);
        std::tuple<int, bool> tuple2;
        inputOutput.read(tuple2);
        assert(tuple == tuple2);



        //        return;

        const std::string sensorName = "sensorName";
        inputOutput.write( sensorName );
        inputOutput.write( sensorName );
        std::cout << "start reading" << std::endl;
        std::string sensorName_read;
        inputOutput.read( sensorName_read );
        assert( sensorName == sensorName_read );
        auto sensorName_read2 = inputOutput.get<std::string>();
        assert( sensorName == sensorName_read2 );

//        inputOutput.write( ref_resolutions );
//        std::cout << "start reading" << std::endl;
//        hub::sensor::Resolutions resolutions_read;
//        inputOutput.read( resolutions_read );
//        assert( ref_resolutions == resolutions_read );

//        inputOutput.write( ref_resolutions );
//        auto resolutions_read2 = inputOutput.get<hub::sensor::Resolutions>();
//        assert( ref_resolutions == resolutions_read2 );

        hub::sensor::SensorSpec::MetaData metaData;
        metaData["a"] = 5;
        inputOutput.write( metaData );
        std::cout << "start reading" << std::endl;
        hub::sensor::SensorSpec::MetaData metaData_read;
        inputOutput.read( metaData_read );
        assert( metaData == metaData_read );

        inputOutput.write( metaData );
        auto metaData_read2 = inputOutput.get<hub::sensor::SensorSpec::MetaData>();
        assert( metaData == metaData_read2 );

        inputOutput.write( ref_sensorSpec );
        hub::sensor::SensorSpec ref_sensorSpec_read = inputOutput.get<hub::sensor::SensorSpec>();
        //                inputOutput.read( ref_sensorSpec_read );
        assert( ref_sensorSpec == ref_sensorSpec_read );

        //        //        InputOutput inputOutput2;

        //        //            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
        //        //            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

        //        assert( inputOutput.isEmpty() );
        //        hub::sensor::OutputSensor outputSensor( inputOutput, ref_sensorSpec );
        //        assert( !inputOutput.isEmpty() );
        //        assert( outputSensor.getSpec() == ref_sensorSpec );
        //        //            hub::sensor::OutputSensor outputSensor2( inputOutput2,
        //        ref_sensorSpec2 );
        //        //            assert(outputSensor2.getSpec() == ref_sensorSpec2);

        //        assert( !inputOutput.isEmpty() );
        //        hub::sensor::InputSensor inputSensor( inputOutput );
        //        //        std::cout << "ref_sensorSpec: " << ref_sensorSpec << std::endl;
        //        assert( inputSensor.getSpec() == ref_sensorSpec );
        //        //        std::cout << "inputSensor.getSpec(): " << inputSensor.getSpec() <<
        //        std::endl; assert( inputOutput.isEmpty() );
        //        //        assert( inputSensor.getSpec() == ref_sensorSpec );

        //        for ( const auto& acq : ref_acqs ) {
        //            outputSensor << acq;
        //            std::cout << "write acq : " << acq << std::endl;
        //        }

        //        auto acq_read = inputSensor.acq();
        //        while ( !inputOutput.isEmpty() ) {
        //            inputSensor >> acq_read;
        //            std::cout << "read acq : " << acq_read << std::endl;
        //        }

        //            for ( const auto& acq2 : ref_acqs2 ) {
        //                outputSensor2 << acq2;
        //                std::cout << "write acq2 : " << acq2 << std::endl;
        //            }

        //        auto inputFile  = hub::input::InputFile( "filepath.txt" );
        //        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );

        //        hub::sensor::InputSensor inputSensor2( inputOutput2 );
        //            assert(inputSensor2.getSpec() == ref_sensorSpec);

        //        auto acq = inputOutput.get<hub::sensor::Acquisition>();

        //        auto acq2_read = inputSensor2.acq();
        //        while ( !inputOutput2.isEmpty() ) {
        //            inputSensor2 >> acq2_read;
        //            std::cout << "read acq2 : " << acq2_read << std::endl;
        //        }

        //        hub::sensor::Acquisition acq;
        //        std::cout << "sync acqs" << std::endl;
        //        std::vector<hub::sensor::Acquisition> sync_acqs;
        //        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

        //            inputSensor >> inputSensor2 >> acq;
        //            std::cout << acq << std::endl;
        //            sync_acqs.push_back( std::move( acq ) );
        //        }

        //        std::cout << std::endl;

        //        assert( sync_acqs.size() == ref_sync_acqs.size() );
        //        for ( int i = 0; i < sync_acqs.size(); ++i ) {
        //            const auto& acq2 = sync_acqs.at( i );

        //            CHECK( acq2 == ref_sync_acqs.at( i ) );
        //        }
    }

    //    ///////////////////////////////// TESTING EACH INPUT IMPLEMENTS

    //    std::cout << "----------------------------------------- Memory "
    //                 "----------------------------------------"
    //                 "---------------------"
    //              << std::endl;

    //    {
    //        std::vector<char> buff;
    //        hub::output::OutputMemory<decltype( buff )> outputMemory( buff );
    //        hub::input::InputMemory<decltype( buff )> inputMemory( buff );
    //        std::vector<char> buff2;
    //        hub::output::OutputMemory<decltype( buff2 )> outputMemory2( buff2 );
    //        hub::input::InputMemory<decltype( buff2 )> inputMemory2( buff2 );

    //        checkSynchronize( std::move( outputMemory ),
    //                          ref_sensorSpec,
    //                          ref_acqs,
    //                          std::move( outputMemory2 ),
    //                          ref_sensorSpec2,
    //                          ref_acqs2,
    //                          std::move( inputMemory ),
    //                          std::move( inputMemory2 ),
    //                          ref_sync_acqs );
    //    }

    //    std::cout
    //        << "----------------------------------------- File
    //        ----------------------------------------"
    //           "---------------------"
    //        << std::endl;

    //    {
    //        {
    //            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
    //            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

    //            hub::sensor::OutputSensor outputSensor( ref_sensorSpec, std::move( outputFile ) );
    //            hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, std::move( outputFile2 )
    //            );

    //            for ( const auto& acq : ref_acqs ) {
    //                outputSensor << acq;
    //            }
    //            for ( const auto& acq2 : ref_acqs2 ) {
    //                outputSensor2 << acq2;
    //            }
    //        }

    //        auto inputFile  = hub::input::InputFile( "filepath.txt" );
    //        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );
    //        hub::sensor::InputSensor inputSensor( std::move( inputFile ) );
    //        hub::sensor::InputSensor inputSensor2( std::move( inputFile2 ) );

    //        hub::sensor::Acquisition acq;
    //        std::cout << "sync acqs" << std::endl;
    //        std::vector<hub::sensor::Acquisition> sync_acqs;
    //        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

    //            inputSensor >> inputSensor2 >> acq;
    //            std::cout << acq << std::endl;
    //            sync_acqs.push_back( std::move( acq ) );
    //        }

    //        std::cout << std::endl;

    //        assert( sync_acqs.size() == ref_sync_acqs.size() );
    //        for ( int i = 0; i < sync_acqs.size(); ++i ) {
    //            const auto& acq2 = sync_acqs.at( i );

    //            CHECK( acq2 == ref_sync_acqs.at( i ) );
    //        }
    //    }

    //    std::cout << "----------------------------------------- Stream "
    //                 "----------------------------------------"
    //                 "---------------------"
    //              << std::endl;

    // #ifdef HUB_BUILD_SERVER
    //     auto outputSensor  = hub::sensor::OutputSensor(ref_sensorSpec, hub::output::OutputStream(
    //     FILE_NAME )); auto outputSensor2 = hub::sensor::OutputSensor(ref_sensorSpec2,
    //     hub::output::OutputStream( FILE_NAME + "2" )); auto inputSensor   =
    //     hub::sensor::InputSensor(hub::input::InputStream( FILE_NAME )); auto inputSensor2  =
    //     hub::sensor::InputSensor(hub::input::InputStream( FILE_NAME + "2" ));

    //        checkSynchronize( outputSensor.getOutput() ,
    //                          ref_acqs,
    //                          outputSensor2.getOutput() ,
    //                          ref_acqs2,
    //                          ref_sync_acqs,
    //                          false,
    //                          inputSensor.getInput(),
    //                          inputSensor2.getInput()
    //                      );
    // #else
    //    {
    //        auto outputStream  = hub::output::OutputStream( FILE_NAME );
    //        auto outputStream2 = hub::output::OutputStream( FILE_NAME "2" );
    //        auto inputStream   = hub::input::InputStream( FILE_NAME );
    //        auto inputStream2  = hub::input::InputStream( FILE_NAME "2" );

    //        checkSynchronize( std::move( outputStream ),
    //                          ref_sensorSpec,
    //                          ref_acqs,
    //                          std::move( outputStream2 ),
    //                          ref_sensorSpec2,
    //                          ref_acqs2,
    //                          std::move( inputStream ),
    //                          std::move( inputStream2 ),
    //                          ref_sync_acqs,
    //                          true );
    //    }
    // #endif

    ///////////////////////////////////////////////////////////////////////////////////////////
}

//    std::vector<char> buff;

//    const auto resolution = hub::sensor::Resolution { hub::sensor::format::BGR8, 1 };
//    const hub::sensor::SensorSpec sensorSpec( "sensorName", { resolution } );
//    unsigned char data[3] { 1, 2, 3 };
//    hub::sensor::OutputSensor outputSensor( sensorSpec, hub::io::Memory<decltype( buff )>( buff )
//    );

//    auto acq = std::move( hub::sensor::Acquisition { 0, 1 } << hub::Measure { data, 3, resolution
//    } ); outputSensor << acq; outputSensor << acq; outputSensor << acq;

//    hub::sensor::InputSensor inputSensor { hub::io::Memory<decltype( buff )>( buff ) };

//    hub::sensor::Acquisition acq2;
//    inputSensor >> acq2;
//    CHECK( acq == acq2 );

//    const auto& input = inputSensor.getInput();
//    CHECK( !input.isEnd() );

//    auto acqs = inputSensor.getAllAcquisitions();
//    CHECK( acqs.size() == 2 );
//    CHECK( acqs[0] == acq );
//    CHECK( acqs[1] == acq );
//    CHECK( input.isEnd() );
