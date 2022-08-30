#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIntValidator>

#include <glm/gtx/string_cast.hpp>
#include <Acquisition.hpp>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    QObject::connect( ui->spinBox_tx, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_ty, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_tz, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_rx, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_ry, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_rz, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_sx, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_sy, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );
    QObject::connect( ui->spinBox_sz, &QSpinBox::valueChanged, this, &MainWindow::updateTransform );

    //    std::string sensorName = "calibrator";
    m_outputSensor =
        new hub::OutputSensor( { "calibrator", { { { 1 }, hub::SensorSpec::Format::MAT4 } } },
                               hub::io::OutputStream( "calibrator" ) );
    updateTransform();
}

MainWindow::~MainWindow() {
    delete ui;

    delete m_outputSensor;
}

void MainWindow::updateTransform() {
    glm::mat4 transform( 1.0 ); // identity

    int tx = ui->spinBox_tx->value();
    int ty = ui->spinBox_ty->value();
    int tz = ui->spinBox_tz->value();
    glm::vec3 translation( tx, ty, tz );

    transform = glm::translate( transform, translation );

    int rx    = ui->spinBox_rx->value();
    int ry    = ui->spinBox_ry->value();
    int rz    = ui->spinBox_rz->value();
    transform = glm::rotate( transform, glm::radians( (float)rx ), glm::vec3( 1.0, 0.0, 0.0 ) );
    transform = glm::rotate( transform, glm::radians( (float)ry ), glm::vec3( 0.0, 1.0, 0.0 ) );
    transform = glm::rotate( transform, glm::radians( (float)rz ), glm::vec3( 0.0, 0.0, 1.0 ) );

    int sx    = ui->spinBox_sx->value();
    int sy    = ui->spinBox_sy->value();
    int sz    = ui->spinBox_sz->value();
    transform = glm::scale( transform, glm::vec3( (float)sx, (float)sy, (float)sz ) );
    //    transform = glm::scale( transform, glm::vec3( 0.0, 1.0, 0.0 ) * (float)sy );
    //    transform = glm::scale( transform, glm::vec3( 0.0, 0.0, 1.0 ) * (float)sz );

    m_transform = transform;

    //    std::string matPrint = glm::to_string( transform );
    std::string matPrint = "mat4 :\n";
    for ( int i = 0; i < 4; ++i ) {
        for ( int j = 0; j < 4; ++j ) {
            //            std::cout << transform[i][j] << ", ";
            matPrint += std::to_string( transform[i][j] ) + ", ";
        }
        matPrint += "\n";
    }
    matPrint += "\n";
    float * array = glm::value_ptr(transform);
    matPrint += "array content (row major) : \n";
    for (int i = 0; i <16; ++i) {
        matPrint += std::to_string(array[i]) + " ";
    }
    matPrint += "\n";

    ui->textBrowser->setText( matPrint.c_str() );

    *m_outputSensor << (hub::Acquisition(0, 0) << hub::Measure((unsigned char*)array, 16 * 4));
}
