

#include <cstring>
#include <set>

#include "core/io/test_core_io_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>

#include <core/Any.hpp>
#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/io/InputOutput.hpp>
#include <core/serializer/SerializerImpl.hpp>

TEST_CASE( "InputOutput speed compare test" ) {
    TEST_BEGIN()

    using namespace testCoreIoCommon;

#ifdef HUB_DEBUG_OUTPUT
    static constexpr size_t s_nReadWrite = 1;
#else
#    ifdef DEBUG
    static constexpr size_t s_nReadWrite = 10'000;
#    else
    static constexpr size_t s_nReadWrite = 1'000'000;
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

    UserData userData { 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } };
    static_assert( !hub::packable_v<UserData> );
    static_assert( !hub::writable_v<UserData> );
    static_assert( !hub::readable_v<UserData> );

    unsigned char* data_write = new unsigned char[s_DataSizePtr];
    memset( data_write, 'a', s_DataSizePtr );

    std::set<BenchStat> dataBenchStats;

    ////            std::this_thread::sleep_for(std::chrono::nanoseconds(1));

#ifdef HUB_DEBUG_OUTPUT
    std::cout << "-------------------- InputOutputImpl ----------------------" << std::endl;
#endif

    // BenchStat benchStatInputOutputImpl { "ArchiveImpl" };

#ifdef HUB_USE_ZPP_BITS
#    ifdef HUB_DEBUG_OUTPUT
    std::cout << std::endl;
    std::cout << "-------------------- InputOutputZppBits ----------------------" << std::endl;
#    endif
    BenchStat benchStatInputOutputZppBits { "ArchiveZppBits" };
    {
        hub::io::ArchiveT<hub::serializer::SerializerZppBits> archive;

        benchStatInputOutputZppBits.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, userData );
        CHECK( archive.isEnd() );

        benchStatInputOutputZppBits.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        CHECK( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputZppBits );
    }
#endif

#ifdef HUB_DEBUG_OUTPUT
    std::cout << std::endl;
    std::cout << "-------------------- InputOutputZpp ----------------------" << std::endl;
#endif
    BenchStat benchStatInputOutputZpp { "ArchiveZpp" };
    {
        hub::io::ArchiveT<hub::SerializerT<hub::serializer::SerializerZpp>> archive;

        benchStatInputOutputZpp.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, userData );
        CHECK( archive.isEnd() );

        benchStatInputOutputZpp.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        CHECK( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputZpp );
    }

    delete[] data_write;

#if !defined( DEBUG ) && defined( HUB_USE_ZPP_BITS )
    const auto ratioData =
        benchStatInputOutputImpl.readWriteDataStat.durationInNanoSecond /
        (double)benchStatInputOutputZppBits.readWriteDataStat.durationInNanoSecond;
    const auto ratioDataPtr =
        benchStatInputOutputImpl.readWriteDataPtrStat.durationInNanoSecond /
        (double)benchStatInputOutputZppBits.readWriteDataPtrStat.durationInNanoSecond;
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

    printStats( dataBenchStats );
    TEST_END()
}
