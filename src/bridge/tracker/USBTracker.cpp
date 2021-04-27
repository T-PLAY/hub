#include <iostream>
#include "USBTracker.h"

void USBTracker::start() {
    std::cout << "start" << std::endl;
}

void USBTracker::end() {
    std::cout << "end" << std::endl;
}

bool USBTracker::connected() const {
    return _connected;
}

TrackerData USBTracker::data() const {
    return _data;
}


