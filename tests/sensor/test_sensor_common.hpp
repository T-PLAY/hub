
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

        const auto& leftMinAcq = leftAcqs.at( iLeftMinDist );

        if ( !( leftMinAcq.getStart() < rightAcq.getStart() &&
                iLeftMinDist == leftAcqs.size() - 1 ) &&
             !( rightAcq.getStart() < leftMinAcq.getStart() && iLeftMinDist == 0 ) ) {

            min_dists[iRightAcq]     = minDist;
            iLeftMinDists[iRightAcq] = iLeftMinDist;
        }
        ++iRightAcq;
    }

    std::vector<hub::sensor::Acquisition> syncAcqs;

    for ( int iRightAcq = 0; iRightAcq < rightAcqs.size(); ++iRightAcq ) {

        int iLeftMinDist = iLeftMinDists.at( iRightAcq );
        if ( iLeftMinDist != -1 ) {
            const auto& leftAcq  = leftAcqs.at( iLeftMinDist );
            const auto& rightAcq = rightAcqs.at( iRightAcq );

            auto sync_acq    = leftAcq << rightAcq;
            sync_acq.start() = rightAcq.getStart();
            sync_acq.end()   = rightAcq.getEnd();
            syncAcqs.push_back( std::move( sync_acq ) );

            auto& syncAcq = syncAcqs.back();

            CHECK( syncAcq.getStart() == rightAcq.getStart() );
            CHECK( syncAcq.getEnd() == rightAcq.getEnd() );
        }
    }

    return syncAcqs;
}

template <typename T>
bool someEnd( const T& t ) {
    if constexpr ( hub::Endable<T> ) { return t.isEnd(); }
    else { return false; }
}

template <typename T, typename... Inputs>
bool someEnd( const T& t, const Inputs&... args ) {
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

// Input sync( Input&& input ) {

template <class Output, class... Inputs>
static void checkSynchronize( Output& output,
                              const hub::sensor::SensorSpec& ref_sensorSpec,
                              const std::vector<hub::sensor::Acquisition>& ref_acqs,
                              Output& output2,
                              const hub::sensor::SensorSpec& ref_sensorSpec2,
                              const std::vector<hub::sensor::Acquisition>& ref_acqs2,
                              Inputs&... inputs ) {

    CHECK( anyEnd( inputs... ) );

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "ref_sync_acqs: " << std::endl;
    for ( const auto& ref_acq : ref_acqs ) {
        CHECK( ref_acq.getResolution() == ref_sensorSpec.getResolution() );
        std::cout << ref_acq << std::endl;
    }
    std::cout << "ref_sync_acqs2: " << std::endl;
    for ( const auto& ref_acq2 : ref_acqs2 ) {
        CHECK( ref_acq2.getResolution() == ref_sensorSpec2.getResolution() );
        std::cout << ref_acq2 << std::endl;
    }

    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );
    std::cout << "ref_sync_acqs:" << std::endl;
    for ( const auto& sync_acq : ref_sync_acqs ) {
        std::cout << sync_acq << std::endl;
    }

    hub::sensor::OutputSensor outputSensor( ref_sensorSpec, output );
    CHECK( outputSensor.getSpec() == ref_sensorSpec );
    hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, output2 );
    CHECK( outputSensor2.getSpec() == ref_sensorSpec2 );

    for ( const auto& acq : ref_acqs ) {
        outputSensor << acq;
    }
    for ( const auto& acq2 : ref_acqs2 ) {
        outputSensor2 << acq2;
    }

    std::vector<hub::sensor::Acquisition> sync_acqs;
    hub::sensor::InputSensor inputSensorSync( inputs... );
    auto read_acq = inputSensorSync.acqMsg();

    std::cout << "sync_acqs:" << std::endl;
    while ( !someEnd( inputs... ) && sync_acqs.size() != ref_sync_acqs.size() ) {
        inputSensorSync >> read_acq;
        sync_acqs.push_back( read_acq.copy() );
        std::cout << read_acq << std::endl;
    }

    clearAll( inputs... );

    CHECK( ref_sync_acqs == sync_acqs );
    CHECK( anyEnd( inputs... ) );
}

static auto generateRefAcqs( int offset, int nAcq, const hub::sensor::SensorSpec& sensorSpec ) {

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
        ref_acqs.push_back( acq.copy() );
    }

    return ref_acqs;
}

struct UserType {
    int a;
    bool b;
    static constexpr auto name() { return "UserType"; };
};
static_assert( sizeof( UserType ) == 8 );

using UserResolution = UserType;

template <class Input, class Output, class... Args>
static void inputOutputSensorAsyncBench( const Args&... args ) {

    const auto userResolution = hub::make_matrix<UserResolution>();

    hub::MetaData metaData;
    metaData["name"] = "gauthier";
    const hub::sensor::SensorSpec sensorSpec( "sensorName", UserResolution(), metaData );
    CHECK( sensorSpec.getResolution() == userResolution );

    hub::sensor::AcquisitionT<UserResolution> acq;
    acq.start()    = 4;
    acq.end()      = 3;
    auto& userType = acq.get<UserType&>();
    userType.a     = 2;
    userType.b     = true;

    {
        hub::sensor::OutputSensorT<UserResolution, Output> outputSensor( sensorSpec, args... );

        outputSensor << acq;
    }

    {
        hub::sensor::InputSensor inputSensor { Input( args... ) };
        CHECK( sensorSpec == inputSensor.getSpec() );

        auto acq_read = inputSensor.acqMsg();
        CHECK( acq_read.getResolution() == userResolution );
        inputSensor >> acq_read;

        std::cout << "acq: " << acq << std::endl;
        std::cout << "acq matrix: " << acq.getMatrix() << std::endl;
        std::cout << "acq_read: " << acq_read << std::endl;

        CHECK( acq == acq_read );
        CHECK( acq == acq_read );
    }
}

template <class Input, class Output, class... Args>
static void inputOutputSensorBench( const Args&... args ) {
    hub::MetaData metaData;
    metaData["name"] = "gauthier";
    const hub::sensor::SensorSpec sensorSpec(
        "sensorName", hub::make_matrix<UserResolution>(), metaData );
    hub::sensor::OutputSensorT<UserResolution, Output> outputSensor( sensorSpec, args... );

    auto acq       = outputSensor.acqMsg();
    acq.start()    = 4;
    acq.end()      = 3;
    auto& userType = acq.template get<UserType&>();
    userType.a     = 2;
    userType.b     = true;

    hub::sensor::InputSensor inputSensor( Input( args... ) );
    // CHECK( input.isEnd() );
    CHECK( outputSensor.getSpec() == inputSensor.getSpec() );

    outputSensor << acq;

    auto acq_read = inputSensor.acqMsg();
    inputSensor >> acq_read;
    std::cout << "acq: " << acq << std::endl;
    std::cout << "acq_read: " << acq_read << std::endl;
    CHECK( acq == acq_read );
}

} // namespace sensor
} // namespace test
