#include "User_Handler.h"

void User_Handler::SendMinuteToKoo(int m_time_sock, mutex& m_mtx, int sock)
{
    SQL sql;
    sql.Connect();

    int seat=0;
    char remain[DefineMember::BUFFSIZE];

    read(sock,&seat,sizeof(int));
    cout<<"seat: "<<seat<<endl;
    read(sock,remain,sizeof(char)*DefineMember::BUFFSIZE);
    cout<<"remain: "<<remain<<endl;
    char id[DefineMember::BUFFSIZE];
    read(sock,id,sizeof(char)*DefineMember::BUFFSIZE);
    cout<<"id: "<<id<<endl;
    int Membership;
    read(sock,&Membership,sizeof(int));
    cout<<"Membership: "<<Membership<<endl;

    m_mtx.lock();
    write(m_time_sock,&seat,sizeof(int));
    write(m_time_sock,remain,sizeof(char)*DefineMember::BUFFSIZE);
    m_mtx.unlock();
    string Nick(id); string UpdateRemain(remain);
    sql.UpdateRemainEveryMinute(Nick,UpdateRemain,Membership);

    sql.Disconnect();
}

void User_Handler::SendMenuToWoo(int sock)
{
    SQL sql;
    sql.Connect();
    
    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.ItemTotalCount();

    string str;
    while(row = mysql_fetch_row(result))
    {
        str=row[0];
    }

    int TotalCount=atoi(str.c_str());
    //cout<<"TotalCount: "<<TotalCount<<endl;
    write(sock,&TotalCount,sizeof(int));
    mysql_free_result(result);
    // 카테고리 0 + 카테고리 1 
    result=sql.PREQUEL();
    while(row = mysql_fetch_row(result))
    {
        str=row[0];
    }

    int PREQUEL_COUNT=atoi(str.c_str());
    //cout<<"PREQUEL_COUNT: "<<PREQUEL_COUNT<<endl;
    write(sock,&PREQUEL_COUNT,sizeof(int));
    mysql_free_result(result);
    // 카테고리 2 + 카테고리 3
    result=sql.SEQUEL();
    while(row = mysql_fetch_row(result))
    {
        str=row[0];
    }

    int SEQUEL_COUNT=atoi(str.c_str());
    //cout<<"SEQUEL_COUNT: "<<SEQUEL_COUNT<<endl;
    write(sock,&SEQUEL_COUNT,sizeof(int));
    mysql_free_result(result);
    // NoodleCategoryCount OR BeverageCategoryCount
    result=sql.calPREQUEL();
    while(row = mysql_fetch_row(result))
    {
        str=row[0];
    }

    int NoodleCategoryCount=atoi(str.c_str());
    //cout<<"NoodleCategoryCount: "<<NoodleCategoryCount<<endl;
    int BeverageCategoryCount = PREQUEL_COUNT - NoodleCategoryCount;
    //cout<<"BeverageCategoryCount: "<<BeverageCategoryCount<<endl;
    write(sock,&NoodleCategoryCount,sizeof(int));
    write(sock,&BeverageCategoryCount,sizeof(int));
    mysql_free_result(result);
    // SnackCategoryCount OR RiceCategoryCount;
    result=sql.calSEQUEL();
    while(row = mysql_fetch_row(result))
    {
        str=row[0];
    }

    int SnackCategoryCount=atoi(str.c_str());
    //cout<<"SnackCategoryCount: "<<SnackCategoryCount<<endl;
    int RiceCategoryCount=SEQUEL_COUNT-SnackCategoryCount;
    //cout<<"RiceCategoryCount: "<<RiceCategoryCount<<endl;
    write(sock,&SnackCategoryCount,sizeof(int));
    write(sock,&RiceCategoryCount,sizeof(int));
    mysql_free_result(result);
    
    Item *item_1; Item *item_2;
    item_1 = new Item[PREQUEL_COUNT]; item_2 = new Item[SEQUEL_COUNT];

    result=sql.ItemList_1();
    string item_code; string item_name; string item_price; string item_category; string item_photo;
    int code; char name[DefineMember::BUFFSIZE]; int price; int category; char photo[DefineMember::BUFFSIZE];
    int increase=0;
    while(row = mysql_fetch_row(result))
    {
        item_code=row[0];
        code=atoi(item_code.c_str());
        item_name=row[1];
        strcpy(name,item_name.c_str());
        item_price=row[2];
        price=atoi(item_price.c_str());
        item_category=row[3];
        category=atoi(item_category.c_str());
        item_photo=row[4];
        strcpy(photo,item_photo.c_str());
        item_1[increase++].SetItem(code,name,price,category,photo);
    }
    for(int i=0; i<PREQUEL_COUNT; i++)
    {
        item_1[i].DebugShowItem();
    }
    write(sock,item_1,sizeof(Item)*PREQUEL_COUNT);
    mysql_free_result(result);
    
    result=sql.ItemList_2();
    increase=0;
    while(row = mysql_fetch_row(result))
    {
        item_code=row[0];
        code=atoi(item_code.c_str());
        item_name=row[1];
        strcpy(name,item_name.c_str());
        item_price=row[2];
        price=atoi(item_price.c_str());
        item_category=row[3];
        category=atoi(item_category.c_str());
        item_photo=row[4];
        strcpy(photo,item_photo.c_str());
        item_2[increase++].SetItem(code,name,price,category,photo);
    }
    for(int i=0; i<SEQUEL_COUNT; i++)
    {
        item_1[i].DebugShowItem();
    }
    write(sock,item_2,sizeof(Item)*SEQUEL_COUNT);
       
    mysql_free_result(result);
    sql.Disconnect();
}

