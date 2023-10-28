#pragma once

#include <chrono>
#include <string>
#include <tuple>
#include <vector>

// #include <core/Macros.hpp>
#include <core/Traits.hpp>

namespace testCoreIoCommon {

struct Character {
    double a;
    int b;
    std::string name;
    std::vector<int> vints;

    constexpr auto serialize() {
        return std::tuple<double&, int&, std::string&, std::vector<int>&> { a, b, name, vints };
    }

    bool operator==( const Character& character ) const {
        return a == character.a && b == character.b && name == character.name &&
               vints == character.vints;
    }
    friend std::ostream& operator<<( std::ostream& os, const Character& character );
};
static_assert( hub::serializable_v<Character> );
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );

//std::ostream& operator<<( std::ostream& os, const Character& character ) {
//    os << character.a << " " << character.b << " " << character.name;
//    return os;
//}

struct ReadWriteStat {
    long long durationInNanoSecond = 0;
    double megaReadWritePerSecond = 0;
    double gigaBytePerSecond = 0;

    bool operator<( const ReadWriteStat& benchStat ) const {
        return durationInNanoSecond < benchStat.durationInNanoSecond;
    }
};

struct BenchStat {
    std::string name;
    ReadWriteStat readWriteDataStat {};
    ReadWriteStat readWriteDataPtrStat {};

    bool operator<( const BenchStat& benchStat ) const {

        return readWriteDataStat.durationInNanoSecond + readWriteDataPtrStat.durationInNanoSecond <
               benchStat.readWriteDataStat.durationInNanoSecond +
                   benchStat.readWriteDataPtrStat.durationInNanoSecond;
    }
    bool operator==(const BenchStat & benchStat) const {
        return name == benchStat.name;
    }
};

template <class BenchStats>
void printStats(const BenchStats & benchStats) {

    //    for ( int iImpl = 0; iImpl < dataBenchStats.size(); ++iImpl ) {
    for ( const auto& dataBenchStat : benchStats ) {
        //        const auto& dataBenchStat = dataBenchStats.at( iImpl );
        const auto& implName = dataBenchStat.name;

        {
            const ReadWriteStat& readWriteDataStat = dataBenchStat.readWriteDataStat;
            std::cout << "------------------------------------" << std::endl;
            std::cout << "[" << implName << "] writing/reading rate: "
                      << std::to_string( readWriteDataStat.megaReadWritePerSecond / 1000.0 )
                      << " GigaReadWrite/s, "
                      << std::to_string( readWriteDataStat.gigaBytePerSecond ) << " Go/s"
                      << std::endl;
            std::cout << "[" << implName
                      << "] total time: " << readWriteDataStat.durationInNanoSecond / 1000'000.0
                      << " ms" << std::endl;
        }

        {
            const ReadWriteStat& readWriteDataPtrStat = dataBenchStat.readWriteDataPtrStat;
            if ( readWriteDataPtrStat.durationInNanoSecond != 0 ) {
                std::cout << "[" << implName << "] writing/reading data ptr rate: "
                          << std::to_string( readWriteDataPtrStat.gigaBytePerSecond ) << " Go/s"
                          << std::endl;
                std::cout << "[" << implName << "] total time: "
                          << readWriteDataPtrStat.durationInNanoSecond / 1000'000.0 << " ms"
                          << std::endl;
            }
        }
    }
}

template <class ReadInputFunc, class WriteOutputFunc, class Data>
ReadWriteStat readWriteData( ReadInputFunc& readInputFunc,
                             WriteOutputFunc& writeOutputFunc,
                             size_t nReadWrite,
                             const Data& data ) {

#ifdef HUB_DEBUG_INPUT
    std::cout << "------------------------------ " << implName
              << " --------------------------------" << std::endl;
#endif

    const auto startClock = std::chrono::high_resolution_clock::now();

    Data data_write = data;

    size_t iReadWrite = 0;
    while ( iReadWrite < nReadWrite ) {

        data_write.b = iReadWrite;

        writeOutputFunc( data_write );
        Data data_read;
        readInputFunc( data_read );

        if ( data_read != data_write ) {
            //            std::cerr << "writeRead data_write != data_read for impl : " << implName
            //            << std::endl;
            std::cerr << data_read << " != " << data_write << std::endl;
            exit( 1 );
        }

        ++iReadWrite;
    }
    const auto endClock = std::chrono::high_resolution_clock::now();
    ReadWriteStat stats;
    stats.durationInNanoSecond =
        std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();

    stats.megaReadWritePerSecond = 1000.0 * nReadWrite / stats.durationInNanoSecond;
    stats.gigaBytePerSecond      = stats.megaReadWritePerSecond * sizeof( Data ) / 1000.0;
    return stats;
};

template <class InputOutput, class Data>
ReadWriteStat readWriteData( InputOutput& inputOutput, size_t nReadWrite, const Data& data ) {
    auto read  = [&]( Data& data ) { inputOutput.read( data ); };
    auto write = [&]( const Data& data ) { inputOutput.write( data ); };
    return readWriteData( read, write, nReadWrite, data );
}

template <class ReadInputFunc, class WriteOutputFunc>
auto readWriteDataPtr( ReadInputFunc& readInputFunc,
                       WriteOutputFunc& writeOutputFunc,
                       size_t nReadWrite,
                       unsigned char* data_write,
                       size_t sizeData ) {

    unsigned char* data_read = new unsigned char[sizeData];

    const auto startClock = std::chrono::high_resolution_clock::now();
    size_t iReadWrite     = 0;
    while ( iReadWrite < nReadWrite ) {

        data_write[0] = iReadWrite;

        writeOutputFunc( data_write, sizeData );
        data_read[0] = 1 + iReadWrite;
        readInputFunc( data_read, sizeData );

        if ( memcmp( data_read, data_write, sizeData ) != 0 ) { exit( 1 ); }

        ++iReadWrite;
    }
    const auto endClock = std::chrono::high_resolution_clock::now();
    ReadWriteStat stats;
    stats.durationInNanoSecond =
        std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
    stats.megaReadWritePerSecond = 1000.0 * nReadWrite / stats.durationInNanoSecond;
    stats.gigaBytePerSecond      = stats.megaReadWritePerSecond * sizeData / 1000.0;

    delete[] data_read;

    return stats;
};

template <class InputOutput>
ReadWriteStat readWriteDataPtr( InputOutput& inputOutput,
                                size_t nReadWrite,
                                unsigned char* data,
                                size_t sizeData ) {
    auto read  = [&]( hub::Data_t* data, hub::Size_t size ) { inputOutput.read( data, size ); };
    auto write = [&]( hub::Data_t* data, hub::Size_t size ) { inputOutput.write( data, size ); };
    return readWriteDataPtr( read, write, nReadWrite, data, sizeData );
}

} // namespace testCoreIoCommon
