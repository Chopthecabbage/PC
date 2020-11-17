#ifndef MEMBER_H
#define MEMBER_H

#include "common.h"

class Member
{
private:
    char mem_id[MEM_BUF]; // ID
    char pw[MEM_BUF]; // Password
    char mem_name[MEM_BUF]; // 이름
    char birth[MEM_BUF]; // 생년월일
    char phone[MEM_BUF]; // 연락처
    int mem_status; // 상태
    char remain[MEM_BUF]; // 남은 시간
    int seat_num; // 앉은 좌석번호
    char start_time[MEM_BUF]; // 시작 시간
    int mem_type; // 멤버 타입

public:
    Member();    
    Member(char* _id, char* _pw, char* _name, char* _birth, char* _phone, int _status, char* _remain, int _seat, char* _start, int _type);
    char* Get_Mem_Id();
    char* Get_Password();
    char* Get_Mem_Name();
    char* Get_Birth();
    char* Get_Phone();
    int Get_Mem_Status() const;
    char* Get_Remain();
    int Get_Seat_Num() const;
    char* Get_Start_Time();
    int Get_Mem_Type() const;

    void Set_Mem_Id(char* _id);
    void Set_Password(char* _pw);
    void Set_Mem_Name(char* _name);
    void Set_Birth(char* _birth);
    void Set_Phone(char* _phone);
    void Set_Mem_Status(int _status);
    void Set_Remain(char* _remain);    
    void Set_Seat_Num(int _seat);
    void Set_Start_Time(char* _start);
    void Set_Mem_Type(int _type);
};

#endif // MEMBER_H

