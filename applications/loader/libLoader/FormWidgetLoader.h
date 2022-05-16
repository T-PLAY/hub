#ifndef FORMWIDGETLOADER_H
#define FORMWIDGETLOADER_H

#include <QWidget>

namespace Ui {
class FormWidgetLoader;
}

class FormWidgetLoader : public QWidget
{
    Q_OBJECT

public:
    explicit FormWidgetLoader(QWidget *parent = nullptr);
    ~FormWidgetLoader();

private:
    Ui::FormWidgetLoader *ui;
};

#endif // FORMWIDGETLOADER_H
