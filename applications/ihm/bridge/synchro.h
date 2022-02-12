#ifndef BRIDGE_SYNCHRO_H
#define BRIDGE_SYNCHRO_H
#include <chrono>
#include <future>
#include <thread>
#include <memory>

#include "sender/Sender.h"
#include "tracker/Tracker.h"

class Synchro {
private :
    std::unique_ptr<Tracker> _tracker;
    std::unique_ptr<Sender> _sender;
    std::thread _sendThread;
    int _sendTime;
    bool _send = true;
    std::string _message = "Sync module is stopped";

    void sendData();

public :

    Synchro(std::unique_ptr<Tracker> tracker, std::unique_ptr<Sender> sender, int sendRate);

    void start();
    void stop();
    void end();

    const std::string& message() const;
    std::string getCurrentPos() const;

};



#endif //BRIDGE_SYNCHRO_H
