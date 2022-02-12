#ifndef BRIDGE_H
#define BRIDGE_H
#include <iostream>
#include <string>
#include "sender/Sender.h"
#include "sender/UDPSender.h"
#include "tracker/Tracker.h"
#include "tracker/BasicTracker.h"
#include "tracker/USBTracker.h"
#include "sender/FileSender.h"
#include "utils.h"
#include "synchro.h"
#include "tracker/FileTracker.h"

#include <memory>
#include <QString>


enum TrackerType {
    BASICTRACKER,
    STREAMTRACKER
};

enum SenderType {
    UDPSENDER,
    STREAMSENDER
};


struct SyncInfo {
    TrackerType trackerType;
    SenderType senderType;
    int sendRate;
    QString trackerInfo; //contains filename for the tracker
    QString senderInfo; //contains filename or ipAddress for the sender
    int port;
};


class Bridge {
private:
    std::unique_ptr<Synchro> _sync = nullptr;
    bool _running = false;

public:
    Bridge() = default;

    bool start();
    bool stop();

    const std::string& syncMessage() const;

    void setupSync(const SyncInfo& infos);

    std::string getCurrentPos() const;

};
















#endif 



