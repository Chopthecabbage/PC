#include "manager.h"
#include "ui_manager.h"


#include <QMessageBox>
#include <QGridLayout>
#include <QHostAddress>
#include <QInputDialog>
#include <QRegExp>
#include <QTextEdit>


Manager::Manager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manager),
    btn(nullptr),
    writing_m_sock(nullptr),
    tot_seat_cnt(0),
    online_users_cnt(0),
    tot_waiting_orders(0),
    isEnded(false)
{
    ui->setupUi(this);
    qDebug() << "Manager()";
    // 피시방 이용자로부터의 시그널을 읽어 데이터를 받는다.
    int type;

    type = TYPE_MANAGER_CLIENT_LOGIN;

    reading_login_sock = new QTcpSocket();
    reading_login_sock->connectToHost(QHostAddress("61.108.2.70"), MIDDLE_PORT);
    connect(reading_login_sock,SIGNAL(readyRead()),this,SLOT(recv_login_client()));
    reading_login_sock->write((char*)(&type), sizeof(int));

    type = TYPE_MANAGER_TIME;

    reading_time_pass_sock = new QTcpSocket();
    reading_time_pass_sock->connectToHost(QHostAddress("61.108.2.70"), MIDDLE_PORT);
    connect(reading_time_pass_sock,SIGNAL(readyRead()),this,SLOT(recv_timepass_info()));
    reading_time_pass_sock->write((char*)(&type), sizeof(int));

    type = TYPE_MANAGER_CHAT;

    reading_chat_sock = new QTcpSocket();
    connect(reading_chat_sock,SIGNAL(readyRead()),this,SLOT(recv_chat_info()));
    reading_chat_sock->connectToHost(QHostAddress("61.108.2.70"), MIDDLE_PORT);
    reading_chat_sock->write((char*)(&type), sizeof(int));

    type = TYPE_MANAGER_ORDER;

    reading_order_sock = new QTcpSocket();
    connect(reading_order_sock,SIGNAL(readyRead()),this,SLOT(recv_order_info()));
    reading_order_sock->connectToHost(QHostAddress("61.108.2.70"), MIDDLE_PORT);
    reading_order_sock->write((char*)(&type), sizeof(int));

}

Manager::~Manager()
{
    delete ui;
    delete [] btn;
    delete [] mems_info;
    delete writing_m_sock;
    delete reading_login_sock;
    delete reading_time_pass_sock;
    delete reading_chat_sock;
    delete reading_order_sock;
    qDebug() << "소멸자";
}

void Manager::getSeatNumber()
{
    QObject * object = QObject::sender();
    SeatButton *btn = dynamic_cast<SeatButton*>(object);
    if(btn->getUserId()=="")
    {
        QMessageBox::information(this,"Warning","빈좌석!");
    }
    // 좌석을 이용중인 유저의 pc사용정보를 위젯에 표시한다.
    else
    {
        ui->seat_num_label->setText(QString::number(btn->getSeatNum()));
        ui->member_label->setText(btn->getPcMemberKind());
        ui->client_id_label->setText(btn->getUserId());
        ui->client_name_label->setText(btn->getUserName());
        ui->start_time_label->setText(btn->getUserStartTime());
        ui->end_time_label->setText(btn->getUserEndTime());
        ui->remain_time_label->setText(btn->getRemainTime());
        ui->pc_usage_fee->setText(QString::number(btn->getPcUsageFee()));
    }
}

void Manager::InitSeats()
{
    qDebug() << "InitSeats()";

    QGridLayout *lay = new QGridLayout(this);

    // 관리자가 등록한 pc대수만큼 좌석을 생성
    btn = new SeatButton[tot_seat_cnt];
    // 수용가능한 좌석수만큼 pc방이용자 정보객체 생성
    mems_info = new Member[tot_seat_cnt];

    // 좌석생성 및 속성설정
    const QSize btnSize = QSize(150,150);
    for(int i=0; i<tot_seat_cnt; i++)
    {
        btn[i].setText("좌석 "+QString::number(btn[i].getSeatNum()));
        btn[i].setFixedSize(btnSize);
    }

    // 좌석을 배치
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<tot_seat_cnt/5; j++)
        {
            lay->addWidget(btn+(((tot_seat_cnt/5)*i)+j),i,j,1,1);
            connect(btn+(((tot_seat_cnt/5)*i)+j),SIGNAL(clicked()),this,SLOT(getSeatNumber()));
        }
    }
    ui->scrollContents->setLayout(lay);
}

