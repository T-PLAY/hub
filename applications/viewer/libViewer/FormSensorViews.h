#ifndef FORMSENSORVIEWS_H
#define FORMSENSORVIEWS_H

#include <QThread>
#include <QWidget>

#include <FormSensorView.h>
#include <MainWindowStreamView.h>
#include <QComboBox>
#include <QMdiArea>
#include <QVBoxLayout>
#include <SensorView.h>
#include <map>
#include <stream.h>

//#include <DialogServerConnect.h>

class FormSensorViews;

class Thread_Client : public QThread {
    Q_OBJECT
public:
    // constructor
    explicit Thread_Client(const FormSensorViews &formSensorViews, QObject* parent = 0);

signals:
    void serverConnected();
    void serverDisconnected();
    void addSensorSignal(std::string sensorName,
        std::string format,
        std::string dims,
        std::string size,
        std::string metaData);
    void delSensorSignal(std::string sensorName);

public:
    // overriding the QThread's run() method
    void run();

//    DialogServerConnect& m_dialog;
    const FormSensorViews & m_formSensorViews;

private:
};

namespace Ui {
class FormSensorViews;
}

class FormSensorViews : public QWidget {
    Q_OBJECT

public:
//    FormSensorViews(QBoxLayout& vboxLayout, QMdiArea& mdiArea, QMainWindow& mainWindow, QWidget* parent = nullptr);
    explicit FormSensorViews(QWidget * parent = nullptr);
    ~FormSensorViews();


signals:
    //    void serverConnected();
    //    void serverDisconnected();
    void streamingStarted(std::string sensorName);
    void streamingStopped(std::string sensorName);
    //    void startStreaming();
    //    void stopStreaming();
    //    void sensorAdded(const std::string & sensorName);
    //    void sensorDeleted(const std::string & sensorName);

public slots:
    void onServerConnect();
    void onServerDisconnect();
    void addSensor(std::string sensorName,
        std::string format,
        std::string dims,
        std::string size,
        std::string metaData);
    void delSensor(std::string sensorName);
    //    void onQuitApp();


private:
    Ui::FormSensorViews* ui;

//    DialogServerConnect m_dialog;

    friend class Thread_Client;
    Thread_Client mThreadClient;

    //    Thread_InputStream mThread;
    //    std::map<std::string, SensorView*> m_sensorViews;
    //    std::map<std::string, MainWindowStreamView*> mStreamViews;
    std::map<std::string, FormSensorView*> m_sensorViews;

//    QBoxLayout& m_vBoxLayout;
//    QMdiArea * m_mdiArea = nullptr;
//    QMainWindow& m_mainWindow;

    bool m_serverConnected = false;
    //    DialogServerConnect & m_dialog;
    //    QComboBox & m_comboBoxScan;
    //    QComboBox & m_comboBoxPose;

public:
    const FormSensorView& getSensorView(const std::string& sensorName) const;
//    void setMdiArea(QMdiArea *newMdiArea);

};

#endif // FORMSENSORVIEWS_H
