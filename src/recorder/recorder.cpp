#include <iostream>
#include <sstream>
#include <iomanip>
#include "recorder.h"

bool Recorder::startSession(const std::string &folder, const std::string &name, int rate) {
    if(_sessionOn) {
        _message = "Session already on";
        return false;
    }

    _rate = rate;
    _sessionDir = QDir(folder.c_str());

    if(!_sessionDir.mkdir(name.c_str())) {
        _message = "Record folder already exists";
        _sessionOn = false;
        return false;
    }

    _message = "Recording session is on";
    _sessionOn = true;
    _sessionDir.cd(name.c_str());
    return true;
}

void Recorder::stopSession() {
    _message = "Saving files...";


    if(_imageBuffer.size() >= 10000) {
        std::cerr << "Too many images to record (>= 10000)" << std::endl;
        _imageBuffer.clear();
        _positionBuffer.clear();
        return;
    }


    std::ofstream _positionFile;
    _positionFile.open(_sessionDir.absoluteFilePath("coords.txt").toStdString());

    _positionFile << _rate << std::endl;
    for(const auto & line : _positionBuffer) {
        _positionFile << line << std::endl;
    }
    _positionFile << "END" << std::endl;
    _positionFile.close();


    int i = 0;
    for(const auto & image : _imageBuffer) {
        Recorder::file.open(_sessionDir.absoluteFilePath((pad(i++,4) + ".jpg").c_str()).toStdString(), std::ios_base::out | std::ios_base::binary);
        TooJpeg::writeJpeg(&output, image.data(), 192, 512);
        Recorder::file.close();

    }

    std::cout << "?" << std::endl;

    _imageBuffer.clear();
    _positionBuffer.clear();

    _message = "Recording session is off";
    _sessionOn = false;
}

void Recorder::record(const std::vector<unsigned char> &image, const std::string &position) {
    _imageBuffer.emplace_back(image);
    _positionBuffer.emplace_back(position);
}

std::string & Recorder::message() {
    return _message;
}

std::string Recorder::pad(int val, int w) {

    std::string number = std::to_string(val);
    int topad = w - number.size();
    if(topad <= 0)
        return number;

    std::string pad(topad,'0');

    return pad+number;
}

