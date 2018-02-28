#include "myserver.h"
#include "mythread.h"
#include <QtWebSockets/QWebSocketServer>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>


//QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
//QT_FORWARD_DECLARE_CLASS(QWebSocket)


MyServer::MyServer( QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(Q_NULLPTR)
{

}


void MyServer::startServer()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,//unsecure mode
                                              this);


    //==========

    QFile certFile(QStringLiteral(":/server.crt"));
    QFile keyFile(QStringLiteral(":/server.key"));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);

    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);

    //certificate.importPkcs12();
    qWarning()<<"is self signed: "<<certificate.issuerInfoAttributes();

    certFile.close();
    keyFile.close();

    QSslConfiguration sslConfiguration;
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);

    m_pWebSocketServer->setSslConfiguration(sslConfiguration);

    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "SSL Echo Server listening on port" << port;
        //connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
          //      this, &SslEchoServer::onNewConnection);
        //connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
       //         this, &SslEchoServer::onSslErrors);
        connect(m_pWebSocketServer,SIGNAL(newConnection()),this,SLOT(incomingConnection()));


    }
    //=====

   // QSslConfiguration sslConfiguration;
   // QFile certFile(QStringLiteral(":/localhost.cert"));
  //  QFile keyFile(QStringLiteral(":/localhost.key"));
  //  certFile.open(QIODevice::ReadOnly);
   // keyFile.open(QIODevice::ReadOnly);
    //QSslCertificate certificate(&certFile, QSsl::Pem);
   // QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
  //  certFile.close();
 //   keyFile.close();

  //  sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
  //  sslConfiguration.setLocalCertificate(certificate);
 //   sslConfiguration.setPrivateKey(sslKey);
 //   sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
   // m_pWebSocketServer->setSslConfiguration(sslConfiguration);


   // if(!m_pWebSocketServer->listen(QHostAddress::Any, port))
 //   {
 //       qDebug() << "Could not start server";
 //   }
 //   else
  //  {
   //     qDebug() << "SERVER STARTED Listening ON port: " << port << ".";
       ///  connect(m_pWebSocketServer,SIGNAL(newConnection()),this,SLOT(incomingConnection()));

         //connect(m_pWebSocketServer,SIGNAL(),this,SLOT(incomingConnection()));

    }
//}//  Starts the Server


// This function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection()
{
    // We have a new connection

    qSocket = m_pWebSocketServer->nextPendingConnection();
    qWarning() << qSocket << " Connecting...";


    // Every new connection will be run in a newly created thread
    MyThread *thread = new MyThread(qSocket, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
/*{

    qWarning()<<"value of socket: "<<pSocket;


    MyThread *thread = new MyThread(this);
    pSocket->moveToThread(thread);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
*/


/*

// This function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " IN INCOMING FUNCTION Connecting.";

    // Every new connection will be run in a newly created thread
    MyThread *thread = new MyThread(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


*/
