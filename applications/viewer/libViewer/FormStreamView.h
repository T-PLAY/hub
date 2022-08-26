#ifndef FORMSENSORVIEW_H
#define FORMSENSORVIEW_H

#include <QWidget>
#include <QThread>
#include <MainWindowStreamView.h>
#include <QMdiArea>

#include <QSortFilterProxyModel>
//#include <FormInputStreamViews.h>

//class Thread_InputStream : public QThread
//{
//    Q_OBJECT
//  public:
//    // constructor
//    explicit Thread_InputStream( std::string sensorName = "", QObject* parent = nullptr );
//    ~Thread_InputStream();

//  signals:
//    void newAcquisition();
////    void streamingStopped(std::string sensorName);

//  public:
//    // overriding the QThread's run() method
//    void run();

//    Stream::Acquisition mAcq;
//    InputStream mInputStream;

//  private:
//};

#include <QStringListModel>

namespace Ui {
class FormStreamView;
}

class FormStreamView : public QWidget {
    Q_OBJECT

public:
    FormStreamView(std::string streamName, const hub::SensorSpec & sensorSpec, QStringListModel & sensorModel, QWidget * parent = nullptr);
//    FormStreamView(const std::string streamName, QWidget* parent = nullptr);
    ~FormStreamView();

    void setRadioButtonOff();

signals:
//    void addViewStreamSignal(std::string streamerSensorName);
//    void delViewStreamSignal(std::string streamerSensorName);
//    void newAcquisition();
    void streamingStarted(const std::string & streamName, const std::string & syncSensorName);
    void streamingStopped(const std::string & streamName, const hub::SensorSpec & sensorSpec);

public slots:
    void on_startStreaming();
    void on_radioButtonOnOff_clicked(bool checked);

private slots:
//    void on_startStreamingPrivate();
//    void on_stopStreaming();
//    void on_closeStreamView();

//    void on_newAcquisition();

//    void on_comboBox_syncSensor_currentTextChanged(const QString &arg1);

public:
//    const InputStream & getInputStream() const;

//    const Thread_InputStream *getInputStreamThread() const;

private:
    Ui::FormStreamView* ui;
    const std::string m_streamName;
    const hub::SensorSpec m_sensorSpec;
    QStringListModel & mSensorModel;
    QSortFilterProxyModel mProxySensorModel;
//    QMdiArea & m_mdiArea;

//    Thread_InputStream * m_inputStreamThread = nullptr;
//    MainWindowStreamView* m_streamView = nullptr;
//    QWidget * m_streamView = nullptr;
//    int mCounterFps;
//    double mFps;
//    std::chrono::time_point<std::chrono::high_resolution_clock> mStartFps;


};

#endif // FORMSENSORVIEW_H
