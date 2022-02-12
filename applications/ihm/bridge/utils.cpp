#include <iostream>
#include "utils.h"

std::string Utils::dataToMessage(TrackerData data) {
    std::string res;
    std::string tmp;

    res.append(std::to_string(data.nId));

    tmp = std::to_string(data.t);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    tmp = std::to_string(data.u);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    tmp = std::to_string(data.v);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    tmp = std::to_string(data.w);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    tmp = std::to_string(data.x);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    tmp = std::to_string(data.y);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    tmp = std::to_string(data.z);
    std::replace(tmp.begin(),tmp.end(),',','.');
    res.append("_");
    res.append(tmp.substr(0,(tmp.length() < 6 ? tmp.length() : 6)));

    return res;
}

std::string Utils::dataToConsole(TrackerData data) {
    std::string res;

    res.append("Num Capteur :\n");
    res.append(std::to_string(data.nId));

    res.append("\nPosition :\n");
    res.append(std::to_string(data.t) + "/" + std::to_string(data.u) + "/" + std::to_string(data.v) + "\n");

    res.append("Rotation :\n");
    res.append(std::to_string(data.w) + "/" + std::to_string(data.x) + "/" + std::to_string(data.y) + "/" + std::to_string(data.z) + "\n");

    return res;
}
