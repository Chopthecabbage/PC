#pragma once
#include "Common.h"
#include "SQL.h"
#include "Member.h"

class Login_Handler
{
public:
    void CreateAccountOverlap(int sock);
    void CreateAccountComplete(int sock);
    void FindAccount(int sock);
    void FindAccountId(int sock);
    void FindAccountPw(int sock);
    void FindAccountUpdate(int sock,int seat_size,Member *m);
    void Log_In(int sock,int *seat);
    void Log_In_Permanent(int sock,int *seat);
    void Log_In_Temporary(int sock,int *seat);
    void SendMemberClass(int m_clnt_login_sock, mutex& m_mtx, int sock,int *seat,int seat_size,Member *m);
    // 구태환
    int CheckMIdCorrect(char *login_id);
};