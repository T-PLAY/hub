#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    QObject::connect( this, &MainWindow::newStreamer, this, &MainWindow::onNewStreamer );
    QObject::connect( this, &MainWindow::delStreamer, this, &MainWindow::onDelStreamer );
    QObject::connect( this, &MainWindow::serverConnected, this, &MainWindow::onServerConnected );
    QObject::connect(
        this, &MainWindow::serverDisconnected, this, &MainWindow::onServerDisconnected );
//    QObject::connect( this, &MainWindow::newAcquisition, this, &MainWindow::onNewAcquisition );

    //    std::cout << "[FormStreamViews] connect viewerQt start" << std::endl;
    //    m_serverView = new FormServerView({}, {});
    // init viewer
    {
        auto _onNewStreamer = [=]( const std::string& streamerName,
                                   const hub::SensorSpec& sensorSpec ) {
            emit newStreamer( streamerName, sensorSpec );
            return true;
        };
        auto _onDelStreamer = [=]( const std::string& streamerName,
                                   const hub::SensorSpec& sensorSpec ) {
            emit delStreamer( streamerName, sensorSpec );
        };
        auto _onServerConnected = [=]( const std::string& ipv4, int port ) {
            //            onServerConnected(ipv4, port);
            emit serverConnected( ipv4, port );
        };
        auto _onServerDisconnected = [=]( const std::string& ipv4, int port ) {
            //            onServerDisconnected(ipv4, port);
            emit serverDisconnected( ipv4, port );
        };
        auto _onNewAcquisition = [=]( const std::string& streamerName,
                                      const hub::Acquisition& acq ) {
                onNewAcquisition( streamerName, acq );
//            emit newAcquisition( streamerName, acq );
        };

        assert( m_serverView == nullptr );
        m_serverView = new FormServerView( {},
                                           {},
                                           _onServerConnected,
                                           _onServerDisconnected,
                                           {},
                                           false,
                                           this );

        //        ui->verticalLayout_2->addWidget(m_serverView);
        ui->verticalLayout->addWidget( m_serverView );
    }
//        ui->verticalLayout->addWidget(m_serverView);

    m_widgetLoader = new FormWidgetLoader( m_serverView->getIpv4(), m_serverView->getPort() );
    m_widgetLoader->setEnabled( false );
    ui->verticalLayout->addWidget( m_widgetLoader );
}

MainWindow::~MainWindow() {
    delete m_widgetLoader;
    delete m_serverView;

    delete ui;
}

bool MainWindow::onNewStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
    std::cout << "[MainWindow] onNewStreamer(" << streamName << ", " << sensorSpec << ")"
              << std::endl;
    return true;
}

void MainWindow::onDelStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
    std::cout << "[MainWindow] onDelStreamer(" << streamName << ", " << sensorSpec << ")"
              << std::endl;
}

void MainWindow::onServerConnected( const std::string& ipv4, int port ) {
    std::cout << "[MainWindow] onServerConnected(" << ipv4 << ", " << port << ")" << std::endl;

    m_widgetLoader->setEnabled( true );
}

void MainWindow::onServerDisconnected( const std::string& ipv4, int port ) {
    std::cout << "[MainWindow] onServerDisconnected(" << ipv4 << ", " << port << ")" << std::endl;

    m_widgetLoader->setEnabled( false );
}

void MainWindow::onNewAcquisition( const std::string& streamName, const hub::Acquisition& acq ) {
//    std::cout << "[MainWindow] onNewAcquisition(" << streamName << ", " << acq << ")" << std::endl;
}
