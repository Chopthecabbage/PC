#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    setWindowTitle("Let bygones be bygones.");

    ui->lol->installEventFilter(this);
    ui->craft->installEventFilter(this);
    ui->pubg->installEventFilter(this);
    ui->maple->installEventFilter(this);

    timer = new QTimer();
    alarm = new QTimer();
    alarmTime = new QTime(0,0);
    ui->UsageLabel->setText(alarmTime->toString ("hh:mm"));
    connect(timer, SIGNAL(timeout()), this, SLOT(RemainTime()));
    timer->start(ONE_MINUTE);
    //timer->start(ONE_SECOND);
    connect(alarm, SIGNAL(timeout()), this, SLOT(UsageTime()));
    alarm->start(ONE_MINUTE);

    QHostAddress IP("61.108.2.70");

    guerrilla_sock = new QTcpSocket(this); // 개별
    guerrilla_sock->connectToHost(IP,PORT);

    ui->label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    ui->label->setOpenExternalLinks(true);
    ui->label->setTextFormat(Qt::RichText);
    ui->label->setText("<a href=\"http://gj.korchamhrd.net/\">프렌차이즈 모집</a>");
    ui->label->setStyleSheet("font-weight: bold");


    connect(guerrilla_sock, SIGNAL(disconnected()),
            this, SLOT(on_normal_disconnected()));

    connect(ui->FoodBtn, SIGNAL(clicked()),
            this, SLOT(Cooking()));
    connect(ui->MsgLineEdit,SIGNAL(returnPressed()),
            this, SLOT(on_MsgBtn_clicked()));
}

void MainDialog::Cooking()
{
    qDebug() << "Cooking()";
    cookdialog = new CookDialog(this);
    cookdialog->setWindowFlags( Qt::SplashScreen );
    cookdialog->setFixedSize(900,750);
    cookdialog->move(500,100);
    cookdialog->setTcpsock(guerrilla_sock);
    connect(this,SIGNAL(SendClass(Member)),cookdialog,SLOT(RecvClass(Member)));
    emit SendClass(people);
    cookdialog->exec();
}

void MainDialog::UsageTime()
{
    *alarmTime = alarmTime->addSecs (+FLOW);
     ui->UsageLabel->setText(alarmTime->toString ("hh:mm"));
}

void MainDialog::RemainTime()
{
    *objTime = objTime->addSecs (-FLOW);
    ui->RemainLabel->setText(objTime->toString ("hh:mm"));
    // 시간 변화 시그널
    Type=DefineMain::DECREASE;
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Type, sizeof(int));
    // 좌석 번호
    int Seat=people.GetMemberSeat();
    Seat=people.GetMemberSeat();
    qDebug() << "Seat: "<<Seat;
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Seat, sizeof(int));
    // objTime -> 문자열...
    QString str=objTime->toString("hh:mm");
    qDebug() <<str;
    char DecreaseTime[DefineMember::BUFFSIZE];
    sprintf(DecreaseTime,(const char *)((QByteArray)(str.toStdString()).data()));
    strcpy(DecreaseTime,str.toStdString().c_str());
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)DecreaseTime, sizeof(char)*(DefineMember::BUFFSIZE));
    // 디비에 저장할 회원의 아이디
    char ID[DefineMember::BUFFSIZE];
    strcpy(ID,people.GetMemberId());
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)ID, sizeof(char)*DefineMember::BUFFSIZE);
    // 회원 유무
    int Membership=people.GetMembership();
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Membership, sizeof(int));
    // 종료...
    if(!strcmp(DecreaseTime,"00:00"))
    {
        timer->stop();
        alarm->stop();
        // 시간 종료
        int judge; // 회원 유무에 따른...
        if(people.GetMembership()==MEMBERSHIP::MEM) // 회원 처리
        {
            Type=DefineMain::TIMEOUT;
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&Type, sizeof(int));
            // 자리 정보
            Sitting=people.GetMemberSeat()-1;
            qDebug()<<"자리 정보: "<<people.GetMemberSeat();
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&Sitting, sizeof(int));

            judge=MEMBERSHIP::MEM;
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&judge, sizeof(int));

            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&people, sizeof(Member));
            qDebug () << "시간 종료;";
        }
        else if(people.GetMembership()==MEMBERSHIP::TEM) // 비회원 처리
        {
            Type=DefineMain::TIMEOUT;
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&Type, sizeof(int));
            // 자리 정보
            Sitting=people.GetMemberSeat()-1;
            qDebug()<<"자리 정보: "<<people.GetMemberSeat();
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&Sitting, sizeof(int));

            judge=MEMBERSHIP::TEM;
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&judge, sizeof(int));

            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&people, sizeof(Member));
            qDebug () << "시간 종료";
        }
        this->close();
    }
}

