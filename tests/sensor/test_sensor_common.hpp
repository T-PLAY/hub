
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <random>
#include <set>
#include <thread>
#include <tuple>

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/io/InputOutput.hpp>
#include <sensor/Acquisition.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

// #include "Macros.hpp"
// #include "Version.hpp"

namespace test {
namespace sensor {

static int computeDist( const hub::sensor::Acquisition& acq,
                        const hub::sensor::Acquisition& acq2 ) {
    return std::abs( acq.getStart() - acq2.getStart() );
}

static std::vector<hub::sensor::Acquisition>
computeSyncAcqs( const std::vector<hub::sensor::Acquisition>& leftAcqs,
                 const std::vector<hub::sensor::Acquisition>& rightAcqs ) {
    std::vector<int> min_dists( rightAcqs.size(), 999999 );
    std::vector<int> iLeftMinDists( rightAcqs.size(), -1 );

    int iRightAcq = 0;
    for ( const auto& rightAcq : rightAcqs ) {
        int iLeftMinDist = 0;
        int minDist      = computeDist( rightAcq, leftAcqs.front() );
        for ( int iLeftAcq = 1; iLeftAcq < leftAcqs.size(); ++iLeftAcq ) {
            const auto& leftAcq = leftAcqs.at( iLeftAcq );
            const auto dist     = computeDist( rightAcq, leftAcq );
            if ( dist <= minDist ) {
                minDist      = dist;
                iLeftMinDist = iLeftAcq;
            }
        }

        //        int leftLeftMinDist;
        //        int leftRightMinDist;

        const auto& leftMinAcq = leftAcqs.at( iLeftMinDist );

        if ( !( leftMinAcq.getStart() < rightAcq.getStart() &&
                iLeftMinDist == leftAcqs.size() - 1 ) &&
             !( rightAcq.getStart() < leftMinAcq.getStart() && iLeftMinDist == 0 ) ) {

            //        if ( !( ( iLeftMinDist == 0 || iLeftMinDist == leftAcqs.size() - 1 ) &&
            //        minDist != 0 ) ) { if ( !(iLeftMinDist == 0 && minDist != 0))

            //            if ( minDist < min_dists.at( iLeftMinDist ) ) {
            min_dists[iRightAcq]     = minDist;
            iLeftMinDists[iRightAcq] = iLeftMinDist;
        }
        //            }
        ++iRightAcq;
    }

    std::vector<hub::sensor::Acquisition> syncAcqs;

    //    const auto & resolutionsLeft = leftAcqs.at(0).getResolution();
    //    const auto & resolutionsRight = rightAcqs.at(0).getResolution();
    //    auto & resolutionLeftRight = leftAcqs << resolutionsRight;
    //    hub::sensor::Resolutions resolutionsLeftRight { resolutionsLeft };
    //    resolutionsLeftRight.insert(resolutionsLeftRight.end(), resolutionsRight.begin(),
    //    resolutionsRight.end()); auto sync_acq_resolution = leftAcqs.at(0) << resolutionsRight;
    //    auto sync_acq = hub::sensor::make_acquisition(sync_acq_resolution);
    //    auto sync_acq = leftAcqs.at(0) << rightAcqs.at(0);
    //    auto & sync_acq = hub::sensor::make_acquisition(resolutionsLeft << resolutionsRight);

    //    std::cout << resolutionsLeftRight << std::endl;

    //    std::cout << "ref_sync_acqs" << std::endl;

    for ( int iRightAcq = 0; iRightAcq < rightAcqs.size(); ++iRightAcq ) {

        int iLeftMinDist = iLeftMinDists.at( iRightAcq );
        if ( iLeftMinDist != -1 ) {
            const auto& leftAcq  = leftAcqs.at( iLeftMinDist );
            const auto& rightAcq = rightAcqs.at( iRightAcq );

            //            hub::sensor::Acquisition sync_acq( rightAcq.getStart(), rightAcq.getEnd()
            //            ); hub::sensor::Acquisition sync_acq{ resolutionsLeftRight }; auto
            //            sync_acq = leftAcq << rightAcq;
            auto sync_acq    = leftAcq << rightAcq;
            sync_acq.start() = rightAcq.getStart();
            sync_acq.end()   = rightAcq.getEnd();
            //            sync_acq << leftAcq.getMeasures() << rightAcq.getMeasures();
            //            auto & measures = sync_acq.getMeasures();
            //            const auto & leftMeasures = leftAcq.getMeasures();
            //            for (int i = 0; i < leftMeasures.size(); ++i) {
            //                const auto & leftMeasure = leftMeasures.at(i);
            //                measures.at(i).setData(leftMeasure.getData(), leftMeasure.nByte());
            //            }
            //            const auto & rightMeasures = rightAcq.getMeasures();
            //            for (int i = 0; i < rightMeasures.size(); ++i) {
            //                const auto & rightMeasure = rightMeasures.at(i);
            //                measures.at(leftMeasures.size() + i).setData(rightMeasure.getData(),
            //                rightMeasure.nByte());
            //            }
            syncAcqs.push_back( std::move( sync_acq ) );

            auto& syncAcq = syncAcqs.back();
            //                syncAcq << rightAcq.getMeasures();
            //            std::cout << "left acq: " << leftAcq << std::endl;
            //            std::cout << "right acq: " << rightAcq << std::endl;

            //            std::cout << syncAcqs.back() << std::endl;

            assert( syncAcq.getStart() == rightAcq.getStart() );
            assert( syncAcq.getEnd() == rightAcq.getEnd() );
            //            assert( syncAcq.nType() == 4 );
            //            assert( leftAcq.nType() == 2 );
            //            assert( rightAcq.nType() == 2 );

            //            const auto& measure = leftAcq.getMeasures().front();
            //            std::cout << syncAcq.getMeasures().at(0) << std::endl;
            //            std::cout << measure << std::endl;
            //            assert( syncAcq.getMeasures().at( 0 ) == measure );

            //            assert( rightAcq.getMeasures().size() == 1 );
            //            const auto& measure2 = rightAcq.getMeasures().front();
            //            assert( syncAcq.getMeasures().at( 1 ) == measure2 );
        }
    }

    //    std::cout << std::endl;
    return syncAcqs;
}

// static std::set<hub::sensor::Acquisition>
// computeSortedAcqs( const std::vector<hub::sensor::Acquisition>& ref_acqs,
//                    const std::vector<hub::sensor::Acquisition>& ref_acqs2 ) {
//     std::set<hub::sensor::Acquisition> sortedAcqs;
//     for ( int i = 1; i < ref_acqs.size(); ++i ) {
//         const auto& acq = ref_acqs.at( i );
//         sortedAcqs.insert( acq.clone() );
//     }
//     for ( int i = 1; i < ref_acqs2.size(); ++i ) {
//         const auto& acq = ref_acqs2.at( i );
//         sortedAcqs.insert( acq.clone() );
//     }
//     assert( sortedAcqs.size() == ref_acqs.size() + ref_acqs2.size() - 2 );
//     return sortedAcqs;
// }

// static void writingProcess( const std::vector<hub::sensor::Acquisition>& refAcqs,
//                             const std::vector<hub::sensor::Acquisition>& refAcqs2,
//                             hub::Output& output,
//                             hub::Output& output2,
//                             const std::set<hub::sensor::Acquisition>& sortedAcqs,
//                             bool delayed ) {

//    const auto& acqFront2 = refAcqs2.front();
//    std::cout << "write acq2: " << acqFront2 << std::endl;
//    output2 << acqFront2;

//    const auto& acqFront = refAcqs.front();
//    std::cout << "write acq: " << acqFront << std::endl;
//    output << acqFront;

//    if ( delayed ) { std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); }

//    for ( const auto& acq : sortedAcqs ) {
//        if ( acq.getMeasures().front().getResolution() ==
//             acqFront.getMeasures().front().getResolution() ) {
//            std::cout << "write acq: " << acq << std::endl;
//            output << acq;
//        }
//        else {
//            std::cout << "write acq2: " << acq << std::endl;
//            output2 << acq;
//        }
//        if ( delayed ) { std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) ); }
//    }
//    std::cout << "write done" << std::endl;
//}

template <typename T>
bool someEnd( const T& t ) {
    if constexpr ( hub::Endable<T> ) { return t.isEnd(); }
    else { return false; }
}

template <typename T, typename... Inputs>
bool someEnd( const T& t, const Inputs&... args ) {
    //    return t.isEnd() && areEnd( args... );
    if constexpr ( hub::Endable<T> ) { return t.isEnd() || someEnd( args... ); }
    else { return someEnd( args... ); }
}

template <typename T>
bool anyEnd( const T& t ) {
    if constexpr ( hub::Endable<T> ) { return t.isEnd(); }
    else { return true; }
}

template <typename T, typename... Inputs>
bool anyEnd( const T& t, const Inputs&... args ) {
    //    return t.isEnd() && areEnd( args... );
    if constexpr ( hub::Endable<T> ) { return t.isEnd() && anyEnd( args... ); }
    else { return anyEnd( args... ); }
}

template <class Input>
void clearAll( Input& input ) {
    input.clear();
}
template <class Input, class... Inputs>
void clearAll( Input& input, Inputs&... inputs ) {
    input.clear();
    if ( sizeof...( Inputs ) > 0 ) { clearAll( inputs... ); }
}

// template <typename Input>
// Input sync( Input&& input ) {
//     return input;
// }

// template <typename Input>
// hub::sensor::Acquisition sync( Input&& input, Input&& input2 ) {
//     return input >> input2;
// }

// template <typename Input, typename ...Args>
// hub::sensor::Acquisition sync(Input && input, Input && input2, Args && ... args) {
//     return sync(input, input2) >> sync(args...);
// }

// template <typename Input, typename ...Args>
// hub::sensor::Acquisition sync(Input && t, T && t2, Args && ... args) {
//     return t >> args...;
// }

// template <class Output, class Input>
template <class Output, class... Inputs>
static void
checkSynchronize( Output& output,
                  const hub::sensor::SensorSpec& ref_sensorSpec,
                  const std::vector<hub::sensor::Acquisition>& ref_acqs,
                  Output& output2,
                  const hub::sensor::SensorSpec& ref_sensorSpec2,
                  const std::vector<hub::sensor::Acquisition>& ref_acqs2,
                  //                               hub::Input& input,
                  //                               hub::Input& input2,
                  //                              const std::vector<hub::sensor::Acquisition>&
                  //                              refSyncAcqs, bool delayed,
                  //                               hub::Input & input,
                  Inputs&... inputs ) {

    //    std::set<hub::sensor::Acquisition> sortedAcqs = computeSortedAcqs( ref_acqs, ref_acqs2 );

    assert( anyEnd( inputs... ) );
    // assert(ref_acqs.begin()->getResolution() == ref_sensorSpec.getResolution());
    // assert(ref_acqs2.begin()->getResolution() == ref_sensorSpec2.getResolution());

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "ref_sync_acqs: " << std::endl;
    for ( const auto& ref_acq : ref_acqs ) {
        assert( ref_acq.getResolution() == ref_sensorSpec.getResolution() );
        std::cout << ref_acq << std::endl;
    }
    std::cout << "ref_sync_acqs2: " << std::endl;
    for ( const auto& ref_acq2 : ref_acqs2 ) {
        assert( ref_acq2.getResolution() == ref_sensorSpec2.getResolution() );
        std::cout << ref_acq2 << std::endl;
    }

    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );
    std::cout << "ref_sync_acqs:" << std::endl;
    for ( const auto& sync_acq : ref_sync_acqs ) {
        std::cout << sync_acq << std::endl;
    }

