
#include "test_common.hpp"
#include <cassert>
#include <list>

#include <sensor/Acquisition.hpp>

TEST_CASE( "AcquisitionT test" ) {
    TEST_BEGIN()

    constexpr int width  = 10;
    constexpr int height = 10;
    using ResolutionCam  = hub::MatrixXD<hub::format::RGB8, width, height>;
    using Resolution     = hub::MatrixTs<int, hub::format::Dof6, ResolutionCam>;

    using Acquisition = hub::sensor::AcquisitionT<Resolution>;
    static_assert( Acquisition::hasType<hub::sensor::Clock>() );
    static_assert( Acquisition::hasType<hub::format::Dof6>() );
    static_assert( Acquisition::hasType<hub::format::RGB8>() );
    static_assert( ResolutionCam::hasType<hub::format::RGB8>() );
    static_assert( !Acquisition::hasType<hub::format::BGR8>() );

    static_assert( Acquisition::nType<hub::sensor::Clock>() == 2 );
    static_assert( Acquisition::nType<hub::format::Dof6>() == 1 );
    static_assert( Acquisition::nType<hub::format::RGB8>() == 1 );
    static_assert( Acquisition::nType<hub::format::BGR8>() == 0 );

    static_assert( Resolution::getOffset<int>() == 0 );
    static_assert( Resolution::getOffset<hub::format::Dof6>() == 4 );
    static_assert( Resolution::getOffset<hub::format::RGB8>() == 32 );

    static_assert( Acquisition::getOffset<hub::sensor::Clock>() == 0 );
    static_assert( Acquisition::getOffset<hub::sensor::Clock, 1>() == 8 );
    static_assert( Acquisition::getOffset<int>() == 16 );
    static_assert( Acquisition::getOffset<hub::format::Dof6>() == 20 );
    static_assert( Acquisition::getOffset<hub::format::RGB8>() == 48 );

    using Resolution2 = hub::MatrixTs<hub::sensor::Clock, hub::sensor::Clock, Resolution>;
    static_assert( Resolution2::getOffset<hub::sensor::Clock>() == 0 );
    static_assert( Resolution2::getOffset<hub::sensor::Clock, 1>() == 8 );
    static_assert( Resolution2::getOffset<int>() == 16 );
    static_assert( Resolution2::getOffset<hub::format::Dof6>() == 20 );
    static_assert( Resolution2::getOffset<hub::format::RGB8>() == 48 );

    static_assert( hub::isMatrix<Resolution> );

    static_assert( hub::sizeOf<hub::sensor::Clock>() == 8 );
    static_assert( hub::sizeOf<hub::format::Dof6>() == 28 );
    static_assert( hub::sizeOf<hub::format::RGB8>() == 3 );
    static_assert( hub::sizeOf<ResolutionCam>() == width * height * 3 );
    static_assert( hub::sizeOf<Resolution>() == 4 + width * height * 3 + 28 );

    static_assert( !hub::isMatrix<hub::sensor::Clock> );
    static_assert( hub::isMatrix<Resolution> );
    static_assert( hub::isMatrix<ResolutionCam> );
    static_assert( !hub::isMatrix<hub::format::Dof6> );
    static_assert( !hub::isMatrix<hub::format::RGB8> );

    static_assert( ResolutionCam::Size == width * height * 3 );
    static_assert( Resolution::Size == 4 + 28 + width * height * 3 );

    TEST_END()
}
