#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <ctime>
#include <mutex>
#include "/usr/include/mariadb/mysql.h"

// DB연동 관련
#define DB_HOST "10.10.20.240"
#define DB_USER "root"
#define DB_PASS "123"
//#define DB_NAME "PC_Project_min"
#define DB_NAME "PC_koo"
#define DB_PORT 3306
// TCP/IP 서버포트
#define PORT 9073
//#define PORT 9013
using namespace std;

enum { TYPE_PAY_SERV = 1}; // 결제 서버 
enum { TYPE_MANAGER_MAIN = 2, TYPE_MANAGER_CLIENT_LOGIN, TYPE_MANAGER_TIME, TYPE_MANAGER_CHAT, TYPE_MANAGER_ORDER }; // 관리자
enum { TYPE_USER_LOGIN = 7, TYPE_USER_MAIN, TYPE_USER_CHARGE, TYPE_USER_CHAT, TYPE_USER_BAN }; // 사용자

//준혁 사용
namespace ManagementSig
{
    enum{ MEMBERS_MANAGEMENT=1,MEMBERS_MODIFY=8,MEMBERS_DELETE,DAY_SERACH,DAY_CHOICE,MONTH_CHOICE };
}

//태환 사용
enum {TYPE_ORDER_PROC=2,TYPE_ORDER_CANCEL,TYPE_CHAT_REQUEST,TYPE_CHAT_CLIENT,TYPE_CHAT_ALL_CLIENT,TYPE_OUT_CLIENT,TYPE_GET_CHAT_RECORD,TYPE_GET_ALLCHAT_RECORD};
enum { TYPE_ADD_TIME=12, TYPE_G_CHAT };

// 민근 사용
#define MEM_BUF 50
namespace PAY_SERV_SIG
{
    enum { SEARCH_ID = 1, GET_REMAIN, CHARGE_TIME_MEMBER, CHARGE_TIME_TEMP,  SEND_ITEM_PAY_INFO };
}
// 형섭 사용
namespace LINE_CONTROL
{
    enum{
        SHIFT=3, UPSTAIRS=25, DOWNSTAIRS=30, PLUS_X=200
    };
}

namespace DELAY
{
    enum{
        TIME_1=100,TIME_2=200,TIME_3=300,TIME_4=400,TIME_5=500,TIME_OMEGA=1000
    };
}

namespace LOG_IN
{
    enum{
        SUCESS=-1,WRONG=1,NOT_ENOUGH,OVER_LAP
    };
}

namespace CATEGORY
{
    enum{
        NOODLE,BEVERAGE,SNACK,RICE
    };
}

namespace PHONE_NUMBER_FORMAT
{
    enum{
        BOTTOM=3,MIDDLE
    };
}

namespace MEMBERSHIP
{
    enum{
      MEM=1,TEM
    };
}

namespace DefineItem
{
    enum{
        BUFFSIZE=50
    };
}

namespace DefineBaggage
{
    enum{
        MAXSIZE=5
    };
}

namespace DefineMember
{
    enum{
        MEM=1,TEM,BUFFSIZE=50
    };
}

namespace DefineWidget
{
    enum{
        PERMANENT=1, TEMPORARY,USER=4, IN, PASS, BUF_SIZE=1024
    };
}

namespace DefineCreate
{
    enum{
        OVERLAP=1,COMPLETE
    };
}

namespace DefineFind
{
    enum{
        NAME=1,PW,FIND
    };
}

namespace DefineUpdate
{
    enum{
        UPDATE=4
    };
}

namespace DefineMain
{
    enum{
        DECREASE=1,TIMEOUT=4,MAIN=5,MEM_SAVETIME,TEM_SAVETIME,PLUS_TIME_IMMEDIATELY=9,SOCK_ADD_TIME=12,SOCK_CHAT,SOCK_KICK
    };
}

namespace DefineCook
{
    enum{
        MENU=2,PAYMENT,MAX_CART=5
    };
}

namespace DefinePay
{
    enum{
        CASH=1,CARD=2
    };
}
