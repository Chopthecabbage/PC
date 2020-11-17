#ifndef MANAGER_H
#define MANAGER_H

#include <QDialog>
#include <QTcpSocket>
#include <QVector>
#include <QMutex>

#include "Common.h"
#include "member.h"
#include "seatbutton.h"
#include "arrangement.h"
#include "orders.h"
#include "chat.h"
#include "management.h"
#include "daysale.h"
#include "dailymonthlysales.h"

namespace Ui {
class Manager;
}

class Manager : public QDialog
{
    Q_OBJECT

private:
    Ui::Manager *ui;
    SeatButton *btn; // 전체 운용좌석들

    QTcpSocket *writing_m_sock; // 중계서버에게 데이터전달용 통신소켓

    QTcpSocket *reading_login_sock; // 중계서버로부터의 로그인 수신용 소켓
    QMutex login_mutex;
    QMutex logout_mutex;
    QMutex order_mutex;
    QMutex timepass_mutex;
    QMutex chat_mutex;

    QTcpSocket *reading_time_pass_sock; // 남은시간 변경 수신용 소켓
    QTcpSocket *reading_chat_sock; // 클라이언트 채팅 수신용 소켓
    QTcpSocket *reading_order_sock; // 클라이언트 음식주문 수신용 소켓

    Member *mems_info; // 수용가능한 모든 pc방이용자의 정보를 저장하는 객체
    QVector<Arrangement> arrangements; // pc방 이용자의 모든 음식주문건을 저장하는 객체

    Orders *order_dialog;



    int tot_seat_cnt;   // 운용가능한 최대 좌석수
    int online_users_cnt; // 이용중인 좌석수
    int tot_waiting_orders; // 처리대기중인 음식주문건수
    bool isEnded; // 당일영업종료여부
    //------------------------------------------준혁
    Management *management; //회원 관리 객체
    Daysale *daysale;
    DailyMonthlySales *dailymonth;
public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();
    void InitSeats();
    void setTodayEnd();
    bool getTodayEnd() const;
    QString getPcEndingTime(QString pc_start_time,QString remain_time);
    void setRemainUsageFee(QString remain_time,int *remain_fee);
    int getRemainUsageFee(int membership,QString remain_time);
signals:
    void send_orders(QTcpSocket* socket,QVector<Arrangement> *arrangements);
    void onDailyMonthClicked(const QDate);
public slots:
    void recv_values(QTcpSocket*,int);
    void recv_login_client();
    void recv_timepass_info();
    void recv_chat_info();
    void recv_order_info();
    void getSeatNumber();
    void on_orderList_btn_clicked();
    void on_exit_client_btn_clicked();
private slots:
    void on_send_msg_btn_clicked();
    void on_end_btn_clicked();
    void on_end_today_sales_btn_clicked();
    void on_find_user_btn_clicked();
    void on_client_list_currentIndexChanged(int index);
    void on_select_client_btn_clicked();
    void on_select_all_btn_clicked();

    void on_mem_managementBtn_clicked();
    void on_samedaysaleBtn_clicked();
    void on_salesBydateBtn_clicked();
};

#endif // MANAGER_H
