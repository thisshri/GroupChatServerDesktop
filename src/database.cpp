#include "database.h"
#include <QFile>
#include <QFileDialog>
#include <QDir>

DataBase::DataBase()
{

    //QString dbDir = QCoreApplication::applicationDirPath();
    //dbDir.append("")

    DB = QSqlDatabase::addDatabase("QSQLITE");
    QString dbLocation = QDir::currentPath().append("/chat.sqlite");

    DB.setDatabaseName(dbLocation);
    if (DB.open()){
        qWarning()<< "Database: Connected.";
    }
    else{
    qWarning()<< "Database: Not connected.";
    }
}


//New user registration
void DataBase::registerUser(QString queryStr){

    QSqlQuery query;
    query.exec("INSERT INTO Users values("+queryStr+")");
}


QChar DataBase::getLogIn(QString loginStr){ // USER LOGIN QUERY

    QString userStatus = "b";
    QVector<QString> lv;
    QString tempStr = "";
    for (int i=0; i < loginStr.length(); i++){
        if(loginStr.at(i) != ','){
            tempStr.append(loginStr.at(i));              //append temp str till , is reached
        }else{
            lv.append(tempStr);                 //when , : append temp str in vector and clear tempstr
            tempStr = "";
        }
    }// the login username and password is in vector now.

    QSqlQuery query;
    query.exec("SELECT status FROM Users WHERE username = '"+lv.at(0)+"' AND password = '"+lv.at(1)+"'");
    while (query.next()){
         userStatus = query.value(0).toString();
         qWarning()<<"LOGIN QUERY :"<<loginStr<<" ACCESS:"<<userStatus;
    }

    if(userStatus.at(0) == 'u'){
        return 'u';
    }else if (userStatus.at(0) == 'b'){
        return 'b';
    }else{
        qWarning()<<"Error DATABASE LOGING IN.";
    }
    qWarning()<<"DATABASE QUERY HAPPENED";
    return 'b';

}// LOGING IN AND SEE IF THE USER HAS ACCESS TO CHAT.


void DataBase::blockUser(QString userName){     //BLOCK USER QUERY
    QSqlQuery query;
    query.exec("UPDATE Users set status = 'b' where username = '"+userName+"'");
}


void DataBase::unBlockUser(QString userName){   //UNBLOCK UER QUERY
    QSqlQuery query;
    query.exec("UPDATE Users set status = 'u' where username = '"+userName+"'");
}


QString DataBase::getAllBlocked(){           // RETURNS ALL THE BLOCKED USERS ie: cshri,shri1,
    QSqlQuery query;
    QString queryResultStr("c");
    QVector<QString> vString;

    query.exec("SELECT userName FROM Users where status = 'b'");

    while (query.next()){
        //appending all the blocked users to vString vector
        vString.append(query.value(0).toString()+",");
    }//while

    for (int i = 0; i < vString.length(); i++){
            queryResultStr.append(vString.at(i));
        }// vString to string

    qWarning()<<"SERVER: ALLBLOCKED USERS:"<<queryResultStr;

    return queryResultStr;
}


QString DataBase::getAllUnBlocked(){     //RETURNS ALL UNBLOCKED USERS, ie: "vshri,shri1"
    QSqlQuery query;
    QString queryResultStr("v");
    QVector<QString> vString;

    query.exec("SELECT userName FROM Users where status = 'u'");

    while (query.next()){
        //appending all the unblocked users to vString vector
        vString.append(query.value(0).toString()+",");
    }
    for (int i = 0; i < vString.length(); i++){
        queryResultStr.append(vString.at(i));
    }// vString to string

    qWarning()<<"SERVER: ALLBLOCKED USERS:"<<queryResultStr;

    return queryResultStr;
}


bool DataBase::isUserNameAvalible(QString inUserName){
    qWarning()<<"-------------------------------------------";

    QSqlQuery query;
    query.exec("SELECT userName FROM Users WHERE userName == '"+inUserName+"'");


    QString userNameFromDb;

    while (query.next()){
        userNameFromDb = query.value(0).toString();
    }

    //qDebug() << query.executedQuery();
    //qDebug() << query.lastError();
    qWarning()<<"USERNAME FROM CLIENT \t: "<<inUserName;
    qWarning()<<"USRNAME FROM DB \t"<<userNameFromDb;

    if(inUserName == userNameFromDb){
        qWarning()<<"username already taken";
        return 0;
    }else{
        qWarning()<<"you can use this name";
        return 1;
    }
}


QChar DataBase::getAdminLogIn(QString adminHPassword){      //checks admin password and returns "aa" or "ab"
    QString queryResultstr;
    QSqlQuery query;
    query.exec("SELECT userName FROM Admin where password = '"+adminHPassword+"'");
    while (query.next()){
         queryResultstr = query.value(0).toString();
    }

    if(queryResultstr == "admin"){
        return 'a';
    }else{
        return 'b';
    }
    return 'b'; //remove this line.
}



bool DataBase::changeAdminPass(QString newAdminPassword){
    QSqlQuery query;
    query.exec("UPDATE Admin SET password = '"+newAdminPassword+"';");
    //change admin
    qWarning()<<"CHANGE ADMIN PASSWORD query.isvalid : "<<query.isValid();

    return query.isValid();
}

QString DataBase::getRecoveryQuestion(QString username){
    QSqlQuery query;
    query.exec("SELECT passRecQ from Users WHERE userName == '"+username+"';");
    QString queryResult;
    while (query.next()) {
        queryResult = query.value(0).toString();
    }


    qWarning()<<"RECOVERY QUESTION : \t"<<queryResult;
    return queryResult;
}

QString DataBase::changeUserPassword(QVector<QString> vTextFromClient){

    QString resultStr="x";
    QSqlQuery query;

    QString username = vTextFromClient.at(0);
    QString password = vTextFromClient.at(1);
    QString securityAns = vTextFromClient.at(2);
    QString dob = vTextFromClient.at(3);
    //QString strRequestNumber = vTextFromClient.at(4);

    query.exec("UPDATE Users SET password ='"+password+"' "
                      "WHERE userName == '"+username+"' AND "
                      "DOB == '"+dob+"' AND "
                      "passRecA == '"+securityAns+"';");

    QSqlQuery query1;
    query1.exec("SELECT userName FROM Users WHERE userName == '"+username+"' AND DOB == '"+dob+"' AND passRecA == '"+securityAns+"';");

    while (query1.next()) {
        qWarning()<<"IN LOOP"<<query1.value(0).toString();
        resultStr = query1.value(0).toString();
    }
    qWarning()<<"IN DB:\t"<<resultStr;
    return resultStr;
}
