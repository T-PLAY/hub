#include "FileTracker.h"

#include <iostream>
#include <sstream>

void FileTracker::start() {
    _fileStream.open(_fileName);
    if(_fileStream.is_open()) {
        std::cout << "Read file " << _fileName << " opened" << std::endl;
        _readThread = std::thread([this](){FileTracker::readData();});
    } else {
        std::cerr << "Could not open " << _fileName << std::endl;
    }
}


void FileTracker::end() {
    _fileStream.close();
    _readThread.join();
}


TrackerData FileTracker::data() const {
    return _data;
}

bool FileTracker::connected() const {
    return _connected;
}

FileTracker::FileTracker(int recordNumber) {
    _fileName = PROJECT_DIR "records/record" + std::to_string(recordNumber);
}

FileTracker::FileTracker(const std::string& fileName) : _fileName(PROJECT_DIR "records/" + fileName) {}

void FileTracker::readData() {
    char buffer[256];
    _fileStream.getline(buffer,256);
    int rate = std::atoi(buffer);
    int begPos = _fileStream.tellg();
    std::cout << begPos << rate << std::endl;
    if(rate <= 0) {
        std::cerr << "Could not read rate or invalid rate from " << _fileName << std::endl;
        return;
    }

    _connected = true;

    while(_fileStream.is_open()) {
        _fileStream.getline(buffer,256);
        if(_fileStream.eof()) {
            _fileStream.close();
            _fileStream.open(_fileName);
            _fileStream.getline(buffer, 256);
            _fileStream.getline(buffer,256);
        }

        std::vector<std::string> tokens = split(buffer,'_');

        _data.nId = std::stoi(tokens[0]);

        _data.t = std::stof(tokens[1]);
        _data.u = std::stof(tokens[2]);
        _data.v = std::stof(tokens[3]);

//        _data.w = std::stof(tokens[4]);
//        _data.z = -std::stof(tokens[5]);
//        _data.x = -std::stof(tokens[6]);
//        _data.y = std::stof(tokens[7]);

        _data.w = std::stof(tokens[4]);
        _data.x = std::stof(tokens[5]);
        _data.y = std::stof(tokens[6]);
        _data.z = std::stof(tokens[7]);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000/rate));
    }
}


std::vector<std::string> FileTracker::split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss{s};
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
