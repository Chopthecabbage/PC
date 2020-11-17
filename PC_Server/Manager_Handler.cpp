#include "Manager_Handler.h"
#include "Chat.h"

// 태환
int Manager_Handler::ProcOrder(int order_num)
{
    SQL sql;
    int result;

    string o_number = std::to_string(order_num);

    sql.Connect();
    result = sql.SetProcOrder(o_number);
    sql.Disconnect();
    return result;
}

int Manager_Handler::CancelOrder(int order_num)
{
    SQL sql;
    int result;
    string o_number = std::to_string(order_num);

    sql.Connect();
    result = sql.SetCancelOrder(o_number);
    sql.Disconnect();
    return result;
}

int Manager_Handler::GetMsgCnt(int seat_num)
{
    SQL sql;
    int cnt;

    sql.Connect();
    cnt = sql.GetMsgCnt(seat_num);
    sql.Disconnect();

    return cnt;
}

void Manager_Handler::SendMsgToManager(int m_sock,int seat_num,int chat_cnt)
{
    SQL sql;
    MYSQL_ROW sql_row;
    MYSQL_RES *result;
    int cnt;

    Chat *chats = nullptr;

    sql.Connect();
    result = sql.GetMsgAll(seat_num,chat_cnt);
    sql.Disconnect();

    // chat_cnt만큼 클래스배열을 생성한다. 
    chats = new Chat[chat_cnt];
    // 생성한 클래스배열에 result를 넣는다. 
    int i=0;
    // 출력할 요소를 클래스배열에 등록한다. 
    while(sql_row = mysql_fetch_row(result))
    {
        chats[i].SetWriter(sql_row[0]);
        chats[i].SetMsg(sql_row[1]);
        i++;
    }
    // 클래스배열을 전달한다. 
    cout << "넘기는 클래스배열크기 :" << chat_cnt << endl;
    
    for(int i=0; i<chat_cnt; i++)
    {
        cout << "채팅 결과 :" << endl;
        cout << chats[i].GetWriter() << " " << chats[i].GetMsg() << endl;
        write(m_sock,&(chats[i]),sizeof(Chat));
    }

}

void Manager_Handler::SendMsgToClient(int *user_chat_socks,int chat_form,int selected_seat_num,char *chat_msg,int seat_size)
{
    SQL sql;
    bool insert_result = false;
    
    // 채팅 형식에(0: 선택채팅 1 : 전체채팅) 맞는 메시지를 저장한다. 
    sql.Connect();
    insert_result = sql.InsertChatInfo(chat_form,selected_seat_num,chat_msg);
    sql.Disconnect();
    // 메시지 저장에 성공하면 메시지를 이용자좌석에 전달한다. 
    cout << "chat_msg :" << chat_msg << endl;
    cout << "insert_result :" << insert_result << endl;
    cout << "chat_form :" << chat_form << endl;
    if(insert_result == true && chat_form == 0)
    {
        // 선택된 좌석에 메시지를 전달한다. 
        write(user_chat_socks[selected_seat_num-1],chat_msg,sizeof(char)*200);
        cout << selected_seat_num << "번좌석에 메시지 전달 성공!" << endl;
    }
    else if(insert_result == true && chat_form == 1)
    {
        for(int i=0; i<seat_size; i++)
        {
            if(user_chat_socks[i]!= 0)
            {
                write(user_chat_socks[i],chat_msg,sizeof(char)*200);
            }
        }
        cout << "전체좌석에 메시지 전달 성공!" << endl;
    }
}

// 준혁
void Manager_Handler::Management(int sock) //회원 관리 함수
{
    char info[DefineWidget::BUF_SIZE];
    Member member;
    SQL sql;
    sql.Connect();
    read(sock,info,sizeof(info)); //회원 이름 수신
    cout<<"조회한 회원 이름: "<<info<<endl;
    member=sql.GetMemberInfo(info); //DB에서 가지고 온 데이터 member클래스에 저장
    cout<<"member..."<<endl;
    write(sock,(const char*)&member,sizeof(Member)); //member 클래스 관리자에게 송신
    sql.Disconnect();
}

