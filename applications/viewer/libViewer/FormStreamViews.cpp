#include "FormStreamViews.h"
#include "ui_FormStreamViews.h"

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMdiSubWindow>

#include <Configurations.hpp>
#include <Net/ClientSocket.hpp>

#include <FormStreamView.h>

// ViewerQt::ViewerQt(FormStreamViews &streamViews, const std::string &ipv4, const int &port) :
//     m_streamViews(streamViews)
//{
////ViewerQt::ViewerQt( const std::string& ipv4, const int& port ) {

//    auto _addStreamer = [this]( const std::string& streamerName,
//                                const hub::SensorSpec& sensorSpec ) {
//        std::cout << "[ViewerQt] add streamer " << streamerName << std::endl;
//        emit newStreamer( streamerName, sensorSpec );
//        return true;
//    };
//    auto _delStreamer = [this]( const std::string& streamerName,
//                                const hub::SensorSpec& sensorSpec ) {
//        std::cout << "[ViewerQt] del streamer " << streamerName << std::endl;
//        emit delStreamer( streamerName, sensorSpec );
//    };
//    auto _onServerConnected = [this]( const std::string& ipv4, int port ) {
//        emit serverConnected();
//        std::cout << "[ViewerQt] connected to server : " << ipv4 << " " << port << std::endl;
//    };
//    auto _onServerDisconnected = [this]( const std::string& ipv4, int port ) {
//        emit serverDisconnected();
//        std::cout << "[ViewerQt] disconnected from server : " << ipv4 << " " << port << std::endl;
//    };
//    auto _onNewAcquisition = [this]( const std::string& streamerName,
//                                     hub::Acquisition&& acq ) {
//        //        std::cout << "[ViewerQt] new acquisition " << streamerName << std::endl;
////        emit newAcquisition( streamerName, std::move(acq) );
//        m_streamViews.onNewAcquisition(streamerName, std::move(acq));
//    };

//    m_viewer = new hub::Viewer(
//        //        _addStreamer, _delStreamer, _onServerConnected, _onServerDisconnected, {}, ipv4,
//        //        port );
//        _addStreamer,
//        _delStreamer,
//        _onServerConnected,
//        _onServerDisconnected,
//        _onNewAcquisition,
//        ipv4,
//        port );
//}

// ViewerQt::~ViewerQt() {
//     delete m_viewer;
// }

FormStreamViews::FormStreamViews( QWidget* parent ) :
    QWidget( parent ), ui( new Ui::FormStreamViews ) {
    m_ipv4 = hub::net::s_defaultServiceIp;
    m_port = hub::net::s_defaultServicePort;

    ui->setupUi( this );

    ui->lineEdit_ip->setValidator( new QRegularExpressionValidator( m_ipv4RegularExpression ) );

    assert( m_ipv4RegularExpression.match( m_ipv4.c_str() ).hasMatch() );
    ui->lineEdit_ip->setText( m_ipv4.c_str() );
    ui->spinBox_port->setValue( m_port );

    QObject::connect( this, &FormStreamViews::newStreamer, this, &FormStreamViews::onNewStreamer );
    QObject::connect( this, &FormStreamViews::delStreamer, this, &FormStreamViews::onDelStreamer );
    QObject::connect(
        this, &FormStreamViews::serverConnected, this, &FormStreamViews::onServerConnected );
    QObject::connect(
        this, &FormStreamViews::serverDisconnected, this, &FormStreamViews::onServerDisconnected );

    //    std::cout << "[FormStreamViews] connect viewerQt start" << std::endl;
    //    {
    //        m_viewerQt = new ViewerQt( *this, m_ipv4, m_port );
    //        QObject::connect(
    //            m_viewerQt, &ViewerQt::newStreamer, this, &FormStreamViews::onNewStreamer );
    //        QObject::connect(
    //            m_viewerQt, &ViewerQt::delStreamer, this, &FormStreamViews::onDelStreamer );
    //        QObject::connect(
    //            m_viewerQt, &ViewerQt::serverConnected, this, &FormStreamViews::onServerConnect );
    //        QObject::connect(
    //            m_viewerQt, &ViewerQt::serverDisconnected, this,
    //            &FormStreamViews::onServerDisconnect );
    ////        QObject::connect(
    ////            m_viewerQt, &ViewerQt::newAcquisition, this, &FormStreamViews::onNewAcquisition
    ///);
    //    }
    //    std::cout << "[FormStreamViews] connect viewerQt end" << std::endl;

    m_sensorModel.setStringList( QStringList( "none" ) );
}

FormStreamViews::~FormStreamViews() {
    std::cout << "[FormStreamViews] ~FormStreamViews() start" << std::endl;

    delete m_viewer;

    std::cout << "[FormStreamViews] ~FormStreamViews() mThreadClient.terminated()" << std::endl;

    for ( auto& pair : m_streamViews ) {
        auto* sensorView = pair.second;
        delete sensorView;
    }

    delete ui;
    std::cout << "[FormStreamViews] ~FormStreamViews() end" << std::endl;
}

