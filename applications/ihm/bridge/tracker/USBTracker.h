#ifndef BRIDGE_USBTRACKER_H
#define BRIDGE_USBTRACKER_H

#include "Tracker.h"


class USBTracker : public Tracker{
private :
    TrackerData _data;
    bool _connected = false;

public :
    void start() override;
    void end() override;

    bool connected() const override;
    TrackerData data() const override;


};


#endif //BRIDGE_USBTRACKER_H