void Manager_Handler::InfoModify(int sock) //회원 정보 수정 함수
{
    Member member;
    SQL sql;
    sql.Connect();
    read(sock,(char*)&member,sizeof(Member));
    sql.ModifiedInfo(member);
    sql.Disconnect();
}

void Manager_Handler::InfoDelete(int sock) //회원 정보 삭제 함수
{
    Member member;
    SQL sql;
    char temp[DefineMember::BUFFSIZE];
    sql.Connect();
    read(sock,(char*)temp,sizeof(temp));
    member.SetMemberId(temp);
    sql.DeletedInfo(member);
    sql.Disconnect();
}

void Manager_Handler::onSale(int sock,int sig) //매출 내역 함수
{
    char temp[DefineWidget::BUF_SIZE];
    int arrcnt;
    SQL sql;
    
    cout<<"매출 함수 실행"<<endl;
    sql.Connect();
    read(sock,(char*)temp,sizeof(temp)); //당일 날짜 임시저장
    cout<<"날짜: "<<temp<<endl;
    arrcnt=sql.GetDateCnt(temp,sig);
    write(sock,(const char*)&arrcnt,sizeof(arrcnt)); //당일 날짜에 대한 DB 카운트 송신
    if(arrcnt!=0)
    {     
        cout<<"arrcnt: "<<arrcnt<<endl;
        salesinfo=new SalesInfo[arrcnt];
        item=new Item[arrcnt];
        sql.GetArrInfo(salesinfo,temp,sig);
        for(int i=0;i<arrcnt;i++)
        {
            item[i]=sql.GetArrItemInfo(&salesinfo[i]);
            cout<<"주문 번호: "<<salesinfo[i].GetArrId()<<endl;
            cout<<"주문 날짜: "<<salesinfo[i].GetArrDate()<<endl;
            cout<<"결제 수단: "<<salesinfo[i].GetPayment()<<endl;
            cout<<"주문 처리상태: "<<salesinfo[i].GetArrStatus()<<endl;
            cout<<"상품 코드: "<<salesinfo[i].GetCode()<<endl;
            cout<<"상품 수량: "<<salesinfo[i].GetAmount()<<endl;
            cout<<"상품 이름: "<<item[i].GetItemName()<<endl;
            cout<<"상품 가격: "<<item[i].GetItemPrice()<<endl;
        }
        cout<<"arrcnt: "<<arrcnt<<endl;
        for(int i=0;i<arrcnt;i++)
        {
            write(sock,(const char*)&salesinfo[i],sizeof(salesinfo[i]));
            usleep(1000);
            cout<<"arr size: "<<sizeof(SalesInfo)<<endl;
        }
        for(int i=0;i<arrcnt;i++)
        {
            write(sock,(const char*)&item[i],sizeof(item[i]));
            usleep(1000);
            cout<<"item size: "<<sizeof(Item)<<endl;
        }
        delete [] salesinfo;
        delete [] item;
    }  
    else
        cout<<"arrcnt: "<<arrcnt<<endl;
    
    sql.Disconnect();
}

// 민근
void Manager_Handler::Send_Order_To_Pay_Serv(int order_num, int pay_serv_sock)
{
    SQL sql;
    MYSQL_ROW sql_row;
    MYSQL_RES *result;
    I_Pay_Info i_info;

    sql.Connect();
    result = sql.Select_Order_Info(order_num);
    sql_row = mysql_fetch_row(result);
    i_info.Set_Arr_Id(atoi(sql_row[0]));
    i_info.Set_Payment(atoi(sql_row[1]));
    i_info.Set_Price(atoi(sql_row[2]));
    sql.Disconnect();
    cout << "arr_id : " << i_info.Get_Arr_Id() << endl;
    cout << "payment : " << i_info.Get_Payment() << endl;
    cout << "price : " << i_info.Get_Price() << endl;
    
    int signal = PAY_SERV_SIG::SEND_ITEM_PAY_INFO;
    write(pay_serv_sock, &signal, sizeof(int));
    write(pay_serv_sock, &i_info, sizeof(I_Pay_Info));
}