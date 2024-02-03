#include "Acquisition.hpp"

namespace hub {
namespace sensor {

Acquisition make_acquisition( const Matrix& resolution ) {
    Acquisition acq;
    acq |= make_matrix<Clock>() | make_matrix<Clock>() | resolution;
    acq.init();
    return acq;
}

} // namespace sensor
} // namespace hub
