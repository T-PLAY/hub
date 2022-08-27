#include "FormStreamViews.h"

#include "ui_FormStreamViews.h"

#include <FormStreamView.h>
#include <QMdiSubWindow>
//#include <QThread>
#include <QApplication>
#include <QMainWindow>

//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//#include <QRegExpValidator>
//#else
//#include <QRegularExpressionValidator>
//#include <QRegularExpression>
//#endif

#include <Net/ClientSocket.hpp>

ViewerQt::ViewerQt( const std::string& ipv4, const int& port ) {
    auto _addStreamer = [this]( const std::string& streamerName,
                                const hub::SensorSpec& sensorSpec ) {
        emit addStreamSignal( streamerName, sensorSpec );
        //        this->addStream( streamerName, sensorSpec );
    };
    //    auto _delStream = std::bind(&FormStreamViews::delStream, this);
    //    auto _delStream = [this] -> {FormStreamViews::delStream();}
    auto _delStreamer = [this]( const std::string& streamerName,
                                const hub::SensorSpec& sensorSpec ) {
        emit delStreamSignal( streamerName, sensorSpec );
    };
    auto _onServerConnected    = [this]() { emit serverConnected(); };
    auto _onServerDisconnected = [this]() { emit serverDisconnected(); };

    m_viewer = new hub::Viewer(
        ipv4, port, _addStreamer, _delStreamer, _onServerConnected, _onServerDisconnected );
}

ViewerQt::~ViewerQt() {
    delete m_viewer;
}

// Thread_Client::Thread_Client( const FormStreamViews& formSensorViews, QObject* parent ) :
//     QThread( parent )
//     //    , m_dialog(dialog)
//     ,
//     m_formSensorViews( formSensorViews ) {
//     std::cout << "[Thread_Client] Thread_Client()" << std::endl;
// }

// void Thread_Client::run() {
//     std::cout << "[Thread_Client] Thread_Client::run()" << std::endl;

//    while ( !this->isInterruptionRequested() ) {
//        try {
//            hub::net::ClientSocket sock( m_formSensorViews.ui->lineEdit_ip->text().toStdString(),
//                                         m_formSensorViews.ui->spinBox_port->value() );
//            sock.write( hub::net::ClientSocket::Type::VIEWER );
//            emit serverConnected();
//            //            m_dialog.hide();

//            while ( !this->isInterruptionRequested() ) {

//                hub::net::ClientSocket::Message serverMessage;
//                sock.read( serverMessage );

//                switch ( serverMessage ) {

//                case hub::net::ClientSocket::Message::PING: {
//                    // server check client connection
//                    // nothing to do
//                } break;

//                case hub::net::ClientSocket::Message::NEW_STREAMER: {
//                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
//                    std::string streamName;
//                    sock.read( streamName );
//                    //                    std::string streamName;
//                    //                    sock.read(streamName);
//                    //                    std::string format;
//                    //                    sock.read(format);
//                    //                    std::string dims;
//                    //                    sock.read(dims);
//                    //                    std::string size;
//                    //                    sock.read(size);
//                    //                    hub::SensorSpec::MetaData metaData;
//                    //                    sock.read(metaData);
//                    hub::SensorSpec sensorSpec;
//                    sock.read( sensorSpec );

//                    std::cout << "[Thread_Client] [viewer] new streamer " <<
//                    sensorSpec.m_sensorName
//                              << ", format:" << sensorSpec.m_format
//                              << ", dims:" << hub::SensorSpec::dims2string( sensorSpec.m_dims )
//                              << ", acquisitionSize:" << sensorSpec.m_acquisitionSize <<
//                              std::endl;
//                    std::cout << "[Thread_Client] [viewer] emit addStreamSignal '" << streamName
//                              << "'" << std::endl;
//                    std::cout << "[Thread_Client] [viewer] metadata : "
//                              << hub::SensorSpec::metaData2string( sensorSpec.m_metaData, true );
//                    emit addStreamSignal(streamName, sensorSpec);
////                    emit addStreamSignal(
////                        sensorSpec.m_sensorName,
////                        hub::SensorSpec::format2string(sensorSpec.m_format),
////                        hub::SensorSpec::dims2string(sensorSpec.m_dims),
////                        std::to_string(sensorSpec.m_acquisitionSize),
////                        hub::SensorSpec::metaData2string( sensorSpec.m_metaData, true ) );

//                    //                    InputStream inputStream(streamName.c_str(), "");

//                } break;

//                case hub::net::ClientSocket::Message::DEL_STREAMER: {
//                    std::string streamName;
//                    sock.read( streamName );
//                    hub::SensorSpec sensorSpec;
//                    sock.read( sensorSpec );
//                    std::cout << "[Thread_Client] [viewer] del streamer '" << streamName << "'"
//                              << std::endl;
//                    emit delStreamSignal( streamName, sensorSpec );
//                } break;

