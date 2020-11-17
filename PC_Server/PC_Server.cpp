#include "PC_Server.h"

Server::Server() : seat_size(0)
{
    int nSockOpt = 1;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; 
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(PORT);
    // 포트 bind에러나도 바로 접속가능하게 설정  
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt)); 
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		Error_Handling("bind error");
}

Server::~Server()
{
    delete[] user_list;
    delete[] seat_info;
    delete[] user_chat_socks;
    delete[] user_charge_socks;
    delete[] user_ban_socks;
     
    close(serv_sock);
}

void Server::Error_Handling(string msg)
{
    cout << msg << endl;
	exit(1);
}

void Server::Server_On()
{
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    int clnt_adr_sz;
    int clnt_type; // 연결한 클라이언트 종류 구분 변수    
    
    if(listen(serv_sock, 5)==-1)
	    Error_Handling("listen error"); 

	while(1)
	{
        clnt_type = 0;
        clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr,(socklen_t *)&clnt_adr_sz);
		cout << "[ 클라이언트 Connect IP: " << inet_ntoa(clnt_adr.sin_addr) << " SOCKET: " << clnt_sock << " ]" << endl;
		read(clnt_sock, &clnt_type, sizeof(int)); // 클라이언트 타입 받아옴 
        cout<<"타입: "<<clnt_type<<endl;
        switch(clnt_type)
        {
            case TYPE_PAY_SERV: // 결제 관리 서버 소켓
            {
                pay_serv_sock = clnt_sock;
                thread thd(&Server::Payment_Thread, this);
                thd.detach();
                break;
            }            
            case TYPE_MANAGER_MAIN: // 관리자 메인 소켓
            {
                Login_Handler login_handler;
                char login_id[20];
                int seat_cnt ;
                int len = 0;
                // 로그인에 성공하지 않거나 로그인창을 종료하지 않을동안 반복한다. 
                while(1)
                {
                    // 로그인 할 아이디를 입력받는다. 
                    cout << "[관리자 id 입력대기중..]" << endl;
                    
                    len = read(clnt_sock,login_id,sizeof(login_id));
                    cout << "login_id :" << login_id << endl;
                    if(len==0)
                    {
                        cout << "[로그인창 닫음,관리자 클라이언트 종료]" << endl;
                        break;
                    }
                    // 아이디 존재유무를 확인한다.
                    //seat_cnt = login_handler.CheckMIdCorrect(login_id);
                    // // 존재한다면 pc방 관리자모드로 전환한다. 
                    seat_cnt=25;
                    if(seat_cnt > 0)
                    {
                        // 로그인 성공메시지를 클라이언트에 전달하다.
                        cout << "[관리자 id일치,로그인성공]" << endl;
                        write(clnt_sock,&seat_cnt,sizeof(int));
                        break;
                    }
                    else if(seat_cnt != 1)
                    {
                        // 로그인 실패메시지를 클라이언트에 전달하다.
                        cout << "[관리자 id불일치,로그인실패]" << endl;
                        cout << "seat_cnt :" << seat_cnt << endl;
                        write(clnt_sock,&seat_cnt,sizeof(int));
                    }
                }         
                if(len != 0)
                {
                    // 관리자 클라이언트 -> 서버 통신을 담당하는 스레드 생성
                    thread thd(&Server::Manager_Thread, this, clnt_sock);
                    thd.detach();
                }           
                break;
            }
            case TYPE_MANAGER_CLIENT_LOGIN: // 관리자 Ready Read 로그인 소켓
                m_clnt_login_sock = clnt_sock;
                cout << "manager_sock :" << m_clnt_login_sock << endl;
                break;    
            case TYPE_MANAGER_TIME:
                m_time_sock = clnt_sock;
                cout << "manager_time_sock :" << m_time_sock << endl;
                break;    
            case TYPE_MANAGER_CHAT:
                m_chat_sock = clnt_sock;
                cout << "m_chat_sock :" << m_chat_sock << endl;
                break;
            case TYPE_MANAGER_ORDER:
                m_order_sock = clnt_sock;
                cout << "m_order_sock :" << m_order_sock << endl;
                break;        
            case TYPE_USER_LOGIN: // PC방 로그인위젯 소켓
            {
                cout<<"TYPE_USER_LOGIN:"<<endl;
                write(clnt_sock, &seat_size, sizeof(int)); // 좌석 수 전달
                thread thd(&Server::Login_Thread, this, clnt_sock);
                thd.detach();
                break;
            }
            case TYPE_USER_MAIN: // 유저 로그인
            {
                cout<<"TYPE_USER_MAIN " << endl;
                thread thd(&Server::User_Thread, this, clnt_sock);
                thd.detach();
                break;
            }
            case TYPE_USER_CHARGE: // 유저 로그인 후 시간충전 Ready Read 소켓 
            {
                cout<<"TYPE_USER_CHARGE " <<endl;
                int seat_num = 0;
                read(clnt_sock, &seat_num, sizeof(int));
                user_charge_mtx.lock();
                user_charge_socks[seat_num-1] = clnt_sock;   
                user_charge_mtx.unlock();
                break;              
            } 
            case TYPE_USER_CHAT: // 유저 로그인 후 채팅 Ready Read 소켓 
            {
                cout<<"TYPE_USER_CHAT " <<endl;
                int seat_num = 0;
                read(clnt_sock, &seat_num, sizeof(int));
                user_chat_mtx.lock();
                user_chat_socks[seat_num-1] = clnt_sock;   
                user_chat_mtx.unlock();
                break;              
            }           
            case TYPE_USER_BAN: // 유저 로그인 후 강제종료 Ready Read 소켓 
            {
                cout<<"TYPE_USER_BAN " <<endl;
                int seat_num = 0;
                read(clnt_sock, &seat_num, sizeof(int));
                user_ban_mtx.lock();
                user_ban_socks[seat_num-1] = clnt_sock;   
                user_ban_mtx.unlock();
                break;              
            }            
        }        
	}
}

