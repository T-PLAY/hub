#include <iostream>
#include "synchro.h"

Synchro::Synchro(std::unique_ptr<Tracker> tracker, std::unique_ptr<Sender> sender, int sendRate) :
_tracker(std::move(tracker)), _sender(std::move(sender))
{
    _sendTime = 1000/sendRate;
}

void Synchro::start() {
    _send = true;
    _tracker->start();
    _sendThread = std::thread([this](){Synchro::sendData();});
}

void Synchro::end() {
    _send = false;
    _sendThread.join();
    _sender->cleanup();
    _message = "Sync module is deleted";
    _tracker->end();
}

void Synchro::sendData() {
    std::cout << "Waiting for tracker" << std::endl;
    _message = "Waiting for tracker";
    while(!_tracker->connected() && _send);
    std::cout << "Tracker connected" << std::endl;
    _message = "Tracker connected";
    while (_send) {
        TrackerData data = _tracker->data();
        _sender->send(data);
        _message = "Sent :\tposition("   + std::to_string(data.t) + " ; "
                                        + std::to_string(data.u) + " ; "
                                        + std::to_string(data.v) + ")"
                                        + "\n\trotation("
                                        + std::to_string(data.x) + " ; "
                                        + std::to_string(data.y) + " ; "
                                        + std::to_string(data.z) + " ; "
                                        + std::to_string(data.w) + ")";

        std::this_thread::sleep_for(std::chrono::milliseconds(_sendTime));
    }
}
void Synchro::stop(){
    _send = false;
    _sendThread.join();
    _message = "Sync module is stopped";
    _tracker->end();
}

const std::string &Synchro::message() const{
    return _message;
}

std::string Synchro::getCurrentPos() const {
    return Utils::dataToMessage(_tracker->data());
}
