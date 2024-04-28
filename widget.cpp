#include "widget.h"
//#include "ui_widget.h"

#include "app/user.h"
#include "registerform.h"
#include "appform.h"
#include "serveripform.h"
#include "server.h"
#include "client.h"

using namespace app;

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    m_client = nullptr;
    m_server = nullptr;
    setAccessibleName("MainWidget");

    this->boostUI();
    this->setWindowFlags(
            Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint
        );


    this->setWindowTitle(QApplication::applicationName());
    this->refreshName();

    User user = User::instance();
    User::Mode mode = user.getLastMode();

    if (!user.checkName()) {
        on_changeNameButton_clicked();
    } //
    else if (mode == User::Mode::Server) {
        m_server = new Server(Server::serverPort);
        m_client = new Client("localhost", Server::serverPort);
//        m_server->show();
        m_client->show();
        this->hide();

    } else if (mode == User::Mode::Client && !user.getLastIp().isEmpty()) {
        m_client = new Client(user.getLastIp(), Server::serverPort);
        m_client->show();
        this->hide();
    }
}

Widget::~Widget()
{
    if (m_client != nullptr) {
        delete m_client;
    }
    if (m_server != nullptr) {
        delete m_server;
    }
}

void Widget::on_ClientButton_clicked()
{
    m_ipForm = new ServerIpForm();
    m_ipForm->show();
    connect(m_ipForm, SIGNAL(signalChangeIp(QString)), this, SLOT(slotClientChangeIp(QString)));
    this->hide();
    User::instance().setLastMode(User::Mode::Client);
}

void Widget::on_serverButton_clicked()
{
    m_server = new Server(Server::serverPort);
    m_client = new Client("localhost", Server::serverPort);

//    m_client->setWindowTitle("ServeClient");   // delete this later
//    m_server->show();
    m_client->show();

    this->hide();

    User::instance().setLastMode(User::Mode::Server);
}

void Widget::on_changeNameButton_clicked()
{
    m_regForm = new RegisterForm();
    connect(m_regForm, SIGNAL(signalChangeName()), this, SLOT(slotChangeName()));
    this->hide();
    m_regForm->show();
}

void Widget::slotClientChangeIp(const QString &ip)
{
    m_client = new Client(ip, Server::serverPort);
    m_ipForm->deleteLater();
    m_client->show();
    User::instance().setLastIp(ip);
}

void Widget::slotChangeName()
{
    User* user = &User::instance();

    if (user->checkName()) {
        m_regForm->deleteLater();
        this->refreshName();
        this->show();
    } else {
        on_changeNameButton_clicked();
    }
}

void Widget::slotChangeSide()
{
    static bool isDarkSide = User::instance().isDarkSide();
    QString cssPath;
    if (isDarkSide) {
        User::instance().setDarkSide(false);
        isDarkSide = false;
        m_darkSideButton->setText("Перейти на темную сторону");
        cssPath = ":/css/style.css";
    } else  {
        User::instance().setDarkSide(true);
        isDarkSide = true;
        m_darkSideButton->setText("Перейти на светлую сторону");
        cssPath = ":/css/darkSide.css";
    }
    QFile styleFile(cssPath);
    styleFile.open(QFile::ReadOnly);
    qApp->setStyleSheet(QLatin1String(styleFile.readAll()));
}

void Widget::refreshName()
{
    User* user = &User::instance();
    if (user->checkName()) {
        m_nameLabel->setText(user->getName());
        this->hide();
        this->show();
    }
}

bool Widget::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {
        qApp->quit();
        return false;      // :D
    }
    return QWidget::event(event);
}

void Widget::boostUI()
{
    m_clientButton = new QPushButton("Клиент");
    m_serverButton = new QPushButton("Сервер");
    m_changeNameButton = new QPushButton ("Сменить никнейм");
    m_darkSideButton = User::instance().isDarkSide() ?
        new QPushButton ("Перейти на светлую сторону") :
        new QPushButton ("Перейти на темную сторону");

    m_nameTitle = new QLabel("<b>Ваш никнейм: </b>");
    m_nameLabel = new QLabel(User::instance().getName());

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_clientButton);
    layout->addWidget(m_serverButton);
    layout->addWidget(m_changeNameButton);
    layout->addWidget(m_darkSideButton);
    layout->addSpacing(30);
    layout->addWidget(m_nameTitle);
    layout->addWidget(m_nameLabel);

    this->setLayout(layout);

    connect(m_clientButton, SIGNAL(clicked()), this, SLOT(on_ClientButton_clicked()));
    connect(m_serverButton, SIGNAL(clicked()), this, SLOT(on_serverButton_clicked()));
    connect(m_changeNameButton, SIGNAL(clicked()), this, SLOT(on_changeNameButton_clicked()));
    connect(m_darkSideButton, SIGNAL(clicked()), this, SLOT(slotChangeSide()));

}