    // hub::sensor::OutputSensor outputSensor( ref_sensorSpec, output );
    // hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, output2 );
    hub::sensor::OutputSensor outputSensor( ref_sensorSpec, output );
    assert( outputSensor.getSpec() == ref_sensorSpec );
    hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, output2 );
    assert( outputSensor2.getSpec() == ref_sensorSpec2 );

    for ( const auto& acq : ref_acqs ) {
        outputSensor << acq;
    }
    for ( const auto& acq2 : ref_acqs2 ) {
        outputSensor2 << acq2;
    }

    std::vector<hub::sensor::Acquisition> sync_acqs;
    hub::sensor::InputSensor inputSensorSync( inputs... );
    auto read_acq = inputSensorSync.acqMsg();

    //    while (! archive.isEnd() && !archive2.isEnd()) {
    //	while ( !areEnd( inputs... ) && sync_acqs.size() != refSyncAcqs.size() ) {

    std::cout << "sync_acqs:" << std::endl;
    while ( !someEnd( inputs... ) && sync_acqs.size() != ref_sync_acqs.size() ) {
        inputSensorSync >> read_acq;
        sync_acqs.push_back( read_acq.clone() );
        std::cout << read_acq << std::endl;
    }

    clearAll( inputs... );

    CHECK( ref_sync_acqs == sync_acqs );
    assert( anyEnd( inputs... ) );
}

