#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIntValidator>
#include <QObject>
#include <QSpinBox>

#include <Acquisition.hpp>
#include <IO/Stream.hpp>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    QObject::connect( ui->spinBox_latency,
                      static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
                      this,
                      &MainWindow::updateLatency );

    const std::string streamName = "Keyboard";
    m_inputSensor                = new hub::InputSensor( hub::io::InputStream( streamName, "" ) );
    m_outputSensor               = new hub::OutputSensor(
        m_inputSensor->m_spec, hub::io::OutputStream( streamName + " (timeShifted)" ) );

    m_thread = std::thread( [this]() {
        while ( true ) {
            //            std::cout << "+" << std::flush;
            auto acq = m_inputSensor->getAcquisition();
            //            std::cout << "receive acq : " << acq << std::endl;
//            *m_outputSensor << acq;
            *m_outputSensor << ( hub::Acquisition { acq.m_start + m_latency * 1000, acq.m_end + m_latency * 1000 }
                                 << acq.getMeasures() );
        }
    } );
}

MainWindow::~MainWindow() {
    delete ui;

    delete m_outputSensor;
}

void MainWindow::updateLatency() {
    m_latency = ui->spinBox_latency->value();
}
