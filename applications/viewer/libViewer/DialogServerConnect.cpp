#include "DialogServerConnect.h"
#include "ui_DialogServerConnect.h"

#include <stream.h>
#include <QRegExpValidator>

DialogServerConnect::DialogServerConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogServerConnect)
{
    ui->setupUi(this);

    ui->lineEdit_ip->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}")));
    ui->lineEdit_ip->setText(SERVICE_IP);


    ui->spinBox_port->setValue(SERVICE_PORT);
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