void Manager::recv_values(QTcpSocket* socket,int seat_cnt)
{
    qDebug() << "recv_values()";

    writing_m_sock = socket;
    tot_seat_cnt = seat_cnt;

    // 좌석수만큼 채팅클라이언트 콤보박스 아이템추가
    ui->client_list->addItem("좌석선택");
    for(int i=0; i<tot_seat_cnt; i++)
    {
        QString s_seat_num = QString::number(i+1);
        ui->client_list->addItem(s_seat_num);
    }

    // 중계서버에 좌석수를 전달
    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((char*)(&tot_seat_cnt),sizeof(int));

    InitSeats();
}

void Manager::recv_order_info()
{

    Arrangement arrangement; // 주문 1건
    QString waiting_order_info;
    qDebug() << "recv_order_info() 주문들어옴" ;
    // 주문한건을 받다.

    order_mutex.lock();

    reading_order_sock->waitForReadyRead(500);
    reading_order_sock->read((char*)(&arrangement),sizeof(Arrangement));


    qDebug() << "reading_order_sock->read" ;
    // 주문한건을 대기중인 주문에 넣다.
    arrangements.push_back(arrangement);

    order_mutex.unlock();

    qDebug() << "arrangements.push_back" ;
    // 버튼의 대기주문건수를 변동.
    waiting_order_info += "음식주문 ";
    waiting_order_info += QString::number(arrangements.size());
    waiting_order_info += "건 대기중";
    ui->orderList_btn->setText(waiting_order_info);
    qDebug() << "ui->orderList_btn->setText" ;

    // 버튼색 변경

}

void Manager::recv_chat_info()
{
    int seat_num;
    int selected_seat_num;
    QString s_chat_msg = "";
    QString red_chat_msg = QString("<span style=' color:#ff0000;'>%1</span>").arg(s_chat_msg.left(4));
    char chat_msg[200] = "";

    // 채팅을 한 좌석을 읽는다.
    reading_chat_sock->read((char*)&seat_num,sizeof(int));
    // 채팅글을 읽는다.
    reading_chat_sock->waitForReadyRead(100);
    reading_chat_sock->read(chat_msg,sizeof(chat_msg));

    qDebug() << seat_num << "번에서 온 채팅 :" << chat_msg;

    qDebug() << "라디오박스 다운여부 :" << ui->select_client_btn->isChecked();

    // 라디오박스가 회원채팅으로 설정되어 있으며
    if(ui->select_client_btn->isChecked())
    {

       selected_seat_num = ui->client_list->currentText().toInt();
       // 콤보박스가 채팅을 걸어온 좌석과 일치한다면
       if(seat_num == selected_seat_num)
       {
           s_chat_msg += "[Seat ";
           s_chat_msg += QString::number(seat_num);
           s_chat_msg += "] :";
           s_chat_msg += chat_msg;
           ui->chat_browser->addItem(s_chat_msg);
       }
    }
    else
    {
        s_chat_msg += "[information] :";
        s_chat_msg += QString::number(seat_num);
        s_chat_msg += " 번 좌석에서 관리자에게 문의를 하였습니다.";
        // 알림문구를 빨간색으로 속성변경
        ui->chat_browser->addItem(s_chat_msg);
    }

}

