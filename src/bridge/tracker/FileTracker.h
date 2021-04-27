#ifndef BRIDGE_FILETRACKER_H
#define BRIDGE_FILETRACKER_H

#include "Tracker.h"
#include <fstream>
#include <thread>
#include <vector>

class FileTracker : public Tracker {
private:
    TrackerData _data = {0,0,0,0,0,0,0,0};
    bool _connected = false;
    std::ifstream _fileStream;
    std::string _fileName;
    std::thread _readThread;

    void readData();
    static std::vector<std::string> split(const std::string &s, char delim);

public:
    explicit FileTracker(int recordNumber);
    explicit FileTracker(const std::string& fileName);

    TrackerData data() const override;
    bool connected()const override;

    void start() override;
    void end() override;
};

#endif //BRIDGE_FILETRACKER_H
