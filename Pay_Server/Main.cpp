#include "Common.h"
#include "Pay_Server.h"

int main(void)
{
    Server serv;

    serv.Connect_To_PC_Server(); // PC방 관리 서버에 연결
    serv.Server_On(); // 결제서버 작동( liten 상태 시작) 

    return 0;
}