void Manager::recv_timepass_info()
{
    char c_remain_time_info[50];
    QString remain_time_info = "";
    int seat_num;
    QString seat_info;

    timepass_mutex.lock();
    // 남은시간이 감소한 좌석을 확인한다.
    reading_time_pass_sock->waitForReadyRead(500);
    reading_time_pass_sock->read((char*)(&seat_num),sizeof(int));

    // 좌석의 변동된 남은시간을 확인한다.
    reading_time_pass_sock->waitForReadyRead(500);
    reading_time_pass_sock->read(c_remain_time_info,sizeof(c_remain_time_info));

    remain_time_info = c_remain_time_info;

    // 남은시간을 갱신한다.
    mems_info[seat_num-1].SetMemberRemain(c_remain_time_info);
    btn[seat_num-1].setUserRemainTime(remain_time_info);

    // 남은 요금을 계산하다. -> 회원은 분당 20원 비회원은 분당 25원
    int remain_usage_fee = getRemainUsageFee(mems_info[seat_num-1].GetMembership(),mems_info[seat_num-1].GetMemberRemain());
    // 남은 요금정보를 좌석에 저장하다.
    btn[seat_num-1].setPcUsageFee(remain_usage_fee);

    // 남은 시간이 0이라면 해당 좌석을 비운다.
    if(btn[seat_num-1].getRemainTime()=="00:00")
    {
        // 좌석정보를 비운다.
        mems_info[seat_num-1].InitializeMember();
        btn[seat_num-1].initializeBtnInfo();
        // 좌석번호를 표시한다.
        seat_info += "좌석 ";
        seat_info += QString::number(btn[seat_num-1].getSeatNum());
        btn[seat_num-1].setText(seat_info);
        return;
    }

    // 좌석에 이용자의 정보를 표시한다. (좌석번호,이름(아이디),시작시간,요금순)
    seat_info.append("no.");
    seat_info.append(QString::number(seat_num)); // 좌석번호
    seat_info.append("(");
    seat_info.append(btn[seat_num-1].getPcMemberKind()); // 회원,비회원
    seat_info.append(")");
    seat_info.append("\n");
    seat_info.append(btn[seat_num-1].getUserName()); // 회원이름
    seat_info.append("(");
    seat_info.append(btn[seat_num-1].getUserId()); // 회원id
    seat_info.append(")\n");
    seat_info.append("남은시간 :");
    seat_info.append(btn[seat_num-1].getRemainTime()); // 남은시간
    seat_info.append("\n");

    btn[seat_num-1].setText(seat_info);

    timepass_mutex.unlock();
}

QString Manager::getPcEndingTime(QString pc_start_time,QString remain_time)
{
    QRegExp tagExp(":");

    QStringList start_time_list;
    QStringList remain_time_list;

    int i_start_time_list[2];
    int i_end_time_list[2];
    int i_remain_time_list[2];

    QString end_time;

    // 시간,분을 자르다.
    start_time_list =  pc_start_time.split(tagExp);
    remain_time_list = remain_time.split(tagExp);
    // 시간,분을 int형으로 변환하다.
    for(int i=0; i<start_time_list.size(); i++)
    {
        i_start_time_list[i]=start_time_list[i].toInt();
        i_remain_time_list[i]=remain_time_list[i].toInt();
    }

    i_end_time_list[0] = i_start_time_list[0];
    i_end_time_list[1] = i_start_time_list[1];

    // 분단위
    int size = i_remain_time_list[1];
    for(int i=0; i<size; i++)
    {
        i_remain_time_list[1]--;
        i_end_time_list[1]++;
        if(i_end_time_list[1]==60)
        {
            i_end_time_list[1]=0;
            i_end_time_list[0]++;
            if(i_end_time_list[0]==25)
            {
                i_end_time_list[0]=0;
            }
        }
    }

    // 시간단위
    size = i_remain_time_list[0];
    for(int i=0; i<size; i++)
    {
        i_remain_time_list[0]--;
        i_end_time_list[0]++;
        if(i_end_time_list[0]==25)
        {
            i_end_time_list[0]=0;
        }
    }

    // 끝나는 시간 설정
    if(i_end_time_list[0]>9)
        end_time+=QString::number(i_end_time_list[0]);
    else
    {
        end_time+="0";
        end_time+=QString::number(i_end_time_list[0]);
    }
    end_time+=":";
    // 끝나는 분 설정
    if(i_end_time_list[1]>9)
        end_time+=QString::number(i_end_time_list[1]);
    else
    {
        end_time+="0";
        end_time+=QString::number(i_end_time_list[1]);
    }

    return end_time;
}

int Manager::getRemainUsageFee(int membership,QString remain_time)
{
    QRegExp tagExp(":");
    QStringList remain_time_list;
    int i_remain_time[2]; // 0번째는 시간, 1번째는 분
    int per_min_fee = 0; // 회원은 분당 20원, 비회원은 분당 25원
    int remain_usage_fee = 0;

    // 남은시간 int형 변환
    remain_time_list = remain_time.split(tagExp);
    for(int i=0; i<remain_time_list.size(); i++)
        i_remain_time[i] = remain_time_list[i].toInt();

    // 회원여부에 따른 분당요금설정
    if(membership==1)
        per_min_fee=20;
    else if(membership==2)
        per_min_fee=25;

    // 분 요금추가
    remain_usage_fee += per_min_fee*i_remain_time[1];
    // 시간 요금추가
    remain_usage_fee += per_min_fee*i_remain_time[0]*60;

    return remain_usage_fee;
}

