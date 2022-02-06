#include "FileSender.h"

FileSender::FileSender(const std::string &fileName, int writeRate) {
    _fileStream.open(PROJECT_DIR "records/" + fileName,std::ofstream::out | std::ofstream::trunc);
    if(_fileStream.is_open()) {
        std::cout << "File opened" << std::endl;
        _fileStream << writeRate << std::endl;
    }
}

void FileSender::send(std::string message) {
    std::cout << "Write : " << message << std::endl << "------" << std::endl;
    _fileStream << message << std::endl;
}

void FileSender::send(TrackerData data) {
    send(Utils::dataToMessage(data));
}



void FileSender::cleanup() {
    _fileStream.close();
}


