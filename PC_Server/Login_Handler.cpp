#include "Login_Handler.h"

void Login_Handler::CreateAccountOverlap(int sock)
{
    SQL sql;
    sql.Connect();
    char tempID[DefineMember::BUFFSIZE];
    read(sock,tempID,sizeof(char)*(DefineMember::BUFFSIZE));  

    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.CreateAccountOverlap();
   
    string str(tempID);
    //cout<<"회원가입 ID: "<<str<<endl;
    int Doppelganger=1;

    while(row = mysql_fetch_row(result))
    {
        //cout << "str : " << str << endl;
        //cout << "row[0] : " << row[0] << endl;
        if(str==row[0])
        {
            //cout<<"row[0]: "<<row[0]<<endl;
            cout<<"CreateAccountOverlap(int sock) => while(row = mysql_fetch_row(result))"<<endl;
            Doppelganger=-1;
            break;
        }
    }
    if(Doppelganger==1 || Doppelganger==-1)
        write(sock, &Doppelganger, sizeof(int)); 
   
    mysql_free_result(result);   
    sql.Disconnect();   
}

void Login_Handler::CreateAccountComplete(int sock)
{
    SQL sql;
    sql.Connect();
    Member temp;
    read(sock,&temp,sizeof(Member));
    temp.DebugShowMember();
    cout<< "void Login_Handler::CreateAccountComplete(int sock)" <<endl;
    string tempID(temp.GetMemberId());
    string tempPW(temp.GetMemberPassword());
    string tempNAME(temp.GetMemberName());
    string tempBIRTH(temp.GetMemberBirth());
    string tempPHONE(temp.GetMemberPhone());
    string tempSTATUS;
    tempSTATUS = to_string(temp.GetMemberStatus());
    string tempREMAIN(temp.GetMemberRemain());
    sql.CreateAccountComplete(tempID,tempPW,tempNAME,tempBIRTH,tempPHONE,tempSTATUS,tempREMAIN);
    sql.Disconnect();
}

void Login_Handler::FindAccount(int sock)
{
    int signal=0;
    read(sock, &signal, sizeof(int));
    //cout<<"signal: "<<signal<<endl;
    switch(signal)
    {
    case 1:
        cout<<"FindAccountId(sock);"<<endl;
        FindAccountId(sock);
        break;
    case 2:
        cout<<"FindAccountPw(sock);"<<endl;
        FindAccountPw(sock);
        break;
    }
}

void Login_Handler::FindAccountId(int sock)
{
    SQL sql;
    sql.Connect();

    char tempNAME[DefineMember::BUFFSIZE];
    read(sock,tempNAME,sizeof(char)*(DefineMember::BUFFSIZE));

    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.FindAccountId();
    string str(tempNAME);
    //cout<<"str: "<<str<<endl;
    int Doppelganger=1;
    char tempID[DefineMember::BUFFSIZE];
    string temp=" ";
    while(row = mysql_fetch_row(result))
    {
        if(str==row[1])
        {
            cout<<"void Login_Handler::FindAccountId(int sock) => while(row = mysql_fetch_row(result))"<<endl;
            temp=row[0];
            strcpy(tempID,temp.c_str());
            Doppelganger=-1;
            break;
        }
    }
        
    if(Doppelganger==1 || Doppelganger==-1)
    {
        write(sock, &Doppelganger, sizeof(int));
        write(sock, tempID, sizeof(char)*(DefineMember::BUFFSIZE));
    }
    mysql_free_result(result);
    sql.Disconnect();
}

void Login_Handler::FindAccountPw(int sock)
{
    SQL sql;
    sql.Connect();

    char tempID[DefineMember::BUFFSIZE];
    read(sock,tempID,sizeof(char)*(DefineMember::BUFFSIZE));
    //cout<<"tempNAME: "<<tempID;
    char tempPHONE[DefineMember::BUFFSIZE];
    read(sock,tempPHONE,sizeof(char)*(DefineMember::BUFFSIZE));
    //cout<<"tempPHONE: "<<tempPHONE;

    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.FindAccountPw();
    string Id(tempID);
    //cout<<"Id: "<<Id<<endl;
    string Phone(tempPHONE);
    //cout<<"phone: "<<Phone<<endl;
    int Doppelganger=1;

    while(row = mysql_fetch_row(result))
    {
        if(Id==row[0] && Phone==row[1])
        {
            cout<<"void Login_Handler::FindAccountPw(int sock) => while(row = mysql_fetch_row(result))";
            Doppelganger=-1;
            break;
        }
    }

    if(Doppelganger==1 || Doppelganger==-1)
    {
        write(sock, &Doppelganger, sizeof(int));
    }
    mysql_free_result(result);
    sql.Disconnect();
}

