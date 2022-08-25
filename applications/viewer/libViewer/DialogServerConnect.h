#ifndef DIALOGSERVERCONNECT_H
#define DIALOGSERVERCONNECT_H

#include <QDialog>


namespace Ui {
class DialogServerConnect;
}

class DialogServerConnect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogServerConnect(QWidget *parent = nullptr);
    ~DialogServerConnect();

    std::string getIpV4() const;
    int getPort() const;
//    std::string m_ip = SERVICE_IP;
//    int m_port = SERVICE_PORT;
private:
    Ui::DialogServerConnect *ui;

};

#endif // DIALOGSERVERCONNECT_H