void MainDialog::RecvObj(Member people)
{
    this->people=people;
    qDebug() << "받은 후 멤버 데이터 ";
    this->people.DebugShowMember();
    qDebug()<<"확인용 RecvObj: "<<this->people.GetMemberSeat();

    Type = SOCK::TYPE_USER_MAIN;
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Type, sizeof(int));

    char *ptr = strtok(this->people.GetMemberRemain(),":");
    char copy[2][50];
    int cnt=0;

    while(ptr!=NULL)
    {
        qDebug()<<ptr;
        strcpy(copy[cnt++],ptr);
        ptr=strtok(NULL,":");
    }

    for(int i=0; i<2; i++)
    {
         qDebug()<<copy[i];
    }
    QString QStringHour(copy[0]);
    QString QStringMin(copy[1]);

    int PresentHour=QStringHour.toInt();
    int PresentMin=QStringMin.toInt();
    qDebug()<<"시간: "<<PresentHour<<"분: "<<PresentMin;

    objTime = new QTime(PresentHour,PresentMin);
    ui->RemainLabel->setText(objTime->toString ("hh:mm"));

    QString QStringSeat = QString::number(this->people.GetMemberSeat());
    ui->SeatLabel->setText(QStringSeat);
    if(people.GetMembership()==MEMBERSHIP::MEM)
    {
        QString Id(this->people.GetMemberId());
        ui->IdLabel->setText(Id+" 회원님");
    }
    else if(people.GetMembership()==MEMBERSHIP::TEM)
    {
        QString Id(this->people.GetMemberId());
        ui->IdLabel->setText(Id+" 비회원님");
    }

    QString Begin(this->people.GetMemberBegin());
    ui->BeginLabel->setText(Begin);

    QHostAddress IP("61.108.2.70");
    int seat_num = this->people.GetMemberSeat();
    qDebug() << "[seat_num] : " << seat_num;
    // 시간 충전 소켓
    Type = SOCK::TYPE_USER_CHARGE;
    plus_sock = new QTcpSocket(this);
    plus_sock->connectToHost(IP,PORT);
    plus_sock->write((char*)&Type,sizeof(int)); // 타입
    connect(plus_sock,SIGNAL(readyRead()),this,SLOT(RecvExtraTimeFromMin()));
    plus_sock->write((char*)&seat_num, sizeof(int)); // 좌석
    // 채팅 소켓
    Type = SOCK::TYPE_USER_CHAT;
    chat_sock = new QTcpSocket(this);
    chat_sock->connectToHost(IP,PORT);
    connect(chat_sock,SIGNAL(readyRead()),this,SLOT(RecvChatFromKoo()));
    chat_sock->write((char*)&Type,sizeof(int)); // 타입
    chat_sock->write((char*)&seat_num, sizeof(int)); // 좌석
    // 강제종료 소켓
    Type = SOCK::TYPE_USER_BAN;
    kick_sock = new QTcpSocket(this);
    kick_sock->connectToHost(IP,PORT);
    connect(kick_sock,SIGNAL(readyRead()),this,SLOT(RecvKickFromKoo()));
    kick_sock->write((char*)&Type,sizeof(int)); // 타입
    kick_sock->write((char*)&seat_num, sizeof(int)); // 좌석

    connect(plus_sock, SIGNAL(disconnected()),
            this, SLOT(on_plus_sock_disconnected()));
    connect(chat_sock, SIGNAL(disconnected()),
            this, SLOT(on_chat_sock_disconnected()));
    connect(kick_sock, SIGNAL(disconnected()),
            this, SLOT(on_kick_sock_disconnected()));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_ShiftBtn_clicked()
{
   char remain[DefineMember::BUFFSIZE];
   timer->stop();
   alarm->stop();

   QString obj=objTime->toString("hh:mm");
   strcpy(remain,obj.toStdString().c_str());
   // 시간 종료
   people.SetMemberRemain(remain);
   qDebug()<<"남은 시간: "<<people.GetMemberRemain();
   if(people.GetMembership()==MEMBERSHIP::MEM) // 회원 처리
   {
       Type=DefineMain::MEM_SAVETIME;
       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&Type, sizeof(int));
       // 자리 정보
       Sitting=people.GetMemberSeat()-1;
       qDebug()<<"자리 정보: "<<people.GetMemberSeat();
       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&Sitting, sizeof(int));

       Force=NOTHING;
       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&Force, sizeof(int));

       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&people, sizeof(Member));
       qDebug () << "시간 종료";
   }
   else if(people.GetMembership()==MEMBERSHIP::TEM) // 비회원 처리
   {
       Type=DefineMain::TEM_SAVETIME;
       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&Type, sizeof(int));
       // 자리 정보
       Sitting=people.GetMemberSeat()-1;
       qDebug()<<"자리 정보: "<<people.GetMemberSeat();
       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&Sitting, sizeof(int));

       Force=NOTHING;
       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&Force, sizeof(int));

       guerrilla_sock->waitForBytesWritten();
       guerrilla_sock->write((char*)&people, sizeof(Member));
       qDebug () << "시간 종료";
   }
   this->close();
}

