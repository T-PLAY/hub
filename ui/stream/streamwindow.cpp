#include "streamwindow.h"
#include "ui_streamwindow.h"

StreamWindow::StreamWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::StreamWindow)
{
    ui->setupUi(this);
}

StreamWindow::~StreamWindow()
{
    delete ui;
}