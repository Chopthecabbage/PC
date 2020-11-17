#ifndef SEATBUTTON_H
#define SEATBUTTON_H

#include <QPushButton>
#include "Common.h"

class SeatButton : public QPushButton
{
public:
    explicit SeatButton(QWidget *parent = nullptr);

    void initializeBtnInfo();
    void setSeatNum(int seat_num);
    void setUserId(QString user_id);
    void setUserName(QString name);
    void setUserStartTime(QString start_time);
    void setUserEndTime(QString end_time);
    void setUserRemainTime(QString remain_time);
    void setPcUsageFee(int fee);
    void setPcMemberKind(QString kind);

    int getSeatNum() const;
    QString getUserId() const;
    QString getUserName() const;
    QString getUserStartTime() const;
    QString getUserEndTime() const;
    QString getRemainTime() const;
    int getPcUsageFee() const;
    QString getPcMemberKind() const;
public:
    static int m_btn_order;
private:
    int m_seat_num; // 좌석번호
    QString m_pc_member; // 회원, 비회원
    QString m_user_name; //  PC이용자 이름
    QString m_user_id; // PC이용자 id
    QString m_user_start_time; // PC이용자 시작이용시간
    QString m_user_end_time; // PC이용자 이용제한시간
    QString m_remain_time; // PC 남은시간
    int m_pc_usage_fee; // 남은요금 : 회원은 분당 20원 비회원은 분당 25원
};

#endif // SEATBUTTON_H
