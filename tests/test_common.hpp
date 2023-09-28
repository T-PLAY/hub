
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <random>
#include <set>

#include <Acquisition.hpp>
#include <Input.hpp>
#include <InputSensor.hpp>
#include <Output.hpp>
#include <OutputSensor.hpp>

#include "Macros.hpp"
#include <Version.h>

#define GET_RANDOM_PORT getRandomPort( __FILE__ )

static int getRandomPort( const char* filename ) {
    // #ifdef BUILD_SERVER
    //     return 4042;
    // #endif
    // #ifdef USE_MQTT
    //     return 1883;
    // #endif
    srand( (unsigned)time( NULL ) );
    constexpr int offset      = 6000;
    const unsigned int random = std::hash<std::string>()( filename ) + rand();
    const unsigned int ret    = offset + random % ( 65535 - offset );
    assert( offset <= ret && ret < 65536 );
    assert( ret != hub::io::StreamServer::s_defaultPort );
    std::cout << "using random port: " << ret << std::endl;
    return ret;
}

static int computeDist( const hub::Acquisition& acq, const hub::Acquisition& acq2 ) {
    return std::abs( acq.getStart() - acq2.getStart() );
}

static std::string ReplaceAll( std::string str, const std::string& from, const std::string& to ) {
    size_t start_pos = 0;
    while ( ( start_pos = str.find( from, start_pos ) ) != std::string::npos ) {
        str.replace( start_pos, from.length(), to );
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

#define START_REPORT()                                           \
    std::ofstream file( "report.txt" );                          \
    assert( file.is_open() );                                    \
    file << std::endl;                                           \
    file << "#################################### START REPORT " \
            "####################################"               \
         << std::endl;                                           \
    file.close();

#define END_REPORT()                                                                               \
    std::ofstream file( "report.txt", std::ios::app );                                             \
    assert( file.is_open() );                                                                      \
    file << std::endl;                                                                             \
    file << "#################################### END REPORT ####################################" \
         << std::endl;                                                                             \
    file.close();

#define PRINT_REPORT()                      \
    do {                                    \
        std::ifstream file( "report.txt" ); \
        assert( file.is_open() );           \
        std::cout << file.rdbuf();          \
        file.close();                       \
    } while ( false );

static std::string s_latestFilename = "";

#define _REPORT( _params, filename, line )                                         \
    do {                                                                           \
        std::string filenameStr = filename;                                        \
        const auto first        = filenameStr.find( "-" ) + 1;                     \
        const auto last         = filenameStr.find( "." );                         \
        const auto testName     = filenameStr.substr( first, last - first );       \
        std::cout << _params << std::endl;                                         \
        std::ofstream file( "report.txt", std::ios::app );                         \
        assert( file.is_open() );                                                  \
        if ( filename != s_latestFilename ) {                                      \
            s_latestFilename = filename;                                           \
            file << std::endl;                                                     \
            file << "----------------------------------" << testName               \
                 << "----------------------------------------------" << std::endl; \
        }                                                                          \
        file << _params << std::endl;                                              \
        file.close();                                                              \
    } while ( false );

#define REPORT( _params ) _REPORT( _params, FILE_NAME, __LINE__ )
#define REPORT_NEW_LINE _REPORT( "", FILE_NAME, __LINE__ )

static void _checkRatio( double ratio,
                         int compare,
                         int gap,
                         const std::string& name,
                         const std::string& filename,
                         int line ) {
    //    const int gap = 10;
    CHECK( ( compare - gap <= ratio && ratio <= compare + gap ) );
    if ( !( compare - gap <= ratio && ratio <= compare + gap ) ) {
        std::cout << "-----------------------------------------------------------------------------"
                     "---------------------------------------------------------------> checkRatio: "
                  << compare - gap << " <= " << ratio << " <= " << compare + gap << std::endl;
    }

    //    std::ofstream file("ouou", std::ios::app);
    auto name2 = name;
    //    name2.replace(name2.begin(), name2.end(), '/', '-');
    name2 = ReplaceAll( name2, "/", "_vs_" );

    //    std::cout << "checkRatio " << filename << std::endl;
    {
        std::ofstream logFile( ( filename + "_" + name2 + ".history" ).c_str(), std::ios::app );
        assert( logFile.is_open() );

        logFile << HUB_COMMIT_HASH << " " << ratio << std::endl;

        logFile.close();
    }

    {
        std::ifstream inFile( ( filename + "_" + name2 + ".history" ).c_str() );
        assert( inFile.is_open() );

        double ratio2;
        std::string hash;
        int iRatio          = 0;
        constexpr int nMaxMean = 4;
        constexpr int nRatio   = 8;
		assert(nRatio == std::pow( 2, nMaxMean - 1 ));
        std::string hashes[nRatio];
        double ratios[nRatio];
        for ( int i = 0; i < nRatio; ++i ) {
            ratios[i] = 0.0;
        }
        while ( !inFile.eof() ) {
            ratio2 = -1;
            hash   = "";
            inFile >> hash >> ratio2;
            if ( ratio2 != -1 ) {
                //                sumRatio += ratio2;
                ratios[iRatio % nRatio] = ratio2;
                hashes[iRatio % nRatio] = hash;
                ++iRatio;
            }
        }
        assert( iRatio > 0 );
        double sumRatios[nMaxMean];
        double minRatios[nMaxMean];
        double maxRatios[nMaxMean];
        for ( int i = 0; i < nMaxMean; ++i ) {
            sumRatios[i] = 0.0;
            minRatios[i] = ratios[( iRatio - 1 ) % nRatio];
            maxRatios[i] = ratios[( iRatio - 1 ) % nRatio];
        }

        const int nEl = std::min( nRatio, iRatio );
        for ( int i = 0; i < nEl; ++i ) {
            //            std::cout << hashes[i] << " " << ratios[i] << std::endl;
            const int idx       = ( iRatio - 1 - i ) % nRatio;
            const auto curRatio = ratios[idx];

            for ( int iMean = 0; iMean < nMaxMean; ++iMean ) {
                if ( i < std::pow( 2.0, iMean ) ) {
                    minRatios[iMean] = std::min( minRatios[iMean], curRatio );
                    maxRatios[iMean] = std::max( maxRatios[iMean], curRatio );
                    sumRatios[iMean] += curRatio;
                }
            }
        }
        //        double meanRatios[nMean];
        //        double deviationRatios[nMean];


        std::string report;

        const int nMean = std::log2(nEl) + 1;
        const auto meanAll = sumRatios[nMean - 1] / std::pow( 2.0, nMean - 1 );

        for ( int iMean = 0; iMean < nMean; ++iMean ) {
            //            for ( int iMean = 0; iMean < nMean; ++iMean ) {
            const auto meanRatio = sumRatios[iMean] / std::pow( 2.0, iMean );
            const auto deviation = maxRatios[iMean] - minRatios[iMean];
            //            }

            //        std::cout << "average of ratio for the last " << std::min(nRatio, iRatio) << "
            //        builds is " << meanRatio << std::endl;
            //            for ( int iMean = 0; iMean < nMean; ++iMean ) {
            std::string meanRatioStr = std::to_string(meanRatio);
            meanRatioStr = meanRatioStr.substr(0, 5);
            std::string deviationStr = std::to_string(deviation);
            deviationStr = deviationStr.substr(0, 5);
            std::string meanCompareStr = std::to_string(meanRatio - meanAll);
            meanCompareStr = meanCompareStr.substr(0, 5);

            report += "(" + std::to_string( (int)std::pow( 2, iMean ) ) +
                      "): " + meanRatioStr + "% " +
                      deviationStr + "+- " +
                        meanCompareStr + "%";
            if ( iMean != std::log2(nEl) ) { report += ",  "; }
            //            }
        }

        report += "  (";
        for ( int i = 0; i < nEl; ++i ) {
            const int idx       = ( iRatio - 1 - i ) % nRatio;
//            const auto curRatio = ratios[idx];
            auto curRatioStr = std::to_string(ratios[idx]);
            curRatioStr = curRatioStr.substr(0, 5);
            report += curRatioStr;

            if (i != nEl - 1) {
                report += " ";
            }
        }
        report += ")";

        _REPORT( "[" << name << "] " << report, filename, line );
        //        _REPORT( "[" << name << "] ratio: " << ratio << " % , mean: " << meanRatio << " %
        //        (" << nEl
        //                     << "), deviation : +/- " << deviation / 2.0,
        //                 filename,
        //                 line );

        inFile.close();
    }
}

#define checkRatio( ... ) _checkRatio( __VA_ARGS__, FILE_NAME, __LINE__ )

static std::vector<hub::Acquisition>
computeSyncAcqs( const std::vector<hub::Acquisition>& leftAcqs,
                 const std::vector<hub::Acquisition>& rightAcqs ) {
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

    std::vector<hub::Acquisition> syncAcqs;

    //    std::cout << "ref_sync_acqs" << std::endl;

    for ( int iRightAcq = 0; iRightAcq < rightAcqs.size(); ++iRightAcq ) {

        int iLeftMinDist = iLeftMinDists.at( iRightAcq );
        if ( iLeftMinDist != -1 ) {
            const auto& leftAcq  = leftAcqs.at( iLeftMinDist );
            const auto& rightAcq = rightAcqs.at( iRightAcq );

            hub::Acquisition sync_acq( rightAcq.getStart(), rightAcq.getEnd() );
            sync_acq << leftAcq.getMeasures() << rightAcq.getMeasures();
            syncAcqs.push_back( std::move( sync_acq ) );

            auto& syncAcq = syncAcqs.back();
            //                syncAcq << rightAcq.getMeasures();

            std::cout << syncAcqs.back() << std::endl;

            assert( syncAcq.getStart() == rightAcq.getStart() );
            assert( syncAcq.getEnd() == rightAcq.getEnd() );
            assert( syncAcq.getMeasures().size() == 2 );

            assert( leftAcq.getMeasures().size() == 1 );
            const auto& measure = leftAcq.getMeasures().front();
            assert( syncAcq.getMeasures().at( 0 ) == measure );

            assert( rightAcq.getMeasures().size() == 1 );
            const auto& measure2 = rightAcq.getMeasures().front();
            assert( syncAcq.getMeasures().at( 1 ) == measure2 );
        }
    }

    std::cout << std::endl;
    return syncAcqs;
}

static std::set<hub::Acquisition>
computeSortedAcqs( const std::vector<hub::Acquisition>& ref_acqs,
                   const std::vector<hub::Acquisition>& ref_acqs2 ) {
    std::set<hub::Acquisition> sortedAcqs;
    for ( int i = 1; i < ref_acqs.size(); ++i ) {
        const auto& acq = ref_acqs.at( i );
        sortedAcqs.insert( acq.clone() );
    }
    for ( int i = 1; i < ref_acqs2.size(); ++i ) {
        const auto& acq = ref_acqs2.at( i );
        sortedAcqs.insert( acq.clone() );
    }
    assert( sortedAcqs.size() == ref_acqs.size() + ref_acqs2.size() - 2 );
    return sortedAcqs;
}

static void writingProcess( const std::vector<hub::Acquisition>& refAcqs,
                            const std::vector<hub::Acquisition>& refAcqs2,
                            hub::Output& output,
                            hub::Output& output2,
                            const std::set<hub::Acquisition>& sortedAcqs,
                            bool delayed ) {

    const auto& acqFront2 = refAcqs2.front();
    std::cout << "write acq2: " << acqFront2 << std::endl;
    output2 << acqFront2;

    const auto& acqFront = refAcqs.front();
    std::cout << "write acq: " << acqFront << std::endl;
    output << acqFront;

    if ( delayed ) { std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); }

    for ( const auto& acq : sortedAcqs ) {
        if ( acq.getMeasures().front().getResolution() ==
             acqFront.getMeasures().front().getResolution() ) {
            std::cout << "write acq: " << acq << std::endl;
            output << acq;
        }
        else {
            std::cout << "write acq2: " << acq << std::endl;
            output2 << acq;
        }
        if ( delayed ) { std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) ); }
    }
    std::cout << "write done" << std::endl;
}

