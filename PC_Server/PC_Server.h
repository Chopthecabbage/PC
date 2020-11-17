#pragma once
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Common.h"
#include "SQL.h"
#include "Member.h"
#include "T_Pay_Info.h"
#include "Payment_Handler.h"
#include "Manager_Handler.h"
#include "Login_Handler.h"
#include "User_Handler.h"

class Server
{
private:
    int serv_sock; // 서버 소켓
    struct sockaddr_in serv_adr; // 주소 구조체  
    Member *user_list; // 로그인중인 회원정보
    int *seat_info; // PC방 좌석 정보  
    int seat_size; // PC방 좌석 수
    // 결제서버
    int pay_serv_sock; // 결제 서버 소켓
    // 관리자
    int m_clnt_login_sock; // 관리자에게 로그인정보 전달할 소켓
    int m_time_sock; // 관리자에게 남은시간정보 전달할 소켓
    int m_chat_sock; // 관리자에게 채팅정보 전달할 소켓
    int m_order_sock; // 관리자에게 주문정보 전달할 소켓
    mutex m_mtx[4]; // 위쪽 소켓 4개 뮤텍스 0:기본 1:시간충전 2:채팅 3:주문
    // 사용자
    int *user_chat_socks; // 유저 채팅 ReadyRead sock
    mutex user_chat_mtx;  // 유저 채팅 ReadyRead sock 뮤텍스
    int *user_charge_socks; // 유저 시간충전 ReadyRead sock
    mutex user_charge_mtx;  // 유저 시간충전 ReayRead sock 뮤텍스
    int *user_ban_socks; // 유저 강제종료 ReadyRead sock
    mutex user_ban_mtx; // 우저 강제종료 ReadyRead sock 뮤텍스  
        
public:
    Server(); // 서버 소켓 설정
    ~Server(); // 서버 소켓 close
    void Error_Handling(string msg); // 에러시 출력 후 프로그램 종료
    void Server_On(); // listen 상태 시작 및 클라이언트 연결 처리   
    void Payment_Thread(); // 결제 관리 서버 쓰레드
    void Manager_Thread(int sock); // 관리자 클라이언트 쓰레드
    void Login_Thread(int sock);   // 유저 로그인창 클라이언트 쓰레드
    void User_Thread(int sock);    // PC방 유저 클라이언트 쓰레드    
};
