#pragma once

#include <chrono>
#include <string>
#include <tuple>
#include <vector>

// #include <core/Macros.hpp>
#include <core/Traits.hpp>
#include <core/ioBase.hpp>
// #include <core/Vector.hpp>

namespace testCoreIoCommon {

struct UserData {
    double a;
    int b;
    std::string name;
    std::vector<int> vints;

    //    constexpr auto serialize() {
    //        return std::tuple<double&, int&, std::string&, std::vector<int>&> { a, b, name, vints
    //        };
    //    }

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, name, vints );
    }

    bool operator==( const UserData& character ) const {
        return a == character.a && b == character.b && name == character.name &&
               vints == character.vints;
    }
    friend std::ostream& operator<<( std::ostream& os, const UserData& character );
};
// static_assert( hub::io::serializable_v<UserData> );
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );

static constexpr size_t s_dataSize = sizeof( UserData );

// std::ostream& operator<<( std::ostream& os, const UserData& character ) {
//     os << character.a << " " << character.b << " " << character.name;
//     return os;
// }

struct ReadWriteStat {
    long long durationInNanoSecond = 0;
    double megaReadWritePerSecond  = 0;
    double gigaBytePerSecond       = 0;

    long long nInputOutputCall = 0;

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
    bool operator==( const BenchStat& benchStat ) const { return name == benchStat.name; }
};

template <class BenchStats>
void printStats( const BenchStats& benchStats ) {

    std::cout << std::endl;

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
            if (readWriteDataStat.nInputOutputCall != 0) {
                std::cout << "[" << implName << "] total io call: " << readWriteDataStat.nInputOutputCall << std::endl;
            }
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

template <class ReadInputFunc, class WriteOutputFunc, class UserData>
ReadWriteStat readWriteData( ReadInputFunc& readInputFunc,
                             WriteOutputFunc& writeOutputFunc,
                             size_t nReadWrite,
                             const UserData& data ) {

    // #ifdef HUB_DEBUG_INPUT
    //     std::cout << "------------------------------ " << implName
    //               << " --------------------------------" << std::endl;
    // #endif

    const auto startClock = std::chrono::high_resolution_clock::now();

    UserData data_write = data;

    size_t iReadWrite = 0;
    while ( iReadWrite < nReadWrite ) {

        data_write.b = iReadWrite;

        writeOutputFunc( data_write );
        UserData data_read;
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
    stats.gigaBytePerSecond      = stats.megaReadWritePerSecond * sizeof( UserData ) / 1000.0;
    return stats;
};

template <class T>
concept GetableNCall = requires( const T a ) { a.getNCall(); };

template <class InputOutput, class UserData>
//    requires GetableNCall<InputOutput>
ReadWriteStat readWriteData( InputOutput& inputOutput, size_t nReadWrite, const UserData& data ) {
    constexpr bool getableNCall = GetableNCall<InputOutput>;
    if constexpr ( getableNCall ) {
#ifdef DEBUG
        size_t nCallFirst = inputOutput.getNCall();
#endif
        auto read  = [&]( UserData& data ) { inputOutput.read( data ); };
        auto write = [&]( const UserData& data ) { inputOutput.write( data ); };
        auto ret   = readWriteData( read, write, nReadWrite, data );
#ifdef DEBUG
        ret.nInputOutputCall = inputOutput.getNCall() - nCallFirst;
#endif
        return ret;
    }
    else {
        auto read  = [&]( UserData& data ) { inputOutput.read( data ); };
        auto write = [&]( const UserData& data ) { inputOutput.write( data ); };
        return readWriteData( read, write, nReadWrite, data );
    }
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
