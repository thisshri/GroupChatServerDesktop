#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

class MyServer : public QObject
{

Q_OBJECT

public:
    MyServer(QObject *parent = Q_NULLPTR);
    void startServer();

signals:

public slots:
    void incomingConnection();

protected:

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    QWebSocket * qSocket;
    quint16 port = 1234;
};

#endif
