
#include <catch2/catch_test_macros.hpp>

// #include <algorithm>
#include <cmath>
// #include <ctime>
#include <fstream>
// #include <random>
// #include <set>
#include <iostream>
#include <thread>

// #include <io/input/Input.hpp>
// #include <io/output/Output.hpp>
// #include <sensor/Acquisition.hpp>
// #include <sensor/InputSensor.hpp>
// #include <sensor/OutputSensor.hpp>

// #include "Macros.hpp"
#include <Version.hpp>

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
    const unsigned int random = static_cast<int>( std::hash<std::string>()( filename ) ) + rand();
    const unsigned int ret    = offset + random % ( 65535 - offset );
    assert( offset <= ret && ret < 65536 );
    //    assert( ret != hub::io::StreamServer::s_defaultPort );
    std::cout << "using random port: " << ret << std::endl;
    return ret;
}

static std::string ReplaceAll( std::string str, const std::string& from, const std::string& to ) {
    size_t start_pos = 0;
    while ( ( start_pos = str.find( from, start_pos ) ) != std::string::npos ) {
        str.replace( start_pos, from.length(), to );
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

#define START_REPORT()                  \
    std::ofstream file( "report.txt" ); \
    assert( file.is_open() );
//    file << std::endl;                                                                             \
//    file << std::endl;                                                                             \
//    file << "####################################################################################" \
//            "##\n"                                                                                 \
//         << "#################################### START REPORT "                                   \
//            "####################################\n"                                               \
//         << "####################################################################################" \
//            "##";                                                                                  \
//    file.close();

#define END_REPORT() (void)0

//    std::ofstream file( "report.txt", std::ios::app );                                             \
//    assert( file.is_open() );                                                                      \
//    file << std::endl;                                                                             \
//    file << "####################################################################################" \
//            "##\n"                                                                                 \
//         << "##################################### END REPORT "                                    \
//            "#####################################\n"                                              \
//         << "####################################################################################" \
//            "##\n"                                                                                 \
//         << std::endl                                                                              \
//         << std::endl;                                                                             \
//    file.close();

#define PRINT_REPORT()                                                                           \
    do {                                                                                         \
        std::ifstream file( "report.txt" );                                                      \
        file.seekg( 0, std::ios::end );                                                          \
        auto len = file.tellg();                                                                 \
        if ( file.is_open() && len != 0 ) {                                                      \
            std::cout                                                                            \
                << std::endl                                                                     \
                << std::endl                                                                     \
                << "###########################################################################" \
                   "#########"                                                                   \
                   "##\n"                                                                        \
                << "#################################### START REPORT "                          \
                   "####################################\n"                                      \
                << "###########################################################################" \
                   "#########"                                                                   \
                   "##"                                                                          \
                << std::endl;                                                                    \
            file.seekg( 0, std::ios::beg );                                                      \
            std::cout << file.rdbuf();                                                           \
            std::cout                                                                            \
                << std::endl                                                                     \
                << "###########################################################################" \
                   "#########"                                                                   \
                   "##\n"                                                                        \
                << "##################################### END REPORT "                           \
                   "#####################################\n"                                     \
                << "###########################################################################" \
                   "#########"                                                                   \
                   "##\n"                                                                        \
                << std::endl                                                                     \
                << std::endl;                                                                    \
        }                                                                                        \
        file.close();                                                                            \
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
            file << "------------> running test " << testName << ":" << std::endl; \
        }                                                                          \
        file << _params << std::endl;                                              \
        file.close();                                                              \
    } while ( false );

#define REPORT( _params ) _REPORT( _params, FILE_NAME, __LINE__ )
#define REPORT_NEW_LINE _REPORT( "", FILE_NAME, __LINE__ )

/////////////////////////////////////////////////////////////////////////////////

static void _checkValue( double value,
                         double compare,
                         double gap,
                         const std::string& name,
                         const std::string& unit,
                         const std::string& filename,
                         int line ) {
    //    const int gap = 10;
    CHECK( ( compare - gap <= value && value <= compare + gap ) );
    if ( !( compare - gap <= value && value <= compare + gap ) ) {
        std::cout << "-----------------------------------------------------------------------------"
                     "---------------------------------------------------------------> checkRatio: "
                  << compare - gap << " <= " << value << " <= " << compare + gap << std::endl;
    }

    // std::cout << "[checkValue] name: '" << name << "'" << std::endl;
    // std::cout << "[checkValue] filename: '" << filename << "'" << std::endl;

    //    std::ofstream file("ouou", std::ios::app);
    std::string name2 = name;
    //    name2.replace(name2.begin(), name2.end(), '/', '-');
    //    if ()
    name2 = ReplaceAll( name2, "/", "_vs_" );
    name2 = ReplaceAll( name2, ":", "_" );
    name2 = ReplaceAll( name2, " ", "" );
    //    std::remove( name2.begin(), name2.end(), ' ' );
    //    name2.erase( std::remove_if( name2.begin(), name2.end(), std::isspace ), name2.end() );

    constexpr auto extension = ".log";

    // std::cout << "[checkValue] name2: '" << name2 << "'" << std::endl;

    // std::string rootPath = HUB_TESTS_BIN_DIR;

    //    std::cout << "checkRatio " << filename << std::endl;
    {
        std::ofstream logFile( ( filename + "_" + name2 + extension ).c_str(),
                               std::ios::out | std::ios::app );
        assert( logFile.is_open() );

        logFile << HUB_COMMIT_HASH << " " << value << " " << unit << std::endl;

        logFile.close();
    }

    {
        std::ifstream inFile( ( filename + "_" + name2 + extension ).c_str() );
        assert( inFile.is_open() );

        double value2;
        std::string hash;
        std::string unit;
        int iRatio             = 0;
        constexpr int nMaxMean = 4;
        constexpr int nRatio   = 8;
        assert( nRatio == std::pow( 2, nMaxMean - 1 ) );
        std::string hashes[nRatio];
        double values[nRatio];
        for ( int i = 0; i < nRatio; ++i ) {
            values[i] = 0.0;
        }
        while ( !inFile.eof() ) {
            value2 = -1;
            hash   = "";
            inFile >> hash >> value2 >> unit;
            if ( value2 != -1 ) {
                //                sumRatio += value2;
                values[iRatio % nRatio] = value2;
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
            minRatios[i] = values[( iRatio - 1 ) % nRatio];
            maxRatios[i] = values[( iRatio - 1 ) % nRatio];
        }

        const int nEl = std::min( nRatio, iRatio );
        for ( int i = 0; i < nEl; ++i ) {
            //            std::cout << hashes[i] << " " << values[i] << std::endl;
            const int idx       = ( iRatio - 1 - i ) % nRatio;
            const auto curRatio = values[idx];

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

        const int nMean    = static_cast<int>( std::log2( nEl ) ) + 1;
        const auto meanAll = sumRatios[nMean - 1] / std::pow( 2.0, nMean - 1 );

        for ( int iMean = 0; iMean < nMean; ++iMean ) {
            //            for ( int iMean = 0; iMean < nMean; ++iMean ) {
            const auto meanRatio = sumRatios[iMean] / std::pow( 2.0, iMean );
            const auto deviation = maxRatios[iMean] - minRatios[iMean];
            //            }

            //        std::cout << "average of ratio for the last " << std::min(nRatio, iRatio) << "
            //        builds is " << meanRatio << std::endl;
            //            for ( int iMean = 0; iMean < nMean; ++iMean ) {
            std::string meanRatioStr   = std::to_string( meanRatio );
            meanRatioStr               = meanRatioStr.substr( 0, 5 );
            std::string deviationStr   = std::to_string( deviation );
            deviationStr               = deviationStr.substr( 0, 5 );
            std::string meanCompareStr = std::to_string( meanRatio - meanAll );
            meanCompareStr             = meanCompareStr.substr( 0, 5 );

            report += "(" + std::to_string( (int)std::pow( 2, iMean ) ) + "): " + meanRatioStr +
                      " " + deviationStr + "+- " + meanCompareStr;
            if ( iMean != std::log2( nEl ) ) { report += ", "; }
            //            }
        }

        report += "  (";
        for ( int i = 0; i < nEl; ++i ) {
            const int idx = ( iRatio - 1 - i ) % nRatio;
            //            const auto curRatio = values[idx];
            auto curRatioStr = std::to_string( values[idx] );
            curRatioStr      = curRatioStr.substr( 0, 5 );
            report += curRatioStr;

            if ( i != nEl - 1 ) { report += " "; }
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

#define CHECK_VALUE( ... ) _checkValue( __VA_ARGS__, FILE_NAME, __LINE__ )

// template <typename T>
// bool areEnd( T&& t ) {
//     return t.isEnd();
// }

// template <typename T, typename... Inputs>
// bool areEnd( T&& t, Inputs&&... args ) {
//     return t.isEnd() && areEnd( args... );
// }

// template <typename Input>
// Input sync( Input&& input ) {
//     return input;
// }
