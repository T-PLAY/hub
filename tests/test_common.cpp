#include "test_common.hpp"

#include "Version.hpp"

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
    assert( nRatio == std::pow( 2, nMaxMean - 1 ) );

    const std::string logFilename = filename2 + "_" + name2 + extension;

    {
        std::ifstream inFile( logFilename.c_str() );
        if ( inFile.is_open() ) {
            assert( inFile.is_open() );

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

            if ( nValue >= 8 ) {
                const auto standardDeviation =
                    algo::StandardDeviation( lastValues.begin(), lastValues.end() );
                const auto mean = algo::Mean( lastValues.begin(), lastValues.end() );
                const auto minRatio =
                    mean - standardDeviation * 3.0; // correspond of 0.1% of the population, should
#ifndef DEBUG
                CHECK( minRatio <= value );
#endif
                if ( !( minRatio <= value ) ) {
                    std::cout << "---------------------------------------------> "
                                 "checkRatio: "
                              << minRatio << "(minRatio) <= " << value
                              << "(value), decline: " << value - mean << " " << unit << std::endl;
                    decline = true;
                }
            }
        }
    }

    if ( !decline ) {
        std::ofstream logFile( logFilename.c_str(), std::ios::out | std::ios::app );
        assert( logFile.is_open() );

        logFile << HUB_COMMIT_HASH << " " << value << " " << unit << std::endl;

        logFile.close();
    }

    {
        std::ifstream inFile( logFilename.c_str() );
        if ( inFile.is_open() ) {
            assert( inFile.is_open() );

            double value2;
            std::string hash;
            std::string unit;
            int iRatio = 0;
            std::vector<double> values( nRatio );
            for ( int i = 0; i < nRatio; ++i ) {
                values[i] = 0.0;
            }
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

            std::string report;

            const int nMean         = static_cast<int>( std::log2( nEl ) ) + 1;
            const auto meanAll      = sumRatios[nMean - 1] / std::pow( 2.0, nMean - 1 );
            const auto deviationAll = maxRatios[nMean - 1] - minRatios[nMean - 1];
            const auto epsilon      = deviationAll * 0.1;

            for ( int iMean = 0; iMean < nMean; ++iMean ) {
                const auto meanRatio = sumRatios[iMean] / std::pow( 2.0, iMean );
                const auto deviation = maxRatios[iMean] - minRatios[iMean];

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
                else if ( minDiff < -epsilon ) {
                    report += "\033[31m";
                }
                else {
                    report += "\033[33m";
                }
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
                else if ( diff < -epsilon ) {
                    report += "\033[31m";
                }
                else {
                    report += "\033[33m";
                }
                report += curRatioStr + "\033[0m";

                if ( i != nEl - 1 ) { report += " "; }
            }
            report += ")";

            if ( decline ) {
                const auto standardDeviation =
                    algo::StandardDeviation( values.begin(), values.begin() + nEl );
                const auto mean = algo::Mean( values.begin(), values.begin() + nEl );
                report += "\n\t\t\033[31mvalue:" + std::to_string( value ) +
                          ", mean:" + std::to_string( mean ) +
                          ", σ:" + std::to_string( standardDeviation ) +
                          ", mean-σ(15.9%):" + std::to_string( mean - standardDeviation ) +
                          ", mean-2σ(2.3%):" + std::to_string( mean - 2 * standardDeviation ) +
                          ", mean-3σ(0.1%):" + std::to_string( mean - 3 * standardDeviation ) +
                          "\033[0m";
            }

            _REPORT( "[" << name << "] " << report, filename, line );
        }
    }
}
