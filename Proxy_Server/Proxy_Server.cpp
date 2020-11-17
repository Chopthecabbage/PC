#include "Proxy_Server.h"

Server::Server()
{
    int nSockOpt = 1;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; 
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(QT_CLNT_PORT);
	
    // 포트 bind에러나도 바로 접속가능하게 설정  
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt)); 
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		Error_Handling("bind error");    
}

Server::~Server()
{
    close(serv_sock);
}

void Server::Server_On()
{
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;

    if(listen(serv_sock, 5)==-1)
	    Error_Handling("listen error");

    while(1)
	{
		clnt_adr_sz =sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, (socklen_t *)&clnt_adr_sz);
        cout << "클라연결 소켓 : " << clnt_sock << endl;
        thread thd(&Server::Proxy_Thread, this, clnt_sock); // 중계 서버 생성
        thd.detach();		
	}
	close(serv_sock);
}

void Server::Proxy_Thread(int qt_clnt_sock)
{
    int pc_serv_sock;
	struct sockaddr_in pc_serv_addr;
	
	// 라즈베리파이에 있는 PC관리 서버에 접속하기 위한 소켓 생성
	pc_serv_sock = socket(PF_INET, SOCK_STREAM, 0);	
	memset(&pc_serv_addr, 0, sizeof(pc_serv_addr));
	pc_serv_addr.sin_family=AF_INET;
	pc_serv_addr.sin_addr.s_addr=inet_addr(PC_SERV_IP);
	pc_serv_addr.sin_port=htons(PC_SERV_PORT);

	// PC관리 서버에 연결  
	if(connect(pc_serv_sock, (struct sockaddr*)&pc_serv_addr, sizeof(pc_serv_addr))==-1)
		Error_Handling("connect() error");
	
	// Qt클라이언트, PC관리서버 각각 read 쓰레드 생성 
	thread thd1(&Server::Data_From_QT_Client, this, qt_clnt_sock, pc_serv_sock);
	thread thd2(&Server::Data_From_PC_Server, this, qt_clnt_sock, pc_serv_sock);
	thd1.join();		
	thd2.join();	
	cout << "중계서버 쓰레드 종료" <<endl;		
}

void Server::Data_From_QT_Client(int qt_clnt_sock, int pc_serv_sock)
{
	int data_len = 0;
	char buf[BUF_SIZE] = {0};
	// Qt클라이언트 -> 중계서버 -> PC관리서버
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		data_len = read(qt_clnt_sock, buf, sizeof(buf));
		cout << "data_len : " << data_len << endl;
		if(data_len == 0) // Disconnect
		{
			cout << "QT Disconnect" << endl;			
			break;
		}
		else if(data_len != -1) // Qt클라한테 정상 데이터 수신
		{			
			cout << "data from Qt / write to Server >> data_len : " << data_len << endl;
			write(pc_serv_sock, buf, data_len); // 받은데이터 PC관리 서버에 전송				
		}
	}	
	close(qt_clnt_sock);
	shutdown(pc_serv_sock, SHUT_RDWR); // Qt클라 disconnect시 Data_From_PC_Server 쓰레드도 강제 종료 
}

void Server::Data_From_PC_Server(int qt_clnt_sock, int pc_serv_sock)
{
	char buf[BUF_SIZE] = {0};
	int data_len = 0;

	// PC관리서버 -> 중계서버 -> Qt클라이언트
	while(1)
	{
		memset(buf, 0, sizeof(buf));		
		data_len = read(pc_serv_sock, buf, sizeof(buf));
		cout << "data_len : " << data_len << endl;
		if(data_len == 0)
		{
			cout << "Serv Disconnect" << endl;			
			break;
		}
		else if(data_len != -1)
		{
			cout << "data from Serv / write to QT  >> data_len : " << data_len << endl;
			write(qt_clnt_sock, buf, data_len);
		}
	}	
}

void Server::Error_Handling(string msg)
{
    cout << msg << endl;
	exit(1);
}