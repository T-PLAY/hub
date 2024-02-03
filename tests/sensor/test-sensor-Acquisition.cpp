
#include "test_common.hpp"
#include <cassert>
#include <list>

#include <sensor/Acquisition.hpp>

TEST_CASE( "Acquisition test" ) {
    TEST_BEGIN()

    using Resolution = hub::format::BGR8;
    auto acq         = hub::sensor::make_acquisition( hub::make_matrix<Resolution>() );

    auto* dataPtr = acq.data();
    std::cout << "dataPtr : " << (uintptr_t)dataPtr << std::endl;

    CHECK( (const hub::Data_t*)acq.get<const hub::format::BGR8*>() == dataPtr + 16 );
    CHECK( (hub::Data_t*)acq.get<hub::format::BGR8*>() == dataPtr + 16 );

    CHECK( (const hub::Data_t*)&acq.get<const hub::format::BGR8&>() == dataPtr + 16 );
    CHECK( (hub::Data_t*)&acq.get<hub::format::BGR8&>() == dataPtr + 16 );

    auto* bgr8Ptr = acq.get<hub::format::BGR8*>();
    std::cout << "bgr8Ptr : " << (uintptr_t)bgr8Ptr << std::endl;
    assert( (const hub::Data_t*)bgr8Ptr == dataPtr + 16 );

    const auto* bgr8CstPtr = acq.get<const hub::format::BGR8*>();
    std::cout << "bgr8CstPtr : " << (uintptr_t)bgr8CstPtr << std::endl;
    assert( (const hub::Data_t*)bgr8CstPtr == dataPtr + 16 );

    const auto& bgr8Ref = acq.get<const hub::format::BGR8&>();
    std::cout << "bgr8Ref : " << (uintptr_t)&bgr8Ref << std::endl;
    CHECK( &bgr8Ref.b == dataPtr + 16 );
    CHECK( &bgr8Ref.g == dataPtr + 17 );
    CHECK( &bgr8Ref.r == dataPtr + 18 );
    assert( (const hub::Data_t*)&bgr8Ref == dataPtr + 16 );

    const auto& acqCstRef = acq;
    assert( acq.data() == acqCstRef.data() );
    const auto& bgr8CstRef = acqCstRef.get<const hub::format::BGR8&>();
    std::cout << "bgr8CstRef : " << (uintptr_t)&bgr8CstRef << std::endl;
    assert( &bgr8Ref == &bgr8CstRef );
    assert( (const hub::Data_t*)&bgr8CstRef == dataPtr + 16 );

    TEST_END()
}
