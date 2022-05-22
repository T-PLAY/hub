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
class FormSensorView;
}

class FormSensorView : public QWidget {
    Q_OBJECT

public:
    FormSensorView(std::string sensorName, std::string format, std::string dims, std::string size, std::string metaData, QStringListModel & sensorModel, QWidget * parent = nullptr);
//    FormSensorView(const std::string sensorName, QWidget* parent = nullptr);
    ~FormSensorView();

    void setRadioButtonOff();

signals:
//    void addViewStreamSignal(std::string streamerSensorName);
//    void delViewStreamSignal(std::string streamerSensorName);
//    void newAcquisition();
    void streamingStarted(const std::string & sensorName, const std::string & syncSensorName);
    void streamingStopped(const std::string & sensorName);

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
    Ui::FormSensorView* ui;
    const std::string mSensorName;
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