// 결제 서버 쓰레드
void Server::Payment_Thread()
{
    cout << "[ 결제 서버 Connect, 소켓 : "<< pay_serv_sock << "]"<< endl;
    Payment_Handler payment;
    int data_size, signal = 0;

    while(1)
    {
        data_size = read(pay_serv_sock, &signal, sizeof(int));
        if(data_size == 0) // 클라이언트 종료 
        {
            break;
        }
        else if(data_size != -1) // 전송한 신호가 있는경우 
        {
            switch(signal)
            {
                case PAY_SERV_SIG::SEARCH_ID: // 회원여부 DB에 검색
                    payment.Check_Member(pay_serv_sock);
                    break;        
                case PAY_SERV_SIG::GET_REMAIN: // 회원 잔여시간 검색                  
                    payment.Get_Remain(pay_serv_sock, user_list, seat_size);
                    break;        
                case PAY_SERV_SIG::CHARGE_TIME_MEMBER: // 회원 시간충전
                {   
                    T_Pay_Info t_info;
                    int idx = 0;
                    read(pay_serv_sock, &t_info, sizeof(t_info)); // 결제 정보받음
                    cout << "t_info.Get_Mem_Stauts " << t_info.Get_Mem_Status() << endl;
                    cout << "t_info.Get_Mem_Id() : " << t_info.Get_Mem_Id() << endl;
                    if(t_info.Get_Mem_Status() == 1) // 로그인중인 회원
                    {
                        for(int i = 0; i < seat_size; i++)
                        {
                            cout << "m[i].GetMemberID : " << user_list[i].GetMemberId() << endl;
                            if(!strcmp(user_list[i].GetMemberId(), t_info.Get_Mem_Id()))
                            {
                                idx = i;
                                break;
                            }
                        }    
                        cout << "로그인 회원 시간 충전" << endl;
                        cout << "좌석idx : " << idx << endl;
                        cout << "charge_sock : " << user_charge_socks[idx] << endl;
                        payment.Charge_Time_Member(pay_serv_sock, t_info, user_charge_socks[idx]);                    
                    }
                    else // 비로그인 회원
                    {
                        cout << "비로그인 회원 시간 충전" << endl;
                        payment.Charge_Time_Member(pay_serv_sock, t_info);
                    }
                    break;
                }
                case PAY_SERV_SIG::CHARGE_TIME_TEMP: // 비회원 시간충전
                    payment.Charge_Time_Temp(pay_serv_sock);
                    break;
            }
        }
    }
    close(pay_serv_sock);    
}

