#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

//#include <QTcpSocket>

#include <QWebSocket>
#include <QDebug>
#include <QVector>
#include <QFile>
#include "database.h"

static QVector<QWebSocket*> vSocket;


class MyThread : public QThread
{
    Q_OBJECT
public:

    explicit MyThread(QWebSocket *qSocket, QObject *parent = 0);
    void sendToGroup(QString data );
    void incomingMessage(QString Data);
    void checkForLogin(QString logInMessage, QString strReqNo);
    void checkForLogin(QString logInMessage);

    void checkAdminLogin (QString logInMessage);
    QVector<QString> commaEndedStr(QString Str);

    void run();

signals:
    //void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead(QString message);
    void disconnected();

private:
    QWebSocket *socket;
    //QTcpSocket *socket;
    //qintptr socketDescriptor;
    QByteArray Data;
    DataBase db;
};

#endif // MYTHREAD_H
