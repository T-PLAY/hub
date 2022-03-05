#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MainWindowStreamView.h>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QThread>
#include <map>
#include <formsensorview.h>

// Q_DECLARE_METATYPE(Streamer);
// Q_DECLARE_METATYPE(std::string)

namespace Ui {
class MainWindow;
}

class Thread_Client : public QThread {
    Q_OBJECT
public:
    // constructor
    explicit Thread_Client(QObject* parent = 0);

signals:
    void addSensorSignal(std::string streamerSensorName, std::string format, std::string dims, std::string size);
//    void addViewStreamSignal(std::string streamerSensorName);
//    void delViewStreamSignal(std::string streamerSensorName);
    void delSensorSignal(std::string streamerSensorName);

public:
    // overriding the QThread's run() method
    void run();

private:
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:

public slots:
    void addSensor(std::string streamerSensorName, std::string format, std::string dims, std::string size);
    void delSensor(std::string streamerSensorName);

    void addStreamView(std::string streamerSensorName);
    void delStreamView(std::string streamerSensorName);

    void onCloseStreamView(std::string streamerSensorName);

private:
    Ui::MainWindow* ui;
    Thread_Client mThreadClient;
//    std::list<MainWindowStreamView*> mStreamViews;
    std::map<std::string, MainWindowStreamView*> mStreamViews;
    std::map<std::string, FormSensorView*> mSensorViews;
};

#endif // MAINWINDOW_H
