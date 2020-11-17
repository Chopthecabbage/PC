#pragma once
#include "Common.h"
#include "SQL.h"
#include "Member.h"
#include "T_Pay_Info.h"

class Payment_Handler
{
public:
    void Check_Member(int pay_serv_sock); // 회원있나 DB에 조회
    void Get_Remain(int pay_serv_sock, Member* member, int seat_size); // 회원 잔여시간 조회
    void Charge_Time_Member(int pay_serv_sock, T_Pay_Info t_info, int charge_sock = -1); // 회원 시간충전
    void Charge_Time_Temp(int pay_serv_sock); // 비회원 시간 충전
};