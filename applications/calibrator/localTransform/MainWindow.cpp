#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIntValidator>
#include <QObject>
#include <QSpinBox>

#include <Acquisition.hpp>
#include <glm/gtx/string_cast.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->spinBox_tx,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->spinBox_ty,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->spinBox_tz,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->doubleSpinBox_rx,
        static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->doubleSpinBox_ry,
        static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->doubleSpinBox_rz,
        static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->spinBox_sx,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->spinBox_sy,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);
    QObject::connect(ui->spinBox_sz,
        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &MainWindow::updateTransform);

    hub::SensorSpec::MetaData metaData;
//    metaData["parent"] = "Polhemus Patriot (sensor 2)";
    m_outputSensor = new hub::OutputSensor(
        { "calibrator", { { { 1 }, hub::Format::MAT4 } }, metaData },
        hub::io::OutputStream("calibrator"));
    updateTransform();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_outputSensor;
}

void MainWindow::updateTransform()
{
    glm::mat4 transform(1.0); // identity

    int tx = ui->spinBox_tx->value();
    int ty = ui->spinBox_ty->value();
    int tz = ui->spinBox_tz->value();
    glm::vec3 translation(tx, ty, tz);

    transform = glm::translate(transform, translation);

    double rx = ui->doubleSpinBox_rx->value();
    double ry = ui->doubleSpinBox_ry->value();
    double rz = ui->doubleSpinBox_rz->value();
    transform = glm::rotate(transform, glm::radians((float)rx), glm::vec3(1.0, 0.0, 0.0));
    transform = glm::rotate(transform, glm::radians((float)ry), glm::vec3(0.0, 1.0, 0.0));
    transform = glm::rotate(transform, glm::radians((float)rz), glm::vec3(0.0, 0.0, 1.0));

    int sx = ui->spinBox_sx->value();
    int sy = ui->spinBox_sy->value();
    int sz = ui->spinBox_sz->value();
    transform = glm::scale(transform, glm::vec3((float)sx, (float)sy, (float)sz));

    m_transform = transform;

    std::string matPrint = "mat4 :\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matPrint += std::to_string(transform[i][j]) + ", ";
        }
        matPrint += "\n";
    }
    matPrint += "\n";
    const float* array = glm::value_ptr(transform);
    matPrint += "array content (row major) : \n";
    for (int i = 0; i < 16; ++i) {
        matPrint += std::to_string(array[i]) + " ";
    }
    matPrint += "\n";

    ui->textBrowser->setText(matPrint.c_str());

    // TODO: why data steal not work with output ?
    *m_outputSensor << (hub::Acquisition(0, 0)
                        << hub::Measure((const unsigned char*)array, 16 * 4, {{1}, hub::Format::MAT4}));
}
