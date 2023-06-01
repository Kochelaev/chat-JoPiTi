#include "appform.h"
#include "ui_appform.h"

AppForm::AppForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppForm)
{
    ui->setupUi(this);
}

AppForm::~AppForm()
{
    delete ui;
}
