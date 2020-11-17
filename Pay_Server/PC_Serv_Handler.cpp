#include "PC_Serv_Handler.h"

void PC_Serv_Handler::Send_Check_Result(int pc_serv_sock)
{
    int kiosk_sock, result;

    read(pc_serv_sock, &result, sizeof(int));
    read(pc_serv_sock, &kiosk_sock, sizeof(int));
    //cout << " result : "<< result << endl;
    write(kiosk_sock, &result, sizeof(int));
}

void PC_Serv_Handler::Send_Remain(int pc_serv_sock)
{
    int kiosk_sock;
    Member mem;

    read(pc_serv_sock, &kiosk_sock, sizeof(int));
    read(pc_serv_sock, &mem, sizeof(Member));
    /*
    cout << mem.Get_Mem_Id() << endl;
    cout << mem.Get_Password() << endl;
    cout << mem.Get_Mem_Name() << endl;
    cout << mem.Get_Remain() << endl;
    */
    write(kiosk_sock, &mem, sizeof(Member));
}

void PC_Serv_Handler::Charge_Time_Member(int pc_serv_sock)
{
    T_Pay_Info t_info;
    SQL sql;

    read(pc_serv_sock, &t_info, sizeof(T_Pay_Info)); // 충전 정보 받음
    sql.Connect();    
    sql.Insert_Time_Payment(t_info);// DB에 결제정보 저장
    sql.Disconnect();    
    //cout << "회원 충전 끝" << endl;
}

void PC_Serv_Handler::Charge_Time_Temp(int pc_serv_sock)
{
    int kiosk_sock, temp_mem_id;
    T_Pay_Info t_info;
    SQL sql;

    read(pc_serv_sock, &kiosk_sock, sizeof(int)); // 전송할 키오스크 소켓받음
    read(pc_serv_sock, &temp_mem_id, sizeof(int)); // 비회원 아이디 받음
    read(pc_serv_sock, &t_info, sizeof(T_Pay_Info)); // 충전 정보 받음

    write(kiosk_sock, &temp_mem_id, sizeof(int)); // 키오스크에 비회원 아이디 전송
    sql.Connect();    
    sql.Insert_Time_Payment(t_info, temp_mem_id);// DB에 결제정보 저장
    sql.Disconnect(); 
    //cout << "비회원 충전 끝" << endl;    
}

void PC_Serv_Handler::Recv_Item_Pay_Info(int pc_serv_sock)
{
    I_Pay_Info i_info;
    SQL sql;

    read(pc_serv_sock, &i_info, sizeof(I_Pay_Info)); // 관리서버에서 정보받음
    cout << "arr_id : " << i_info.Get_Arr_Id() << endl;
    cout << "payment : " << i_info.Get_Payment() << endl;
    cout << "price : " << i_info.Get_Price() << endl;

    sql.Connect();
    sql.Insert_Item_Payment(i_info); // DB에 결제정보 저장
    sql.Disconnect();

} 



