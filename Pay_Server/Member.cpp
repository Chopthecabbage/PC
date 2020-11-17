#include "Member.h"

Member::Member()
{
    memset(mem_id, 0, sizeof(mem_id));
    memset(pw, 0, sizeof(pw));
    memset(mem_name, 0, sizeof(mem_name));
    memset(birth, 0, sizeof(birth));
    memset(phone, 0, sizeof(phone));
    mem_status = 0;
    memset(remain, 0, sizeof(remain));
    seat_num = 0;
    memset(start_time, 0, sizeof(start_time));
    mem_type = 0;
}

Member::Member(char* _id, char* _pw, char* _name, char* _birth, char* _phone, int _status, char* _remain, int _seat, char* _start, int _type)
{
    strcpy(mem_id, _id);
    strcpy(pw, _pw);
    strcpy(mem_name, _name);
    strcpy(birth, _birth);
    strcpy(phone, _phone);
    mem_status = _status;
    strcpy(remain, _remain);
    seat_num = _seat;
    strcpy(start_time, _start);
    mem_type = _type;
}

char* Member::Get_Mem_Id() { return mem_id; }
char* Member::Get_Password(){ return pw; }
char* Member::Get_Mem_Name(){ return mem_name; }
char* Member::Get_Birth(){ return birth; }
char* Member::Get_Phone(){ return phone; }
int Member::Get_Mem_Status() const { return mem_status; }
char* Member::Get_Remain(){ return remain; }
int Member::Get_Seat_Num() const { return seat_num; }
char* Member::Get_Start_Time() { return start_time; }
int Member::Get_Mem_Type() const { return mem_type; }

void Member::Set_Mem_Id(char* _id) { strcpy(mem_id, _id); }
void Member::Set_Password(char* _pw) { strcpy(pw, _pw); }
void Member::Set_Mem_Name(char* _name) { strcpy(mem_name, _name); }
void Member::Set_Birth(char* _birth) { strcpy(birth, _birth); }
void Member::Set_Phone(char* _phone) { strcpy(phone, _phone); }
void Member::Set_Mem_Status(int _status) { mem_status = _status; }
void Member::Set_Remain(char* _remain) { strcpy(remain, _remain); }
void Member::Set_Seat_Num(int _seat) { seat_num = _seat; }
void Member::Set_Start_Time(char* _start) { strcpy(start_time, _start); }
void Member::Set_Mem_Type(int _type) { mem_type = _type; }






