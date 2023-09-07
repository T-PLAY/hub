
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

static void _checkRatio( double ratio,
                         int compare,
                         int gap,
                         const std::string& name,
                         const std::string& filename ) {
    //    const int gap = 10;
    CHECK( ( compare - gap <= ratio && ratio <= compare + gap ) );
    if ( !( compare - gap <= ratio && ratio <= compare + gap ) ) {
        std::cout << "-----------------------------------------------------------------------------"
                     "---------------------------------------------------------------> checkRatio: "
                  << compare - gap << " <= " << ratio << " <= " << compare + gap << std::endl;
    }

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
        int iRatio      = 0;
        constexpr int nRatio = 10;
        std::string hashes[nRatio];
        double ratios[nRatio] = {0.0};
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
        assert(iRatio > 0);
        double sumRatio = 0.0;
        double minRatio = ratios[0];
        double maxRatio = ratios[0];
        const int nEl = std::min(nRatio, iRatio);
        for (int i = 0;i < nEl; ++i) {
//            std::cout << hashes[i] << " " << ratios[i] << std::endl;
            sumRatio += ratios[i];
            minRatio = std::min(minRatio, ratios[i]);
            maxRatio = std::max(maxRatio, ratios[i]);
        }
        const double meanRatio = sumRatio / nEl;
        const double deviation = maxRatio - minRatio;

//        std::cout << "average of ratio for the last " << std::min(nRatio, iRatio) << " builds is " << meanRatio << std::endl;
        std::cout << "[" << name << "] ratio : " << ratio  << ", mean : " << meanRatio << " (" << nEl << "), deviation : +/- " << deviation / 2.0 << std::endl;

        inFile.close();
    }
}

#define checkRatio( ... ) _checkRatio( __VA_ARGS__, FILE_NAME )

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

template <class Output, class Input>
static std::vector<hub::Acquisition> synchronize( Output&& output,
                                                  const hub::SensorSpec& sensorSpec,
                                                  const std::vector<hub::Acquisition>& ref_acqs,
                                                  Output&& output2,
                                                  const hub::SensorSpec& sensorSpec2,
                                                  const std::vector<hub::Acquisition>& ref_acqs2,
                                                  Input&& input,
                                                  Input&& input2,
                                                  bool delayed = false ) {

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

    hub::OutputSensor outputSensor( sensorSpec, std::move( output ) );
    std::cout << "outputSensor created" << std::endl;
    hub::InputSensor inputSensor( std::move( input ) );
    std::cout << "inputSensor created" << std::endl;

    hub::OutputSensor outputSensor2( sensorSpec2, std::move( output2 ) );
    std::cout << "outputSensor2 created" << std::endl;
    hub::InputSensor inputSensor2( std::move( input2 ) );
    std::cout << "inputSensor2 created" << std::endl;

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
            while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

                inputSensor >> inputSensor2 >> acq;
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

    const auto& acqFront2 = ref_acqs2.front();
    std::cout << "write acq2: " << acqFront2 << std::endl;
    outputSensor2 << acqFront2;

    const auto& acqFront = ref_acqs.front();
    std::cout << "write acq: " << acqFront << std::endl;
    outputSensor << acqFront;

    if ( delayed ) { std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); }

    for ( const auto& acq : sortedAcqs ) {
        if ( acq.getMeasures().front().getResolution() == sensorSpec.getResolutions().front() ) {
            std::cout << "write acq: " << acq << std::endl;
            outputSensor << acq;
        }
        else {
            std::cout << "write acq2: " << acq << std::endl;
            outputSensor2 << acq;
        }
        if ( delayed ) { std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) ); }
    }
    writeDone = true;

    assert( thread.joinable() );
    thread.join();

    return sync_acqs;
}

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