void Login_Handler::FindAccountUpdate(int sock,int seat_size,Member *m)
{
    SQL sql;
    sql.Connect();
  
    char tempID[DefineMember::BUFFSIZE];
    read(sock,tempID,sizeof(char)*(DefineMember::BUFFSIZE));
//    cout<<"tempNAME: "<<tempID;
    char tempPHONE[DefineMember::BUFFSIZE];
    read(sock,tempPHONE,sizeof(char)*(DefineMember::BUFFSIZE));
//    cout<<" tempPHONE: "<<tempPHONE;
    char tempPW[DefineMember::BUFFSIZE];
    read(sock,tempPW,sizeof(char)*(DefineMember::BUFFSIZE));
//    cout<<" tempPW: "<<tempPW;
    for(int i=0; i<seat_size; i++)
    {
        if(strcmp(m[i].GetMemberId(),tempID)==0 && strcmp(m[i].GetMemberPhone(),tempPHONE)==0)
        {
            m[i].SetMemberPassword(tempPW);
            cout<<"void Login_Handler::FindAccountUpdate(int sock,int seat_size,Member *m) => m[i].SetMemberPassword(tempPW);"<<endl;
        } 
    }
    /*
    for(int i=0; i<seat_size; i++)
    {
        m[i].DebugShowMember();
    }
    */
    string Name(tempID);
    string Phone(tempPHONE);
    string Pw(tempPW);
    
    sql.FindAccountUpdate(Name,Phone,Pw);
    sql.Disconnect();
}

void Login_Handler::Log_In(int sock,int *seat)
{
    int signal=0;
    read(sock, &signal, sizeof(int));
    //cout<<"Login_Handler::Log_In => signal: "<<signal<<endl;
    switch(signal)
    {
    case 1:
        Log_In_Permanent(sock,seat);
        break;
    case 2:
        Log_In_Temporary(sock,seat);
        break;
    }
}

void Login_Handler::Log_In_Permanent(int sock,int *seat)
{
    SQL sql;
    Member member;

    char tempNAME[DefineMember::BUFFSIZE];
    char tempBIRTH[DefineMember::BUFFSIZE];
    char tempPHONE[DefineMember::BUFFSIZE];
    int tempSTATUS;
    char tempREMAIN[DefineMember::BUFFSIZE];
    string id;
    string pw;
    string name;
    string birth;
    string phone;
    string status;
    string remain;

    sql.Connect();

    char tempID[DefineMember::BUFFSIZE];
    read(sock,tempID,sizeof(char)*(DefineMember::BUFFSIZE));
    //cout<<"tempID: "<<tempID;
    char tempPW[DefineMember::BUFFSIZE];
    read(sock,tempPW,sizeof(char)*(DefineMember::BUFFSIZE));
    //cout<<" tempPW: "<<tempPW<<endl;
    int duplicate;
    read(sock,&duplicate,sizeof(int));
    //cout<<"duplicate: "<<duplicate<<endl;
    int Doppelganger=1;
    if(seat[duplicate]==1)
    {
        Doppelganger=3;
        cout<<"void Login_Handler::Log_In_Permanent(int sock,int *seat) => 이미 사용 중인 자리"<<endl;
        write(sock, &Doppelganger, sizeof(int));
        return;
    }
    else if(seat[duplicate]==0)
        cout<<"빈자리의 정보 =>seat[duplicate]: "<<seat[duplicate]<<endl;
    /*
    for(int i=0; i<20; i++)
    {
        cout<<seat[i] <<" ";
    }
    */
    string Id(tempID);
    string Pw(tempPW);
    cout<<"Id: "<<Id<<endl;
    cout<<"Pw: "<<Pw<<endl;

    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.Log_In_Permanent();
    
    while(row = mysql_fetch_row(result))
    {
        if(Id==row[0] && Pw==row[1])
        {
            cout<<"sql.Log_In_Permanent_Update(Id,Pw)..."<<endl;
            Doppelganger=-1;
            //
            id=row[0];
            pw=row[1];
            name=row[2];
            birth=row[3];
            phone=row[4];
            status=row[5];
            remain=row[6];
            //
            break;
        }
    }
    
    if(remain!="00:00")
    {
        if(Doppelganger==1)
            write(sock, &Doppelganger, sizeof(int));
        else if(Doppelganger==-1)
        {
            sql.Log_In_Permanent_Update(Id,Pw);
            write(sock, &Doppelganger, sizeof(int));
            strcpy(tempID,id.c_str());
            strcpy(tempPW,pw.c_str());
            strcpy(tempNAME,name.c_str());
            strcpy(tempBIRTH,birth.c_str());
            strcpy(tempPHONE,phone.c_str());
            //tempSTATUS=atoi(status.c_str());
            tempSTATUS=1;
            strcpy(tempREMAIN,remain.c_str());
            member.SetMembership(DefineMember::MEM);
            member.SettingNotSeatAndBegin(tempID,tempPW,tempNAME,tempBIRTH,tempPHONE,tempSTATUS,tempREMAIN);
            write(sock, &member, sizeof(Member));
        }
    }
    else if(remain=="00:00")
    {
        Doppelganger=2;
        write(sock, &Doppelganger, sizeof(int));
    }

    mysql_free_result(result);
    sql.Disconnect();
}

