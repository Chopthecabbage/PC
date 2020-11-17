#pragma once
#include "Common.h"
#include "SQL.h"
#include "Arrangement.h"
#include "Member.h"

class User_Handler
{
public:
    void SendMinuteToKoo(int m_time_sock, mutex& m_mtx, int sock);
    void SendMenuToWoo(int sock);
    void SendArrangementToKoo(int m_order_sock, mutex& m_mtx, int sock);
    int TimeOutFromWoo(int m_clnt_login_sock,mutex& m_mtx,int sock,int *seat,Member *m);
    int SlideDownOn_Mem(int m_clnt_login_sock,mutex& m_mtx,int sock,int *seat);
    int SlideDownOn_Tem(int m_clnt_login_sock,mutex& m_mtx,int sock,int *seat);
    void ChattingToDbAndKoo(int m_chat_sock, mutex& m_mtx,int sock);
};