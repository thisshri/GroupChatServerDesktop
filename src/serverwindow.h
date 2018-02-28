#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "myserver.h"

namespace Ui {
class serverWindow;
}

class serverWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit serverWindow(QWidget *parent = 0);
    ~serverWindow();

private slots:
    void on_pbStart_clicked();

    void on_pbStop_clicked();
    void openPassChangeWindow();

private:
    Ui::serverWindow *ui;
    MyServer *WebSockServer;
};

#endif // SERVERWINDOW_H
