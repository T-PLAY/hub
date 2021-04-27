#include <iostream>
#include <QMessageBox>

#include "uploader.h"


Uploader::Uploader(const QDir& dir, QStringList list, QString ipAddress, const QString& port) : _dir{dir}, _list{std::move(list)}, _ipAddress{std::move(ipAddress)}{
    _port = port.toInt();

    _sock = socket(AF_INET, SOCK_STREAM, 0);


    _addr.sin_family = AF_INET;
    inet_pton(AF_INET, _ipAddress.toStdString().c_str(), &_addr.sin_addr);
    _addr.sin_port = htons(_port);
    _addrLen = sizeof(_addr);
}

void Uploader::upload() {
    progress = new QProgressDialog("Trying to connect to : " + _ipAddress + ":" + QString(std::to_string(_port).c_str()), "Cancel", 0, 100);
    _uploadThread = std::thread([this](){Uploader::uploadTask();});
    progress->exec();
    _uploadThread.join();
    QMessageBox mb;
    mb.setText(_uped ? "Uploaded the images" : "Failed to upload");
    mb.exec();

    _uped = false;
}

void Uploader::uploadTask() {
    float count = 0;
    int fileNum = _list.size();
    auto max = static_cast<float>(fileNum+1);

    if(connect(_sock, reinterpret_cast<const sockaddr*>(&_addr), _addrLen) < 0) {
        progress->setLabelText("Could not connect");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress->setValue(100);
        return;
    }

    std::ifstream stream;
    stream.open(_dir.absoluteFilePath("coords.txt").toStdString());
    if(!stream.is_open()) {
        progress->setLabelText("Could not open coords.txt");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress->setValue(100);
        return;
    }
    char buf[256];

    stream.getline(buf, 256);
    int rate = std::stoi(buf);
    std::cout << "rate : " << rate << std::endl;

    std::vector<std::string> positions;
    while(!stream.eof()) {
        stream.getline(buf, 256);
        positions.emplace_back(buf);
        std::cout << positions.back() << std::endl;
    }

    if(positions.size() != _list.size()) {
        progress->setLabelText("inconsistent coords and images");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress->setValue(100);
        return;
    }

    int strsize = positions.back().size();

    send(_sock,&rate,sizeof(rate),0);

    send(_sock,&fileNum,sizeof(fileNum),0);
    progress->setValue(static_cast<int>(++count/max)*100);

    int i = 0;
    for(const auto & file : _list) {
        progress->setLabelText("Sending : " + file + " ...");
        std::ifstream in(_dir.absoluteFilePath(file).toStdString(),std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(in), {});

        buffer.shrink_to_fit();
        int imsize = buffer.size();

        send(_sock, &strsize, sizeof(strsize), 0);
        send(_sock, positions[i].c_str(), strsize, 0);
        send(_sock, &imsize, sizeof(imsize), 0);
        send(_sock, buffer.data(), imsize, 0);


        progress->setValue(static_cast<int>(++count/max)*100);
        ++i;
    }
    _uped = true;
}