// template <class Type>
static auto generateRefAcqs( int offset, int nAcq, const hub::sensor::SensorSpec& sensorSpec ) {

    //    std::cout << "ref_acqs" << std::endl;
    // const auto ref_resolution = hub::make_matrix<Type>();
    // const hub::sensor::SensorSpec ref_sensorSpec( sensorName, ref_resolution );
    hub::sensor::Acquisition acq = hub::sensor::make_acquisition( sensorSpec.getResolution() );

    std::vector<hub::sensor::Acquisition> ref_acqs;
    auto ref_dataSize = acq.size() - 2 * sizeof( hub::sensor::Clock );
    auto* data        = acq.data() + 2 * sizeof( hub::sensor::Clock );
    for ( int iAcq = 0; iAcq < nAcq; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = offset + iAcq + 1;
        }
        acq.start() = offset + iAcq + 1;
        acq.end()   = offset + iAcq + 2;
        ref_acqs.push_back( acq.clone() );
        //        std::cout << ref_acqs.back() << std::endl;
    }
    //    std::cout << std::endl;

    return ref_acqs;
    // return std::tuple<hub::sensor::SensorSpec, std::vector<hub::sensor::Acquisition>>(
    // std::move( sensorSpec ), std::move( ref_acqs ) );
}

// template <class Type>
// static auto generateRefAcqs( int offset, int nAcq, std::string sensorName ) {

//     //    std::cout << "ref_acqs" << std::endl;
//     const auto ref_resolution = hub::make_matrix<Type>();
//     const hub::sensor::SensorSpec ref_sensorSpec( sensorName, ref_resolution );
//     hub::sensor::Acquisition acq = hub::sensor::make_acquisition( ref_resolution );

//     std::vector<hub::sensor::Acquisition> ref_acqs;
//     auto ref_dataSize = acq.size() - 2 * sizeof( hub::sensor::Clock );
//     auto* data        = acq.data() + 2 * sizeof( hub::sensor::Clock );
//     for ( int iAcq = 0; iAcq < nAcq; ++iAcq ) {
//         for ( int i = 0; i < ref_dataSize; ++i ) {
//             data[i] = offset + iAcq + 1;
//         }
//         acq.start() = offset + iAcq + 1;
//         acq.end()   = offset + iAcq + 2;
//         ref_acqs.push_back( acq.clone() );
//         //        std::cout << ref_acqs.back() << std::endl;
//     }
//     //    std::cout << std::endl;

//     return std::tuple<hub::sensor::SensorSpec, std::vector<hub::sensor::Acquisition>>(
//         std::move( ref_sensorSpec ), std::move( ref_acqs ) );
// }

// template <class Output>
// static void outputSensorBench( Output& output ) {

//     using Resolution = hub::sensor::format::Y8;
//     const hub::sensor::SensorSpec sensorSpec( "sensorName", hub::make_matrix<Resolution>() );
//     const auto acqs = test::sensor::generateRefAcqs( 0, 10, sensorSpec );
//     hub::output::OutputStream outputStream(
//         hub::io::make_header( sensorSpec ), FILE_NAME, SERVER_PORT );
//     const auto& [ref_sensorSpec, ref_acqs] =
//         generateRefAcqs<hub::sensor::format::Y8>( 0, 10, "sensorName" );
//     //    assert( anyEnd( output... ) );