// 관리자 클라 쓰레드
void Server::Manager_Thread(int sock)
{
    int seat_num;
    char chat_msg[200];
    int chat_cnt;
    cout << "[ 관리자 클라 Connect, 소켓 : "<< sock << "]"<< endl;
    
    read(sock, &seat_size, sizeof(int)); // 관리자에게 좌석 수 받음
    user_list = new Member[seat_size]; // 이용자 정보
    seat_info = new int[seat_size]; // 좌석 이용 정보
    memset(seat_info,0,sizeof(int)*seat_size);    
    user_chat_socks = new int[seat_size]; // 이용자 채팅소켓
    memset(user_chat_socks,0,sizeof(int)*seat_size);
    user_charge_socks = new int[seat_size]; // 이용자 시간충전 소켓
    memset(user_charge_socks,0,sizeof(int)*seat_size);
    user_ban_socks = new int[seat_size]; // 이용자 강제종료 소켓
    memset(user_ban_socks,0,sizeof(int)*seat_size);
    
    Manager_Handler manager;
    int data_size, signal = 0;
    int order_num,order_update_res;

     while(1)
    {
        cout << "시그널 대기중..." << endl;
        data_size = read(sock, &signal, sizeof(int));
        if(data_size == 0) // 클라이언트 종료 
        {
            break;
        }
        else if(data_size != -1) // 전송한 신호가 있는경우 
        {
            switch(signal)
            {   // 태환                
                case TYPE_ORDER_PROC: 
                    read(sock,&order_num,sizeof(int));
                    order_update_res = manager.ProcOrder(order_num);
                    write(sock,&order_update_res,sizeof(int));
                    // 결제정보 결제서버에 전송
                    manager.Send_Order_To_Pay_Serv(order_num, pay_serv_sock);
                    break;        
                case TYPE_ORDER_CANCEL: 
                    read(sock,&order_num,sizeof(int));
                    order_update_res = manager.CancelOrder(order_num);
                    write(sock,&order_update_res,sizeof(int));
                    break;
                case TYPE_CHAT_REQUEST: 
                    // 채팅할 좌석을 전달받는다. 
                    cout << "case TYPE_CHAT_REQUEST:" << endl;
                    read(sock,&seat_num,sizeof(int));
                    cout << "어느 좌석과의 채팅기록?? :" << seat_num << endl;
                    // 출력할 채팅개수를 보낸다. 
                    chat_cnt = manager.GetMsgCnt(seat_num);
                    cout << "채팅기록의 개수는?? :" << chat_cnt << endl;
                    write(sock,&chat_cnt,sizeof(int));
                    if(chat_cnt > 0)
                    { // 채팅기록을 전달하다.
                        manager.SendMsgToManager(sock,seat_num,chat_cnt);
                    }    
                    break;
                case TYPE_CHAT_CLIENT: // 선택채팅
                    read(sock,&seat_num,sizeof(int)); // 대화할 좌석번호
                    cout << "선택된 좌석 :" << seat_num << endl;
                    read(sock,chat_msg,sizeof(char)*200); // 전달할 채팅글
                    cout << "[Chat_MSG] : " << chat_msg << endl;
                    // 채팅글을 전달하다.
                    manager.SendMsgToClient(user_chat_socks,0,seat_num,chat_msg,seat_size);
                    break;
                case TYPE_CHAT_ALL_CLIENT: // 전체채팅
                    read(sock,chat_msg,sizeof(char)*200);
                    manager.SendMsgToClient(user_chat_socks,1,0,chat_msg,seat_size);
                    break;
                case TYPE_OUT_CLIENT:
                    // 강퇴할 좌석번호를 받는다. 
                    cout<<"강퇴 ㅁㅇㄴㄻㄴㅇㄻㄴㅇ"<<endl;
                    read(sock,&seat_num,sizeof(int));
                    // 강퇴할 이용자에게 강퇴 메시지를 전달한다.,데이터베이스의 강퇴회원 상태를 변경한다. 
                    write(user_ban_socks[seat_num-1],"1",strlen("1"));
                    break; 
                //준혁
                case ManagementSig::MEMBERS_MANAGEMENT: //회원 관리 함수
                {
                    manager.Management(sock);
                    signal=0;
                    break;        
                }
                case ManagementSig::MEMBERS_MODIFY: //회원 정보 수정 함수
                {
                    manager.InfoModify(sock);
                    signal=0;
                    break;  
                }
                case ManagementSig::MEMBERS_DELETE: //회원 정보 삭제 함수
                {
                    manager.InfoDelete(sock);
                    signal=0;
                    break;      
                }
                case ManagementSig::DAY_SERACH: //당일 매출 내역 함수
                {
                    manager.onSale(sock,signal);
                    signal=0;
                    break; 
                }
                case ManagementSig::MONTH_CHOICE: //월별 매출 내역 함수
                {
                    manager.onSale(sock,signal);
                    signal=0;
                    break; 
                }
                case ManagementSig::DAY_CHOICE: //일별 매출 내역 함수
                {
                    manager.onSale(sock,signal);
                    signal=0;
                    break; 
                } 
            }
        }
    }
    close(m_clnt_login_sock); 
    close(m_time_sock); 
    close(m_chat_sock); 
    close(m_order_sock); 
    close(sock);      
}

