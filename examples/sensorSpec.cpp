

#include <SensorSpec.hpp>

int main() {

    hub::SensorSpec::MetaData metaData;
    hub::SensorSpec::Format format = hub::SensorSpec::Format::BGR8;
    hub::SensorSpec::Dims dims     = { 1 };
    hub::SensorSpec::Resolutions resolutions { { dims, format } };
    hub::SensorSpec sensorSpec {
        "sensorName", { { dims, hub::SensorSpec::Format::BGR8 } }, metaData };
    hub::SensorSpec sensorSpec2 {
        "sensorName",
        { { { 1 }, hub::SensorSpec::Format::BGR8 }, { { 1, 2, 3 }, format } },
        metaData };
    hub::SensorSpec sensorSpec3 { "sensorName", {}, metaData };

    constexpr auto sensorSpec4 = hub::SensorSpec();
}
