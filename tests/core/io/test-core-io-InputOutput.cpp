

#include <cstring>
#include <iostream>
#include <set>

#include "test_common.hpp"
#include "test_core_io_common.hpp"

#include <core/Macros.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
#include <core/io/InputOutput.hpp>

#include <core/serializer/SerializerZpp.hpp>

using namespace testCoreIoCommon;

template <class Archive>
void process( Archive& archive ) {
    CHECK( archive.isEnd() );

#ifndef HUB_DEBUG_OUTPUT
    for ( int i = 0; i < 1'000; ++i ) {
#endif

        checkType<int>( archive, 5 );
        checkType<double>( archive, 2.0 );
        checkType<std::string>( archive, "hello" );
        checkType<UserEnum2>( archive, UserEnum2::B );
        checkType<SerializedClass>( archive, SerializedClass { 2, true } );
        checkType<WritableReadableData>( archive, WritableReadableData { "gauthier", 30 } );

#ifndef HUB_DEBUG_OUTPUT
    }
#endif
#ifdef DEBUG
    std::cout << "nCall : " << archive.getNCall() << std::endl;
#endif
}

TEST_CASE( "InputOutput test" ) {
    TEST_BEGIN()

    long long durationArchiveZppBits = 0;
    long long durationArchiveImpl    = 0;
    long long durationArchiveZpp     = 0;

#ifdef HUB_USE_ZPP_BITS
    {
        std::cout << "-------------------- ArchiveZppBits --------" << std::endl;
        hub::io::ArchiveT<hub::serializer::SerializerZppBits> archiveZppBits;
        const auto startClock = std::chrono::high_resolution_clock::now();
        process( archiveZppBits );
        const auto endClock = std::chrono::high_resolution_clock::now();
        durationArchiveZppBits =
            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
        std::cout << "duration: " << durationArchiveZppBits / 1'000'000.0 << " ms" << std::endl;
    }
#endif

    {
        std::cout << "-------------------- ArchiveZpp --------" << std::endl;
        hub::io::ArchiveT<hub::SerializerT<hub::serializer::SerializerZpp>> archiveZpp;
        const auto startClock = std::chrono::high_resolution_clock::now();
        process( archiveZpp );
        const auto endClock = std::chrono::high_resolution_clock::now();
        durationArchiveZpp =
            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
        std::cout << "duration: " << durationArchiveZpp / 1'000'000.0 << " ms" << std::endl;
    }

    TEST_END()
}
