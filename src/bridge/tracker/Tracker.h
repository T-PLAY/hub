#ifndef BRIDGE_TRACKER_H
#define BRIDGE_TRACKER_H
#include "../utils.h"


class Tracker {
public:
    virtual TrackerData data() const = 0;
    virtual bool connected() const = 0;

    virtual void start() = 0;
    virtual void end() = 0;

};

#endif //BRIDGE_TRACKER_H
