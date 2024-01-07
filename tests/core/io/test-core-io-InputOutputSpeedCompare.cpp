
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include <cstring>
#include <set>

//#include "zpp_bits.h"

#include "core/io/test_core_io_common.hpp"
//#include "core/test_core_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>

#include <core/io/InputOutput.hpp>
// #include <core/InputOutput.hpp>
#include <core/Any.hpp>
#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/io/Book.hpp>
#include <core/serializer/SerializerImpl.hpp>

TEST_CASE( "InputOutput speed compare test" ) {
    TEST_BEGIN()

    using namespace testCoreIoCommon;

#ifdef HUB_DEBUG_OUTPUT
    static constexpr size_t s_nReadWrite = 1;
#else
#    ifdef DEBUG
    static constexpr size_t s_nReadWrite        = 10'000;
#    else
    static constexpr size_t s_nReadWrite        = 1'000'000;
#    endif
#endif

#ifdef HUB_DEBUG_OUTPUT
    static constexpr size_t s_nReadWriteDataPtr = 1;
    static constexpr size_t s_DataSizePtr       = 10'000'000;
#else
#    ifdef DEBUG
    static constexpr size_t s_nReadWriteDataPtr = 2;
    static constexpr size_t s_DataSizePtr       = 100'000'000;
#    else
    static constexpr size_t s_nReadWriteDataPtr = 5;
    static constexpr size_t s_DataSizePtr       = 100'000'000;
#    endif
#endif

    std::cout << "nb read/write: " << s_nReadWrite / 1000000.0 << " M" << std::endl;
    std::cout << "read/write size: " << s_dataSize * s_nReadWrite / 1'000'000.0 << " Mo"
              << std::endl;
    std::cout << "nb read/write ptr: " << s_nReadWriteDataPtr << std::endl;
    std::cout << "read/write ptr size: " << s_nReadWriteDataPtr * s_DataSizePtr / 1'000'000.0
              << " Mo" << std::endl;
//    std::cout << std::endl;

    UserData userData{ 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } };
    static_assert( !hub::packable_v<UserData> );
    // static_assert( hub::serializable_v<UserData> );
    static_assert( !hub::writable_v<UserData> );
    static_assert( !hub::readable_v<UserData> );

    unsigned char* data_write = new unsigned char[s_DataSizePtr];
    memset( data_write, 'a', s_DataSizePtr );

    std::set<BenchStat> dataBenchStats;

    //    {
    //        std::vector<unsigned char> buff;
    //        zpp::bits::out out( buff );
    //        out( userData ).or_throw();
    //        const auto serializeSize = out.position();
    //        unsigned char* data      = new unsigned char[serializeSize];
    //        memset( data, 55, serializeSize );
    //        unsigned char* data_read = new unsigned char[serializeSize];

    //        const auto startClock = std::chrono::high_resolution_clock::now();
    //        for ( int i = 0; i < s_nReadWrite; ++i ) {
    //            data[0] = i;
    //            memcpy( data_read, data, serializeSize );

    //            assert (memcmp(data, data_read, serializeSize) == 0);
    ////            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    //        }
    //        const auto endClock = std::chrono::high_resolution_clock::now();
    //        const auto durationInNanoSecond =
    //            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock
    //            ).count();

    //        const auto megaReadWritePerSecond = 1000.0 * s_nReadWrite / durationInNanoSecond;
    //        const auto gigaBytePerSecond      = megaReadWritePerSecond * sizeof( UserData ) /
    //        1000.0;

    //        delete[] data_read;
    //        delete[] data;

    //        std::cout << "------------------------------------" << std::endl;
    //        std::cout << "[reference] writing/reading rate: "
    //                  << std::to_string( megaReadWritePerSecond / 1000.0 ) << " GigaReadWrite/s, "
    //                  << std::to_string( gigaBytePerSecond ) << " Go/s" << std::endl;
    //        std::cout << "[reference]"
    //                  << "total time: " << durationInNanoSecond / 1000'000.0 << " ms" <<
    //                  std::endl;
    //    }

    //    BenchStat benchStatInputOutput { "InputOutput" };
    //    {
    //        hub::io::Archive archive;

    //        benchStatInputOutput.readWriteDataStat =
    //            readWriteData( archive, s_nReadWrite, userData );
    //        assert( archive.isEnd() );

    //        benchStatInputOutput.readWriteDataPtrStat =
    //            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
    //        assert( archive.isEnd() );
    //        dataBenchStats.insert( benchStatInputOutput );
    //    }

