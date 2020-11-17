#include "Pay_Server.h"

Server::Server()
{
    // 서버 소켓 설정 - 키오스크 클라이언트 받기위한 소켓  
    int nSockOpt = 1;
    pay_serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&pay_serv_adr, 0, sizeof(pay_serv_adr));
	pay_serv_adr.sin_family = AF_INET; 
	pay_serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	pay_serv_adr.sin_port = htons(KIOSK_CLNT_PORT);
    // 포트 bind에러나도 바로 접속가능하게 설정  
    setsockopt(pay_serv_sock, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt)); 
	if(bind(pay_serv_sock, (struct sockaddr*) &pay_serv_adr, sizeof(pay_serv_adr))==-1)
		Error_Handling("bind error");

    // PC방 관리서버 통신용 소켓 설정 - 결제서버에서 관리서버로 connect
    pc_serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&pc_serv_adr, 0, sizeof(pc_serv_adr));
	pc_serv_adr.sin_family=AF_INET;
	pc_serv_adr.sin_addr.s_addr=inet_addr(PC_SERV_IP);
	pc_serv_adr.sin_port=htons(PC_SERV_PORT);    
}

Server::~Server()
{
    close(pay_serv_sock);   
}

void Server::Error_Handling(string msg)
{
    cout << msg << endl;
	exit(1);
}

void Server::Connect_To_PC_Server()
{
    int type = TYPE_PAY_SERV;
    if(connect(pc_serv_sock, (struct sockaddr*)&pc_serv_adr, sizeof(pc_serv_adr))==-1)
		Error_Handling("connect() error");
    write(pc_serv_sock, &type, sizeof(int));
    thread thd(&Server::PC_Serv_Thread, this); // PC방 관리서버 read 쓰레드
    thd.detach();
}

// PC방 관리 서버 -> 결제 관리 서버
void Server::PC_Serv_Thread() 
{
    PC_Serv_Handler pc_serv;
    int data_size, signal = 0;
    while(1)
    {
        data_size = read(pc_serv_sock, &signal, sizeof(int));
        if(data_size == 0) // 클라이언트 종료 
        {
            break;
        }
        else if(data_size != -1) // 전송한 신호가 있는경우 
        {
            switch(signal)
            {
                case PC_SERV_SIG::CHECK_ID:
                    pc_serv.Send_Check_Result(pc_serv_sock);
                    break;
                case PC_SERV_SIG::GET_REMAIN:
                    pc_serv.Send_Remain(pc_serv_sock);
                    break;
                case PC_SERV_SIG::CHARGE_TIME_MEMBER:
                    pc_serv.Charge_Time_Member(pc_serv_sock);
                    break;
                case PC_SERV_SIG::CHARGE_TIME_TEMP:
                    pc_serv.Charge_Time_Temp(pc_serv_sock);
                    break;
                case PC_SERV_SIG::RECV_ITEM_PAY_INFO:
                    pc_serv.Recv_Item_Pay_Info(pc_serv_sock);
                    break;
            }
        }
    }
    close(pc_serv_sock);     
}

void Server::Server_On()
{
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    int clnt_adr_sz;

    if(listen(pay_serv_sock, 5)==-1)
	    Error_Handling("listen error");
	
	while(1)
	{
        clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(pay_serv_sock, (struct sockaddr*)&clnt_adr,(socklen_t *)&clnt_adr_sz);
		cout << "[ 결제 키오스크 Connect IP: " << inet_ntoa(clnt_adr.sin_addr) << " SOCKET: " << clnt_sock << " ]" << endl;

        kiosk_mtx.lock();
        kiosk_socks.push_back(clnt_sock);
        kiosk_mtx.unlock();
        thread thd(&Server::Kiosk_Thread, this, clnt_sock);
        thd.detach();          
	}
}

// 결제 키오스크 쓰레드
void Server::Kiosk_Thread(int kiosk_sock)
{
    Kiosk_Handler kiosk;
    int data_size, signal = 0;

    while(1)
    {
        data_size = read(kiosk_sock, &signal, sizeof(int));
        if(data_size == 0) // 클라이언트 종료 
        {
            break;
        }
        else if(data_size != -1) // 전송한 신호가 있는경우 
        {
            switch(signal)
            {
                case KIOSK_SIG::CHECK_ID:
                    kiosk.Check_Member(kiosk_sock, pc_serv_sock);                          
                    break;        
                case KIOSK_SIG::GET_REMAIN:
                    kiosk.Get_Remain(kiosk_sock, pc_serv_sock);
                    break;        
                case KIOSK_SIG::CHARGE_TIME:
                    kiosk.Charge_Time(kiosk_sock, pc_serv_sock);
                    break;                
            }
        }
    }
    Clnt_Disconnect(kiosk_sock);       
}

void Server::Clnt_Disconnect(int sock)
{
    vector<int>::iterator iter = kiosk_socks.begin(); 
    
    kiosk_mtx.lock();
    for(int i = 0; i < kiosk_socks.size(); i++)
    {
        if(kiosk_socks[i] == sock)
        {
            iter += i;
            kiosk_socks.erase(iter);
            break;
        }
    }
    kiosk_mtx.unlock();
    cout << "[ 결제 키오스크 Disconnect 소켓 : " << sock << " ]" << endl;
    close(sock);
}
