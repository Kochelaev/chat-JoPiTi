#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serveripform.h"
#include "registerform.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_ClientButton_clicked();

    void on_serverButton_clicked();

    void on_changeNameButton_clicked();

private:
    Ui::Widget *ui;

public:
    void refreshName();
};

#endif // WIDGET_H