void User_Handler::SendArrangementToKoo(int m_order_sock, mutex& m_mtx, int sock)
{
    Arrangement arr;
    SQL sql;

    m_mtx.lock();
    sql.Connect();    
    read(sock,&arr,sizeof(Arrangement));
    sql.ProcessIsCompleted(arr);
    
    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.need_arr_id();
    row=mysql_fetch_row(result);    
    int arr_pk=atoi(row[0]);
    
    cout<<"arr_pk: "<<arr_pk<<endl;
    arr.SetArrangementNum(arr_pk);
    mysql_free_result(result);
    
    write(m_order_sock,&arr,sizeof(Arrangement));
    m_mtx.unlock();

    sql.Disconnect();
}

int User_Handler::TimeOutFromWoo(int m_clnt_login_sock, mutex& m_mtx, int sock,int *seat,Member *m)
{
    Member member;
    SQL sql;
    sql.Connect();
    int Sitting;
    
    read(sock,&Sitting,sizeof(int));
    cout<<"Sitting: "<<Sitting<<endl;
    seat[Sitting]=0;
    
    int judge;
    read(sock,&judge,sizeof(int));
    read(sock,&member,sizeof(Member));
    // 서버 멤버 동적 배열 초기화
    m[Sitting].InitializeMember();
    //
    sql.CloseEventChangeData(member,judge); // 상태: 0, 남은 시간: 00:00
    sql.Disconnect();   

    int LoginStatus=2; // 로그아웃 시그널
    m_mtx.lock();
    write(m_clnt_login_sock,&LoginStatus,sizeof(int));
    // 좌석 번호...전송
    ++Sitting;
    write(m_clnt_login_sock,&Sitting,sizeof(int));
    m_mtx.unlock();
    cout<<"시간 종료로 나갔을때의 Sitting: "<<Sitting<<endl;

    return Sitting-1;
}

int User_Handler::SlideDownOn_Mem(int m_clnt_login_sock, mutex& m_mtx, int sock,int *seat)
{
    Member member;
    SQL sql;
    sql.Connect();
    int Sitting;
    
    read(sock,&Sitting,sizeof(int));
    cout<<"Sitting: "<<Sitting<<endl;
    seat[Sitting]=0;
    
    int Force;
    read(sock,&Force,sizeof(int));
    cout<<"Force: "<<Force<<endl;
    
    read(sock,&member,sizeof(Member));
    sql.ShiftEventChangeData_Mem(member); 
    
    sql.Disconnect();   
    
    if(Force==0)
    {
        int LoginStatus=2; // 로그아웃 시그널
        m_mtx.lock();
        write(m_clnt_login_sock,&LoginStatus,sizeof(int));
        // 좌석 번호...전송
        ++Sitting;
        write(m_clnt_login_sock,&Sitting,sizeof(int));
        m_mtx.unlock();
        cout<<"자리이동, 자진종료 =>나갔을때의 Sitting: "<<Sitting<<endl;
    }
    return Sitting-1;
}

int User_Handler::SlideDownOn_Tem(int m_clnt_login_sock, mutex& m_mtx, int sock,int *seat)
{
    Member member;
    SQL sql;
    sql.Connect();
    int Sitting;
    
    read(sock,&Sitting,sizeof(int));
    cout<<"Sitting: "<<Sitting<<endl;
    seat[Sitting]=0;
    
    int Force;
    read(sock,&Force,sizeof(int));
    cout<<"Force: "<<Force<<endl;

    read(sock,&member,sizeof(Member));
    sql.ShiftEventChangeData_Tem(member); 
    sql.Disconnect();  

    if(Force==0)
    {
        int LoginStatus=2; // 로그아웃 시그널
        cout<<"자리이동, 자진종료 =>나갔을때의 LoginStatus: "<<LoginStatus<<endl;
        m_mtx.lock();
        write(m_clnt_login_sock,&LoginStatus,sizeof(int));
        // 좌석 번호...전송
        ++Sitting;
        write(m_clnt_login_sock,&Sitting,sizeof(int));
        m_mtx.unlock();
        cout<<"자리이동, 자진종료 =>나갔을때의 Sitting: "<<Sitting<<endl;
    }
    return Sitting-1; 
}

void User_Handler::ChattingToDbAndKoo(int m_chat_sock, mutex& m_mtx,int sock)
{
    SQL sql;
    sql.Connect();
    // 좌석 받고
    int Sitting;
    read(sock,&Sitting,sizeof(int));
    cout<<"Sitting: "<<Sitting<<endl;

    char Msg[200];
    read(sock,Msg,sizeof(char)*200);
  
    string Message(Msg);
    int Writer=1;
    sql.UpdatChatting(Sitting,Writer,Msg);

    ++Sitting;
    cout<<"좌석: "<<Sitting<<endl; 
    m_mtx.lock();
    write(m_chat_sock,&Sitting,sizeof(int));
    write(m_chat_sock,Msg,sizeof(char)*200);
    m_mtx.unlock();
}