#include "FormServerView.h"
#include "ui_FormServerView.h"

#include <QThread>

FormServerView::FormServerView(
    std::function<bool( const std::string& streamName, const hub::SensorSpec& )> onNewStreamer,
    std::function<void( const std::string& streamName, const hub::SensorSpec& )> onDelStreamer,
    std::function<void( const std::string& ipv4, int port )> onServerConnected,
    std::function<void( const std::string& ipv4, int port )> onServerDisconnected,
    std::function<void( const std::string& streamName, const hub::Acquisition& )> onNewAcquisition,
    bool autoSync,
    QWidget* parent ) :

    QWidget( parent ), ui( new Ui::FormServerView ) {

    const auto& ipv4 = hub::net::s_defaultServiceIp;
    const auto& port = hub::net::s_defaultServicePort;

    ui->setupUi( this );

    ui->lineEdit_ip->setValidator( new QRegularExpressionValidator( m_ipv4RegularExpression ) );

    assert( m_ipv4RegularExpression.match( ipv4.c_str() ).hasMatch() );
    ui->lineEdit_ip->setText( ipv4.c_str() );
    ui->spinBox_port->setValue( port );

    QObject::connect(
        this, &FormServerView::serverConnected, this, &FormServerView::onServerConnected );
    QObject::connect(
        this, &FormServerView::serverDisconnected, this, &FormServerView::onServerDisconnected );

    assert( m_viewer == nullptr );

    auto _onServerConnected = [=]( const std::string& ipv4, int port ) {
        emit serverConnected( ipv4, port );
        onServerConnected( ipv4, port );
    };
    auto _onServerDisconnected = [=]( const std::string& ipv4, int port ) {
        emit serverDisconnected( ipv4, port );
        onServerDisconnected( ipv4, port );
    };
    m_viewer = new hub::Viewer( onNewStreamer,
                                onDelStreamer,
                                _onServerConnected,
                                _onServerDisconnected,
                                onNewAcquisition,
                                ipv4,
                                port,
                                autoSync );
}

FormServerView::~FormServerView() {
    delete m_viewer;

    delete ui;
}

void FormServerView::onServerConnected( const std::string& ipv4, int port ) {
    assert( QThread::currentThread() == this->thread() );

    std::cout << "[FormServerView] connected to server : " << ipv4 << " " << port << std::endl;
    //    m_serverConnected = true;
    ui->lineEdit_ip->setEnabled( false );
    ui->spinBox_port->setEnabled( false );
    //    ui->widgetServerView->setEnable( false );
    //    m_serverPing = true;

    //    ui->verticalLayout->addWidget( new QLabel( "hello" ) );
    //    ui->verticalLayout->update();

    //    emit serverConnected();
}

void FormServerView::onServerDisconnected( const std::string& ipv4, int port ) {
    assert( QThread::currentThread() == this->thread() );

    std::cout << "[FormServerView] disconnected from server : " << ipv4 << " " << port << std::endl;
    //    m_serverConnected = false;
    ui->lineEdit_ip->setEnabled( true );
    ui->spinBox_port->setEnabled( true );
    //    ui->widget_serverView->setEnable( true );
    //    m_serverPing = true;

    //    #ifdef OS_LINUX
    //        ++m_port;
    //        ui->spinBox_port->setValue( m_port );
    //    //    ui->widgetServerView->setPort(ui->widgetServerView->getPort() + 1);
    //    #endif

    //    emit serverDisconnected();
}

void FormServerView::on_lineEdit_ip_textChanged( const QString& ipv4 ) {
    std::cout << "[FormServerView] on_lineEdit_ip_textChanged(" << ipv4.toStdString() << ")"
              << std::endl;

    if ( m_viewer != nullptr ) {
        auto ipv4Str = ipv4.toStdString();

        if ( m_ipv4RegularExpression.match( ipv4Str.c_str() ).hasMatch() ) {
            m_viewer->setIpv4( ipv4Str );
            ui->lineEdit_ip->setStyleSheet( "background-color: none" );
        }
        else { ui->lineEdit_ip->setStyleSheet( "background-color: red" ); }
    }
}

void FormServerView::on_spinBox_port_valueChanged( int port ) {
    std::cout << "[FormServerView] on_spinBox_port_valueChanged(" << port << ")" << std::endl;

    if ( m_viewer != nullptr ) {
        //        m_port = port;
        m_viewer->setPort( port );
    }
}

// void FormServerView::setPort( int newPort ) {
//     m_port = newPort;
// }

const int& FormServerView::getPort() const {
    assert( m_viewer != nullptr );
    return m_viewer->getPort();
}

const std::string& FormServerView::getIpv4() const {
    assert( m_viewer != nullptr );
    return m_viewer->getIpv4();
}

void FormServerView::setAutoSync(bool autoSync)
{
    m_viewer->setAutoSync(autoSync);
}
