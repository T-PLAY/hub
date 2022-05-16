#include "FormWidgetLoader.h"
#include "ui_FormWidgetLoader.h"

FormWidgetLoader::FormWidgetLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWidgetLoader)
{
    ui->setupUi(this);
}

FormWidgetLoader::~FormWidgetLoader()
{
    delete ui;
}
