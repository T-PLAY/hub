
#include <catch2/catch_test_macros.hpp>

#include <ctime>
#include <random>

#include <Acquisition.hpp>

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
    return ret;
}

static int computeDist( const hub::Acquisition& acq, const hub::Acquisition& acq2 ) {
    return std::abs( acq.getStart() - acq2.getStart() );
}

static void checkRatio( double ratio, int compare, int gap ) {
    //    const int gap = 10;
    CHECK( ( compare - gap <= ratio && ratio <= compare + gap ) );
    if ( !( compare - gap <= ratio && ratio <= compare + gap ) ) {
        std::cout << "-----------------------------------------------------------------------------"
                     "---------------------------------------------------------------> checkRatio: "
                  << compare - gap << " <= " << ratio << " <= " << compare + gap << std::endl;
    }
}

static std::vector<hub::Acquisition>
computeSyncAcqs( const std::vector<hub::Acquisition>& leftAcqs,
                 const std::vector<hub::Acquisition>& rightAcqs ) {
    std::vector<int> min_dists( rightAcqs.size(), 999999 );
    std::vector<int> iMin_dists( rightAcqs.size(), -1 );

    int iAcq2 = 0;
    for ( const auto& acq2 : rightAcqs ) {
        int iMinDist = 0;
        int minDist  = computeDist( acq2, leftAcqs.front() );
        for ( int iAcq = 1; iAcq < leftAcqs.size(); ++iAcq ) {
            const auto& acq = leftAcqs.at( iAcq );
            const auto dist = computeDist( acq2, acq );
            if ( dist <= minDist ) {
                minDist  = dist;
                iMinDist = iAcq;
            }
        }

        if ( !( ( iMinDist == 0 || iMinDist == leftAcqs.size() - 1 ) && minDist != 0 ) ) {

            //            if ( minDist < min_dists.at( iMinDist ) ) {
            min_dists[iAcq2]  = minDist;
            iMin_dists[iAcq2] = iMinDist;
        }
        //            }
        ++iAcq2;
    }

    std::vector<hub::Acquisition> syncAcqs;

//    std::cout << "ref_sync_acqs" << std::endl;

    for ( int i = 0; i < rightAcqs.size(); ++i ) {
        if ( iMin_dists[i] != -1 ) {
            const auto& acq  = leftAcqs.at( iMin_dists.at( i ) );
            const auto& acq2 = rightAcqs.at( i );

            hub::Acquisition sync_acq( acq2.getStart(), acq2.getEnd() );
            sync_acq << acq.getMeasures() << acq2.getMeasures();
            syncAcqs.push_back( std::move( sync_acq ) );

            auto& syncAcq = syncAcqs.back();
            //                syncAcq << acq2.getMeasures();

            std::cout << syncAcqs.back() << std::endl;

            assert( syncAcq.getStart() == acq2.getStart() );
            assert( syncAcq.getEnd() == acq2.getEnd() );
            assert( syncAcq.getMeasures().size() == 2 );

            assert( acq.getMeasures().size() == 1 );
            const auto& measure = acq.getMeasures().front();
            assert( syncAcq.getMeasures().at( 0 ) == measure );

            assert( acq2.getMeasures().size() == 1 );
            const auto& measure2 = acq2.getMeasures().front();
            assert( syncAcq.getMeasures().at( 1 ) == measure2 );
        }
    }

    std::cout << std::endl;
    return syncAcqs;
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
