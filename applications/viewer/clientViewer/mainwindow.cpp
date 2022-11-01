#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMdiSubWindow>

#include <iostream>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {

    ui->setupUi( this );

    //    setAttribute(Qt::WA_DeleteOnClose);
    //    m_sensorViews = new SensorViews(*ui->verticalLayoutSensor, *ui->mdiArea, *this);
    ui->widgetSensorViews->setMdiArea( ui->mdiArea );

    //    m_sensorViews = new SensorViews(*ui->verticalLayoutSensor, *ui->mdiArea, *this, this);
}

MainWindow::~MainWindow() {
    std::cout << "[MainWindow] ~MainWindow()" << std::endl;

    //    delete m_sensorViews;
    //    m_sensorViews = nullptr;

    delete ui;
    std::cout << "[MainWindow] ~MainWindow() deleted ui" << std::endl;
}