#ifdef HUB_DEBUG_OUTPUT
    std::cout << "-------------------- InputOutputImpl ----------------------" << std::endl;
#endif

    // todo serializer
    // BenchStat benchStatInputOutputImpl { "ArchiveImpl" };
    // {
    //     hub::io::ArchiveT<hub::serializer::SerializerImpl> archive;

    //     benchStatInputOutputImpl.readWriteDataStat =
    //         readWriteData( archive, s_nReadWrite, userData );
    //     assert( archive.isEnd() );

    //     benchStatInputOutputImpl.readWriteDataPtrStat =
    //         readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
    //     assert( archive.isEnd() );
    //     dataBenchStats.insert( benchStatInputOutputImpl );
    // }

#ifdef HUB_USE_ZPP_BITS
#ifdef HUB_DEBUG_OUTPUT
    std::cout << std::endl;
    std::cout << "-------------------- InputOutputZppBits ----------------------" << std::endl;
#endif
    BenchStat benchStatInputOutputZppBits { "ArchiveZppBits" };
    {
        hub::io::ArchiveT<hub::serializer::SerializerZppBits> archive;

        benchStatInputOutputZppBits.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, userData );
        assert( archive.isEnd() );

        benchStatInputOutputZppBits.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputZppBits );
    }
#endif

// #ifdef HUB_USE_ZPP_BITS
#ifdef HUB_DEBUG_OUTPUT
    std::cout << std::endl;
    std::cout << "-------------------- InputOutputZpp ----------------------" << std::endl;
#endif
    BenchStat benchStatInputOutputZpp { "ArchiveZpp" };
    {
        hub::io::ArchiveT<hub::SerializerT<hub::serializer::SerializerZpp>> archive;

        benchStatInputOutputZpp.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, userData );
        assert( archive.isEnd() );

        benchStatInputOutputZpp.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputZpp );
    }
// #endif

    delete[] data_write;


//#ifndef DEBUG
#if ! defined(DEBUG) && defined(HUB_USE_ZPP_BITS)
    const auto ratioData = benchStatInputOutputImpl.readWriteDataStat.durationInNanoSecond / (double)benchStatInputOutputZppBits.readWriteDataStat.durationInNanoSecond;
    const auto ratioDataPtr = benchStatInputOutputImpl.readWriteDataPtrStat.durationInNanoSecond / (double)benchStatInputOutputZppBits.readWriteDataPtrStat.durationInNanoSecond;
    // CHECK_VALUE( ratioData, 2.0, 1.0, "Data: ArchiveZppBits/ArchiveImpl", "/" );
    CHECK_DECLINE( ratioData, "Data: ArchiveZppBits/ArchiveImpl", "/" );
    // CHECK_VALUE( ratioDataPtr, 1.0, 0.5, "DataPtr: ArchiveZppBits/ArchiveImpl", "/" );
    CHECK_DECLINE( ratioDataPtr, "DataPtr: ArchiveZppBits/ArchiveImpl", "/" );

    CHECK( benchStatInputOutputZppBits < benchStatInputOutputImpl );
    CHECK( benchStatInputOutputZppBits.readWriteDataStat.nInputOutputCall <=
           benchStatInputOutputImpl.readWriteDataStat.nInputOutputCall );
#endif

#ifdef HUB_USE_ZPP_BITS
    static_assert(
        std::is_same_v<hub::io::Archive, hub::io::ArchiveT<hub::serializer::SerializerZppBits>> );
#endif

    //    static_assert( std::is_same_v<hub::io::InputOutput, hub::io::InputOutputZppBits> );
    //    static_assert( std::is_same_v<hub::Input, hub::io::input::InputZppBits> );
    //    static_assert( std::is_same_v<hub::Output, hub::io::output::OutputZppBits> );

    //    auto inputOutputRank = std::distance(dataBenchStats.begin(),
    //    std::find(dataBenchStats.begin(), dataBenchStats.end(), benchStatInputOutput)); const auto
    //    inputOutputRank = std::distance(dataBenchStats.begin(),
    //    dataBenchStats.find(benchStatInputOutput)); CHECK(inputOutputRank < 2);

    printStats( dataBenchStats );
    TEST_END()
}
