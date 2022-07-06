#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MinimalApp.hpp>
#include <QMainWindow>

#include <GuiManager.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    MinimalApp* m_app = nullptr;

    GuiManager m_guiManager;

};
#endif // MAINWINDOW_H
