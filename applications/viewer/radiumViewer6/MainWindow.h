#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MinimalApp.hpp>
#include <MinimalComponent.hpp>
#include <QMainWindow>

#include <SensorViews.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

  public slots:

  public:
private slots:
    void on_action2D_triggered();

    void on_action3D_triggered();

private:
    Ui::MainWindow* ui;

    MinimalApp* m_app        = nullptr;
    MinimalComponent* m_comp = nullptr;

    SensorViews * m_sensorViews = nullptr;

};
#endif // MAINWINDOW_H
