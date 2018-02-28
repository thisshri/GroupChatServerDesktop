#ifndef CHANGEADMINPASSWORD_H
#define CHANGEADMINPASSWORD_H

#include <QDialog>

namespace Ui {
class ChangeAdminPassword;
}

class ChangeAdminPassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeAdminPassword(QWidget *parent = 0);
    ~ChangeAdminPassword();

private slots:
    void on_pbOk_clicked();

private:
    Ui::ChangeAdminPassword *ui;
};

#endif // CHANGEADMINPASSWORD_H
