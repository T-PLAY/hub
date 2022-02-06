#ifndef BRIDGE_SENDER_H
#define BRIDGE_SENDER_H

#include <string>
#include <bridge/utils.h>
class Sender {
public :
    virtual void send(std::string message) = 0;
    virtual void send(TrackerData data) = 0;
    virtual void cleanup() = 0;
};


#endif //BRIDGE_SENDER_H