void MainDialog::on_CloseBtn_clicked()
{
    char remain[DefineMember::BUFFSIZE];
    timer->stop();
    alarm->stop();

    QString obj=objTime->toString("hh:mm");
    strcpy(remain,obj.toStdString().c_str());
    // 시간 종료
    people.SetMemberRemain(remain);
    if(people.GetMembership()==MEMBERSHIP::MEM) // 회원 처리
    {
        Type=DefineMain::MEM_SAVETIME;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Type, sizeof(int));
        // 자리 정보
        Sitting=people.GetMemberSeat()-1;
        qDebug()<<"자리 정보: "<<people.GetMemberSeat();
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Sitting, sizeof(int));

        Force=NOTHING;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Force, sizeof(int));

        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&people, sizeof(Member));
        qDebug () << "시간 종료";
    }
    else if(people.GetMembership()==MEMBERSHIP::TEM) // 비회원 처리
    {
        Type=DefineMain::TEM_SAVETIME;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Type, sizeof(int));
        // 자리 정보
        Sitting=people.GetMemberSeat()-1;
        qDebug()<<"자리 정보: "<<people.GetMemberSeat();
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Sitting, sizeof(int));

        Force=NOTHING;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Force, sizeof(int));

        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&people, sizeof(Member));
        qDebug () << "시간 종료";
    }
    this->close();
}

QString MainDialog::Time_Add(QString time1, QString time2)
{
    QStringList split_remain = time1.split(":");
    int hour1 = split_remain[0].toInt();
    int minute1 = split_remain[1].toInt();

    QStringList split_time = time2.split(":");
    int hour2 = split_time[0].toInt();
    int minute2 = split_time[1].toInt();

    int result_hour = hour1 + hour2;
    int result_minute = minute1 + minute2;

    if(result_minute >= 60)
    {
        int temp;
        result_hour += result_minute/60;
        temp = result_minute%60;
        result_minute = temp;
    }
    QString result;
    result += QString::number(result_hour);
    result += ':';
    if(result_minute < 10) result += '0';
    result += QString::number(result_minute);
    qDebug() << "result" << result;

    return result;
}

