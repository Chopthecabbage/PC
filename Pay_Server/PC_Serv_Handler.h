#pragma once
#include "Common.h"
#include "SQL.h"
#include "Member.h"
#include "T_Pay_Info.h"
#include "I_Pay_Info.h"

class PC_Serv_Handler
{
public:
    void Send_Check_Result(int pc_serv_sock); // 회원 조회결과 
    void Send_Remain(int pc_serv_sock); // 남은시간 전송
    void Charge_Time_Member(int pc_serv_sock); // 회원 시간충전
    void Charge_Time_Temp(int pc_serv_sock); // 비회원 시간충전
    void Recv_Item_Pay_Info(int pc_serv_sock); // 상품 결제정보 DB에 저장 
};