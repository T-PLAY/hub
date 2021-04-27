#include <iostream>
#include "hub.h"
#include "ui_hub.h"

Hub::Hub(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hub)
{
    ui->setupUi(this);

    ui->_modeCombo->addItem("Diapo");
    ui->_modeCombo->addItem("Select");
    ui->_modeCombo->addItem("Stream");

    connect(ui->_detectButton,SIGNAL(clicked()),this,SLOT(OnDetectClicked()));
    connect(ui->_launchButton,SIGNAL(clicked()),this,SLOT(OnLaunchClicked()));

}

Hub::~Hub()
{
    delete ui;
}

void Hub::OnDetectClicked() {
    QMessageBox msgBox;
    msgBox.setText("Not Implemented");
    msgBox.exec();
}

void Hub::OnLaunchClicked() {
    switch (ui->_modeCombo->currentIndex()) {
        case 0 : //Diapo
            _mode = 0;
            if(sendMode()) {
                _dw.setIp(ui->_ipEdit->text().toStdString());
                _dw.show();
                hide();
            }else {
                showError(("Could not connect to : "
                           + ui->_ipEdit->text().toStdString() + ":"
                           + std::to_string(4042)).c_str());
            }
            break;
        case 1 : //Select
            _mode = 1;
            if(sendMode()) {
                _sw.show();
                hide();
            } else {
                showError(("Could not connect to : "
                           + ui->_ipEdit->text().toStdString() + ":"
                           + std::to_string(4042)).c_str());
            }
            break;
        case 2 : //Stream
            _mode = 2;
            if(sendMode()) {
                _stw.show();
                hide();
            } else {
                showError(("Could not connect to : "
                            + ui->_ipEdit->text().toStdString() + ":"
                            + std::to_string(4042)).c_str());
            }
            break;

        default :

            break;
    }
}

bool Hub::sendMode() {
    ModeSender ms(_mode, ui->_ipEdit->text().toStdString());
    return ms.sendMode();
}

void Hub::showError(const QString& msg) {
    _em.showMessage(msg,"network");
}


