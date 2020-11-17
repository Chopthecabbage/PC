#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <QDebug>

#define MIDDLE_PORT 9033
// #define BUFFSIZE 50
enum { TYPE_MANAGER_MAIN = 2, TYPE_MANAGER_CLIENT_LOGIN, TYPE_MANAGER_TIME, TYPE_MANAGER_CHAT, TYPE_MANAGER_ORDER };

enum {TYPE_ORDER_PROC=2,TYPE_ORDER_CANCEL,TYPE_CHAT_REQUEST,TYPE_CHAT_CLIENT,TYPE_CHAT_ALL_CLIENT,TYPE_OUT_CLIENT,TYPE_GET_CHAT_RECORD,TYPE_GET_ALLCHAT_RECORD};

namespace ManagementSig //준혁
{
    enum{
        MEMBERS_MANAGEMENT=1,MEMBERS_MODIFY=8,MEMBERS_DELETE,DAY_SERACH,DAY_CHOICE,MONTH_CHOICE
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
        BUFFSIZE=50
    };
}

namespace DefineWidget
{
    enum{
        PERMANENT=1, TEMPORARY,USER=4, IN, PASS, SEND, BUF_SIZE=1024
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
        DECREASE=1,MAIN=5
    };
}

namespace DefineCook
{
    enum{
        MENU=2
    };
}
#endif // COMMON_H
