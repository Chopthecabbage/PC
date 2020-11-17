#include "payment_info.h"

Payment_Info::Payment_Info()
{
    memset(mem_id, 0, sizeof(mem_id));
    mem_status = 0;
    memset(mem_remain, 0, sizeof(mem_remain));
    memset(charge_time, 0, sizeof(charge_time));
    price = 0;
    payment = 0;
    mem_type= 0;
}

Payment_Info::Payment_Info(char* mem_id, int mem_status, char* mem_remain, char* charge_time, int price, int payment, int mem_type)
{
    strcpy(this->mem_id, mem_id);
    this->mem_status = mem_status;
    strcpy(this->mem_remain, mem_remain);
    strcpy(this->charge_time, charge_time);
    this->price = price;
    this->payment = payment;
    this->mem_type = mem_type;
}

void Payment_Info::Set_Mem_Id(char* mem_id) { strcpy(this->mem_id, mem_id); }
void Payment_Info::Set_Mem_Status(int mem_status) { this->mem_status = mem_status; }
void Payment_Info::Set_Mem_Remain(char* mem_remain) { strcpy(this->mem_remain, mem_remain); }
void Payment_Info::Set_Charge_Time(char* charge_time) { strcpy(this->charge_time, charge_time); }
void Payment_Info::Set_Price(int price) { this->price = price; }
void Payment_Info::Set_Payment(int payment) { this->payment = payment; }
void Payment_Info::Set_Mem_Type(int mem_type) { this->mem_type = mem_type; }

char* Payment_Info::Get_Mem_Id() { return mem_id; }
int Payment_Info::Get_Mem_Status() const { return mem_status; }
char* Payment_Info::Get_Mem_Remain() { return mem_remain; }
char* Payment_Info::Get_Charge_Time() { return charge_time; }
int Payment_Info::Get_Price() const { return price; }
int Payment_Info::Get_Payment() const { return payment; }
int Payment_Info::Get_Mem_Type() const { return mem_type; }


