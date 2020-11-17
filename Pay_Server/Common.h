#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include "/usr/include/mariadb/mysql.h"

// DB연동 관련
#define DB_HOST "10.10.20.241"
#define DB_USER "root"
#define DB_PASS "123"
#define DB_NAME "Pay_DB_Min"
#define DB_PORT 3306
// TCP/IP 포트
#define PC_SERV_PORT 9073
#define PC_SERV_IP "10.10.20.240"
#define KIOSK_CLNT_PORT 9093

#define MEM_BUF 50

using namespace std;
// 소켓 타입 번호 
enum { TYPE_PAY_SERV = 1}; // 결제 서버 
enum { TYPE_MANAGER_MAIN = 2, TYPE_MANAGER_CLIENT_LOGIN, TYPE_MANAGER_TIME, TYPE_MANAGER_CHAT, TYPE_MANAGER_ORDER }; // 관리자
enum { TYPE_USER_LOGIN = 7, TYPE_USER_MAIN, TYPE_USER_CHARGE, TYPE_USER_CHAT, TYPE_USER_BAN }; // 사용자

namespace KIOSK_SIG
{
    enum { CHECK_ID = 1, GET_REMAIN, CHARGE_TIME };
}

namespace PC_SERV_SIG
{
    enum { CHECK_ID = 1, GET_REMAIN, CHARGE_TIME_MEMBER, CHARGE_TIME_TEMP, RECV_ITEM_PAY_INFO };
}