// pc방 이용자 로그인신호 read
void Manager::recv_login_client()
{

    QString seat_info;
    Member login_member;
    int signal;

    login_mutex.lock();
    // 로그인 또는 로그아웃 시그널을 받는다.
    reading_login_sock->read((char*)&signal,sizeof(int));

    if(signal == 1)
    {
        qDebug() << "로그인시그널";
        login_member.InitializeMember();
        // 로그인 한 이용자의 정보를 받는다.
        reading_login_sock->waitForReadyRead(1000);
        reading_login_sock->read((char*)&login_member,sizeof(Member));
        online_users_cnt++;

        if(online_users_cnt>tot_seat_cnt)
            online_users_cnt=tot_seat_cnt;
        ui->current_user->setText(QString::number(online_users_cnt));
        // 이용자가 선택한 좌석번호를 확인한다.
        int seat_num = login_member.GetMemberSeat();
        int membership = login_member.GetMembership();
        qDebug() << "로그인좌석 :" << seat_num;
        // 전체 이용자배열에 로그인한 이용자의 정보를 저장한다.
        mems_info[seat_num-1].InitializeMember();
        mems_info[seat_num-1] = login_member;

        // 좌석에 이용자의 정보를 표시한다. (좌석번호,이름(아이디),시작시간,요금순)
        for(int i=0; i<tot_seat_cnt; i++)
        {
            if(seat_num == btn[i].getSeatNum())
            {
              // 좌석에 정보를 저장하다.
              btn[i].setSeatNum(seat_num); // 좌석번호
              if(membership==1)
              {
                 btn[i].setPcMemberKind("회원");
              }
              else if(membership==2)
              {
                 btn[i].setPcMemberKind("비회원");
              }
              btn[i].setUserName(login_member.GetMemberName()); // 회원이름
              btn[i].setUserId(login_member.GetMemberId()); // 회원id
              btn[i].setUserStartTime(login_member.GetMemberBegin()); // 시작시간
              // 끝나는 시간을 계산하다.
              btn[i].setUserEndTime(getPcEndingTime(login_member.GetMemberBegin(),login_member.GetMemberRemain()));

              btn[i].setUserRemainTime(login_member.GetMemberRemain()); // 남은시간

              // 남은 요금을 계산하다. -> 회원은 분당 20원 비회원은 분당 25원
              int remain_usage_fee = getRemainUsageFee(login_member.GetMembership(),login_member.GetMemberRemain());
              // 남은 요금을 저장하다.
              btn[i].setPcUsageFee(remain_usage_fee);

              seat_info.append("no.");
              seat_info.append(QString::number(seat_num)); // 좌석번호
              seat_info.append("(");
              seat_info.append(btn[i].getPcMemberKind()); // 회원,비회원
              seat_info.append(")");
              seat_info.append("\n");
              seat_info.append(btn[i].getUserName()); // 회원이름
              seat_info.append("(");
              seat_info.append(btn[i].getUserId()); // 회원id
              seat_info.append(")\n");
              seat_info.append("남은시간 :");
              seat_info.append(btn[i].getRemainTime()); // 남은시간
              seat_info.append("\n");

              // 좌석에 정보를 표시하다.
              btn[i].setText(seat_info);

              break;
            }
        }
    }
    else if(signal == 2)
    {
        int seat_num;
        qDebug() << "로그아웃시그널";
        reading_login_sock->waitForReadyRead(1000);
        reading_login_sock->read((char*)&seat_num,sizeof(int));
        // 버튼의 회원정보 삭제
        qDebug() << "로그아웃좌석 :" << seat_num;
        btn[seat_num-1].initializeBtnInfo();
        // 멤버배열의 정보삭제
        mems_info[seat_num-1].InitializeMember();
        online_users_cnt--;
        if(online_users_cnt<0)
            online_users_cnt=0;
        ui->current_user->setText(QString::number(online_users_cnt));
        // 좌석번호를 표시한다.
        seat_info += "좌석 ";
        seat_info += QString::number(btn[seat_num-1].getSeatNum());
        btn[seat_num-1].setText(seat_info);
    }
    else if(signal == 3) // 시간추가.
    {
        // 좌석번호를 읽는다.
        int seat_num;
        char remain_time[20];
        QString s_remain_time;
        qDebug() << "시간추가시그널";
        reading_login_sock->waitForReadyRead(300);
        reading_login_sock->read((char*)&seat_num,sizeof(int));
        qDebug() << "시간추가좌석 :" << seat_num;
        // 남은시간을 읽는다.
        reading_login_sock->waitForReadyRead(300);
        reading_login_sock->read(remain_time,sizeof(remain_time));
        // 좌석의 남은시간을 갱신한다.
        mems_info[seat_num-1].SetMemberRemain(remain_time);
        s_remain_time = remain_time;
        btn[seat_num-1].setUserRemainTime(s_remain_time);
        // 남은 요금을 계산하다. -> 회원은 분당 20원 비회원은 분당 25원
        int remain_usage_fee = getRemainUsageFee(mems_info[seat_num-1].GetMembership(),mems_info[seat_num-1].GetMemberRemain());
        // 남은 요금정보를 좌석에 저장하다.
        btn[seat_num-1].setPcUsageFee(remain_usage_fee);

        // 좌석에 이용자 정보를 표시하다.
        seat_info = "";
        seat_info.append("no.");
        seat_info.append(QString::number(seat_num)); // 좌석번호
        seat_info.append("(");
        seat_info.append(btn[seat_num-1].getPcMemberKind()); // 회원,비회원
        seat_info.append(")");
        seat_info.append("\n");
        seat_info.append(btn[seat_num-1].getUserName()); // 회원이름
        seat_info.append("(");
        seat_info.append(btn[seat_num-1].getUserId()); // 회원id
        seat_info.append(")\n");
        seat_info.append("남은시간 :");
        seat_info.append(btn[seat_num-1].getRemainTime()); // 남은시간
        seat_info.append("\n");

        // 좌석에 정보를 표시하다.
        btn[seat_num-1].setText(seat_info);
    }
    login_mutex.unlock();
}