template <typename T>
bool areEnd( T&& t ) {
    return t.isEnd();
}

template <typename T, typename... Inputs>
bool areEnd( T&& t, Inputs&&... args ) {
    return t.isEnd() && areEnd( args... );
}

template <typename Input>
Input sync( Input&& input ) {
    return input;
}

template <typename Input>
hub::Acquisition sync( Input&& input, Input&& input2 ) {
    return input >> input2;
}

// template <typename Input, typename ...Args>
// hub::Acquisition sync(Input && input, Input && input2, Args && ... args) {
//     return sync(input, input2) >> sync(args...);
// }

// template <typename Input, typename ...Args>
// hub::Acquisition sync(Input && t, T && t2, Args && ... args) {
//     return t >> args...;
// }

// template <class Output, class Input>
template <class... Inputs>
static void checkSynchronize( hub::Output& output,
                              //                               const hub::SensorSpec& sensorSpec,
                              const std::vector<hub::Acquisition>& ref_acqs,
                              hub::Output& output2,
                              //                               const hub::SensorSpec& sensorSpec2,
                              const std::vector<hub::Acquisition>& ref_acqs2,
                              //                               hub::Input& input,
                              //                               hub::Input& input2,
                              const std::vector<hub::Acquisition>& refSyncAcqs,
                              bool delayed,
                              //                               hub::Input & input,
                              Inputs&&... inputs ) {

    std::set<hub::Acquisition> sortedAcqs = computeSortedAcqs( ref_acqs, ref_acqs2 );

    std::cout << "synching acqs" << std::endl;
    std::vector<hub::Acquisition> sync_acqs;
    bool writeDone     = false;
    std::thread thread = std::thread( [&]() {
        hub::Acquisition acq;

        if ( !delayed ) {
            while ( !writeDone ) {
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        }
        //	    for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
        try {

            //            while ( !inputSensor.getInput().isEnd() &&
            //            !inputSensor2.getInput().isEnd() ) {
            //            while ( !inputSensor.getInput().isEnd() &&
            //            !inputSensor2.getInput().isEnd() && while ( !input.isEnd() &&
            //            !input2.isEnd() &&
            while ( !areEnd( inputs... ) && sync_acqs.size() != refSyncAcqs.size() ) {

                //                input >> input2 >> acq;
                //                input >> acq;
                sync( inputs... ) >> acq;
                //            input >> input2 >> acq;
                std::cout << "\tread synched acq: " << acq << std::endl;

                sync_acqs.push_back( std::move( acq ) );
            }
        }
        catch ( hub::io::StreamInterface::exception& ex ) {
            std::cout << "[test_common] synchronize() catch exception : " << ex.what() << std::endl;
        }
        //        catch ( std::exception& ex ) {
        //            throw ex;
        //        }
        std::cout << "synching acqs done" << std::endl;
    } );

    writingProcess( ref_acqs, ref_acqs2, output, output2, sortedAcqs, delayed );
    writeDone = true;

    assert( thread.joinable() );
    thread.join();

    assert( sync_acqs.size() == refSyncAcqs.size() );
    for ( int i = 0; i < sync_acqs.size(); ++i ) {
        const auto& acq2 = sync_acqs.at( i );

        CHECK( acq2 == refSyncAcqs.at( i ) );
    }
}

// template <class Output, class Input>
// static void _checkSynchronize( Output&& output,
//                                const hub::SensorSpec& sensorSpec,
//                                const std::vector<hub::Acquisition>& ref_acqs,
//                                Output&& output2,
//                                const hub::SensorSpec& sensorSpec2,
//                                const std::vector<hub::Acquisition>& ref_acqs2,
//                                Input&& input,
//                                const std::vector<hub::Acquisition>& refSyncAcqs,
//                                bool delayed = false ) {

//    std::set<hub::Acquisition> sortedAcqs = computeSortedAcqs( ref_acqs, ref_acqs2 );

//    hub::OutputSensor outputSensor( sensorSpec, std::move( output ) );
//    std::cout << "outputSensor created" << std::endl;

//    hub::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
//    std::cout << "outputSensor2 created" << std::endl;
////    hub::InputSensor inputSensor2( std::move( input2 ) );
////    std::cout << "inputSensor2 created" << std::endl;

//    hub::InputSensor inputSensor( std::move( input ) );
//    std::cout << "inputSensor created" << std::endl;

//    std::cout << "synching acqs" << std::endl;
//    std::vector<hub::Acquisition> sync_acqs;
//    bool writeDone     = false;
//    std::thread thread = std::thread( [&]() {
//        hub::Acquisition acq;

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
// static void checkSynchronize( hub::OutputSensor&& outputSensor,
////                              const hub::SensorSpec& sensorSpec,
//                              const std::vector<hub::Acquisition>& ref_acqs,
//                              hub::OutputSensor&& outputSensor2,
////                              const hub::SensorSpec& sensorSpec2,
//                              const std::vector<hub::Acquisition>& ref_acqs2,
//                              hub::InputSensor&& inputSensor,
//                              hub::InputSensor&& inputSensor2,
//                              const std::vector<hub::Acquisition>& refSyncAcqs,
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

template <class Output, class Input>
static void checkSynchronize( Output&& output,
                              const hub::SensorSpec& sensorSpec,
                              const std::vector<hub::Acquisition>& ref_acqs,
                              Output&& output2,
                              const hub::SensorSpec& sensorSpec2,
                              const std::vector<hub::Acquisition>& ref_acqs2,
                              Input&& input,
                              Input&& input2,
                              const std::vector<hub::Acquisition>& refSyncAcqs,
                              bool delayed = false ) {

    hub::OutputSensor outputSensor( sensorSpec, std::move( output ) );
    std::cout << "outputSensor created" << std::endl;
    hub::InputSensor inputSensor( std::move( input ) );
    std::cout << "inputSensor created" << std::endl;

    hub::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
    std::cout << "outputSensor2 created" << std::endl;
    hub::InputSensor inputSensor2( std::move( input2 ) );
    std::cout << "inputSensor2 created" << std::endl;

    checkSynchronize( outputSensor.getOutput(),
                      //                       sensorSpec,
                      ref_acqs,
                      outputSensor2.getOutput(),
                      //                       sensorSpec2,
                      ref_acqs2,
                      refSyncAcqs,
                      delayed,
                      inputSensor.getInput(),
                      inputSensor2.getInput() );
}

// template <class Output, class Input>
// static void checkSynchronize( Output&& output,
//                               const hub::SensorSpec& sensorSpec,
//                               const std::vector<hub::Acquisition>& ref_acqs,
//                               Output&& output2,
//                               const hub::SensorSpec& sensorSpec2,
//                               const std::vector<hub::Acquisition>& ref_acqs2,
//                               Input&& input,
//                               Input&& input2,
//                               bool delayed = false ) {

//    std::cout << "ref_sync_acqs" << std::endl;
//    std::vector<hub::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

//    hub::OutputSensor outputSensor( sensorSpec, std::move( output ) );
//    std::cout << "outputSensor created" << std::endl;
//    hub::InputSensor inputSensor( std::move( input ) );
//    std::cout << "inputSensor created" << std::endl;

//    hub::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
//    std::cout << "outputSensor2 created" << std::endl;
//    hub::InputSensor inputSensor2( std::move( input2 ) );
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
//                               const hub::SensorSpec& sensorSpec,
//                               const std::vector<hub::Acquisition>& ref_acqs,
//                               Output&& output2,
//                               const hub::SensorSpec& sensorSpec2,
//                               const std::vector<hub::Acquisition>& ref_acqs2,
//                               Input&& input,
//                               bool delayed = false ) {

//    std::cout << "ref_sync_acqs" << std::endl;
//    std::vector<hub::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

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
