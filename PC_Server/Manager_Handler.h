#pragma once

#include "Common.h"
#include "SQL.h"
#include "Chat.h"
#include "Member.h"
#include "SalesInfo.h"
#include "Item.h"
#include "I_Pay_Info.h"
class Manager_Handler
{
private:
    SalesInfo *salesinfo;
    Item *item;
public:
    // 태환
    int ProcOrder(int order_num);
    int CancelOrder(int order_num);
    int GetMsgCnt(int seat_num);
    // 관리자에게 채팅기록 전달함수
    void SendMsgToManager(int m_sock,int seat_num,int chat_cnt);
    // 관리자가 입력한 채팅을 좌석이용자에게 전달
    void SendMsgToClient(int *user_chat_socks,int chat_form,int selected_seat_num,char *chat_msg,int seat_size);

    // 준혁
    void Management(int sock);
    void InfoModify(int sock);
    void InfoDelete(int sock);
    void onSale(int sock,int sig);
        
    //민근
    void Send_Order_To_Pay_Serv(int order_num, int pay_serv_sock);
};