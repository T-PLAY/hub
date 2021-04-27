#include "Bridge.h"

bool Bridge::start() {
    if(_sync && !_running) {
        _running = true;
        _sync->start();
        return true;
    }

    return false;
}

bool Bridge::stop() {
    if(_sync && _running) {
        _running = false;
        _sync->stop();
        return true;
    }

    return false;
}

void Bridge::setupSync(const SyncInfo& infos) {
    stop();

    std::unique_ptr<Tracker> t;
    std::unique_ptr<Sender> s;

    switch (infos.trackerType) {
        case BASICTRACKER :
            t = std::make_unique<BasicTracker>();
            break;
        case STREAMTRACKER :
            t = std::make_unique<FileTracker>(infos.trackerInfo.toStdString());
            break;
    }
    switch (infos.senderType) {
        case UDPSENDER :
            s = std::make_unique<UDPSender>(infos.senderInfo.toStdString(), infos.port);
            break;
        case STREAMSENDER :
            s = std::make_unique<FileSender>(infos.senderInfo.toStdString(), infos.sendRate);
            break;
    }

    _sync = std::make_unique<Synchro>(std::move(t),std::move(s),infos.sendRate);
}

const std::string &Bridge::syncMessage() const {
    return _sync->message();
}