//     hub::sensor::OutputSensor outputSensor( ref_sensorSpec, output );

//     for ( const auto& acq : ref_acqs ) {
//         outputSensor << acq;
//     }
// }

// template <class Input>
// static void inputSensorBench( Input& input ) {
//     assert( !input.isEnd() );

//     const auto& [ref_sensorSpec, ref_acqs] =
//         generateRefAcqs<hub::sensor::format::Y8>( 0, 10, "sensorName" );

//     hub::sensor::InputSensor inputSensor( input );
//     assert( ref_sensorSpec == inputSensor.getSpec() );
//     auto acq_read = inputSensor.acqMsg();

//     for ( const auto& acq : ref_acqs ) {
//         inputSensor >> acq_read;
//         CHECK( acq == acq_read );
//     }

//     //    assert(input.isEnd());
// }

struct UserType {
    int a;
    bool b;
    static constexpr auto name() { return "UserType"; };
};
static_assert( sizeof( UserType ) == 8 );

using UserResolution = UserType;
// const hub::sensor::SensorSpec s_sensorSpec_ref( "sensorName", hub::make_matrix<UserResolution>()
// ); static const hub::io::Header s_header_ref = hub::io::make_header( s_sensorSpec_ref );

// #define TEST_SENSOR_HEADER s_header_ref

template <class Input, class Output, class... Args>
static void inputOutputSensorAsyncBench( const Args&... args ) {

    hub::MetaData metaData;
    metaData["name"] = "gauthier";
    const hub::sensor::SensorSpec sensorSpec(
        "sensorName", hub::make_matrix<UserResolution>(), metaData );

    hub::sensor::AcquisitionT<UserResolution> acq;
    acq.start()    = 4;
    acq.end()      = 3;
    auto& userType = acq.get<UserType&>();
    userType.a     = 2;
    userType.b     = true;

    // outputSensor
    {
        hub::sensor::OutputSensorT<UserResolution, Output> outputSensor(sensorSpec, args...);
        // hub::sensor::OutputSensorT<UserResolution> outputSensor(
            // Output( hub::io::make_header( sensorSpec ), args... ) );

        // write output acq
        outputSensor << acq;
    }

    // inputSensor
    {
        hub::sensor::InputSensor inputSensor( Input( args... ) );
        // CHECK( input.isEnd() );
        CHECK( sensorSpec == inputSensor.getSpec() );

        // read input acq
        auto acq_read = inputSensor.acqMsg();
        inputSensor >> acq_read;

        std::cout << "acq: " << acq << std::endl;
        std::cout << "acq_read: " << acq_read << std::endl;
        CHECK( acq == acq_read );
    }

    // assert( inputSensor.getInput().isEnd() );
    // assert( input.isEnd() );
}

template <class Input, class Output, class... Args>
static void inputOutputSensorBench( const Args&... args ) {
    hub::MetaData metaData;
    metaData["name"] = "gauthier";
    const hub::sensor::SensorSpec sensorSpec(
        "sensorName", hub::make_matrix<UserResolution>(), metaData );
    hub::sensor::OutputSensorT<UserResolution, Output> outputSensor(sensorSpec, args...);
    // hub::sensor::OutputSensorT<UserResolution> outputSensor(
        // Output( hub::io::make_header( sensorSpec ), args... ) );

    auto acq       = outputSensor.acqMsg();
    acq.start()    = 4;
    acq.end()      = 3;
    auto& userType = acq.template get<UserType&>();
    userType.a     = 2;
    userType.b     = true;

    hub::sensor::InputSensor inputSensor( Input( args... ) );
    // CHECK( input.isEnd() );
    CHECK( outputSensor.getSpec() == inputSensor.getSpec() );

    // write output acq
    outputSensor << acq;

    // read input acq
    auto acq_read = inputSensor.acqMsg();
    inputSensor >> acq_read;
    std::cout << "acq: " << acq << std::endl;
    std::cout << "acq_read: " << acq_read << std::endl;
    CHECK( acq == acq_read );

    // assert( inputSensor.getInput().isEnd() );
    // assert( input.isEnd() );
}

// template <class InputOutput>
// static void inputOutputSensorBench( InputOutput& inputOutput, InputOutput& inputOutput2 ) {
//     //    InputOutput inputOutput;
//     //    InputOutput inputOutput2;
//     //    const int ref_offset    = 5;
//     //    constexpr int ref_nAcqs = 10;
//     //    const auto & [ref_sensorSpec, ref_acqs] = generateRefAcqs<hub::sensor::format::Y8>(0,
//     10,
//     //    "sensorName"); const auto & [ref_sensorSpec2, ref_acqs2] =
//     //    generateRefAcqs<hub::sensor::format::Z16>(5, 10, "sensorName2"); const auto &
//     //    [ref_sensorSpec3, ref_acqs3] = generateRefAcqs<hub::sensor::format::Y8>(10, 10,
//     //    "sensorName3");

//     //    assert(inputOutput.isEnd());
//     //    assert(inputOutput2.isEnd());

//     outputSensorBench( inputOutput );
//     inputSensorBench( inputOutput2 );

//     //    assert(inputOutput.isEnd());
//     //    assert(inputOutput2.isEnd());

//     outputSensorBench( inputOutput2 );
//     inputSensorBench( inputOutput );

//     //    assert(inputOutput.isEnd());
//     //    assert(inputOutput2.isEnd());
// }

