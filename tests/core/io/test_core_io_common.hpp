#pragma once

#include <chrono>
#include <cstring>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

// #include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <core/Serializer.hpp>
#include <core/io/Archive.hpp>
#include <core/ios.hpp>

namespace testCoreIoCommon {

class WritableReadableData
{
  public:
    WritableReadableData( std::string name, int value ) : m_name { name }, m_value { value } {};
    WritableReadableData() = default;
    auto toString() const { return "name: '" + m_name + "'" + " " + std::to_string( m_value ); }

    void write( hub::Serializer& serializer ) const { serializer.writeAll( m_name, m_value ); }
    void read( hub::Serializer& serializer ) { serializer.readAll( m_name, m_value ); }
    bool operator==( const WritableReadableData& other ) const {
        return m_name == other.m_name && m_value == other.m_value;
    }

  private:
    std::string m_name;
    int m_value;
};

class SerializedClass
{
  public:
    SerializedClass( int a, bool b ) : m_a { a }, m_b { b } {};
    SerializedClass() = default;
    bool operator==( const SerializedClass& other ) const {
        return m_a == other.m_a && m_b == other.m_b;
    }
    auto toString() const { return hub::to_string( m_a, m_b ); }

    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_a, self.m_b );
    }

  private:
    int m_a;
    bool m_b;
};

enum class UserEnum2 { A, B, C };
static auto toString( UserEnum2 ue ) -> std::string {
    switch ( ue ) {
    case UserEnum2::A:
        return "A";
        break;
    case UserEnum2::B:
        return "B";
        break;
    case UserEnum2::C:
        return "C";
        break;
    default:
        break;
    }
    return "None";
}

template <class T>
void checkType( hub::io::Archive& archive, const T& t ) {
    CHECK( archive.isEnd() );

    archive.write( t );
    T t_read;
    archive.read( t_read );
    CHECK( t == t_read );

    CHECK( archive.isEnd() );
#ifdef DEBUG_CHECK_TYPE
    std::cout << "<" << TYPE_NAME( t ) << ">: " << t << std::endl;
#endif
}

struct UserData {
    double a;
    int b;
    std::string name;
    std::vector<int> vints;

    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.a, self.b, self.name, self.vints );
    }

    auto toString() const {
        return std::to_string( a ) + " " + std::to_string( b ) + " " + name + " " +
               hub::to_string( vints );
    }

    bool operator==( const UserData& character ) const {
        return a == character.a && b == character.b && name == character.name &&
               vints == character.vints;
    }
};
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );

static constexpr size_t s_dataSize = sizeof( UserData );

struct ReadWriteStat {
    long long durationInNanoSecond = 0;
    double megaReadWritePerSecond  = 0;
    double gigaBytePerSecond       = 0;

    long long nInputOutputCall    = 0;
    long long inputOutputCallSize = 0;

    bool operator<( const ReadWriteStat& benchStat ) const {
        return durationInNanoSecond < benchStat.durationInNanoSecond;
    }
};

struct BenchStat {
    std::string name;
    ReadWriteStat readWriteDataStat {};
    ReadWriteStat readWriteDataPtrStat {};

    bool operator<( const BenchStat& benchStat ) const {
        const auto fatorDataDataPtr = (double)readWriteDataStat.durationInNanoSecond /
                                      readWriteDataPtrStat.durationInNanoSecond;
        const auto fatorDataDataPtr2 = (double)benchStat.readWriteDataStat.durationInNanoSecond /
                                       benchStat.readWriteDataPtrStat.durationInNanoSecond;
        const auto fatorDataDataPtrMean = ( fatorDataDataPtr + fatorDataDataPtr2 ) / 2.0;

        return readWriteDataStat.durationInNanoSecond +
                   readWriteDataPtrStat.durationInNanoSecond * fatorDataDataPtrMean <
               benchStat.readWriteDataStat.durationInNanoSecond +
                   benchStat.readWriteDataPtrStat.durationInNanoSecond * fatorDataDataPtrMean;
    }
    bool operator==( const BenchStat& benchStat ) const { return name == benchStat.name; }
};

template <class BenchStats>
void printStats( const BenchStats& benchStats ) {

    for ( const auto& dataBenchStat : benchStats ) {
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
            if ( readWriteDataStat.nInputOutputCall != 0 ) {
                std::cout << "[" << implName << "] io call per read/write iteration: "
                          << readWriteDataStat.nInputOutputCall
                          << ", size: " << readWriteDataStat.inputOutputCallSize << std::endl;
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

    const auto startClock = std::chrono::high_resolution_clock::now();

    UserData data_write = data;
    UserData data_read;

    size_t iReadWrite = 0;
    while ( iReadWrite < nReadWrite ) {

        data_write.b = iReadWrite;

        writeOutputFunc( data_write );
        readInputFunc( data_read );

        CHECK( data_read == data_write );

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

#if CPP_VERSION >= 20 // concept
template <class T>
concept getableNCall_v = requires( const T a ) {
    a.getNCall();
};

#else
template <typename T>
using getableNCall_t = decltype( std::declval<T>().read( std::declval<T&>().getNCall() ) );

template <typename T, typename = std::void_t<>>
struct getableNCall : std::false_type {};

template <typename T>
struct getableNCall<T, std::void_t<getableNCall_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool getableNCall_v = getableNCall<T>::value;
#endif

template <class InputOutput, class UserData>
ReadWriteStat readWriteData( InputOutput& inputOutput, size_t nReadWrite, const UserData& data ) {
    if constexpr ( getableNCall_v<InputOutput> ) {
#ifdef DEBUG
        size_t nCallFirst = inputOutput.getNCall();
#endif
        auto read  = [&]( UserData& data ) { inputOutput.read( data ); };
        auto write = [&]( const UserData& data ) { inputOutput.write( data ); };
        auto ret   = readWriteData( read, write, nReadWrite, data );
#ifdef DEBUG
        ret.nInputOutputCall    = ( inputOutput.getNCall() - nCallFirst ) / nReadWrite;
        ret.inputOutputCallSize = inputOutput.getLastCallSize();
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
