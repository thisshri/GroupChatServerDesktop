#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QVector>

class DataBase
{
    QSqlDatabase DB;

public:
    DataBase();
    QString getUserInfo(QString inUserName);
    bool isUserNameAvalible(QString inUserName); //usename avalible
    void registerUser(QString queryStr);
    QChar getLogIn(QString loginStr);
    QChar getAdminLogIn(QString adminHPassword);
    void blockUser(QString userName);
    void unBlockUser(QString userName);
    QString getAllBlocked();
    QString getAllUnBlocked();
    bool changeAdminPass(QString query);
    QString getRecoveryQuestion(QString username); // forget password
    QString changeUserPassword(QVector<QString>);

};

#endif // DATABASE_H
