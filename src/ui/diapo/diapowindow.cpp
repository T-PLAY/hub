#include "diapowindow.h"
#include "ui_diapowindow.h"

DiapoWindow::DiapoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiapoWindow)
{
    ui->setupUi(this);

    connect(ui->_selectFolder, SIGNAL(clicked()), this, SLOT(select_clicked()));
    connect(ui->_upload, SIGNAL(clicked()), this, SLOT(upload_clicked()));

    ui->_folderName->setText(PROJECT_DIR "images/base");
    ui->_ipAddress->setText("127.0.0.1");
    ui->_port->setText("4041");
}

DiapoWindow::~DiapoWindow()
{
    delete ui;
}

void DiapoWindow::select_clicked() {
    QFileDialog dialog;
//    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory(PROJECT_DIR "images");
    if (dialog.exec()) {
        ui->_folderName->setText(dialog.directory().path());
    }
}

void DiapoWindow::upload_clicked() {
    QDir dir(ui->_folderName->text());
    QStringList list = dir.entryList(QStringList() << "*.png" << "*.PNG");

    Uploader up(dir,list,ui->_ipAddress->text(),ui->_port->text());
    up.upload();
}

void DiapoWindow::setIp(const std::string& ip) {
    ui->_ipAddress->setText(ip.c_str());
}
