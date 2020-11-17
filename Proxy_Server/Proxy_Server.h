#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#define BUF_SIZE 2048
#define QT_CLNT_PORT 9063
//9063 
#define PC_SERV_PORT 9073
//9073
#define PC_SERV_IP "10.10.20.240"
using namespace std;

class Server
{
private:
    int serv_sock; // 서버 소켓
    struct sockaddr_in serv_adr; // 주소 구조체      

public:    
     Server(); // 서버 소켓 설정
    ~Server(); // 서버 소켓 close
    void Server_On(); // listen 상태 시작 및 클라이언트 연결 처리   
    void Proxy_Thread(int qt_clnt_sock); // 중계서버 생성
    void Data_From_QT_Client(int qt_clnt_sock, int pc_serv_sock); // 클라이언트 read 쓰레드
    void Data_From_PC_Server(int qt_clnt_sock, int pc_serv_sock); // 서버 read 쓰레드 
    void Error_Handling(string msg); // 에러시 출력 후 프로그램 종료
};