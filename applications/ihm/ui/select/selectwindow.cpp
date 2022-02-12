#include "selectwindow.h"
#include "ui_selectwindow.h"

SelectWindow::SelectWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::SelectWindow)
{
    ui->setupUi(this);
    ui->_trackerComboBox->addItem("Basic Tracker");
    ui->_trackerComboBox->addItem("File Tracker");
    ui->_trackerComboBox->setCurrentIndex(1);
    _infos.trackerType = STREAMTRACKER;

    ui->_senderComboBox->addItem("UDP Sender");
    ui->_senderComboBox->addItem("File Sender");
    _infos.senderType = UDPSENDER;




    ui->_stopButton->setEnabled(false);
    ui->_startButton->setEnabled(true);

    update_fields();
    update_values();
    update_infos();

    _bridge.setupSync(_infos);

    connect(ui->_selectFolder, SIGNAL(clicked()), this, SLOT(select_clicked()));
    connect(ui->_upload, SIGNAL(clicked()), this, SLOT(upload_clicked()));
    connect(ui->_startButton, SIGNAL(clicked()), this, SLOT(start_clicked()));
    connect(ui->_stopButton, SIGNAL(clicked()), this, SLOT(stop_clicked()));
    connect(ui->_updateButton, SIGNAL(clicked()), this, SLOT(update_clicked()));

    connect(ui->_trackerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(tracker_changed(int)));
    connect(ui->_senderComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sender_changed(int)));


    ui->_folderName->setText(PROJECT_DIR "images/base");
    ui->_ipAddress->setText(_baseIp.c_str());
    ui->_port->setText("4041");

    _updateThread= std::thread([this](){ SelectWindow::message_update();});

}

void SelectWindow::select_clicked() {
    QFileDialog dialog;
//    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory(PROJECT_DIR "images");
    if (dialog.exec()) {
        ui->_folderName->setText(dialog.directory().path());
    }
}

void SelectWindow::upload_clicked() {
    QDir dir(ui->_folderName->text());
    QStringList list = dir.entryList(QStringList() << "*.png" << "*.PNG");

    Uploader up(dir,list,ui->_ipAddress->text(),ui->_port->text());
    up.upload();
}

void SelectWindow::setIp(const std::string& ip) {
    _baseIp = ip;
    ui->_ipAddress->setText(ip.c_str());
    ui->_senderLine1->setText(ip.c_str());

    update_clicked();
}


SelectWindow::~SelectWindow()
{
    delete ui;
}

void SelectWindow::update_values() {
    _infos.sendRate = ui->_sendRateSpin->value();
    _status = false;
    _infos.trackerInfo = ui->_trackerLine1->text();
    _infos.senderInfo = ui->_senderLine1->text();
    _infos.port = ui->_senderLine2->text().toInt();

    switch (ui->_trackerComboBox->currentIndex()) {
        case 0 :
            _infos.trackerType = BASICTRACKER;
            break;
        case 1 :
            _infos.trackerType = STREAMTRACKER;
            break;

        default:
            break;
    }
    switch (ui->_senderComboBox->currentIndex()) {
        case 0 :
            _infos.senderType = UDPSENDER;
            break;
        case 1 :
            _infos.senderType = STREAMSENDER;
            break;
        default:
            break;
    }
}

void SelectWindow::update_fields() {
    switch (_infos.trackerType) {
        case STREAMTRACKER :
            ui->_trackerLine1->setText("record4.txt");
            break;
        case BASICTRACKER:
        default:
            break;
    }
    switch (_infos.senderType) {
        case UDPSENDER :
            ui->_senderLine1->setText(_baseIp.c_str());
            ui->_senderLine2->setText("4040");
            break;

        case STREAMSENDER:
            ui->_senderLine1->setText("tmp.txt");
            ui->_senderLine2->setText("");
            break;

    }
}

void SelectWindow::update_infos() {
    ui->_statusInfoLabel->setText(_status ? "On" : "Off");

    switch(_infos.trackerType) {
        case BASICTRACKER :
            ui->_trackerInfoLabel->setText("Basic Tracker");
            break;
        case STREAMTRACKER:
            ui->_trackerInfoLabel->setText("File Tracker :\n  " + _infos.trackerInfo);
            break;
    }
    switch(_infos.senderType) {
        case UDPSENDER :
            ui->_senderInfoLabel->setText("UDP Sender :\n  " + _infos.senderInfo + "\n  " + QString(std::to_string(_infos.port).c_str()));
            break;
        case STREAMSENDER:
            ui->_senderInfoLabel->setText("File Sender :\n  " + _infos.senderInfo);
            break;
    }

    ui->_sendRateInfoLabel->setText(std::to_string(_infos.sendRate).c_str());
}

bool SelectWindow::start_sync() {
    return _bridge.start();
}

bool SelectWindow::stop_sync() {
    return _bridge.stop();
}

void SelectWindow::message_update() {
    while (_alive) {
        ui->_infoLabel->setText(_bridge.syncMessage().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SelectWindow::start_clicked() {
    if(!start_sync()) return;

    _status = true;
    ui->_stopButton->setEnabled(true);
    ui->_startButton->setEnabled(false);


    update_infos();
    std::cout << "Start" << std::endl;
}

void SelectWindow::stop_clicked() {
    if(!stop_sync()) return;

    _status = false;
    ui->_stopButton->setEnabled(false);
    ui->_startButton->setEnabled(true);

    update_infos();
    std::cout << "Stop" << std::endl;
}

void SelectWindow::update_clicked() {
    std::cout << "Update" << std::endl;

    _status = false;

    ui->_stopButton->setEnabled(false);
    ui->_startButton->setEnabled(true);

    update_values();
    update_infos();

    _bridge.setupSync(_infos);
}

void SelectWindow::tracker_changed(int index) {
    std::cout << "Tracker : " << index << std::endl;
    switch(index) {
        case 0 :
            ui->_trackerHint1->setVisible(false);
            ui->_trackerLine1->setVisible(false);
            break;
        case 1 :
            ui->_trackerHint1->setVisible(true);
            ui->_trackerLine1->setVisible(true);
            break;

        default :
            break;
    }

    update_values();
    update_fields();
}

void SelectWindow::sender_changed(int index) {
    std::cout << "Sender : " << index << std::endl;
    switch(index) {
        case 0 :
            ui->_senderHint2->setVisible(true);
            ui->_senderLine2->setVisible(true);

            ui->_senderHint1->setText("IP Address :");
            ui->_senderHint2->setText("Port n° :");
            break;
        case 1 :
            ui->_senderHint1->setText("File Name :");
            ui->_senderHint2->setVisible(false);
            ui->_senderLine2->setVisible(false);
            break;

        default:
            break;
    }


    update_values();
    update_fields();
}
