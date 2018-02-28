#include "changeadminpassword.h"
#include "ui_changeadminpassword.h"
#include "database.h"
ChangeAdminPassword::ChangeAdminPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeAdminPassword)
{
    ui->setupUi(this);
}

ChangeAdminPassword::~ChangeAdminPassword()
{
    delete ui;
}

void ChangeAdminPassword::on_pbOk_clicked()
{

    ui->lError->setText("");
    DataBase db;
    QString pass1 = ui->lePassword1->text();
    QString pass2 = ui->lePassword2->text();
    if (pass1 == pass2){
        QString hashedPass = QString("%1").arg(QString(QCryptographicHash::hash(pass1.toUtf8(),QCryptographicHash::Sha1).toHex()));

        db.changeAdminPass(hashedPass);
        ui->lError->setText("Password change sucessful!");

    }
    else {
        ui->lError->setText("ERROR: Both passwords are not same.");
    }
}