void MainDialog::RecvExtraTimeFromMin()
{
    char AddTime[DefineMember::BUFFSIZE];

    plus_sock->read((char*)AddTime, sizeof(char)*DefineMember::BUFFSIZE);

    Markov_moment=new QTime;
    Markov_moment=objTime;
    qDebug () <<"Markov_moment: "<<Markov_moment;
    qDebug () <<"obj: "<<objTime;

    QString OriginalTime=Markov_moment->toString("hh:mm");

    QString CalTime=Time_Add(OriginalTime,AddTime);

    QStringList list=CalTime.split(":");
    qDebug () <<"list: "<<list;
    int Alpha = list[0].toInt();
    int Omega = list[1].toInt();

    qDebug () <<"쪼갠 시간: "<<Alpha;
    qDebug () <<"쪼갠 분: "<<Omega;

    objTime = new QTime(Alpha,Omega);
    qDebug () <<"최종 변화된 시간: "<<objTime;
    ui->RemainLabel->setText(objTime->toString ("hh:mm"));
    // 추가된 시간
    Type=DefineMain::DECREASE;
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Type, sizeof(int));
    // 좌석 번호
    int Seat=people.GetMemberSeat();
    Seat=people.GetMemberSeat();
    qDebug() << "Seat: "<<Seat;
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Seat, sizeof(int));
    // objTime -> 문자열...
    QString str=objTime->toString("hh:mm");
    qDebug() <<str;
    char DecreaseTime[DefineMember::BUFFSIZE];
    sprintf(DecreaseTime,(const char *)((QByteArray)(str.toStdString()).data()));
    strcpy(DecreaseTime,str.toStdString().c_str());
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)DecreaseTime, sizeof(char)*(DefineMember::BUFFSIZE));
    // 디비에 저장할 회원의 아이디
    char ID[DefineMember::BUFFSIZE];
    strcpy(ID,people.GetMemberId());
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)ID, sizeof(char)*DefineMember::BUFFSIZE);
    // 회원 유무
    int Membership=people.GetMembership();
    guerrilla_sock->waitForBytesWritten();
    guerrilla_sock->write((char*)&Membership, sizeof(int));
}

void MainDialog::RecvChatFromKoo()
{
    char Msg[CHAT_SIZE];

    chat_sock ->read((char*)Msg, sizeof(char)*CHAT_SIZE);

    QString message(Msg);

    ui->MsgListWidget->addItem(QString("%1 %2").arg("[관리자]").arg(message));
    ui->MsgListWidget->scrollToBottom();
}

void MainDialog::RecvKickFromKoo()
{
    QMessageBox *noc = new QMessageBox;
    noc->setStandardButtons(0);
    noc->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    noc->setWindowTitle("메시지");
    QString er = tr("%1\n%2").arg("Ends after 3 seconds.").arg("You reap what you sow.");
    noc->setText(er);
    QTimer::singleShot(CLOSE_TIME, noc, SLOT(hide()));
    noc->exec();

    char remain[DefineMember::BUFFSIZE];
    timer->stop();
    alarm->stop();

    QString obj=objTime->toString("hh:mm");
    strcpy(remain,obj.toStdString().c_str());
    // 시간 종료
    people.SetMemberRemain(remain);
    if(people.GetMembership()==MEMBERSHIP::MEM) // 회원 처리
    {
        Type=DefineMain::MEM_SAVETIME;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Type, sizeof(int));
        // 자리 정보
        Sitting=people.GetMemberSeat()-1;
        qDebug()<<"자리 정보: "<<people.GetMemberSeat();
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Sitting, sizeof(int));

        Force=KICK;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Force, sizeof(int));

        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&people, sizeof(Member));
        qDebug () << "시간 종료";
    }
    else if(people.GetMembership()==MEMBERSHIP::TEM) // 비회원 처리
    {
        Type=DefineMain::TEM_SAVETIME;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Type, sizeof(int));
        // 자리 정보
        Sitting=people.GetMemberSeat()-1;
        qDebug()<<"자리 정보: "<<people.GetMemberSeat();
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Sitting, sizeof(int));

        Force=KICK;
        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&Force, sizeof(int));

        guerrilla_sock->waitForBytesWritten();
        guerrilla_sock->write((char*)&people, sizeof(Member));
        qDebug () << "시간 종료";
    }
    this->close();
}

