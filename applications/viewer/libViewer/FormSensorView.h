#ifndef FORMSENSORVIEW_H
#define FORMSENSORVIEW_H

#include <QWidget>
#include <QThread>
#include <MainWindowStreamView.h>
#include <QMdiArea>

class Thread_InputStream : public QThread
{
    Q_OBJECT
  public:
    // constructor
    explicit Thread_InputStream( std::string sensorName = "", QObject* parent = nullptr );
    ~Thread_InputStream();

  signals:
    void newImage();

  public:
    // overriding the QThread's run() method
    void run();

    Stream::Acquisition mAcq;

    InputStream mInputStream;

  private:
};


namespace Ui {
class FormSensorView;
}

class FormSensorView : public QWidget {
    Q_OBJECT

public:
    FormSensorView(std::string sensorName, std::string format, std::string dims, std::string size, std::string metaData, QMdiArea & mdiArea, QWidget * parent = nullptr);
//    FormSensorView(const std::string sensorName, QWidget* parent = nullptr);
    ~FormSensorView();

    void setRadioButtonOff();

signals:
//    void addViewStreamSignal(std::string streamerSensorName);
//    void delViewStreamSignal(std::string streamerSensorName);

private slots:
    void on_radioButtonOnOff_clicked(bool checked);
    void startStreaming();
    void stopStreaming();
    void onCloseStreamView();

    void newImage();

private:
    Ui::FormSensorView* ui;
    const std::string mSensorName;
    QMdiArea & m_mdiArea;

    Thread_InputStream * m_inputStreamThread = nullptr;
    MainWindowStreamView* m_streamView = nullptr;
//    QWidget * m_streamView = nullptr;
    int mCounterFps;
    double mFps;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartFps;


};

#endif // FORMSENSORVIEW_H
