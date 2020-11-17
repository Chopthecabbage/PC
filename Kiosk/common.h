#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QDialog>
#include <QWidget>
#include <QTabWidget>
#include <QRadioButton>
#include <QPixmap>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QMenuBar>
#include <QAction>
#include <QShortcut>
#include <QTimer>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QDateTime>
#include <QComboBox>
#include <QUrl>
#include <QDesktopServices>
#include <QHostAddress>
#include <QTcpSocket>
//
#include <QElapsedTimer>
//
#define PORT 9033
#define ID_RX "^[A-Za-z]{1}[A-Za-z0-9]{3,9}$" // 첫 문자 영어 + 영어/숫자 4~10자
#define PW_RX "^.*(?=^.{6,12}$)(?=.*\\d)(?=.*[a-zA-Z])(?=.*[!@#$%^&+=]).*$"
#define TP_RX "[1-9]\\d{2}" // 3자리 정수
#define NM_RX "^[가-힣]{2,10}" // 한글 10자 ...^[가-힣]+$
#define FRONT_RX "[0-9]{3,4}" // 핸드폰 앞자리
#define BACK_RX "[0-9]{4}" // 핸드폰 뒷자리
#define HP_RX "01{1}[016789]{1}[0-9]{7,8}"

using namespace std;

enum {NOTHING, ALREADY_HAVE, MIN_NAME, MIN_CARD, MIN_ID, MIN_PASSWORD=6, MIN_PHONE=10, MAX_PASSWORD=12, FLOW=60, CHAT_SIZE=200, ONE_SECOND=1000, CLOSE_TIME=3000, ONE_MINUTE=60000};

enum {KICK=1};

namespace SOCK
{
    enum { TYPE_USER_LOGIN = 7, TYPE_USER_MAIN, TYPE_USER_CHARGE, TYPE_USER_CHAT, TYPE_USER_BAN };
}

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
        BUFFSIZE=50
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
        DECREASE=1,TIMEOUT=4,MAIN=5,MEM_SAVETIME,TEM_SAVETIME,SOCK_ADD_TIME=12,SOCK_CHAT,SOCK_KICK
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

#endif // COMMON_H
