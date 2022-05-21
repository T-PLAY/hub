#include "FormSensorViews.h"

#include "ui_FormSensorViews.h"

#include <FormSensorView.h>
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

Thread_Client::Thread_Client(const FormSensorViews& formSensorViews, QObject* parent)
    : QThread(parent)
    //    , m_dialog(dialog)
    , m_formSensorViews(formSensorViews)
{
    std::cout << "[Thread_Client] Thread_Client()" << std::endl;
}

void Thread_Client::run()
{
    std::cout << "[Thread_Client] Thread_Client::run()" << std::endl;

    while (!this->isInterruptionRequested()) {
        try {
            ClientSocket sock(m_formSensorViews.ui->lineEdit_ip->text().toStdString(), m_formSensorViews.ui->spinBox_port->value());
            sock.write(ClientSocket::Type::VIEWER);
            emit serverConnected();
            //            m_dialog.hide();

            while (!this->isInterruptionRequested()) {

                Socket::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {

                case Socket::Message::PING: {
                    // server check client connection
                    // nothing to do
                } break;

                case Socket::Message::NEW_STREAMER: {
                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
                    std::string sensorName;
                    sock.read(sensorName);
                    std::string format;
                    sock.read(format);
                    std::string dims;
                    sock.read(dims);
                    std::string size;
                    sock.read(size);

                    Stream::MetaData metaData;
                    sock.read(metaData);

                    std::cout << "[Thread_Client] [viewer] new streamer " << sensorName
                              << ", format:" << format << ", dims:" << dims
                              << ", acquisitionSize:" << size << std::endl;
                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '" << sensorName
                              << "'" << std::endl;
                    std::cout << "[Thread_Client] [viewer] metadata : "
                              << Stream::to_string(metaData, true);
                    emit addSensorSignal(
                        sensorName, format, dims, size, Stream::to_string(metaData, true));

                    //                    InputStream inputStream(sensorName.c_str(), "");

                } break;

                case Socket::Message::DEL_STREAMER: {
                    std::string sensorName;
                    sock.read(sensorName);
                    std::cout << "[Thread_Client] [viewer] del streamer '" << sensorName << "'"
                              << std::endl;
                    emit delSensorSignal(sensorName);
                } break;

                default:
                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
                }
            }
        } catch (std::exception& e) {
            emit serverDisconnected();
            //            m_dialog.show();
            std::cout << "[Thread_Client] [viewer] no server, catch exception : " << e.what()
                      << std::endl;
        }
    } // while (!this->isInterruptionRequested())
}

///////////////////////////////////////////////////////////////////////////////////////

FormSensorViews::FormSensorViews(
    //        QBoxLayout& vboxLayout,
    //    QMdiArea& mdiArea,
    //    QMainWindow& mainWindow,
    QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormSensorViews)
    , mThreadClient(*this, this)
//    , m_dialog(&mainWindow)
//    , m_dialog()
//    , mThreadClient(m_dialog, this)
//    , m_vBoxLayout(vboxLayout)
//    , m_mdiArea(mdiArea)
//    , m_mainWindow(mainWindow)
{
    ui->setupUi(this);
    //    mainWindow.setAttribute(Qt::WA_DeleteOnClose); // for dialog window

    ui->lineEdit_ip->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}")));
    ui->lineEdit_ip->setText(SERVICE_IP);
    ui->spinBox_port->setValue(SERVICE_PORT);

    QObject::connect(
        &mThreadClient, &Thread_Client::addSensorSignal, this, &FormSensorViews::addSensor);
    QObject::connect(
        &mThreadClient, &Thread_Client::delSensorSignal, this, &FormSensorViews::delSensor);

    QObject::connect(&mThreadClient, &Thread_Client::serverConnected, this, &FormSensorViews::onServerConnect);
    QObject::connect(&mThreadClient, &Thread_Client::serverDisconnected, this, &FormSensorViews::onServerDisconnect);

    //    QObject::connect(QApplication::instance(), &QApplication::aboutToQuit, this, &FormSensorViews::onQuitApp);

    //    QObject::connect(&mainWindow, &QMainWindow::c)
    //    QObject::connect( &mThreadClient, &Thread_Client::serverConnected, this, [this]() {
    //        m_mainWindow.setEnabled( true );
    //    } );
    //    QObject::connect( &mThreadClient, &Thread_Client::serverDisconnected, this, [this]() {
    //        m_mainWindow.setEnabled( false );
    //    } );
    //    m_mainWindow.setEnabled(false);
    mThreadClient.start();

    m_sensorModel.setStringList(QStringList("none"));
    //    m_mainWindow.setEnabled(false);
}

