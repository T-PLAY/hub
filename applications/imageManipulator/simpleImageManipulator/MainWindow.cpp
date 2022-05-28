#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_imageManipulator = new FormImageManipulator(this);
//    m_imageManipulator->setMinimumWidth(640);
//    m_imageManipulator->setMinimumHeight(1000);

//    FormIm
    ui->mdiArea->addSubWindow(m_imageManipulator);
}

MainWindow::~MainWindow()
{
    delete ui;
}
