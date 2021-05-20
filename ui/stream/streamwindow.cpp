#include <QFileDialog>
#include "streamwindow.h"
#include "ui_streamwindow.h"
#include "../../src/imStream/FileStreamer.h"

StreamWindow::StreamWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::StreamWindow)
{
    ui->setupUi(this);
    ui->_trackerComboBox->addItem("Basic Tracker");
    ui->_trackerComboBox->addItem("File Tracker");
    ui->_trackerComboBox->setCurrentIndex(1);
    _infos.trackerType = STREAMTRACKER;

    ui->_senderComboBox->addItem("UDP Sender");
    ui->_senderComboBox->addItem("File Sender");
    _infos.senderType = UDPSENDER;

    ui->_sourceCombo->addItem("Folder");
    //ui->_sourceCombo->addItem("Test");


    ui->_stopButton->setEnabled(false);
    ui->_startButton->setEnabled(true);
    ui->_streamStart->setEnabled(true);
    ui->_streamStop->setEnabled(false);

    update_fields();
    update_values();
    update_infos();

    _bridge.setupSync(_infos);

    connect(ui->_selectFolder, SIGNAL(clicked()), this, SLOT(select_clicked()));
    connect(ui->_startButton, SIGNAL(clicked()), this, SLOT(start_clicked()));
    connect(ui->_streamStart, SIGNAL(clicked()), this, SLOT(start_stream_clicked()));
    connect(ui->_stopButton, SIGNAL(clicked()), this, SLOT(stop_clicked()));
    connect(ui->_streamStop, SIGNAL(clicked()), this, SLOT(stop_stream_clicked()));
    connect(ui->_updateButton, SIGNAL(clicked()), this, SLOT(update_clicked()));
    connect(ui->_updateStream, SIGNAL(clicked()), this, SLOT(updateStream_clicked()));

    connect(ui->_trackerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(tracker_changed(int)));
    connect(ui->_senderComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sender_changed(int)));
    connect(ui->_sourceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(source_changed(int)));

    _updateThread = std::thread([this](){ StreamWindow::message_update();});
    _streamUpdateThread = std::thread([this](){ StreamWindow::stream_update();});

    ui->_folderName->setText("../streams/echosPNG");
    ui->_sendRateEdit->setText("5");


}

StreamWindow::~StreamWindow()
{
    delete ui;
}

void StreamWindow::setIp(const std::string &ip) {
    _baseIp = ip;
    ui->_ipAddress->setText(ip.c_str());
    ui->_senderLine1->setText(ip.c_str());

}

void StreamWindow::update_values() {
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

void StreamWindow::update_fields() {
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

void StreamWindow::update_infos() {
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

bool StreamWindow::start_sync() {
    return _bridge.start();
}

bool StreamWindow::stop_sync() {
    return _bridge.stop();
}

void StreamWindow::message_update() {
    while (_alive) {
        ui->_infoLabel->setText(_bridge.syncMessage().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void StreamWindow::start_clicked() {
    if(!start_sync()) return;

    _status = true;
    ui->_stopButton->setEnabled(true);
    ui->_startButton->setEnabled(false);


    update_infos();
    std::cout << "Start" << std::endl;
}

void StreamWindow::stop_clicked() {
    if(!stop_sync()) return;

    _status = false;
    ui->_stopButton->setEnabled(false);
    ui->_startButton->setEnabled(true);

    update_infos();
    std::cout << "Stop" << std::endl;
}

void StreamWindow::update_clicked() {
    std::cout << "Update" << std::endl;

    _status = false;

    ui->_stopButton->setEnabled(false);
    ui->_startButton->setEnabled(true);

    update_values();
    update_infos();

    _bridge.setupSync(_infos);
}

void StreamWindow::tracker_changed(int index) {
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

void StreamWindow::sender_changed(int index) {
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

void StreamWindow::stream_update() {
    while (_alive) {
        if(_stream) {
            ui->_label->setText(_stream->message().c_str());
        } else {
            ui->_label->setText("Stream is stopped");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void StreamWindow::start_stream_clicked() {
    if(!_stream) {
        std::unique_ptr<ImageAcquisition> acq;
        switch(ui->_sourceCombo->currentIndex()) {
            case 0 :
                acq = std::make_unique<FileStreamer>(ui->_folderName->text(), 10);
                break;
            default:
                std::cout << "unimplemented source for streaming" << std::endl;
                return;
        }
        _stream = std::make_unique<ImageStream>(std::move(acq), ui->_sendRateEdit->text().toInt());
    }
    _stream->start(ui->_ipAddress->text());

    while(!(_stream->error() || _stream->connected()));

    if(_stream->connected()) {
        ui->_streamStop->setEnabled(true);
        ui->_streamStart->setEnabled(false);
        ui->_updateStream->setEnabled(false);
    }
}

void StreamWindow::stop_stream_clicked() {
    _stream->stop();

    ui->_streamStop->setEnabled(false);
    ui->_streamStart->setEnabled(true);
    ui->_updateStream->setEnabled(true);

}

void StreamWindow::select_clicked() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory("../streams");
    if (dialog.exec()) {
        ui->_folderName->setText(dialog.directory().path());
    }
}

void StreamWindow::source_changed(int index) {
    switch(index) {
        case 0 : //File
            ui->_folderLabel->setVisible(true);
            ui->_folderName->setVisible(true);
            ui->_selectFolder->setVisible(true);
            break;

        default :
            ui->_folderLabel->setVisible(false);
            ui->_folderName->setVisible(false);
            ui->_selectFolder->setVisible(false);
            break;
    }
}

void StreamWindow::updateStream_clicked() {
    _stream.reset(nullptr);
}
