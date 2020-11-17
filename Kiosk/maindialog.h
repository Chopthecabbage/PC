#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "member.h"
#include "cookdialog.h"
#include "qprocess.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();
    QString Time_Add(QString time1, QString time2);
    virtual bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainDialog *ui;
    //
    CookDialog * cookdialog;
    //
    QTcpSocket *plus_sock;
    QTcpSocket *chat_sock;
    QTcpSocket *kick_sock;
    QTcpSocket *guerrilla_sock;
    QTime *objTime;
    Member people;

    int Force;
    int Type;
    int Sitting;
    QTimer *timer;
    //
    QTimer *alarm;
    QTime *alarmTime;
    //
    QTime *Markov_moment;

private slots:
    void RemainTime();
    void RecvObj(Member);
    void Cooking();
    //
    void UsageTime();
    void on_ShiftBtn_clicked();
    void on_CloseBtn_clicked();
    //
    void RecvExtraTimeFromMin();
    void RecvChatFromKoo();
    void RecvKickFromKoo();
    //
    void on_MsgBtn_clicked();
    //
    void on_normal_discconnect();
    void on_plus_sock_disconnected();
    void on_chat_sock_disconnected();
    void on_kick_sock_disconnected();

signals:
    void SendClass(Member);

};

#endif // MAINDIALOG_H
