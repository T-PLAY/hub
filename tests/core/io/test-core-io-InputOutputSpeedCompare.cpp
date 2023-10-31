
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include <set>
#include <cstring>

#include "core/io/test_core_io_common.hpp"
#include "core/test_core_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>

#include <core/io/InputOutput.hpp>
#include <core/io/InputOutputImpl.hpp>
#include <core/io/InputOutputZppBits.hpp>
// #include <core/InputOutput.hpp>
#include <core/Any.hpp>
#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/io/Book.hpp>


TEST_CASE( "InputOutput test" ) {
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
    static constexpr size_t s_nReadWriteDataPtr = 5;
    static constexpr size_t s_DataSizePtr       = 10'000'000;
#    else
    static constexpr size_t s_nReadWriteDataPtr = 1;
    static constexpr size_t s_DataSizePtr       = 1'000'000'000;
#    endif
#endif

    std::cout << "nb read/write: " << s_nReadWrite / 1000000.0 << " M" << std::endl;
    std::cout << "read/write size: " << s_dataSize * s_nReadWrite / 1'000'000.0 << " Mo"
              << std::endl;
    std::cout << "nb read/write ptr: " << s_nReadWriteDataPtr << std::endl;
    std::cout << "read/write ptr size: " << s_nReadWriteDataPtr * s_DataSizePtr / 1'000'000.0
              << " Mo" << std::endl;
    std::cout << std::endl;

    UserData userData( 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } );

    unsigned char* data_write = new unsigned char[s_DataSizePtr];
    memset( data_write, 'a', s_DataSizePtr );

    std::set<BenchStat> dataBenchStats;

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
    BenchStat benchStatInputOutputImpl { "InputOutputImpl" };
    {
        hub::io::Archive<hub::serializer::SerializerImpl> archive;

        benchStatInputOutputImpl.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, userData );
        assert( archive.isEnd() );

        benchStatInputOutputImpl.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputImpl );
    }

#ifdef HUB_DEBUG_OUTPUT
    std::cout << std::endl;
    std::cout << "-------------------- InputOutputZppBits ----------------------" << std::endl;
#endif
    BenchStat benchStatInputOutputZppBits { "InputOutputZppBits" };
    {
        hub::io::Archive<hub::serializer::SerializerZppBits> archive;

        benchStatInputOutputZppBits.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, userData );
        assert( archive.isEnd() );

        benchStatInputOutputZppBits.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputZppBits );
    }

    delete[] data_write;

    CHECK( benchStatInputOutputZppBits < benchStatInputOutputImpl );
    CHECK( benchStatInputOutputZppBits.readWriteDataStat.nInputOutputCall <=
           benchStatInputOutputImpl.readWriteDataStat.nInputOutputCall );
    static_assert( std::is_same_v<hub::io::Archive<>, hub::io::Archive<hub::serializer::SerializerZppBits>>);

    //    static_assert( std::is_same_v<hub::io::InputOutput, hub::io::InputOutputZppBits> );
    //    static_assert( std::is_same_v<hub::Input, hub::io::input::InputZppBits> );
    //    static_assert( std::is_same_v<hub::Output, hub::io::output::OutputZppBits> );

    //    auto inputOutputRank = std::distance(dataBenchStats.begin(),
    //    std::find(dataBenchStats.begin(), dataBenchStats.end(), benchStatInputOutput)); const auto
    //    inputOutputRank = std::distance(dataBenchStats.begin(),
    //    dataBenchStats.find(benchStatInputOutput)); CHECK(inputOutputRank < 2);

    printStats( dataBenchStats );
}