// template <class Input, class Output>
// static void
// // inputOutputSensorBench( hub::Input& input, hub::Input& input2, hub::Output& output,
// hub::Output&
// // output2 ) {
// inputOutputSensorSyncBench( Input& input, Input& input2, Output& output, Output& output2 ) {

//     // const auto& [ref_sensorSpec, ref_acqs] =
//     //     generateRefAcqs<hub::sensor::format::Y8>( 0, 10, "sensorName" );
//     // const auto& [ref_sensorSpec2, ref_acqs2] =
//     //     generateRefAcqs<hub::sensor::format::Z16>( 5, 10, "sensorName2" );
//     // const auto& [ref_sensorSpec3, ref_acqs3] =
//     //     generateRefAcqs<hub::sensor::format::Y8>( 10, 10, "sensorName3" );

//     // checkSynchronize(
//         // output, ref_sensorSpec, ref_acqs, output2, ref_sensorSpec2, ref_acqs2, input, input2
//         );

//     // const auto restSize = ref_acqs2.size() * 5;
//     // std::vector<unsigned char> vector(restSize);
//     // input2.read(vector.data(), restSize);

//     // auto acq2_read = ref_acqs2.front().clone();
//     // for ( int i = 0; i < 5; ++i ) {
//     //     input2.read( acq2_read );
//     //     std::cout << "clearing input2: read acq: " << acq2_read << std::endl;
//     // }

//     // checkSynchronize(
//     //     output2, ref_sensorSpec2, ref_acqs2, output, ref_sensorSpec3, ref_acqs3, input2, input
//     );
// }

// template <class InputOutput>
// // static void inputOutputSensorBench( hub::io::InputOutput<>& inputOutput,
// //                               hub::io::InputOutput<>& inputOutput2 ) {
// static void inputOutputSensorBench() {
//     InputOutput inputOutput;
//     InputOutput inputOutput2;
//     //    const int ref_offset    = 5;
//     //    constexpr int ref_nAcqs = 10;
//     const auto& [ref_sensorSpec, ref_acqs] =
//         generateRefAcqs<hub::sensor::format::Y8>( 0, 10, "sensorName" );
//     const auto& [ref_sensorSpec2, ref_acqs2] =
//         generateRefAcqs<hub::sensor::format::Z16>( 5, 10, "sensorName2" );
//     const auto& [ref_sensorSpec3, ref_acqs3] =
//         generateRefAcqs<hub::sensor::format::Y8>( 10, 10, "sensorName3" );

//     checkSynchronize( inputOutput,
//                       ref_sensorSpec,
//                       ref_acqs,
//                       inputOutput2,
//                       ref_sensorSpec2,
//                       ref_acqs2,
//                       inputOutput,
//                       inputOutput2 );

//     checkSynchronize( inputOutput,
//                       ref_sensorSpec2,
//                       ref_acqs2,
//                       inputOutput2,
//                       ref_sensorSpec3,
//                       ref_acqs3,
//                       inputOutput,
//                       inputOutput2 );

//     //    checkSynchronize( inputOutput2,
//     //                      ref_sensorSpec2,
//     //                      ref_acqs2,
//     //                      inputOutput,
//     //                      ref_sensorSpec,
//     //                      ref_acqs,
//     //                      inputOutput2,
//     //                      inputOutput );
// }

// template <class Output, class Input>
// static void checkSynchronize( Output&& output,
//                               const hub::sensor::SensorSpec& sensorSpec,
//                               const std::vector<hub::sensor::Acquisition>& ref_acqs,
//                               Output&& output2,
//                               const hub::sensor::SensorSpec& sensorSpec2,
//                               const std::vector<hub::sensor::Acquisition>& ref_acqs2,
//                               Input&& input,
//                               Input&& input2,
//                               const std::vector<hub::sensor::Acquisition>& refSyncAcqs,
//                               bool delayed = false ) {

//    hub::sensor::OutputSensor outputSensor( sensorSpec, std::move( output ) );
//    std::cout << "outputSensor created" << std::endl;
//    hub::sensor::InputSensor inputSensor( std::move( input ) );
//    std::cout << "inputSensor created" << std::endl;

//    hub::sensor::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
//    std::cout << "outputSensor2 created" << std::endl;
//    hub::sensor::InputSensor inputSensor2( std::move( input2 ) );
//    std::cout << "inputSensor2 created" << std::endl;

//    checkSynchronize( outputSensor.getOutput(),
//                      //                       sensorSpec,
//                      ref_acqs,
//                      outputSensor2.getOutput(),
//                      //                       sensorSpec2,
//                      ref_acqs2,
//                      refSyncAcqs,
//                      delayed,
//                      inputSensor.getInput(),
//                      inputSensor2.getInput() );
//}

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

// template <class Output, class Input>
// static void checkSynchronize( Output&& output,
//                               const hub::sensor::SensorSpec& sensorSpec,
//                               const std::vector<hub::sensor::Acquisition>& ref_acqs,
//                               Output&& output2,
//                               const hub::sensor::SensorSpec& sensorSpec2,
//                               const std::vector<hub::sensor::Acquisition>& ref_acqs2,
//                               Input&& input,
//                               Input&& input2,
//                               bool delayed = false ) {

//    std::cout << "ref_sync_acqs" << std::endl;
//    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

//    hub::sensor::OutputSensor outputSensor( sensorSpec, std::move( output ) );
//    std::cout << "outputSensor created" << std::endl;
//    hub::sensor::InputSensor inputSensor( std::move( input ) );
//    std::cout << "inputSensor created" << std::endl;

