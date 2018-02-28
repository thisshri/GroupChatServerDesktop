#include "serverwindow.h"
#include "ui_serverwindow.h"

#include <QApplication>
#include "myserver.h"

#include <QDir>
#include <QAction>
#include "ui_changeadminpassword.h"
#include "changeadminpassword.h"

serverWindow::serverWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serverWindow)
{
    ui->setupUi(this);
    ui->pbStop->setDisabled(1);
    //TcpServer = new MyServer(this);
    //webSockServer = new MyServer(1234,parent);
    WebSockServer = new MyServer(this);

    //ui->actionChangeAdminPass;

    connect(ui->actionChangeAdminPass, SIGNAL(triggered(bool)), this, SLOT(openPassChangeWindow()));

}

void serverWindow::openPassChangeWindow(){
    ChangeAdminPassword * changeAdminPassword = new ChangeAdminPassword(this);
    changeAdminPassword->show();
}

serverWindow::~serverWindow()
{
    delete ui;
}


void serverWindow::on_pbStart_clicked()
{
    ui->pbStart->setDisabled(1);
    ui->pbStop->setEnabled(1);

    WebSockServer->startServer();

}

void serverWindow::on_pbStop_clicked()
{
    ui->pbStop->setDisabled(1);
    ui->pbStart->setEnabled(1);


    // need to write this function in server.cpp
    //WebSockServer->close()
}

//---------------------MAIN------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serverWindow w;
    w.show();

    return a.exec();
}


