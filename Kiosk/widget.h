#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
//
#include "arrangement.h"
#include "member.h"
#include "createaccountdialog.h"
#include "findaccountdialog.h"
#include "maindialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void SendMemberClass();
    void Login_Process();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;
    CreateAccountDialog *createaccountdialog;
    FindAccountDialog *findaccountdialog;
    MainDialog *maindialog;
    //
    QLabel *lbl[3];
    QLineEdit *edit[3];
    QRegExpValidator *vali[3];
    //
    QPushButton *btn[3];
    //
    QTimer *timer;
    QLabel *label_date_time;
    //
    QComboBox *combo;
    //
    int Type;
    int Chair;
    //
    Member people;
    int Doppelganger=0;

private slots:
    void myfunction();
    void Log_In(); // 로그인
    void CreateAccount(); // 회원 가입
    void FindAccount(); // 계정 찾기
    void on_discconnected();

signals:
    void SendObj(Member);

};
#endif // WIDGET_H
