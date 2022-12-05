#pragma once

#include <GuiManager.h>

//#include <MinimalApp.hpp>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class GuiMainWindow;
}
QT_END_NAMESPACE

class GuiMainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    GuiMainWindow( Ra::Engine::RadiumEngine* engine,
                   Ra::Gui::Viewer* viewer,
                   QWidget* parent = nullptr );
    ~GuiMainWindow();

  private slots:

  private:
    Ui::GuiMainWindow* ui;

    GuiManager m_guiManager;
};
