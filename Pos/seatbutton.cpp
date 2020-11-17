#include "seatbutton.h"

SeatButton::SeatButton(QWidget *parent):
    QPushButton(parent),
    m_seat_num(0),
    m_pc_member(""),
    m_user_name(""),
    m_user_id(""),
    m_user_start_time("0"),
    m_user_end_time(""),
    m_remain_time(""),
    m_pc_usage_fee(0)
{
    m_btn_order++;
    m_seat_num = m_btn_order;
}

void SeatButton::initializeBtnInfo()
{
    m_pc_member=""; // 회원,비회원
    m_user_name="";
    m_user_id="";
    m_user_start_time=""; // PC이용자 시작이용시간
    m_user_end_time=""; // PC이용자 이용제한시간
    m_remain_time=""; // PC 남은시간
    m_pc_usage_fee=0; // 남은요금
}

int SeatButton::getSeatNum() const
{
    return m_seat_num;
}

void SeatButton::setSeatNum(int seat_num)
{
    m_seat_num = seat_num;
}

void SeatButton::setUserId(QString id)
{
    m_user_id = id;
}

void SeatButton::setUserName(QString name)
{
    m_user_name = name;
}

void SeatButton::setUserStartTime(QString start_time)
{
    m_user_start_time = start_time;
}

void SeatButton::setUserEndTime(QString end_time)
{
    m_user_end_time = end_time;
}

void SeatButton::setUserRemainTime(QString remain_time)
{
    m_remain_time = remain_time;
}

void SeatButton::setPcUsageFee(int fee)
{
    m_pc_usage_fee = fee;
}

void SeatButton::setPcMemberKind(QString kind)
{
    m_pc_member = kind;
}

QString SeatButton::getUserId() const
{
    return m_user_id;
}

QString SeatButton::getUserName() const
{
    return m_user_name;
}

QString SeatButton::getUserStartTime() const
{
    return m_user_start_time;
}

QString SeatButton::getUserEndTime() const
{
    return m_user_end_time;
}

QString SeatButton::getRemainTime() const
{
    return m_remain_time;
}

int SeatButton::getPcUsageFee() const
{
    return m_pc_usage_fee;
}

QString SeatButton::getPcMemberKind() const
{
    return m_pc_member;
}

int SeatButton::m_btn_order=0;
