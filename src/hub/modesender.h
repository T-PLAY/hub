#ifndef HUBV2_MODESENDER_H
#define HUBV2_MODESENDER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

class ModeSender {
private :
    int _sock;
    struct sockaddr_in _addr;
    socklen_t _addrLen = sizeof(_addr);
    uint _mode;
    std::string _ip;


public:
    ModeSender(uint mode, std::string ip);

    bool sendMode();


};

#endif //HUBV2_MODESENDER_H
