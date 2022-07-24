
#pragma once

#include <string>
#include <vector>

// constexpr std::string_view g_probeScanSensorName = "ULA-OP 256";
// constexpr std::string_view g_probePoseSensorName = "Polhemus Patriot (probe)";
const std::string g_probeScanSensorName = "ULA-OP 256";
const std::string g_probePoseSensorName = "Polhemus Patriot (probe)";

const std::vector<double> g_stepSizes = { 1.0, 2.0, 5.0, 10.0, 20.0, 25.0, 50.0, 75.0, 100.0 };
