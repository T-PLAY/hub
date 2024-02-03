

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <thread>

#include <core/Macros.hpp>
#include <core/Utils.hpp>

#define CONSTRUCT_BEGIN( name )                                                              \
    std::cout << "\033[1;32m[test] -------------------------------------------------> " name \
              << "() ..."                                                                    \
              << "\033[0m" << std::endl;

#define CONSTRUCT_END( name )                                                                   \
    std::cout << "\033[1;32m[test] -------------------------------------------------> " << name \
              << "() done"                                                                      \
              << "\033[0m" << std::endl;

#define DESTRUCT_BEGIN( name )                                                          \
    std::cout << "\033[1;31m[test] =================================================> " \
              << "~" << name << "() ..."                                                \
              << "\033[0m" << std::endl;

#define DESTRUCT_END( name )                                                            \
    std::cout << "\033[1;31m[test] =================================================> " \
              << "~" << name << "() done"                                               \
              << "\033[0m" << std::endl;

#define TEST_BEGIN() auto start_test = std::chrono::high_resolution_clock::now();

#define TEST_END()                                                                              \
    const auto end_test = std::chrono::high_resolution_clock::now();                            \
    const auto duration =                                                                       \
        std::chrono::duration_cast<std::chrono::milliseconds>( end_test - start_test ).count(); \
    std::cout << "[" << FILE_NAME << "] duration : " << duration << " ms" << std::endl;         \
    std::ofstream file( "duration_tests.txt", std::ios::app );                                  \
    file << duration << " " << FILE_NAME << std::endl;                                          \
    file.close();

#define GET_RANDOM_PORT getRandomPort( __FILE__ )

static int getRandomPort( const char* filename ) {

    const std::string randomUsedPortsFilename = "randomUsedPorts.txt";
    std::map<int, std::string> usedPorts;

    std::ifstream inFile( randomUsedPortsFilename.c_str() );
    if ( inFile.is_open() ) {

        std::string testName;
        int port;
        while ( !inFile.eof() ) {
            testName = "";
            port     = 0;
            inFile >> port >> testName;
            if ( port != 0 ) { usedPorts.insert( std::make_pair( port, testName ) ); }
        }
        inFile.close();
    }

    srand( (unsigned)time( NULL ) );
    int randomPort;
    do {
        const unsigned int random =
            static_cast<int>( std::hash<std::string>()( filename ) ) + rand();
        randomPort = 10'000 + ( random % 1'000 ) * 10;
    } while ( usedPorts.find( randomPort ) != usedPorts.end() );

    std::ofstream outFile( randomUsedPortsFilename.c_str(), std::ios::out | std::ios::app );
    assert( outFile.is_open() );
    outFile << randomPort << " " << filename << std::endl;
    outFile.close();

    std::cout << "using random port: " << randomPort << std::endl;
    return randomPort;
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

#define END_REPORT() (void)0

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

namespace algo {

#if CPP_VERSION >= 20
static double Mean( const auto& first, const auto& last ) {
#else
template <class T>
static double Mean( const T& first, const T& last ) {
#endif
    return std::accumulate( first, last, 0.0 ) / (double)( last - first );
}

#if CPP_VERSION >= 20
static double Variance( const auto& first, const auto& last ) {
#else
template <class T>
static double Variance( const T& first, const T& last ) {
#endif
    const auto mean = Mean( first, last );

    auto variance = 0.0;
    auto it       = first;
    while ( it != last ) {
        const auto& sample     = *it;
        const auto diff        = sample - mean;
        const auto diff_square = diff * diff;
        variance += diff_square;
        ++it;
    }
    variance /= (double)( last - first );
    return variance;
}

#if CPP_VERSION >= 20
static double StandardDeviation( const auto& first, const auto& last ) {
#else
template <class T>
static double StandardDeviation( const T& first, const T& last ) {
#endif
    return sqrt( Variance( first, last ) );
}
} // namespace algo

void _checkValue( double value,
                  const std::string& name,
                  const std::string& unit,
                  const std::string& filename,
                  int line );

#define CHECK_DECLINE( ... ) _checkValue( __VA_ARGS__, FILE_NAME, __LINE__ )

// Input sync( Input&& input ) {