//    hub::sensor::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
//    std::cout << "outputSensor2 created" << std::endl;
//    hub::sensor::InputSensor inputSensor2( std::move( input2 ) );
//    std::cout << "inputSensor2 created" << std::endl;

//    _checkSynchronize( outputSensor.getOutput(),
////                       sensorSpec,
//                       ref_acqs,
//                       outputSensor2.getOutput(),
////                       sensorSpec2,
//                       ref_acqs2,
//                       inputSensor.getInput(),
//                       inputSensor2.getInput(),
//                       ref_sync_acqs,
//                       delayed );
//}

// template <class Output, class Input>
// static void checkSynchronize( Output&& output,
//                               const hub::sensor::SensorSpec& sensorSpec,
//                               const std::vector<hub::sensor::Acquisition>& ref_acqs,
//                               Output&& output2,
//                               const hub::sensor::SensorSpec& sensorSpec2,
//                               const std::vector<hub::sensor::Acquisition>& ref_acqs2,
//                               Input&& input,
//                               bool delayed = false ) {

//    std::cout << "ref_sync_acqs" << std::endl;
//    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

//    _checkSynchronize( std::move(output),
//                       sensorSpec,
//                       ref_acqs,
//                       std::move(output2),
//                       sensorSpec2,
//                       ref_acqs2,
//                       std::move(input),
//                       ref_sync_acqs,
//                       delayed );
//}

//        std::vector<int> min_dists( ref_acqs2.size(), 999999 );
//        std::vector<int> iMin_dists( ref_acqs2.size(), -1 );

//        int iAcq = 0;
//        for ( const auto& acq : ref_acqs ) {
//            int iMinDist = 0;
//            int minDist  = computeDist( acq, ref_acqs2.front() );
//            for ( int iAcq2 = 1; iAcq2 < ref_acqs2.size(); ++iAcq2 ) {
//                const auto& acq2 = ref_acqs2.at( iAcq2 );
//                const auto dist  = computeDist( acq, acq2 );
//                if ( dist <= minDist ) {
//                    minDist  = dist;
//                    iMinDist = iAcq2;
//                }
//            }

//            if ( minDist < min_dists.at( iMinDist ) ) {
//                min_dists[iMinDist]  = minDist;
//                iMin_dists[iMinDist] = iAcq;
//            }
//            ++iAcq;
//        }

//        for ( int i = 0; i < ref_acqs2.size(); ++i ) {
//            if ( iMin_dists[i] != -1 ) {
//                const auto& acq  = ref_acqs.at( iMin_dists.at( i ) );
//                const auto& acq2 = ref_acqs2.at( i );
//                ref_sync_acqs.push_back( acq.clone() );

//                auto& syncAcq = ref_sync_acqs.back();
//                syncAcq << acq2.getMeasures();

//                std::cout << ref_sync_acqs.back() << std::endl;

//                CHECK( syncAcq.getStart() == acq.getStart() );
//                CHECK( syncAcq.getEnd() == acq.getEnd() );
//                CHECK( syncAcq.getMeasures().size() == 2 );

//                CHECK( acq.getMeasures().size() == 1 );
//                const auto& measure = acq.getMeasures().front();
//                CHECK( syncAcq.getMeasures().at( 0 ) == measure );

//                CHECK( acq2.getMeasures().size() == 1 );
//                const auto& measure2 = acq2.getMeasures().front();
//                CHECK( syncAcq.getMeasures().at( 1 ) == measure2 );
//            }
//        }
//        std::cout << std::endl;

// static std::string ReplaceAll( std::string str, const std::string& from, const std::string& to )
// {
//     size_t start_pos = 0;
//     while ( ( start_pos = str.find( from, start_pos ) ) != std::string::npos ) {
//         str.replace( start_pos, from.length(), to );
//         start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
//     }
//     return str;
// }

//#define START_REPORT()                                                                             \
//    std::ofstream file( "report.txt" );                                                            \
//    assert( file.is_open() );                                                                      \
//    file << std::endl;                                                                             \
//    file << "####################################################################################" \
//            "##\n"                                                                                 \
//         << "#################################### START REPORT "                                   \
//            "####################################\n"                                               \
//         << "####################################################################################" \
//            "##\n"                                                                                 \
//         << std::endl;                                                                             \
//    file.close();

//#define END_REPORT()                                                                               \
//    std::ofstream file( "report.txt", std::ios::app );                                             \
//    assert( file.is_open() );                                                                      \
//    file << std::endl;                                                                             \
//    file << "####################################################################################" \
//            "##\n"                                                                                 \
//         << "#################################### END REPORT "                                     \
//            "####################################\n"                                               \
//         << "####################################################################################" \
//            "##\n"                                                                                 \
//         << std::endl                                                                              \
//         << std::endl;                                                                             \
//    file.close();

//#define PRINT_REPORT()                      \
//    do {                                    \
//        std::ifstream file( "report.txt" ); \
//        assert( file.is_open() );           \
//        std::cout << file.rdbuf();          \
//        file.close();                       \
//    } while ( false );

// static std::string s_latestFilename = "";

//#define _REPORT( _params, filename, line )                                         \
//    do {                                                                           \
//        std::string filenameStr = filename;                                        \
//        const auto first        = filenameStr.find( "-" ) + 1;                     \
//        const auto last         = filenameStr.find( "." );                         \
//        const auto testName     = filenameStr.substr( first, last - first );       \
//        std::cout << _params << std::endl;                                         \
//        std::ofstream file( "report.txt", std::ios::app );                         \
//        assert( file.is_open() );                                                  \
//        if ( filename != s_latestFilename ) {                                      \
//            s_latestFilename = filename;                                           \
//            file << std::endl;                                                     \
//            file << "------------> running test " << testName << ":" << std::endl; \
//        }                                                                          \
//        file << _params << std::endl;                                              \
//        file.close();                                                              \
//    } while ( false );

