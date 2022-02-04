#include <iostream>
#include "FileStreamer.h"


FileStreamer::FileStreamer(const QString& folder, int readRate) : _readTime(1000/readRate) {
    std::srand(time(nullptr));

    _dir = QDir(folder);
    _list = _dir.entryList(QStringList() << "*.png" << "*.PNG" << "*.jpg" << "*.jpeg" << "*.JPG" << "*.JPEG");
}

std::vector<unsigned char> FileStreamer::image(bool consumePending) {
    if(consumePending)
        _pending = false;
    return _currImage;
}

bool FileStreamer::connected() const {
    return _connected;
}

bool FileStreamer::imagePending() const {
    return _pending;
}

void FileStreamer::readImages() {

    _connected = true;
    while(_read) {
        int i = std::rand() % _list.size();
        QString file = _list[i];

        std::cout << "reading " << file.toStdString() << std::endl;

        std::ifstream in(_dir.absoluteFilePath(file).toStdString(),std::ios::binary);
        _currImage = std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});
        _pending = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(_readTime));
    }
    _connected = false;
}

bool FileStreamer::start() {
    if(_list.empty()) {
        return false;
    }

    _read = true;
    _readThread = std::thread([this](){FileStreamer::readImages();});
    return true;
}

void FileStreamer::stop() {
    _read = false;
    if(_readThread.joinable())
        _readThread.join();
}



