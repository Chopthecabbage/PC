#pragma once
#include "Common.h"
#include "SQL.h"
#include "Member.h"
#include "T_Pay_Info.h"

class Kiosk_Handler
{
public:
    void Check_Member(int kiosk_sock, int pc_serv_sock); // 검색id 회원 있나 체크
    void Get_Remain(int kiosk_sock, int pc_serv_sock); // 회원 남은시간 체크
    void Charge_Time(int kiosk_sock, int pc_serv_sock); // 시간 충전
};
    