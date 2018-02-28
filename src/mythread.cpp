#include "mythread.h"

/*
MyThread::MyThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;


    connect(socket,SIGNAL(textMessageReceived(QString)),this,SLOT(readyRead(QString)));


}
*/

MyThread::MyThread(QWebSocket *qSocket, QObject *parent):
    QThread(parent)
{
    this->socket = qSocket;
    qWarning()<<"IN thread constructor ";
}

void MyThread::run()
{    // thread starts here
    //-------socket = new QWebSocket();

    // set the ID
    //socket
/*
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }
*/
    vSocket.append(socket);
    qWarning()<<"NUMBER OF SOCKETS:"<<vSocket.length();
    //qWarning()<<"socket:\t"<<(QString)socket;
    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(readyRead(QString)),Qt::DirectConnection);
   // connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));


    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void MyThread::readyRead(QString message)
{
    qWarning()<<"Message received at server: "<<message;
    incomingMessage(message);
}// readyRead()

void MyThread::sendToGroup(QString data){
    qWarning()<<"Entered Sent to all+++";
    for (int i = 0; i < vSocket.length(); i++){
        //sending the Data to all connected users
        //qWarning()<<"writing on socket: "<<vSocket.at(i);
        vSocket.at(i)->setParent(0);

        //vSocket.at(i)->write(data);
        vSocket.at(i)->sendTextMessage(data);
        vSocket.at(i)->flush();
    }
    qWarning()<<"SEND TO GROUP CALLED : SIZE OF VECTOR:"<<vSocket.size();
}//send to group




void MyThread::incomingMessage(QString inMessage){
    //qWarning()<<"Message received from client"<<inMessage;

    qWarning()<<"in message at 0"<<inMessage.at(0);
    if (inMessage.at(0) == 'm'){                //removing m and send to group, it is a message.
            sendToGroup(inMessage);


            //write the message to chatlog.txt
            QFile file("/home/shri/Desktop/major/00 starting major with preivously working programs/DOC Server/chatlog/log.txt");
            if (file.open(QIODevice::WriteOnly | QIODevice::Append)){


                QByteArray dateInChatLog = QDateTime::currentDateTime().toString().toUtf8();
                file.write(dateInChatLog);
                //removing the "m" and writing the message
                //QByteArray dateTime = QDateTime::currentDateTime().toString().toUtf8();
                QByteArray bArray = inMessage.remove(0,1).toUtf8();
                file.write(bArray);
                file.close();

            }else{
                qWarning()<<"SERVER: FILE WRITE ERROR";

            }


            //####################

    }else if(inMessage.at(0) == 'l'){           /// INCOMING: LOGIN
        inMessage.remove(0,1);
        qWarning()<<"adfsssssssssssssssssssssssssssssssssssss"<<inMessage;
        //seperating login info and strRandom
        QVector<QString> vLoginInfo = commaEndedStr(inMessage);
        //QString username = vLoginInfo.at(0);
        //QString hashedPassword = vLoginInfo.at(1);

        QString strRequestNumber = vLoginInfo.at(2);

        qWarning()<<"adfsssssssssssssssssssssssssssssssssssss"<<inMessage;

        qWarning()<<"usename:\t"<<vLoginInfo.at(0);

        qWarning()<<"hashed Password\t"<<vLoginInfo.at(1);

        qWarning()<<"req no.\t"<<vLoginInfo.at(2);

       // qWarning()<<"adfsssssssssssssssssssssssssssssssssssss"<<inMessage;
        checkForLogin(inMessage,strRequestNumber);
       // qWarning()<<"adfsssssssssssssssssssssssssssssssssssss"<<inMessage;

    }else if(inMessage.at(0) == 'r' && inMessage.at(1) == 'r'){           // incoming message is user reg query "rusername,hashedpassword,etc..."
        inMessage.remove(0,2);
        db.registerUser(inMessage);
        qWarning()<<"SERVER RECEIVED:"<<inMessage;

        //user registration string and query
    }else if(inMessage.at(0) == 'r' && inMessage.at(1) == 'a'){         // in reg window is username avalible
        inMessage.remove(0,2);
        QString strRequestNum = inMessage.mid(0,8);
        inMessage.remove(0,8);

        qWarning()<<"REQ NO: \t"<<strRequestNum;
        qWarning()<<"inMessage username to be checked \t"<<inMessage;


        bool isNameAvalible = db.isUserNameAvalible(inMessage);
        if(isNameAvalible){
            socket->sendTextMessage("ra"+strRequestNum);
            socket->flush();
        }else{
            socket->sendTextMessage("rn"+strRequestNum);
            socket->flush();
        }


    }else if(inMessage.at(0) == 'f' && inMessage.at(1)=='q'){           //forget password
        //forgot password
        inMessage.remove(0,2);  //removie fq from the client message

        QVector<QString> vClientMessage = commaEndedStr(inMessage);
        QString username = vClientMessage.at(0);
        QString strRequestNumber = vClientMessage.at(1);

        qWarning()<<"STRING USENAME AT SERVER: \t "<<username;
        qWarning()<<"STRING REQUEST NUMBER AT SERVER: \t "<<strRequestNumber;


        QString recQuestion = db.getRecoveryQuestion(username);
        socket->sendTextMessage("fa"+strRequestNumber+","+recQuestion+",");
        socket->flush();


    }else if (inMessage.at(0) == 'f' && inMessage.at(1)=='c') {         //forgot password change password request send.
        inMessage.remove(0,2);

        QVector<QString> vTextFromClient = commaEndedStr(inMessage);
        QString requestNumber = vTextFromClient.at(4);

        QString changePasswordReply = db.changeUserPassword(vTextFromClient);

        socket->sendTextMessage("fd"+requestNumber+","+changePasswordReply);
        socket->flush();

        QString temp = "fd"+requestNumber+","+changePasswordReply;
        qWarning()<<"REPLY FROM DB:\t"<<changePasswordReply;
        qWarning()<<"reply from server :\t"<<temp;



    }else if(inMessage.at(0) == 'b'){           //block user
        inMessage.remove(0,1);
        qWarning()<<"#########BLOCK :"<<inMessage<<"###################";
        db.blockUser(inMessage);
        //write the returning string to the socket
        qWarning()<<"#################################################";
    }else if (inMessage.at(0) == 'c'){          //sends all blocked users "vshri,shri1,..."

        qWarning()<<"##########GET ALL BLOCKED USERS#########";
        socket->sendTextMessage("c"+db.getAllBlocked());
        socket->flush();
        qWarning()<<"########################################";

    }
    else if(inMessage.at(0) == 'u'){            //unblock user query
        inMessage.remove(0,1);
        qWarning()<<"#########UNBLOCK :"<<inMessage<<"###################";
        db.unBlockUser(inMessage);

        qWarning()<<"########################################";

    }else if(inMessage.at(0) == 'v'){           //sends all blocked users "vshri,shri1,..."
        qWarning()<<"##########SEND ALL BLOCKED USERS########";
        socket->sendTextMessage("v"+db.getAllUnBlocked());
        socket->flush();
        qWarning()<<"########################################";

    }else if(inMessage.at(0) == 'a'){           //admin login : aa or ab
        inMessage.remove(0,1);
        qWarning()<<"size of inmessage login for admin"<<inMessage.size();

        QString adminPassword = inMessage.mid(0,40);
        QString strRequestNumber = inMessage.mid(41,9);

        //qWarning()<<"hased pass"<<hashedPass<<"\t"<<hashedPass.length();
        qWarning()<<"size of inmessage login for admin"<<inMessage;

        //checkAdminLogin(inMessage);
        QChar status = db.getAdminLogIn(adminPassword);
        QString loginReply = "a";
        loginReply.append(status);  //if password correct aa else ab

        this->socket->sendTextMessage(loginReply+","+strRequestNumber);
        this->socket->flush();




    }
    else{
        qWarning()<<"INCORRECT MESSAGE";
    }
    qWarning()<<"Message received at server: "<<inMessage;

}

