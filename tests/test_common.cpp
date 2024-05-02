#include "test_common.hpp"

#include <filesystem>

#include "Version.hpp"

using namespace hub::utils;

void _checkValue( double value,
                  const std::string& name,
                  const std::string& unit,
                  const std::string& filename,
                  int line ) {

    std::string filename2 = filename;
    filename2             = ReplaceAll( filename, ".", "_" );

    std::string name2 = name;
    name2             = ReplaceAll( name2, "/", "_vs_" );
    name2             = ReplaceAll( name2, ":", "_" );
    name2             = ReplaceAll( name2, " ", "" );
    name2             = ReplaceAll( name2, "(", "_" );
    name2             = ReplaceAll( name2, ")", "_" );
    name2             = ReplaceAll( name2, ">", "_" );

    constexpr auto extension = "_v1_0_5.log";
    bool decline             = false;
    constexpr int nMaxMean   = 4;
    constexpr int nRatio     = 8;
    CHECK( nRatio == std::pow( 2, nMaxMean - 1 ) );

    const std::string declineFolder = HUB_STATS_DIR;

    const std::string logFilename = filename2 + "_" + name2 + extension;

    {
        std::ifstream inFile( declineFolder + logFilename.c_str() );
        if ( inFile.is_open() ) {
            CHECK( inFile.is_open() );

            std::vector<double> lastValues;
            std::string hash;
            std::string unit;
            double value2;
            while ( !inFile.eof() ) {
                value2 = -1;
                inFile >> hash >> value2 >> unit;
                if ( value2 != -1 ) { lastValues.push_back( value2 ); }
            }
            inFile.close();

            const int nValue = lastValues.size();

            if ( nValue >= nRatio ) {
                const auto bInf = std::prev(lastValues.end(), nRatio);
                assert(std::distance(bInf, lastValues.end()) == nRatio);
                const auto standardDeviation = algo::StandardDeviation(
                    bInf, lastValues.end() );
                const auto mean =
                    algo::Mean( bInf, lastValues.end() );
                const auto minRatio = mean - standardDeviation * 2.0;
#ifndef DEBUG
                CHECK( minRatio <= value );
#endif
                if ( !( minRatio <= value ) ) {
                    std::cout << "---------------------------------------------> "
                                 "checkRatio: "
                              << value << "(value) < " << minRatio
                              << "(minRatio), decline: " << value - mean << " " << unit
                              << std::endl;
                    decline = true;
                }
            }
        }
    }

    {
        int iRatio = 0;
        std::vector<double> values( nRatio );
        std::string report;

        std::ifstream inFile( declineFolder + logFilename.c_str() );
        if ( inFile.is_open() ) {
            CHECK( inFile.is_open() );

            // get latests nRatio values in input file
            for ( int i = 0; i < nRatio; ++i ) {
                values[i] = 0.0;
            }
            double value2;
            std::string hash;
            std::string unit;
            while ( !inFile.eof() ) {
                value2 = -1;
                hash   = "";
                inFile >> hash >> value2 >> unit;
                if ( value2 != -1 ) {
                    values[iRatio % nRatio] = value2;
                    ++iRatio;
                }
            }
            inFile.close();
        }

        if ( !decline ) {
            values[iRatio % nRatio] = value;
            ++iRatio;
        }

        // get sum, min and max for each nMaxMean pack
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

        const int nMean         = static_cast<int>( std::log2( nEl ) ) + 1;
        const auto meanAll      = sumRatios[nMean - 1] / std::pow( 2.0, nMean - 1 );
        const auto deviationAll = ( maxRatios[nMean - 1] - minRatios[nMean - 1] ) / 2.0;
        const auto epsilon      = deviationAll * 0.2;

        for ( int iMean = 0; iMean < nMean; ++iMean ) {
            const auto meanRatio = sumRatios[iMean] / std::pow( 2.0, iMean );
            const auto deviation = ( maxRatios[iMean] - minRatios[iMean] ) / 2.0;

            std::string meanRatioStr   = std::to_string( meanRatio );
            meanRatioStr               = meanRatioStr.substr( 0, 5 );
            std::string deviationStr   = std::to_string( deviation );
            deviationStr               = deviationStr.substr( 0, 5 );
            const auto minDiff         = meanRatio - meanAll;
            std::string meanCompareStr = std::to_string( minDiff );
            meanCompareStr             = meanCompareStr.substr( 0, 5 );

            report += "(" + std::to_string( (int)std::pow( 2, iMean ) ) + "): " + meanRatioStr +
                      " " + deviationStr + "+- ";

            if ( minDiff > epsilon ) { report += "\033[32m"; }
            else if ( minDiff < -epsilon ) { report += "\033[31m"; }
            else { report += "\033[33m"; }
            report += meanCompareStr + "\033[0m";

            if ( iMean != std::log2( nEl ) ) { report += ", "; }
        }

        report += "  (";
        for ( int i = 0; i < nEl; ++i ) {
            const int idx        = ( iRatio - 1 - i ) % nRatio;
            const auto lastValue = values[idx];
            const auto diff      = lastValue - meanAll;
            auto curRatioStr     = std::to_string( lastValue );
            curRatioStr          = curRatioStr.substr( 0, 5 );

            if ( diff > epsilon ) { report += "\033[32m"; }
            else if ( diff < -epsilon ) { report += "\033[31m"; }
            else { report += "\033[33m"; }
            report += curRatioStr + "\033[0m";

            if ( i != nEl - 1 ) { report += " "; }
        }
        report += ")";

        const auto standardDeviation =
            algo::StandardDeviation( values.begin(), values.begin() + nEl );
        const auto mean = algo::Mean( values.begin(), values.begin() + nEl );

        if ( decline ) {
            report +=
                "\n\t\t\033[31mvalue:" + std::to_string( value ) +
                ", mean:" + std::to_string( mean ) + ", σ:" + std::to_string( standardDeviation ) +
                ", mean-σ(15.9%):" + std::to_string( mean - standardDeviation ) +
                ", mean-2σ(2.3%):" + std::to_string( mean - 2 * standardDeviation ) +
                ", mean-3σ(0.1%):" + std::to_string( mean - 3 * standardDeviation ) + "\033[0m";
        }

        _REPORT( "[" << name << "] " << report, filename, line );
    }

    if ( !decline ) {
        {
            if ( !std::filesystem::exists( declineFolder ) ) {
                std::filesystem::create_directories( declineFolder );
            }
            std::ofstream logFile( declineFolder + logFilename.c_str(),
                                   std::ios::out | std::ios::app );
            CHECK( logFile.is_open() );

            logFile << HUB_COMMIT_HASH << " " << value << " " << unit << std::endl;

            logFile.close();
        }
    }
}
