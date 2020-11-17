#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>

#include "Common.h"
#include "manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_login_btn_clicked();
signals:
    void send_values(QTcpSocket*,int);
private:
    Ui::Login *ui;
    QTcpSocket *_socket;
    Manager *manager_dialog;
};
#endif // LOGIN_H