// #define REPORT( _params ) _REPORT( _params, FILE_NAME, __LINE__ )
// #define REPORT_NEW_LINE _REPORT( "", FILE_NAME, __LINE__ )

// static void _checkRatio( double ratio,
//                          int compare,
//                          int gap,
//                          const std::string& name,
//                          const std::string& filename,
//                          int line ) {
//     //    const int gap = 10;
//     CHECK( ( compare - gap <= ratio && ratio <= compare + gap ) );
//     if ( !( compare - gap <= ratio && ratio <= compare + gap ) ) {
//         std::cout <<
//         "-----------------------------------------------------------------------------"
//                      "--------------------------------------------------------------->
//                      checkRatio: "
//                   << compare - gap << " <= " << ratio << " <= " << compare + gap << std::endl;
//     }

//    //    std::ofstream file("ouou", std::ios::app);
//    auto name2 = name;
//    //    name2.replace(name2.begin(), name2.end(), '/', '-');
//    name2 = ReplaceAll( name2, "/", "_vs_" );

//    //    std::cout << "checkRatio " << filename << std::endl;
//    {
//        std::ofstream logFile( ( filename + "_" + name2 + ".history" ).c_str(), std::ios::app );
//        assert( logFile.is_open() );

//        logFile << HUB_COMMIT_HASH << " " << ratio << std::endl;

//        logFile.close();
//    }

//    {
//        std::ifstream inFile( ( filename + "_" + name2 + ".history" ).c_str() );
//        assert( inFile.is_open() );

//        double ratio2;
//        std::string hash;
//        int iRatio             = 0;
//        constexpr int nMaxMean = 4;
//        constexpr int nRatio   = 8;
//        assert( nRatio == std::pow( 2, nMaxMean - 1 ) );
//        std::string hashes[nRatio];
//        double ratios[nRatio];
//        for ( int i = 0; i < nRatio; ++i ) {
//            ratios[i] = 0.0;
//        }
//        while ( !inFile.eof() ) {
//            ratio2 = -1;
//            hash   = "";
//            inFile >> hash >> ratio2;
//            if ( ratio2 != -1 ) {
//                //                sumRatio += ratio2;
//                ratios[iRatio % nRatio] = ratio2;
//                hashes[iRatio % nRatio] = hash;
//                ++iRatio;
//            }
//        }
//        assert( iRatio > 0 );
//        double sumRatios[nMaxMean];
//        double minRatios[nMaxMean];
//        double maxRatios[nMaxMean];
//        for ( int i = 0; i < nMaxMean; ++i ) {
//            sumRatios[i] = 0.0;
//            minRatios[i] = ratios[( iRatio - 1 ) % nRatio];
//            maxRatios[i] = ratios[( iRatio - 1 ) % nRatio];
//        }

//        const int nEl = std::min( nRatio, iRatio );
//        for ( int i = 0; i < nEl; ++i ) {
//            //            std::cout << hashes[i] << " " << ratios[i] << std::endl;
//            const int idx       = ( iRatio - 1 - i ) % nRatio;
//            const auto curRatio = ratios[idx];

//            for ( int iMean = 0; iMean < nMaxMean; ++iMean ) {
//                if ( i < std::pow( 2.0, iMean ) ) {
//                    minRatios[iMean] = std::min( minRatios[iMean], curRatio );
//                    maxRatios[iMean] = std::max( maxRatios[iMean], curRatio );
//                    sumRatios[iMean] += curRatio;
//                }
//            }
//        }
//        //        double meanRatios[nMean];
//        //        double deviationRatios[nMean];

//        std::string report;

//        const int nMean    = std::log2( nEl ) + 1;
//        const auto meanAll = sumRatios[nMean - 1] / std::pow( 2.0, nMean - 1 );

//        for ( int iMean = 0; iMean < nMean; ++iMean ) {
//            //            for ( int iMean = 0; iMean < nMean; ++iMean ) {
//            const auto meanRatio = sumRatios[iMean] / std::pow( 2.0, iMean );
//            const auto deviation = maxRatios[iMean] - minRatios[iMean];
//            //            }

//            //        std::cout << "average of ratio for the last " << std::min(nRatio, iRatio) <<
//            "
//            //        builds is " << meanRatio << std::endl;
//            //            for ( int iMean = 0; iMean < nMean; ++iMean ) {
//            std::string meanRatioStr   = std::to_string( meanRatio );
//            meanRatioStr               = meanRatioStr.substr( 0, 5 );
//            std::string deviationStr   = std::to_string( deviation );
//            deviationStr               = deviationStr.substr( 0, 5 );
//            std::string meanCompareStr = std::to_string( meanRatio - meanAll );
//            meanCompareStr             = meanCompareStr.substr( 0, 5 );

//            report += "(" + std::to_string( (int)std::pow( 2, iMean ) ) + "): " + meanRatioStr +
//                      "% " + deviationStr + "+- " + meanCompareStr + "%";
//            if ( iMean != std::log2( nEl ) ) { report += ",  "; }
//            //            }
//        }

