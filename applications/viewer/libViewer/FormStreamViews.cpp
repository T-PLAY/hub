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

#include <Configurations.hpp>

#include <Net/ClientSocket.hpp>

ViewerQt::ViewerQt( const std::string& ipv4, const int& port ) {
    auto _addStreamer = [this]( const std::string& streamerName,
                                const hub::SensorSpec& sensorSpec ) {
        std::cout << "[ViewerQt] add streamer " << streamerName << std::endl;
        emit addStreamSignal( streamerName, sensorSpec );
        return true;
    };
    auto _delStreamer = [this]( const std::string& streamerName,
                                const hub::SensorSpec& sensorSpec ) {
        std::cout << "[ViewerQt] del streamer " << streamerName << std::endl;
        emit delStreamSignal( streamerName, sensorSpec );
    };
    auto _onServerConnected = [this]( const std::string& ipv4, int port ) {
        emit serverConnected();
        std::cout << "[ViewerQt] connected to server : " << ipv4 << " " << port << std::endl;
    };
    auto _onServerDisconnected = [this]( const std::string& ipv4, int port ) {
        emit serverDisconnected();
        std::cout << "[ViewerQt] disconnected from server : " << ipv4 << " " << port << std::endl;
    };

    m_viewer = new hub::Viewer(
        _addStreamer, _delStreamer, _onServerConnected, _onServerDisconnected, {}, ipv4, port );
}

ViewerQt::~ViewerQt() {
    delete m_viewer;
}

// Thread_Client::Thread_Client( const FormStreamViews& formSensorViews, QObject* parent ) :
// }

// void Thread_Client::run() {

//}

///////////////////////////////////////////////////////////////////////////////////////

FormStreamViews::FormStreamViews( QWidget* parent ) :
    QWidget( parent ), ui( new Ui::FormStreamViews ) {
    m_ipv4 = hub::net::s_defaultServiceIp;
    m_port = hub::net::s_defaultServicePort;

    ui->setupUi( this );

    ui->lineEdit_ip->setValidator( new QRegularExpressionValidator( m_ipv4RegularExpression ) );

    assert( m_ipv4RegularExpression.match( m_ipv4.c_str() ).hasMatch() );
    ui->lineEdit_ip->setText( m_ipv4.c_str() );
    ui->spinBox_port->setValue( m_port );

    std::cout << "[FormStreamViews] connect viewerQt start" << std::endl;
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
    std::cout << "[FormStreamViews] connect viewerQt end" << std::endl;

    m_sensorModel.setStringList( QStringList( "none" ) );
}

FormStreamViews::~FormStreamViews() {
    std::cout << "[FormStreamViews] ~FormStreamViews() start" << std::endl;

    delete m_viewerQt;

    std::cout << "[FormStreamViews] ~FormStreamViews() mThreadClient.terminated()" << std::endl;

    for ( auto& pair : m_sensorViews ) {
        auto* sensorView = pair.second;
        delete sensorView;
    }

    delete ui;
    std::cout << "[FormStreamViews] ~FormStreamViews() end" << std::endl;
}

// void FormStreamViews::closeAllStream()
//{
// }

// void FormStreamViews::startStreaming()
//{
// }

void FormStreamViews::onServerConnect() {
    m_serverConnected = true;
    ui->lineEdit_ip->setEnabled( false );
    ui->spinBox_port->setEnabled( false );
    m_serverPing = true;

    emit serverConnected();
}

void FormStreamViews::onServerDisconnect() {
    m_serverConnected = false;
    ui->lineEdit_ip->setEnabled( true );
    ui->spinBox_port->setEnabled( true );
    m_serverPing = true;

#ifdef OS_LINUX
    ++m_port;
#endif
    ui->spinBox_port->setValue( m_port );

    emit serverDisconnected();
}

void FormStreamViews::addStream( const std::string& streamName,
                                 const hub::SensorSpec& sensorSpec ) {

    std::cout << "[FormStreamViews] FormStreamViews::addStream '" << streamName << "'" << std::endl;
    assert( m_sensorViews.find( streamName ) == m_sensorViews.end() );

    auto* sensorView = new FormStreamView( streamName, sensorSpec, m_sensorModel, nullptr );
    ui->verticalLayout->insertWidget( static_cast<int>( m_sensorViews.size() ), sensorView );

    m_sensorViews[streamName] = sensorView;
    auto stringList           = m_sensorModel.stringList();
    stringList.append( streamName.c_str() );
    m_sensorModel.setStringList( stringList );

    QObject::connect(
        sensorView, &FormStreamView::streamingStarted, this, &FormStreamViews::streamingStarted );
    QObject::connect(
        sensorView, &FormStreamView::streamingStopped, this, &FormStreamViews::streamingStopped );

    std::cout << "[FormStreamViews] FormStreamViews::addStream '" << streamName << "' done."
              << std::endl;

    if ( m_autoStartStream ) { sensorView->on_startStreaming(); }
}

void FormStreamViews::delStream( const std::string& streamName,
                                 const hub::SensorSpec& sensorSpec ) {
    std::cout << "[FormStreamViews] FormStreamViews::delStream '" << streamName << "'" << std::endl;

    assert( m_sensorViews.find( streamName ) != m_sensorViews.end() );
    auto* sensorView = m_sensorViews.at( streamName );
    m_sensorViews.erase( streamName );
    auto stringList = m_sensorModel.stringList();
    stringList.removeAll( streamName.c_str() );
    m_sensorModel.setStringList( stringList );
    delete sensorView;

    emit streamingStopped( streamName, sensorSpec );

    std::cout << "[FormStreamViews] FormStreamViews::delStream end '" << streamName << "'"
              << std::endl;
}

// int FormStreamViews::getPort() const {
// }

const std::string& FormStreamViews::getIpv4() const {
    return m_ipv4;
}

const int& FormStreamViews::getPort() const {
    return m_port;
}

// bool FormStreamViews::isServerConnected() const {
// }

// void FormStreamViews::onQuitApp()
//{
// }

const FormStreamView& FormStreamViews::getSensorView( const std::string& streamName ) const {
    assert( m_sensorViews.find( streamName ) != m_sensorViews.end() );
    return *m_sensorViews.at( streamName );
}

// void FormStreamViews::setMdiArea(QMdiArea *newMdiArea)
//{
// }

void FormStreamViews::on_lineEdit_ip_textChanged( const QString& ipv4 ) {
    if ( m_viewerQt != nullptr ) {
        m_ipv4 = ipv4.toStdString();

        if ( m_ipv4RegularExpression.match( m_ipv4.c_str() ).hasMatch() ) {
            m_viewerQt->m_viewer->setIpv4( m_ipv4 );
            ui->lineEdit_ip->setStyleSheet( "background-color: none" );
        }
        else { ui->lineEdit_ip->setStyleSheet( "background-color: red" ); }
    }
}

void FormStreamViews::on_spinBox_port_valueChanged( int port ) {
    if ( m_viewerQt != nullptr ) {
        m_port = port;
        m_viewerQt->m_viewer->setPort( m_port );
    }
}
