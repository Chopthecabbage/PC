#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "common.h"
#include "member.h"
#include "payment_info.h"
#include "select_payment.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    Ui::Widget *ui;
    QTcpSocket sock;   
    Payment_Info pay_info;
    Member member;
    int mem_check; // 회원조회시 있는지 없는지 구분
    int type; // 요금결제 회원, 비회원 구분
    int price; // 요금결제 선택한 가격
    int payment; // 결제 수단
    QString time; // 요금결제 선택한 시간

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
       void on_id_searchBtn_clicked(); // 아이디 검색버튼
       void Change_Label(); // 버튼 클릭시 UI변경(Label 변경, 결제버튼 변경)
       void Recv_Payment(int _payment);
       // 회원 요금충전 버튼
       void on_memBtn_1_clicked();
       void on_memBtn_2_clicked();
       void on_memBtn_3_clicked();
       void on_memBtn_4_clicked();
       void on_memBtn_5_clicked();
       void on_memBtn_6_clicked();
       // 비회원 요금충전 버튼
       void on_tempBtn_1_clicked();
       void on_tempBtn_2_clicked();
       void on_tempBtn_3_clicked();
       void on_tempBtn_4_clicked();
       void on_tempBtn_5_clicked();
       void on_tempBtn_6_clicked();

       void on_tabWidget_currentChanged(int index);
       void on_paymentBtn_clicked();

       QString Time_Add(QString time1, QString time2);

};
#endif // WIDGET_H
