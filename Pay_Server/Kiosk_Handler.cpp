#include "Kiosk_Handler.h"

void Kiosk_Handler::Check_Member(int kiosk_sock, int pc_serv_sock)
{
    char check_id[MEM_BUF];
    int signal;
    
    read(kiosk_sock, check_id, sizeof(check_id)); // 검색할 아이디 받아옴
    //cout << "검색할 id : " << check_id << endl;
    
    // 관리서버 DB에서 검색하기위해 전송 
    signal = KIOSK_SIG::CHECK_ID;
    write(pc_serv_sock, &signal, sizeof(int));
    write(pc_serv_sock, check_id, sizeof(check_id));   
    write(pc_serv_sock, &kiosk_sock, sizeof(int)); // 키오스크 소켓정보도 전송  
}

void Kiosk_Handler::Get_Remain(int kiosk_sock, int pc_serv_sock)
{
    char check_id[MEM_BUF];
    int signal;

    read(kiosk_sock, check_id, sizeof(check_id)); // 검색할 아이디 받아옴
    //cout << "검색할 id : " << check_id << endl;
    
    // 관리서버 DB에서 검색하기위해 전송 
    signal = KIOSK_SIG::GET_REMAIN;
    write(pc_serv_sock, &signal, sizeof(int));
    write(pc_serv_sock, check_id, sizeof(check_id));   
    write(pc_serv_sock, &kiosk_sock, sizeof(int)); // 키오스크 소켓정보도 전송  
}

void Kiosk_Handler::Charge_Time(int kiosk_sock, int pc_serv_sock)
{
    T_Pay_Info t_info;    
    int signal;

    read(kiosk_sock, &t_info, sizeof(t_info));
    
    cout << t_info.Get_Mem_Id() << endl;
    cout << t_info.Get_Mem_Remain() << endl;
    cout << t_info.Get_Mem_Status() << endl;
    cout << t_info.Get_Price() << endl;
    cout << t_info.Get_Payment() << endl;    
    t_info.Get_Mem_Type() == 1 ? cout << "회원" << endl : cout << "비회원" << endl;
    
    if(t_info.Get_Mem_Type() == 1) // 회원
    {
        cout << "회원 시간 충전" << endl;
        signal=PC_SERV_SIG::CHARGE_TIME_MEMBER;
        cout << "t_info.status  " << t_info.Get_Mem_Status() << endl;
        write(pc_serv_sock, &signal, sizeof(int)); // 시그널
        write(pc_serv_sock, &t_info, sizeof(t_info)); // 결제 정보        
    }
    else // 비회원
    {
        signal=PC_SERV_SIG::CHARGE_TIME_TEMP;
        write(pc_serv_sock, &signal, sizeof(int)); // 시그널
        write(pc_serv_sock, &kiosk_sock, sizeof(int)); // 키오스크 소켓
        write(pc_serv_sock, &t_info, sizeof(t_info)); // 결제 정보
    }    
}
