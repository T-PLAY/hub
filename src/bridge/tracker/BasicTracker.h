#ifndef BRIDGE_BASICTRACKER_H
#define BRIDGE_BASICTRACKER_H
#include "Tracker.h"
#include <thread>
#include <QUdpSocket>

class BasicTracker : public Tracker{

private :
    QUdpSocket _sock;

    bool _connected = false;
    TrackerData _data;
    std::thread _readThread;
    bool _listen = true;
    const float div = 39.37f;

    void readData();

public :
    bool connected() const override;
    TrackerData data() const override;

    void start() override;
    void end() override;
};


#endif //BRIDGE_BASICTRACKER_H
