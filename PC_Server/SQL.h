#pragma once
#include "Common.h"
#include "Arrangement.h"
#include "Member.h"
#include "SalesInfo.h"

class SQL
{
private:
    MYSQL *conn;
    MYSQL_RES *sql_result;
    MYSQL_ROW sql_row;
    
public:
    SQL();
    bool Connect();
    void Disconnect(); 
    // 형섭
    MYSQL_RES* CreateAccountOverlap();
    bool CreateAccountComplete(string id,string pw,string name,string birth,string phone,string status,string reamain);
    MYSQL_RES* FindAccountId();
    MYSQL_RES* FindAccountPw();
    bool FindAccountUpdate(string name,string phone,string pw);
    bool Log_In_Permanent_Update(string id,string pw);
    bool Log_In_Temporary_Update(string id);
    MYSQL_RES* Log_In_Permanent();
    MYSQL_RES* Log_In_Temporary();
    MYSQL_RES* ItemTotalCount();
    MYSQL_RES* ItemList_1();
    MYSQL_RES* ItemList_2();
    MYSQL_RES* PREQUEL();
    MYSQL_RES* SEQUEL();
    MYSQL_RES* calPREQUEL();
    MYSQL_RES* calSEQUEL();
    bool ProcessIsCompleted(Arrangement arr);
    MYSQL_RES* need_arr_id();
    bool CloseEventChangeData(Member member,int judge);
    bool ShiftEventChangeData_Mem(Member member);
    bool ShiftEventChangeData_Tem(Member member);
    bool UpdatChatting(int Sitting,int Writer,string Message);
    bool UpdateRemainEveryMinute(string Nick,string UpdateRemain,int Membership);

    // 민근
    int Select_Exists_Member(string check_id);
    MYSQL_RES* Select_Member_By_ID(string search_id);
    int Insert_Temp_Member(string remain);
    void Update_Member_Remain(string mem_id, string remain);
    MYSQL_RES* Select_Order_Info(int order_num);


    // 태환
    bool isManagerExist(string input_id);
    int GetSeatCnt(string input_id);
    int SetProcOrder(string order_num);
    int SetCancelOrder(string order_num);
    int GetMsgCnt(int seat_num);
    bool InsertChatInfo(int chat_form,int selected_seat_num,char *chat_msg);
    MYSQL_RES* GetMsgAll(int seat_num,int chat_cnt);

    // 준혁
    Member GetMemberInfo(string mem_id); //해당 id로 멤버 정보 불러오는 쿼리 함수
    void ModifiedInfo(Member member);
    void DeletedInfo(Member member);
    int GetDateCnt(string date,int sig); //해당 날짜에 대한 행 카운트 반환
    SalesInfo* GetArrInfo(SalesInfo *salesinfo,string date,int sig);
    Item GetArrItemInfo(SalesInfo *salesinfo);
    void error_check();
};