FormSensorViews::~FormSensorViews()
{
    std::cout << "[FormSensorViews] ~FormSensorViews() start" << std::endl;

    //    m_mdiArea = nullptr;

    mThreadClient.requestInterruption();
    mThreadClient.wait();

    std::cout << "[FormSensorViews] ~FormSensorViews() mThreadClient.terminated()" << std::endl;

    //    m_dialog.close();

    //    for ( auto& pair : mStreamViews ) {
    //        auto* streamView = pair.second;

    //        assert( streamView != nullptr );
    //        std::cout << "[FormSensorViews] ~FormSensorViews() delete " << streamView << std::endl;
    //        delete streamView;
    //        streamView = nullptr;
    //    }
    //    mStreamViews.clear();
    for (auto& pair : m_sensorViews) {
        auto* sensorView = pair.second;
        delete sensorView;
    }

    delete ui;
    std::cout << "[FormSensorViews] ~FormSensorViews() end" << std::endl;
}

void FormSensorViews::onServerConnect()
{
    //    m_mainWindow.setEnabled(true);
    //    m_dialog.hide();
    m_serverConnected = true;
    ui->lineEdit_ip->setEnabled(false);
    ui->spinBox_port->setEnabled(false);
}

void FormSensorViews::onServerDisconnect()
{
    //    m_mainWindow.setEnabled(false);
    //    m_dialog.setEnabled(true);
    //    m_dialog.show();
    m_serverConnected = false;
    ui->lineEdit_ip->setEnabled(true);
    ui->spinBox_port->setEnabled(true);

    //    m_dialog = new DialogServerConnect(&m_mainWindow);
}

void FormSensorViews::addSensor(std::string sensorName,
    std::string format,
    std::string dims,
    std::string size,
    std::string metaData)
{
    //    assert(m_mdiArea != nullptr);

    std::cout << "[FormSensorViews] FormSensorViews::addSensor '" << sensorName << "'" << std::endl;
    assert(m_sensorViews.find(sensorName) == m_sensorViews.end());

    auto* sensorView = new FormSensorView(sensorName, format, dims, size, metaData, m_sensorModel, nullptr);
    ui->verticalLayout->insertWidget(static_cast<int>(m_sensorViews.size()), sensorView);

    m_sensorViews[sensorName] = sensorView;
    auto stringList = m_sensorModel.stringList();
    stringList.append(sensorName.c_str());
    m_sensorModel.setStringList(stringList);

    //    m_sensorModel.in

    //    QObject::connect(
    //        sensorView, &FormSensorView::addViewStreamSignal, this, &FormSensorViews::addStreamView );
    //    QObject::connect(
    //            sensorView, &FormSensorView::delViewStreamSignal, this,
    //            &FormSensorViews::delStreamView );
    //    emit sensorAdded(sensorName);

    QObject::connect(
        sensorView, &FormSensorView::streamingStarted, this, &FormSensorViews::streamingStarted);
    QObject::connect(
        sensorView, &FormSensorView::streamingStopped, this, &FormSensorViews::streamingStopped);

    sensorView->on_startStreaming();
    //    sensorView->on_radioButtonOnOff_clicked(true);
}

void FormSensorViews::delSensor(std::string sensorName)
{
    std::cout << "[FormSensorViews] FormSensorViews::delSensor '" << sensorName << "'" << std::endl;

    //    delStreamView( sensorName );

    assert(m_sensorViews.find(sensorName) != m_sensorViews.end());
    auto* sensorView = m_sensorViews.at(sensorName);
    m_sensorViews.erase(sensorName);
    auto stringList = m_sensorModel.stringList();
    stringList.removeAll(sensorName.c_str());
    m_sensorModel.setStringList(stringList);
    delete sensorView;

    //    assert( mSensorViews.find( sensorName ) !=
    //            mSensorViews.end() ); // todo: fix one more time error
    //    FormSensorView* sensorView = mSensorViews.at( sensorName );
    //    delete sensorView;
    //    mSensorViews.erase( sensorName );

    std::cout << "[FormSensorViews] FormSensorViews::delSensor end '" << sensorName << "'" << std::endl;

    //    emit sensorDeleted(sensorName);
}

// void FormSensorViews::onQuitApp()
//{
//     std::cout << "[FormSensorViews] onQuitApp" << std::endl;
// }

const FormSensorView& FormSensorViews::getSensorView(const std::string& sensorName) const
{
    assert(m_sensorViews.find(sensorName) != m_sensorViews.end());
    return *m_sensorViews.at(sensorName);
}

// void FormSensorViews::setMdiArea(QMdiArea *newMdiArea)
//{
//     m_mdiArea = newMdiArea;
// }
