#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMdiSubWindow>

#include <iostream>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    m_sensorViews = new SensorViews(*ui->verticalLayoutSensor, *ui->mdiArea);

}

MainWindow::~MainWindow()
{
    std::cout << "[MainWindow] ~MainWindow()" << std::endl;

    delete m_sensorViews;

    delete ui;
    std::cout << "[MainWindow] ~MainWindow() deleted ui" << std::endl;

}
