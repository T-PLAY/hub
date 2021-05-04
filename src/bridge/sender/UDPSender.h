#ifndef BRIDGE_UDPSENDER_H
#define BRIDGE_UDPSENDER_H
#include "Sender.h"
#include <QUdpSocket>


class UDPSender : public Sender {
private :
    QUdpSocket _sock;
    QHostAddress _address;
    int _port;
public:
    UDPSender(const std::string&  ipAddress, int port);

    void send(std::string message) override;
    void send(TrackerData data) override;
    void cleanup() override;
};


#endif
