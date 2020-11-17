#ifndef PAYMENT_INFO_H
#define PAYMENT_INFO_H
#include "common.h"

class Payment_Info
{
private:
    char mem_id[MEM_BUF]; // 회원 ID
    int mem_status; // 상태
    char mem_remain[MEM_BUF]; // 남은 시간
    char charge_time[MEM_BUF]; // 충전할 시간
    int price; // 결제 금액
    int payment; // 결제 수단 현금:1 카드:2
    int mem_type; // 회원 비회원 구분 1:회원 2:비회원

public:
    Payment_Info();
    Payment_Info(char* mem_id, int mem_status, char* mem_remain, char* charge_time, int price, int payment, int mem_type);

    void Set_Mem_Id(char* mem_id);
    void Set_Mem_Status(int mem_status);
    void Set_Mem_Remain(char* mem_remain);
    void Set_Charge_Time(char* charge_time);
    void Set_Price(int price);
    void Set_Payment(int payment);
    void Set_Mem_Type(int mem_type);

    char* Get_Mem_Id();
    int Get_Mem_Status() const;
    char* Get_Mem_Remain();
    char* Get_Charge_Time();
    int Get_Price() const;
    int Get_Payment() const;
    int Get_Mem_Type() const;
};

#endif // PAYMENT_INFO_H