void Manager::on_orderList_btn_clicked()
{
    QString waiting_order_info;
    order_dialog = new Orders;

    if(arrangements.size()==0)
    {
        QMessageBox::information(this,"없음","대기중인 주문이 없습니다!");
    }
    else
    {
        // 주문 다이얼로그 창을 띄운다.
        connect(this,SIGNAL(send_orders(QTcpSocket*,QVector<Arrangement>*)),order_dialog,SLOT(recv_orders(QTcpSocket*,QVector<Arrangement>*)));
        emit send_orders(writing_m_sock,&arrangements);
        order_dialog->exec();

        // 남은 주문 체크
        if(this->arrangements.size()==0)
        {
            waiting_order_info += "음식주문 ";
            waiting_order_info += QString::number(0);
            waiting_order_info += "건 대기중";
            ui->orderList_btn->setText(waiting_order_info);
            // 버튼색상을 원상복귀 시킨다.
        }
    }
}

void Manager::on_exit_client_btn_clicked()
{
    bool ok;
    QString seat_info;

    int seat_num = QInputDialog::getText(this, "좌석종료", "좌석번호: ",
    QLineEdit::Normal, "좌석번호 입력", &ok).toInt();

    if(seat_num==0)
    {
        QMessageBox::information(this,"알림","입력형식 불일치");
    }
    else if(seat_num>0)
    {
        // 좌석강퇴 시그널을 전달한다.
        int out_signal;
        out_signal = TYPE_OUT_CLIENT;
        writing_m_sock->waitForBytesWritten(500);
        writing_m_sock->write((char*)&out_signal,sizeof(int));

        // 강퇴할 좌석번호를 전달한다.
        writing_m_sock->waitForBytesWritten(500);
        writing_m_sock->write((char*)&seat_num,sizeof(int));

        // 이용자정보를 삭제한다.
        btn[seat_num-1].initializeBtnInfo();
        mems_info[seat_num-1].InitializeMember();
        seat_info += "좌석 ";
        seat_info += QString::number(btn[seat_num-1].getSeatNum());
        btn[seat_num-1].setText(seat_info);
        online_users_cnt--;
        if(online_users_cnt<0)
        {
            online_users_cnt=0;
        }
        ui->current_user->setText(QString::number(online_users_cnt));
        QMessageBox::information(this,"알림","강퇴완료!");
    }
}