// 로그인 클라 쓰레드
void Server::Login_Thread(int sock)
{
    cout << "[ 로그인 클라 Connect, 소켓 : "<< sock << "]"<< endl;    
    Login_Handler log;
    int data_size, signal = 0;

    while(1)
    {
        data_size = read(sock, &signal, sizeof(int));       
        cout<<"Login_Thread >> signal: "<<signal <<endl;
        if(data_size == 0) // 클라이언트 종료 
        {
            break;
        }
        else if(data_size != -1) // 전송한 신호가 있는경우 
        {
            switch(signal)
            {
                case 1:
                    log.CreateAccountOverlap(sock);
                    break;        
                case 2:
                    log.CreateAccountComplete(sock);
                    break;
                case 3:
                    log.FindAccount(sock);
                    break;
                case 4:
                    log.FindAccountUpdate(sock,seat_size,user_list);
                    break;
                case 5:
                    log.Log_In(sock,seat_info);   
                    break;    
                case 6:
                    log.SendMemberClass(m_clnt_login_sock, m_mtx[0], sock,seat_info,seat_size,user_list);
                    break;
            }
        }
    }
    close(sock);   
}
// 유저 클라 쓰레드
void Server::User_Thread(int sock)
{
    cout << "[ 유저 클라 Connect, 소켓 : "<< sock << "]"<< endl;
    User_Handler user;
    int data_size, signal = 0;
    int seat_num = 0;

    while(1)
    {
        data_size = read(sock, &signal, sizeof(int));
        cout<<"User_Thread >> signal: "<<signal <<endl;
        if(data_size == 0) // 클라이언트 종료 
        {
            break;
        }
        else if(data_size != -1) // 전송한 신호가 있는경우 
        {
            switch(signal)
            {
                case 1:
                    cout<<"SendMinuteToKoo(m_time_sock, sock);"<<endl;
                    user.SendMinuteToKoo(m_time_sock, m_mtx[1], sock);
                    break;        
                case 2:
                    cout<<"SendMenuToWoo(sock);"<<endl;
                    user.SendMenuToWoo(sock);
                    break;      
                case 3:
                    cout<<"SendArrangeToKoo..."<<endl;
                    user.SendArrangementToKoo(m_order_sock, m_mtx[3], sock);
                    break;
                case 4:
                    cout<<"TimeOutFromWoo..."<<endl;
                    seat_num = user.TimeOutFromWoo(m_clnt_login_sock,m_mtx[0], sock,seat_info,user_list);
                    break;
                case 6:
                    cout<<"회원 자리 이동과 종료..."<<endl;
                    seat_num = user.SlideDownOn_Mem(m_clnt_login_sock,m_mtx[0],sock,seat_info);
                    break;
                case 7:
                    cout<<"비회원 자리 이동과 종료"<<endl;
                    seat_num = user.SlideDownOn_Tem(m_clnt_login_sock,m_mtx[0],sock,seat_info);
                    break;
                case 8:
                    cout<<"쿠에게 채팅 전송 함수 발동"<<endl;
                    user.ChattingToDbAndKoo(m_chat_sock, m_mtx[2], sock);
                    break;
            }
        }
    }
    user_charge_mtx.lock();
    close(user_charge_socks[seat_num]);
    user_charge_socks[seat_num] = 0;
    user_charge_mtx.unlock();

    user_chat_mtx.lock();
    close(user_chat_socks[seat_num]);
    user_chat_socks[seat_num] = 0;
    user_chat_mtx.unlock();

    user_ban_mtx.lock();
    close(user_ban_socks[seat_num]);
    user_ban_socks[seat_num] = 0;   
    user_ban_mtx.unlock();

    close(sock);      
}