void MainDialog::on_MsgBtn_clicked()
{
    QString message=ui->MsgLineEdit->text().trimmed();
    char Msg[CHAT_SIZE];
    sprintf(Msg,"%s",(const char *)message.toStdString().c_str());

    if(!message.isEmpty())
    {
        Type=8;
        guerrilla_sock->waitForBytesWritten(DELAY::TIME_1);
        guerrilla_sock->write((char*)&Type,sizeof(int));

        Sitting=people.GetMemberSeat()-1;
        qDebug()<<"자리 정보: "<<people.GetMemberSeat();
        guerrilla_sock->waitForBytesWritten(DELAY::TIME_1);
        guerrilla_sock->write((char*)&Sitting, sizeof(int));

        guerrilla_sock->waitForBytesWritten(DELAY::TIME_1);
        guerrilla_sock->write((char*)Msg,sizeof(char)*CHAT_SIZE);
        ui->MsgListWidget->addItem(QString("%1 %2").arg("[이용자]").arg(message));
        ui->MsgListWidget->setStyleSheet("color: rbg(155,0,0)");
    }
    ui->MsgListWidget->scrollToBottom();
    ui->MsgLineEdit->clear();
    ui->MsgLineEdit->setFocus();
}

bool MainDialog::eventFilter(QObject *target, QEvent *event)
{
    if ((target == ui->lol) && (event->type() == QEvent::MouseButtonPress))
    {
        QDesktopServices::openUrl(QUrl("https://na.leagueoflegends.com/ko-kr/"));
        return true;
    }
    else if ((target == ui->craft) && (event->type() == QEvent::MouseButtonPress))
    {
        QDesktopServices::openUrl(QUrl("https://starcraft.com/ko-kr/"));
        return true;
    }
    else if ((target == ui->pubg) && (event->type() == QEvent::MouseButtonPress))
    {
        QDesktopServices::openUrl(QUrl("https://pubg.game.daum.net/pubg/index.daum"));
        return true;
    }
    else if ((target == ui->maple) && (event->type() == QEvent::MouseButtonPress))
    {
        QDesktopServices::openUrl(QUrl("https://maplestory.nexon.com/Home/Main"));
        return true;
    }
    return QDialog::eventFilter(target, event);
}

void MainDialog::on_normal_discconnect()
{
    qDebug() << "disconnected => guerrilla_sock";
    disconnect(guerrilla_sock, SIGNAL(disconnected()));
    guerrilla_sock->deleteLater();
}

void MainDialog::on_plus_sock_disconnected()
{
    qDebug() << "disconnected => plus_sock";
    disconnect(plus_sock, SIGNAL(disconnected()));
    disconnect(plus_sock, SIGNAL(readyRead()));
    plus_sock->deleteLater();
}
void MainDialog::on_chat_sock_disconnected()
{
    qDebug() << "disconnected => chat_sock";
    disconnect(chat_sock, SIGNAL(disconnected()));
    disconnect(chat_sock, SIGNAL(readyRead()));
    chat_sock->deleteLater();
}
void MainDialog::on_kick_sock_disconnected()
{
    qDebug() << "disconnected => kick_sock";
    disconnect(kick_sock, SIGNAL(disconnected()));
    disconnect(kick_sock, SIGNAL(readyRead()));
    kick_sock->deleteLater();
}