void Manager::on_send_msg_btn_clicked()
{
    QString chat_line = ui->chat_line->text();
    char c_chat_line[200] ;
    int selected_seat_num;

    strcpy(c_chat_line,chat_line.toStdString().c_str());

    selected_seat_num = ui->client_list->currentText().toInt();
    qDebug() << "채팅보낼 유저좌석 :" << selected_seat_num;
    // 라디오박스 선택종류를 확인한다.
    if(chat_line != "" && ui->select_client_btn->isChecked())
    {   // 몇번좌석의 유저인지 확인한다.
        // 좌석이 이용중이라면
        if(strcmp(mems_info[selected_seat_num-1].GetMemberId(),"")!=1)
        {
            qDebug() << "채팅보낼 유저아이디 :" << mems_info[selected_seat_num-1].GetMemberId() ;

            // 채팅창에 보낼 메시지를 출력한다.
            ui->chat_browser->addItem("[Admin] :"+chat_line);
            ui->chat_line->setText("");
            // 메시지 전달 시그널
            int signal = TYPE_CHAT_CLIENT;
            writing_m_sock->waitForBytesWritten();
            writing_m_sock->write((char*)&signal,sizeof(int));
            // 메시지 전달할 좌석번호
            writing_m_sock->waitForBytesWritten(300);
            writing_m_sock->write((char*)&selected_seat_num,sizeof(int));
            // 입력한 채팅문구를 전달하다.
            qDebug() << "채팅메시지 :" << c_chat_line;
            writing_m_sock->waitForBytesWritten(300);
            writing_m_sock->write(c_chat_line,sizeof(c_chat_line));
      }
        else
        {
            QMessageBox::warning(this,"없음","로그인 되어있지 않은 좌석입니다.");
        }
    }

    else if(chat_line != "" && ui->select_all_btn->isChecked())
    {
        // 채팅창에 보낼 메시지를 출력한다.
        ui->chat_browser->addItem("[Admin] :"+chat_line);
        ui->chat_line->setText("");
        // 메시지 전달 시그널
        int signal = TYPE_CHAT_ALL_CLIENT;
        writing_m_sock->waitForBytesWritten(300);
        writing_m_sock->write((char*)&signal,sizeof(int));
        // 입력한 채팅문구를 전달하다.
        qDebug() << "채팅메시지 :" << c_chat_line;
        writing_m_sock->waitForBytesWritten(300);
        writing_m_sock->write(c_chat_line,sizeof(c_chat_line));
    }
    else
    {
       QMessageBox::warning(this,"경고","입력형식이 잘못됨");
    }
}

bool Manager::getTodayEnd() const
{
    return isEnded;
}

void Manager::setTodayEnd()
{
    isEnded=true;
}

void Manager::on_end_btn_clicked()
{
    if(getTodayEnd()!=true)
    {
        QMessageBox::information(this,"경고","당일영업 정산이 끝나지 않았습니다.");
    }
    else
    {
        this->close();
    }
}

void Manager::on_end_today_sales_btn_clicked()
{
    if(arrangements.size()!=0)
    {
        QMessageBox::information(this,"경고","처리되지 않은 주문이 남아있습니다.");
    }
    else
    {
        setTodayEnd();
        QMessageBox::information(this,"알림","정산완료!");
    }
}

void Manager::on_find_user_btn_clicked()
{
    bool ok;
    int seat_num = 0;

    QString id = QInputDialog::getText(this, "이용자 찾기", "아이디: ",
    QLineEdit::Normal, "아이디 입력", &ok);

    for(int i=0; i<tot_seat_cnt; i++)
    {
        if(mems_info[i].GetMemberId()==id)
        {
            seat_num = i+1;
        }
    }

    if(seat_num > 0)
    {
        QMessageBox::information(this,"알림",id+"회원은"+QString::number(seat_num)+"번 좌석에서 이용중");
    }
    else
    {
        QMessageBox::information(this,"알림","로그인 되지 않은 회원입니다.");
    }
}