//                default:
//                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
//                }
//            }
//        }
//        catch ( std::exception& e ) {
//            emit serverDisconnected();
//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//            //            m_dialog.show();
//            std::cout << "[Thread_Client] [viewer] no server, catch exception : " << e.what()
//                      << std::endl;
//        }
//    } // while (!this->isInterruptionRequested())
//}

///////////////////////////////////////////////////////////////////////////////////////

FormStreamViews::FormStreamViews(
    //        QBoxLayout& vboxLayout,
    //    QMdiArea& mdiArea,
    //    QMainWindow& mainWindow,
    QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::FormStreamViews )
//    mThreadClient( *this, this )
//    , m_dialog(&mainWindow)
//    , m_dialog()
//    , mThreadClient(m_dialog, this)
//    , m_vBoxLayout(vboxLayout)
//    , m_mdiArea(mdiArea)
//    , m_mainWindow(mainWindow)
{
    ui->setupUi( this );
    //    mainWindow.setAttribute(Qt::WA_DeleteOnClose); // for dialog window

    ui->lineEdit_ip->setValidator( new QRegularExpressionValidator(
        QRegularExpression( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" ) ) );
    ui->lineEdit_ip->setText( hub::net::Socket::s_defaultServiceIp );
    ui->spinBox_port->setValue( hub::net::Socket::s_defaultServicePort );

    m_ipv4 = hub::net::Socket::s_defaultServiceIp;
    m_port = hub::net::Socket::s_defaultServicePort;

    //    QObject::connect(
    //        &mThreadClient, &Thread_Client::addStreamSignal, this, &FormStreamViews::addStream );
    //    QObject::connect(
    //        &mThreadClient, &Thread_Client::delStreamSignal, this, &FormStreamViews::delStream );

    //    QObject::connect(
    //        &mThreadClient, &Thread_Client::serverConnected, this,
    //        &FormStreamViews::onServerConnect );
    //    QObject::connect( &mThreadClient,
    //                      &Thread_Client::serverDisconnected,
    //                      this,
    //                      &FormStreamViews::onServerDisconnect );

//    {
//        auto _addStreamer = [this]( const std::string& streamerName,
//                                    const hub::SensorSpec& sensorSpec ) {
//            addStream( streamerName, sensorSpec );
//            //        this->addStream( streamerName, sensorSpec );
//        };
////        auto _addStream = std::bind(&FormStreamViews::addStream, this);
//        //    auto _delStream = std::bind(&FormStreamViews::delStream, this);
//        //    auto _delStream = [this] -> {FormStreamViews::delStream();}
//        auto _delStreamer = [this]( const std::string& streamerName,
//                                    const hub::SensorSpec& sensorSpec ) {
//            delStream( streamerName, sensorSpec );
//        };
//        auto _onServerConnected    = [this]() { onServerConnect(); };
//        auto _onServerDisconnected = [this]() { onServerDisconnect(); };

//    m_viewer = new hub::Viewer(
//        m_ipv4, m_port, _addStreamer, _delStreamer, _onServerConnected, _onServerDisconnected );
//    }

    {
        m_viewerQt = new ViewerQt( m_ipv4, m_port );
        QObject::connect(
            m_viewerQt, &ViewerQt::addStreamSignal, this, &FormStreamViews::addStream );
        QObject::connect(
            m_viewerQt, &ViewerQt::delStreamSignal, this, &FormStreamViews::delStream );
        QObject::connect(
            m_viewerQt, &ViewerQt::serverConnected, this, &FormStreamViews::onServerConnect );
        QObject::connect(
            m_viewerQt, &ViewerQt::serverDisconnected, this, &FormStreamViews::onServerDisconnect );
    }

    //    //    QObject::connect(QApplication::instance(), &QApplication::aboutToQuit, this,
    //    //    &FormStreamViews::onQuitApp);

    //    //    QObject::connect(&mainWindow, &QMainWindow::c)
    //    //    QObject::connect( &mThreadClient, &Thread_Client::serverConnected, this, [this]() {
    //    //        m_mainWindow.setEnabled( true );
    //    //    } );
    //    //    QObject::connect( &mThreadClient, &Thread_Client::serverDisconnected, this, [this]()
    //    {
    //    //        m_mainWindow.setEnabled( false );
    //    //    } );
    //    //    m_mainWindow.setEnabled(false);
    //    mThreadClient.start();

    m_sensorModel.setStringList( QStringList( "none" ) );
    //    m_mainWindow.setEnabled(false);
    //    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

FormStreamViews::~FormStreamViews() {
    std::cout << "[FormStreamViews] ~FormStreamViews() start" << std::endl;

    //    m_mdiArea = nullptr;

    //    mThreadClient.requestInterruption();
    //    mThreadClient.wait();
    //    assert(m_viewer != nullptr);

    delete m_viewerQt;
//    delete m_viewer;

    std::cout << "[FormStreamViews] ~FormStreamViews() mThreadClient.terminated()" << std::endl;

    //    m_dialog.close();

    //    for ( auto& pair : mStreamViews ) {
    //        auto* streamView = pair.second;

    //        assert( streamView != nullptr );
    //        std::cout << "[FormStreamViews] ~FormStreamViews() delete " << streamView <<
    //        std::endl; delete streamView; streamView = nullptr;
    //    }
    //    mStreamViews.clear();
    for ( auto& pair : m_sensorViews ) {
        auto* sensorView = pair.second;
        delete sensorView;
    }

    delete ui;
    std::cout << "[FormStreamViews] ~FormStreamViews() end" << std::endl;
}

// void FormStreamViews::startStreaming()
//{
//     for (auto & pair : m_sensorViews) {
//         pair.second->on_startStreaming();
//     }
// }

void FormStreamViews::onServerConnect() {
    //    m_mainWindow.setEnabled(true);
    //    m_dialog.hide();
    m_serverConnected = true;
    ui->lineEdit_ip->setEnabled( false );
    ui->spinBox_port->setEnabled( false );
    m_serverPing = true;
}

void FormStreamViews::onServerDisconnect() {
    //    m_mainWindow.setEnabled(false);
    //    m_dialog.setEnabled(true);
    //    m_dialog.show();
    m_serverConnected = false;
    ui->lineEdit_ip->setEnabled( true );
    ui->spinBox_port->setEnabled( true );
    m_serverPing = true;
    //    for (auto& pair : m_sensorViews) {
    //        auto* sensorView = pair.second;
    //        delete sensorView;
    //    }

    //    m_dialog = new DialogServerConnect(&m_mainWindow);
    emit serverDisconnected();
}

void FormStreamViews::addStream( const std::string& streamName,
                                 const hub::SensorSpec& sensorSpec ) {
    //    assert(m_mdiArea != nullptr);

    std::cout << "[FormStreamViews] FormStreamViews::addStream '" << streamName << "'" << std::endl;
    assert( m_sensorViews.find( streamName ) == m_sensorViews.end() );

    auto* sensorView = new FormStreamView( streamName, sensorSpec, m_sensorModel, nullptr );
    ui->verticalLayout->insertWidget( static_cast<int>( m_sensorViews.size() ), sensorView );

    m_sensorViews[streamName] = sensorView;
    auto stringList           = m_sensorModel.stringList();
    stringList.append( streamName.c_str() );
    m_sensorModel.setStringList( stringList );

    //    m_sensorModel.in

    //    QObject::connect(
    //        sensorView, &FormStreamView::addViewStreamSignal, this,
    //        &FormStreamViews::addStreamView );
    //    QObject::connect(
    //            sensorView, &FormStreamView::delViewStreamSignal, this,
    //            &FormStreamViews::delStreamView );
    //    emit sensorAdded(streamName);

    QObject::connect(
        sensorView, &FormStreamView::streamingStarted, this, &FormStreamViews::streamingStarted );
    QObject::connect(
        sensorView, &FormStreamView::streamingStopped, this, &FormStreamViews::streamingStopped );

    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //    sensorView->on_radioButtonOnOff_clicked(true);
    std::cout << "[FormStreamViews] FormStreamViews::addStream '" << streamName << "' done."
              << std::endl;

    if (m_autoStartStream) {
    sensorView->on_startStreaming();
    }
}

void FormStreamViews::delStream( const std::string& streamName,
                                 const hub::SensorSpec& sensorSpec ) {
    std::cout << "[FormStreamViews] FormStreamViews::delStream '" << streamName << "'" << std::endl;

    //    delStreamView( streamName );

    assert( m_sensorViews.find( streamName ) != m_sensorViews.end() );
    auto* sensorView = m_sensorViews.at( streamName );
    m_sensorViews.erase( streamName );
    auto stringList = m_sensorModel.stringList();
    stringList.removeAll( streamName.c_str() );
    m_sensorModel.setStringList( stringList );
    delete sensorView;

    emit streamingStopped( streamName, sensorSpec );

    //    assert( mSensorViews.find( streamName ) !=
    //            mSensorViews.end() ); // todo: fix one more time error
    //    FormStreamView* sensorView = mSensorViews.at( streamName );
    //    delete sensorView;
    //    mSensorViews.erase( streamName );

    std::cout << "[FormStreamViews] FormStreamViews::delStream end '" << streamName << "'"
              << std::endl;

    //    emit sensorDeleted(streamName);
}

bool FormStreamViews::isServerConnected() const {
    //    while (!m_serverPing) {
    //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //    };
    return m_serverConnected;
}

// void FormStreamViews::onQuitApp()
//{
//     std::cout << "[FormStreamViews] onQuitApp" << std::endl;
// }

const FormStreamView& FormStreamViews::getSensorView( const std::string& streamName ) const {
    assert( m_sensorViews.find( streamName ) != m_sensorViews.end() );
    return *m_sensorViews.at( streamName );
}

// void FormStreamViews::setMdiArea(QMdiArea *newMdiArea)
//{
//     m_mdiArea = newMdiArea;
// }

void FormStreamViews::on_lineEdit_ip_textChanged( const QString& ipv4 ) {
    m_ipv4 = ipv4.toStdString();
}

void FormStreamViews::on_spinBox_port_valueChanged( int port ) {
    m_port = port;
}
