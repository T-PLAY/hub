#ifndef IMAGELOADER_UPLOADER_H
#define IMAGELOADER_UPLOADER_H


#include <QDir>
#include <thread>
#include <utility>
#include <QProgressDialog>
#include <fstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Uploader {
private:
    QDir _dir;
    QStringList _list;
    QString _ipAddress;
    int _port;
    QProgressDialog* progress;

    int _sock;
    struct sockaddr_in _addr;
    socklen_t _addrLen;

    std::thread _uploadThread;
    bool _uped = false;

    void uploadTask();
public:
    Uploader(const QDir& dir, QStringList list, QString ipAddress, const QString& port);
    void upload();



};

#endif //IMAGELOADER_UPLOADER_H