void Manager::on_client_list_currentIndexChanged(int index)
{
    Chat chat;
    Chat *chats;
    int signal;
    int seat_num;
    int chat_cnt;
    QString msg;

    seat_num = index;

    qDebug() << "몇번좌석?ㄴㅇㅁ랸 :" << seat_num;
    ui->chat_browser->clear();
    // 라디오버튼이 눌려저 있고
    if(ui->select_client_btn->isChecked()==true && seat_num > 0)
    {
        // 좌석에 회원이 앉아있다면
        if(strcmp(mems_info[index-1].GetMemberId(),"")!=0)
        {
            qDebug() << "좌석에 화원이 이용중입니다." ;
            // 대화기록요청 시그널을 보낸다.
            signal = TYPE_CHAT_REQUEST;

            writing_m_sock->write((char*)&signal,sizeof(int));

            // 좌석번호 index번과의 대화기록을 요청한다.(0: 좌석선택문구 , 1~ : 좌석번호)
            writing_m_sock->write((char*)&seat_num,sizeof(int));

            // 대화개수를 받는다.
            writing_m_sock->waitForReadyRead(500);
            writing_m_sock->read((char*)&chat_cnt,sizeof(int));

            qDebug() << "on_client_list_currentIndexChanged _ chat_cnt :" << chat_cnt;

            if(chat_cnt > 0)
            {
                // 브라우저를 비운다.


                // 대화개수만큼 chat클래스배열 동적할당
                chats = new Chat[chat_cnt];

                // 대화개수만큼 chat받기
                QThread::usleep(300000);
                writing_m_sock->waitForReadyRead(500);
                writing_m_sock->read((char*)chats,sizeof(Chat)*(chat_cnt));

                for(int i=0; i<chat_cnt; i++)
                {
                    msg="";
                    msg += "[";
                    msg += chats[i].GetWriter();
                    msg += "] :";
                    msg += chats[i].GetMsg();
                    ui->chat_browser->addItem(msg);
                }
                ui->chat_browser->scrollToBottom();
                delete [] chats;
            }
            else
            {
                qDebug() << "가져올 채팅없음";
            }
        }
        else
        {
            QMessageBox::information(this,"알림","이용중이지 않은 좌석");
        }
    }
    else
    {
        qDebug() << "회원좌석을 선택하지 않았습니다.";
    }
}


// 이용자 선택 채팅버튼
void Manager::on_select_client_btn_clicked()
{
      // 콤보박스리스트를 첫 인덱스로 변경한다.
      ui->client_list->setCurrentIndex(0);
}

// 전체채팅 라디오박스 선택
void Manager::on_select_all_btn_clicked()
{
    Chat *chats = nullptr;
    int signal;
    int chat_cnt;
    QString msg ;
    // 콤보박스리스트를 첫 인덱스로 변경한다.
    ui->client_list->setCurrentIndex(0);

    ui->chat_browser->clear();

    signal = TYPE_CHAT_REQUEST;


    writing_m_sock->waitForBytesWritten();
    writing_m_sock->write((char*)&signal,sizeof(int));

    int seat_num = 0;
    writing_m_sock->waitForBytesWritten();
    writing_m_sock->write((char*)&seat_num,sizeof(int));

    // 채팅개수를 받는다.
    writing_m_sock->waitForReadyRead(500);
    writing_m_sock->read((char*)&chat_cnt,sizeof(int));

    qDebug() << "전체채팅 기록수 :" << chat_cnt;

    if(chat_cnt>0)
    {
        chats = new Chat[chat_cnt];

        // 응답받은 주문정보(클래스배열)를 서버로부터 받다.
        QThread::usleep(300000);
        writing_m_sock->waitForReadyRead(500);
        writing_m_sock->read((char*)chats,sizeof(Chat)*chat_cnt);

        for(int i=0; i<chat_cnt; i++)
        {
            msg="";
            msg += "[";
            msg += "Admin";
            msg += "] :";
            qDebug() << "전달받은 메시지 :" << chats[i].GetMsg();
            msg += chats[i].GetMsg();
            ui->chat_browser->addItem(msg);
        }
        ui->chat_browser->scrollToBottom();
        delete [] chats;
    }
    else
    {
        qDebug() << "전체채팅기록 없음" ;
    }
}


void Manager::on_mem_managementBtn_clicked()
{
    management=new Management(this->writing_m_sock, this);
    management->exec();
}

void Manager::on_samedaysaleBtn_clicked()
{
    daysale=new Daysale(this->writing_m_sock,this);
    daysale->exec();
}

void Manager::on_salesBydateBtn_clicked()
{
    dailymonth=new DailyMonthlySales(this->writing_m_sock,this);
    const QDate date=QDate::currentDate();
    connect(this,SIGNAL(onDailyMonthClicked(const QDate)),dailymonth,SLOT(InitDate(const QDate)));
    emit onDailyMonthClicked(date);
    dailymonth->exec();
}