void Login_Handler::Log_In_Temporary(int sock,int *seat)
{
    SQL sql;
    Member member;
    string status;
    string remain;
    sql.Connect();

    char cardID[DefineMember::BUFFSIZE];
    int cardSTATUS;
    char cardREMAIN[DefineMember::BUFFSIZE];
    read(sock,cardID,sizeof(char)*(DefineMember::BUFFSIZE));
    //cout<<"cardID: "<<cardID<<endl;

    int duplicate;
    read(sock,&duplicate,sizeof(int));
    //cout<<"duplicate: "<<duplicate<<endl;

    int Doppelganger=1;
    if(seat[duplicate]==1)
    {
        Doppelganger=3;
        cout<<"void Login_Handler::Log_In_Temporary(int sock,int *seat) => 이미 사용 중인 자리"<<endl;
        write(sock, &Doppelganger, sizeof(int));
        return;
    }
    else if(seat[duplicate]==0)
        cout<<"빈자리의 정보 =>seat[duplicate]: "<<seat[duplicate]<<endl;
    /*
    for(int i=0; i<20; i++)
    {
        cout<<seat[i] <<" ";
    }
    */
    string id(cardID);
    //cout<<"비회원 string id(cardID): "<<id<<endl;
 
    MYSQL_RES *result;
	MYSQL_ROW row;
    result=sql.Log_In_Temporary();

    while(row = mysql_fetch_row(result))
    {
        if(id==row[0])
        {
            Doppelganger=-1;
            //
            id=row[0];
            status=row[1];
            remain=row[2];
            //
            break;
        }
    }

    if(remain!="00:00")
    {
        if(Doppelganger==1)
            write(sock, &Doppelganger, sizeof(int));
        else if(Doppelganger==-1)
        {
            sql.Log_In_Temporary_Update(id);
            write(sock, &Doppelganger, sizeof(int));
            strcpy(cardID,id.c_str());
            //cardSTATUS=atoi(status.c_str());
            cardSTATUS=1;
            strcpy(cardREMAIN,remain.c_str());
            member.SetMembership(DefineMember::TEM);
            member.SetMemberId(cardID);
            member.SetMemberStatus(cardSTATUS);
            member.SetMemberRemain(cardREMAIN);
            member.DebugShowMember();
            write(sock, &member, sizeof(Member));
        }
    }
    else if(remain=="00:00")
    {
        Doppelganger=2;
        write(sock, &Doppelganger, sizeof(int));
    }
    
    mysql_free_result(result);
    sql.Disconnect();
}

void Login_Handler::SendMemberClass(int m_clnt_login_sock, mutex& m_mtx, int sock,int *seat,int seat_size,Member *m)
{
    Member people;
    m_mtx.lock();
    read(sock,&people,sizeof(Member));
    //cout<<"좌석 변경 확인용: "<<people.GetMemberSeat()<<endl;
    for(int i=0; i<seat_size; i++)
    {
        if(i==people.GetMemberSeat()-1)
        {
            if(seat[i] != 1)
            {
                seat[i]=1;
                cout<<"void Login_Handler::SendMemberClass => m[i]=people..."<<endl;
                m[i]=people;     
                break;
            }
        }
    }
    int LoginStatus=1; // 로그인 시그널
    write(m_clnt_login_sock,&LoginStatus,sizeof(int));
    write(m_clnt_login_sock,&people,sizeof(Member));
    m_mtx.unlock();
}

// 구태환
int Login_Handler::CheckMIdCorrect(char *login_id)
{
    SQL sql;
    string s_login_id(login_id);
    bool isExist = false;
    int seat_cnt = -1;

    sql.Connect();
    // 아이디 존재여부를 확인하다.
    isExist = sql.isManagerExist(s_login_id);
    // 존재하는 아이디라면 좌석정보를 확인하다. (1:존재)
    cout << "isExist :" << isExist << endl;
    if(isExist == true)
    {
        seat_cnt = sql.GetSeatCnt(s_login_id);
    }
    else
    {
        sql.Disconnect();
        return -1;
    }
    sql.Disconnect();
    // 결과를 리턴하다.
    return seat_cnt;
}
