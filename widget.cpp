#include "widget.h"
#include "ui_widget.h"

#include "app/user.h"
#include "registerform.h"
#include "appform.h"
#include "serveripform.h"
#include "server.h"
#include "client.h"

using namespace app;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    m_client = nullptr;
    m_server = nullptr;
    setAccessibleName("MainWidget");
    ui->setupUi(this);
    QPalette pal;
    pal.setBrush(this->backgroundRole(), QBrush(QPixmap(":/img/image.jpg")));
    this->setPalette(pal);
    this->setWindowTitle(QApplication::applicationName());
    this->refreshName();

    User user = User::instance();
    User::Mode mode = user.getLastMode();

    this->show();
    if (!user.checkName()) {
        qDebug() << "herak1";
        on_changeNameButton_clicked();
    } //
//    else if (mode == User::Mode::Server) {
//        on_serverButton_clicked();
////        m_server = new Server(Server::serverPort);
////        m_client = new Client("localhost", Server::serverPort);
////        m_server->show();
////        m_client->show();
////        this->hide();

//        qDebug() << "herak2";
//    } else if (mode == User::Mode::Client && !user.getLastIp().isEmpty()) {
//        on_clie
////        m_client = new Client(user.getLastIp(), Server::serverPort);
////        m_client->show();
//        qDebug() << "herak3";
////        this->hide();
//    }
}

Widget::~Widget()
{
    delete ui;
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

    m_client->setWindowTitle("ServeClient");   // delete this later

    m_server->show();
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
    qDebug() << "slotChangeName";

    User* user = &User::instance();

    if (user->checkName()) {
        m_regForm->deleteLater();
        this->refreshName();
        this->show();
    } else {
        on_changeNameButton_clicked();
    }
}

void Widget::refreshName()
{
    User* user = &User::instance();
    if (user->checkName()) {
        ui->nickName->setText(user->getName());
        this->hide();
        this->show();
    }
}

bool Widget::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {
        qApp->quit();
        return true;      // :D
    }
    return false;
}




