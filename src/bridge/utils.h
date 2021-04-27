#ifndef BRIDGE_UTILS_H
#define BRIDGE_UTILS_H

#include <string>
#include <algorithm>

struct TrackerData {
    unsigned short nId;
    float t;
    float u;
    float v;

    float w;
    float x;
    float y;
    float z;
};

class Utils {
public:
    static std::string dataToMessage(TrackerData data);
    static std::string dataToConsole(TrackerData data);
};

#endif //BRIDGE_UTILS_H
