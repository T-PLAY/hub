#ifndef BRIDGE_FILESENDER_H
#define BRIDGE_FILESENDER_H

#include "Sender.h"
#include <iostream>
#include <fstream>

class FileSender : public Sender {
private:
    std::ofstream _fileStream;

public:
    explicit FileSender(const std::string& fileName, int writeRate = 60);

    void send(std::string message) override;
    void send(TrackerData data) override;
    void cleanup() override;
};

#endif //BRIDGE_FILESENDER_H
