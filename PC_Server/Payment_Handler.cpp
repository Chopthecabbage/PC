#include "Payment_Handler.h"

void Payment_Handler::Check_Member(int pay_serv_sock)
{
    SQL sql;    
    char temp_id[DefineMember::BUFFSIZE];
    int kiosk_sock, signal , check; 

    read(pay_serv_sock, temp_id, sizeof(temp_id));
    read(pay_serv_sock, &kiosk_sock, sizeof(int));
    string check_id(temp_id);
    //cout << "검색할 id : " << temp_id <<" / " << check_id << endl;
    sql.Connect();
    check = sql.Select_Exists_Member(check_id);
    sql.Disconnect();
       
    //cout << "check : " << check << endl;
    signal =  PAY_SERV_SIG::SEARCH_ID; // 1
    write(pay_serv_sock, &signal, sizeof(signal));
    write(pay_serv_sock, &check, sizeof(int));    
    write(pay_serv_sock, &kiosk_sock, sizeof(int)); // 키오스크 소켓정보도 전송
}

void Payment_Handler::Get_Remain(int pay_serv_sock, Member* member, int seat_size)
{
    SQL sql;    
    MYSQL_RES *result;
    MYSQL_ROW row;
    char temp_id[DefineMember::BUFFSIZE];
    int kiosk_sock, signal;   
        
    read(pay_serv_sock, temp_id, sizeof(temp_id));
    read(pay_serv_sock, &kiosk_sock, sizeof(int));
    string check_id(temp_id);
    // cout << "get_remain 검색할 id : " << check_id << endl;    
    
    Member temp_mem;
    sql.Connect();             
    result = sql.Select_Member_By_ID(check_id);    
    row = mysql_fetch_row(result);  
    temp_mem.SettingNotSeatAndBegin((char*)row[0], row[1], row[2], row[3], row[4], atoi(row[5]), row[6]);
    signal =  PAY_SERV_SIG::GET_REMAIN; // 2
    write(pay_serv_sock, &signal, sizeof(int));
    write(pay_serv_sock, &kiosk_sock, sizeof(int));        
    write(pay_serv_sock, &temp_mem, sizeof(Member));
    sql.Disconnect();
}

void Payment_Handler::Charge_Time_Member(int pay_serv_sock, T_Pay_Info t_info, int charge_sock)
{    
    if(charge_sock > 0) // 로그인중인 회원인경우
    {
        cout << "로그인회원" << endl;
        write(charge_sock, t_info.Get_Charge_Time(), sizeof(char)*MEM_BUF); // 충전시간 전달
    }   
    else // 비로그인 회원인경우
    {
        cout << "비로그인회원" << endl;
        SQL sql;
        string mem_id(t_info.Get_Mem_Id());
        string remain(t_info.Get_Mem_Remain());
        sql.Connect();
        sql.Update_Member_Remain(mem_id, remain); // DB에 회원 잔여시간 변경
        sql.Disconnect();
    }
    int signal = PAY_SERV_SIG::CHARGE_TIME_MEMBER; // 결제서버에 결제정보 전송
    write(pay_serv_sock, &signal, sizeof(int));       
    write(pay_serv_sock, &t_info, sizeof(T_Pay_Info));             
}      

void Payment_Handler::Charge_Time_Temp(int pay_serv_sock)
{
    SQL sql;
    T_Pay_Info t_info;
    int kiosk_sock;    
    int signal, temp_mem_id;
    
    read(pay_serv_sock, &kiosk_sock, sizeof(int)); // 키오스크 소켓
    read(pay_serv_sock, &t_info, sizeof(t_info)); // 결제 정보

    string remain(t_info.Get_Mem_Remain());
    sql.Connect();
    temp_mem_id = sql.Insert_Temp_Member(remain); // 비회원 생성, 비회원id 반환
    sql.Disconnect();

    cout << "비회원_id : " << temp_mem_id << endl;
    
    signal = PAY_SERV_SIG::CHARGE_TIME_TEMP; // 결제서버에 결제정보 전송
    write(pay_serv_sock, &signal, sizeof(int));
    write(pay_serv_sock, &kiosk_sock, sizeof(int));
    write(pay_serv_sock, &temp_mem_id, sizeof(int));
    write(pay_serv_sock, &t_info, sizeof(T_Pay_Info));        
}

