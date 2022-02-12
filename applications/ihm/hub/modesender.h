#ifndef HUBV2_MODESENDER_H
#define HUBV2_MODESENDER_H

//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

#include <string>
#include <QTcpSocket>

class ModeSender {
private :
    QTcpSocket _sock;
    std::string _address;
    uint _mode;

public:
    ModeSender(uint mode, std::string  ip);
    ~ModeSender() = default;

    bool sendMode();


};

#endif //HUBV2_MODESENDER_H
