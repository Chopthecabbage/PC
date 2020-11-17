#pragma once
#include "Common.h"

class T_Pay_Info
{
private:
    char mem_id[MEM_BUF]; // 회원 ID
    int mem_status; // 상태
    char mem_remain[MEM_BUF]; // 남은 시간
    char charge_time[MEM_BUF]; // 충전할 시간
    int price; // 총 금액
    int payment; // 결제 수단 현금:1 카드:2
    int mem_type; // 회원 비회원 구분    

public:
    T_Pay_Info()
    {
        memset(mem_id, 0, sizeof(mem_id));
        mem_status = 0;
        memset(mem_remain, 0, sizeof(mem_remain));
        memset(charge_time, 0, sizeof(charge_time));    
        price = 0;
        payment = 0;
        mem_type = 0;
    }

    T_Pay_Info(char* mem_id, int mem_status, char* mem_remain,char* charge_time, int price, int payment, int mem_type)
    {
        strcpy(this->mem_id, mem_id);
        this->mem_status = mem_status;
        strcpy(this->mem_remain, mem_remain);
        strcpy(this->charge_time, charge_time);
        this->price = price;
        this->payment = payment;
        this->mem_type = mem_type;
    }

    void Set_Mem_Id(char* mem_id) { strcpy(this->mem_id, mem_id); }
    void Set_Mem_Status(int mem_status) { this->mem_status = mem_status; }
    void Set_Mem_Remain(char* mem_remain) { strcpy(this->mem_remain, mem_remain); }
    void Set_Charge_Time(char* charge_time) { strcpy(this->charge_time, charge_time); }
    void Set_Price(int price) { this->price = price; }
    void Set_Payment(int payment) { this->payment = payment; }
    void Set_Mem_Type(int mem_type) { this->mem_type = mem_type; }

    char* Get_Mem_Id() { return mem_id; }
    int Get_Mem_Status() const { return mem_status; }
    char* Get_Mem_Remain() { return mem_remain; }
    char* Get_Charge_Time() { return charge_time; }
    int Get_Price() const { return price; }
    int Get_Payment() const { return payment; }
    int Get_Mem_Type() const { return mem_type; }
    
};

