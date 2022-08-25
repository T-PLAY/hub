#include "DialogServerConnect.h"
#include "ui_DialogServerConnect.h"

#include <Net/Socket.hpp>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QRegExpValidator>
#else
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#endif

DialogServerConnect::DialogServerConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogServerConnect)
{
    ui->setupUi(this);

    ui->lineEdit_ip->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}")));
    ui->lineEdit_ip->setText(hub::net::Socket::s_defaultServiceIp);


    ui->spinBox_port->setValue(hub::net::Socket::s_defaultServicePort);
}

DialogServerConnect::~DialogServerConnect()
{
    delete ui;
}

std::string DialogServerConnect::getIpV4() const
{
    return ui->lineEdit_ip->text().toStdString();
}

int DialogServerConnect::getPort() const
{
    return ui->spinBox_port->value();
}