//        report += "  (";
//        for ( int i = 0; i < nEl; ++i ) {
//            const int idx = ( iRatio - 1 - i ) % nRatio;
//            //            const auto curRatio = ratios[idx];
//            auto curRatioStr = std::to_string( ratios[idx] );
//            curRatioStr      = curRatioStr.substr( 0, 5 );
//            report += curRatioStr;

//            if ( i != nEl - 1 ) { report += " "; }
//        }
//        report += ")";

//        _REPORT( "[" << name << "] " << report, filename, line );
//        //        _REPORT( "[" << name << "] ratio: " << ratio << " % , mean: " << meanRatio << "
//        %
//        //        (" << nEl
//        //                     << "), deviation : +/- " << deviation / 2.0,
//        //                 filename,
//        //                 line );

//        inFile.close();
//    }
//}

// #define checkRatio( ... ) _checkRatio( __VA_ARGS__, FILE_NAME, __LINE__ )

// template <class Output, class Input>
// static void _checkSynchronize( Output&& output,
//                                const hub::sensor::SensorSpec& sensorSpec,
//                                const std::vector<hub::sensor::Acquisition>& ref_acqs,
//                                Output&& output2,
//                                const hub::sensor::SensorSpec& sensorSpec2,
//                                const std::vector<hub::sensor::Acquisition>& ref_acqs2,
//                                Input&& input,
//                                const std::vector<hub::sensor::Acquisition>& refSyncAcqs,
//                                bool delayed = false ) {

//    std::set<hub::sensor::Acquisition> sortedAcqs = computeSortedAcqs( ref_acqs, ref_acqs2 );

//    hub::sensor::OutputSensor outputSensor( sensorSpec, std::move( output ) );
//    std::cout << "outputSensor created" << std::endl;

//    hub::sensor::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
//    std::cout << "outputSensor2 created" << std::endl;
////    hub::sensor::InputSensor inputSensor2( std::move( input2 ) );
////    std::cout << "inputSensor2 created" << std::endl;

//    hub::sensor::InputSensor inputSensor( std::move( input ) );
//    std::cout << "inputSensor created" << std::endl;

//    std::cout << "synching acqs" << std::endl;
//    std::vector<hub::sensor::Acquisition> sync_acqs;
//    bool writeDone     = false;
//    std::thread thread = std::thread( [&]() {
//        hub::sensor::Acquisition acq;

//        if ( !delayed ) {
//            while ( !writeDone ) {
//                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//            }
//        }
//        //	    for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
//        try {
//            //            while ( !inputSensor.getInput().isEnd() &&
//            //            !inputSensor2.getInput().isEnd() ) {
//            while ( !inputSensor.getInput().isEnd()  &&
//                    sync_acqs.size() != refSyncAcqs.size() ) {

//                inputSensor >> acq;
//                //            input >> input2 >> acq;
//                std::cout << "\tread synched acq: " << acq << std::endl;

//                sync_acqs.push_back( std::move( acq ) );
//            }
//        }
//        catch ( hub::io::StreamInterface::exception& ex ) {
//            std::cout << "[test_common] synchronize() catch exception : " << ex.what() <<
//            std::endl;
//        }
//        //        catch ( std::exception& ex ) {
//        //            throw ex;
//        //        }
//        std::cout << "synching acqs done" << std::endl;
//    } );

//    writingProcess( ref_acqs, ref_acqs2, outputSensor, outputSensor2, sortedAcqs, delayed );
//    writeDone = true;

//    assert( thread.joinable() );
//    thread.join();

//    assert( sync_acqs.size() == refSyncAcqs.size() );
//    for ( int i = 0; i < sync_acqs.size(); ++i ) {
//        const auto& acq2 = sync_acqs.at( i );

//        CHECK( acq2 == refSyncAcqs.at( i ) );
//    }
//}

////template <class Output, class Input>
// static void checkSynchronize( hub::sensor::OutputSensor&& outputSensor,
////                              const hub::sensor::SensorSpec& sensorSpec,
//                              const std::vector<hub::sensor::Acquisition>& ref_acqs,
//                              hub::sensor::OutputSensor&& outputSensor2,
////                              const hub::sensor::SensorSpec& sensorSpec2,
//                              const std::vector<hub::sensor::Acquisition>& ref_acqs2,
//                              hub::sensor::InputSensor&& inputSensor,
//                              hub::sensor::InputSensor&& inputSensor2,
//                              const std::vector<hub::sensor::Acquisition>& refSyncAcqs,
//                              bool delayed = false ) {

//    _checkSynchronize( outputSensor.getOutput(),
////                       sensorSpec,
//                       ref_acqs,
//                       outputSensor2.getOutput(),
////                       sensorSpec2,
//                       ref_acqs2,
//                       refSyncAcqs,
//                       delayed,
//                       inputSensor.getInput()
////                       inputSensor2.getInput()
//                       );
//}

// #define GET_RANDOM_PORT getRandomPort( __FILE__ )
//
// static int getRandomPort( const char* filename ) {
//     // #ifdef BUILD_SERVER
//     //     return 4042;
//     // #endif
//     // #ifdef USE_MQTT
//     //     return 1883;
//     // #endif
//     srand( (unsigned)time( NULL ) );
//     constexpr int offset      = 6000;
//     const unsigned int random = std::hash<std::string>()( filename ) + rand();
//     const unsigned int ret    = offset + random % ( 65535 - offset );
//     assert( offset <= ret && ret < 65536 );
//     assert( ret != hub::io::StreamServer::s_defaultPort );
//     std::cout << "using random port: " << ret << std::endl;
//     return ret;
// }

} // namespace sensor
} // namespace test