void FormStreamViews::initViewer(
    std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition ) {
    assert( m_viewer == nullptr );
    // init viewer
    {
        auto _onNewStreamer = [this]( const std::string& streamerName,
                                      const hub::SensorSpec& sensorSpec ) {
            emit newStreamer( streamerName, sensorSpec );
            return true;
        };
        auto _onDelStreamer = [this]( const std::string& streamerName,
                                      const hub::SensorSpec& sensorSpec ) {
            emit delStreamer( streamerName, sensorSpec );
        };
        auto _onServerConnected = [this]( const std::string& ipv4, int port ) {
            emit serverConnected(ipv4, port);
        };
        auto _onServerDisconnected = [this]( const std::string& ipv4, int port ) {
            emit serverDisconnected(ipv4, port);
        };
        //        auto _onNewAcquisition = [this]( const std::string& streamerName, const
        //        hub::Acquisition& acq ) {
        //            onNewAcquisition( streamerName, acq);
        //        };

        m_viewer = new hub::Viewer(
            //        _addStreamer, _delStreamer, _onServerConnected, _onServerDisconnected, {},
            //        ipv4, port );
            _onNewStreamer,
            _onDelStreamer,
            _onServerConnected,
            _onServerDisconnected,
            //                _onNewAcquisition,
            onNewAcquisition,
            m_ipv4,
            m_port );
    }
}

bool FormStreamViews::onNewStreamer( const std::string& streamName,
                                     const hub::SensorSpec& sensorSpec ) {
    assert( QThread::currentThread() == this->thread() );

    std::cout << "[FormStreamViews] FormStreamViews::onNewStreamer '" << streamName << "'"
              << std::endl;
    assert( m_streamViews.find( streamName ) == m_streamViews.end() );

    auto* sensorView = new FormStreamView( streamName, sensorSpec, m_sensorModel, nullptr );
    ui->verticalLayout->insertWidget( static_cast<int>( m_streamViews.size() ), sensorView );

    m_streamViews[streamName] = sensorView;
    auto stringList           = m_sensorModel.stringList();
    stringList.append( streamName.c_str() );
    m_sensorModel.setStringList( stringList );

#ifndef USE_COMPLETE_VIEWER
    QObject::connect(
        sensorView, &FormStreamView::streamingStarted, this, &FormStreamViews::streamingStarted );
    QObject::connect(
        sensorView, &FormStreamView::streamingStopped, this, &FormStreamViews::streamingStopped );
    if ( m_autoStartStream ) { sensorView->on_startStreaming(); }
#endif

    emit streamingStarted(streamName, sensorSpec);

    std::cout << "[FormStreamViews] FormStreamViews::onNewStreamer '" << streamName << "' done."
              << std::endl;
    return true;
}

void FormStreamViews::onDelStreamer( const std::string& streamName,
                                     const hub::SensorSpec& sensorSpec ) {
    assert( QThread::currentThread() == this->thread() );
    std::cout << "[FormStreamViews] FormStreamViews::onDelStreamer '" << streamName << "'"
              << std::endl;

    assert( m_streamViews.find( streamName ) != m_streamViews.end() );
    auto* sensorView = m_streamViews.at( streamName );
    m_streamViews.erase( streamName );
    auto stringList = m_sensorModel.stringList();
    stringList.removeAll( streamName.c_str() );
    m_sensorModel.setStringList( stringList );
    delete sensorView;

//#ifndef USE_COMPLETE_VIEWER
    emit streamingStopped( streamName, sensorSpec );
//#endif

    std::cout << "[FormStreamViews] FormStreamViews::onDelStreamer end '" << streamName << "'"
              << std::endl;
}

void FormStreamViews::onServerConnected( const std::string& ipv4, int port ) {
    assert( QThread::currentThread() == this->thread() );
    std::cout << "[FormStreamViews] connected to server : " << ipv4 << " " << port << std::endl;
    m_serverConnected = true;
    ui->lineEdit_ip->setEnabled( false );
    ui->spinBox_port->setEnabled( false );
    m_serverPing = true;

    ui->verticalLayout->addWidget( new QLabel( "hello" ) );
    ui->verticalLayout->update();

//    emit serverConnected();
}

void FormStreamViews::onServerDisconnected( const std::string& ipv4, int port ) {
    assert( QThread::currentThread() == this->thread() );
    std::cout << "[FormStreamViews] disconnected from server : " << ipv4 << " " << port
              << std::endl;
    m_serverConnected = false;
    ui->lineEdit_ip->setEnabled( true );
    ui->spinBox_port->setEnabled( true );
    m_serverPing = true;

#ifdef OS_LINUX
    ++m_port;
#endif
    ui->spinBox_port->setValue( m_port );

//    emit serverDisconnected();
}

// void FormStreamViews::onNewAcquisition( const std::string& streamName, const hub::Acquisition&
// acq ) {
////    std::cout << "[FormStreamViews] FormStreamViews::onNewAcquisition '" << streamName
////              << "' : " << acq << std::endl;
//    emit newAcquisition(streamName, acq);
//}

const std::string& FormStreamViews::getIpv4() const {
    return m_ipv4;
}

const int& FormStreamViews::getPort() const {
    return m_port;
}

// const FormStreamView& FormStreamViews::getStreamView( const std::string& streamName ) const {
//     assert( m_streamViews.find( streamName ) != m_streamViews.end() );
//     return *m_streamViews.at( streamName );
// }

void FormStreamViews::on_lineEdit_ip_textChanged( const QString& ipv4 ) {
    if ( m_viewer != nullptr ) {
        m_ipv4 = ipv4.toStdString();

        if ( m_ipv4RegularExpression.match( m_ipv4.c_str() ).hasMatch() ) {
            m_viewer->setIpv4( m_ipv4 );
            ui->lineEdit_ip->setStyleSheet( "background-color: none" );
        }
        else { ui->lineEdit_ip->setStyleSheet( "background-color: red" ); }
    }
}

void FormStreamViews::on_spinBox_port_valueChanged( int port ) {
    if ( m_viewer != nullptr ) {
        m_port = port;
        m_viewer->setPort( m_port );
    }
}
