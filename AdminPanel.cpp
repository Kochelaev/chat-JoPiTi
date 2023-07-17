#include "AdminPanel.h"

AdminPanel::AdminPanel(QWidget *parent) : QWidget(parent)
{
    m_sendOlegButton = new QPushButton("Отправить Олега");

    QVBoxLayout layout;
    layout.addWidget(m_sendOlegButton);
    setLayout(&layout);

    resize(200, 100);
}

