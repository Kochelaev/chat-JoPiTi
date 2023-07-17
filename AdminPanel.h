#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QtWidgets>

class AdminPanel : public QWidget
{
    Q_OBJECT
public:
    explicit AdminPanel(QWidget *parent = nullptr);

private:
    QPushButton* m_sendOlegButton;

public:
    QPushButton* getOlegButtonPointer() {return this->m_sendOlegButton;}

};

#endif // ADMINPANEL_H
