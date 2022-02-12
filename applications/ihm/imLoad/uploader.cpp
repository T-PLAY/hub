#include <iostream>
#include <QMessageBox>

#include "uploader.h"


Uploader::Uploader(const QDir& dir, QStringList list, QString ipAddress, const QString& port) : _dir{dir}, _list{std::move(list)}, _ipAddress{std::move(ipAddress)}{
    _port = port.toInt();


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

    _sock.connectToHost(_ipAddress,_port);

    if(!_sock.waitForConnected(1000)) {
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
    stream.getline(buf, 256);
    while(!stream.eof() && strcmp(buf,"END") != 0) {
        positions.emplace_back(buf);
        std::cout << positions.back() << std::endl;
        stream.getline(buf, 256);
    }

    if(positions.size() != _list.size()) {
        std::cout << positions.size() << "/" << _list.size() << std::endl;
        progress->setLabelText("inconsistent coords and images");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress->setValue(100);
        return;
    }

    int strsize = positions.back().size();

    _sock.write((const char *)&rate,sizeof(rate));
    _sock.write((const char *)&fileNum,sizeof(fileNum));
    progress->setValue(static_cast<int>(++count/max)*100);

    _sock.waitForBytesWritten(1000);
    std::vector<unsigned char> buffer;

    int i = 0;
    for(const auto & file : _list) {
        progress->setLabelText("Sending : " + file + " ...");
        std::cout << _list.size() << "  " << file.toStdString() << std::endl;
        std::ifstream in(_dir.absoluteFilePath(file).toStdString(),std::ios::binary);
        buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});

        buffer.shrink_to_fit();
        int imsize = buffer.size();

        _sock.write((const char *)&strsize,sizeof(strsize));
        _sock.write(positions[i].c_str(),strsize);
        _sock.write((const char *)&imsize, sizeof(imsize));
        _sock.write((const char *)buffer.data(),imsize);
        _sock.flush();

        _sock.waitForBytesWritten(5000);
        std::cout << "sent : " << file.toStdString() << std::endl;
        progress->setValue(static_cast<int>((float)(++count)/(float)max)*100);
        ++i;
    }

    _uped = true;
}