void MyThread::checkForLogin(QString logInMessage){

    qWarning()<<"+++++++++++++++++++++++++++++++++++++++++++++++++="<<logInMessage;

    QChar status = db.getLogIn(logInMessage);
    QString loginReply = "l";
    loginReply.append(status);

    qWarning()<<"this socket valit at sever: "<<this->socket->isValid();

    this->socket->sendTextMessage(loginReply);
    qWarning()<<"this socket valit at sever: "<<this->socket->isValid();

    this->socket->flush();
    qWarning()<<"this socket valit at sever: "<<this->socket->isValid();

    qWarning()<<"SERVER:LOGIN REPLY SENT: "<<loginReply;

}


void MyThread::checkForLogin(QString logInMessage, QString strReqNo){

    QChar status = db.getLogIn(logInMessage);
    QString loginReply = "l";
    loginReply.append(status);
    loginReply.append(strReqNo);

    this->socket->sendTextMessage(loginReply);
    this->socket->flush();

    qWarning()<<"SERVER: LOGIN REPLY: "<<loginReply;

}


//del later
void MyThread::checkAdminLogin(QString logInMessage){

    QChar status = db.getAdminLogIn(logInMessage);
    QString loginReply = "a";
    loginReply.append(status);  //if password correct aa else ab
    this->socket->sendTextMessage(loginReply);
    qWarning()<<"this socket valit at sever: "<<this->socket->isValid();

    this->socket->flush();

}

void MyThread::disconnected()
{
    socket->deleteLater();

    for(int i = 0; i < vSocket.length(); i++){
        if (vSocket.at(i) == socket){
            vSocket.removeAt(i);
            i = vSocket.length();
        }//removing the disconnected socket from global socket Vector
    }
    exit(0);
}


QVector<QString> MyThread::commaEndedStr(QString Str){

    //QVector<QStringRef> vStr = Str.splitRef(QRegExp("\\s*,\\s*"),QString::SkipEmptyParts);
    //return vStr;

    //
    //QString Str(baStr);
    QString tempStr = "";
    QVector<QString> sv;

    for (int i=0; i < Str.length(); i++){
        if(Str.at(i) != ','){
            tempStr.append(Str.at(i));              //append temp str till , is reached
        }else{
            sv.append(tempStr);                 //when , : append temp str in vector and clear tempstr
            tempStr = "";
        }
    }//end for

    return sv;

}
