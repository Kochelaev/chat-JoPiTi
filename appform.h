#ifndef APPFORM_H
#define APPFORM_H

#include <QWidget>

namespace Ui {
class AppForm;
}

class AppForm : public QWidget
{
    Q_OBJECT

public:
    explicit AppForm(QWidget *parent = 0);
    ~AppForm();

private:
    Ui::AppForm *ui;
};

#endif // APPFORM_H
