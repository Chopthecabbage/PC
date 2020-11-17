#pragma once
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Common.h"
#include "SQL.h"
#include "Kiosk_Handler.h"
#include "PC_Serv_Handler.h"

class Server
{
private:
    int pay_serv_sock; // kiosk 클라이언트 받기위한 서버 소켓
    struct sockaddr_in pay_serv_adr;
    int pc_serv_sock; // PC방 관리 서버 통신용 소켓
    struct sockaddr_in pc_serv_adr;     
    vector <int> kiosk_socks; // 시간충전 키오스크 소켓
    mutex kiosk_mtx; // 시간충전 키오스크 관리 뮤텍스 
public:
    Server(); // 서버 소켓 설정
    ~Server(); // 서버 소켓 close
    void Error_Handling(string msg); // 에러시 출력 후 프로그램 종료
    void Connect_To_PC_Server(); // PC방 관리 서버에 연결
    void PC_Serv_Thread(); // PC방 관리 서버 Read 쓰레드
    void Server_On(); // listen 상태 시작 및 클라이언트 연결 처리   
    void Kiosk_Thread(int sock); // 시간충전 키오스크 클라이언트 쓰레드
    void Clnt_Disconnect(int sock); // 클라이언트 Discoonect처리       
};
