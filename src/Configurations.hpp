#pragma once

#include <string>

namespace hub {
namespace net {

static constexpr int s_defaultServicePort = 4042;
// static constexpr const char * const s_defaultServiceIp = "127.0.0.1";
static std::string s_defaultServiceIp = "127.0.0.1";
// static std::string s_defaultServiceIp = "141.115.68.30"; // work ip
// static constexpr std::string_view s_defaultServiceIp = "127.0.0.1";

} // namespace net
} // namespace hub
