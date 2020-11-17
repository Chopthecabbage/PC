#include "widget.h"
#include "ui_widget.h"
#include "arrangement.h"
// 대기화면
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    QPixmap bg(":/example/background.jpg");
    bg = bg.scaled(1200,742, Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bg);
    this->setPalette(palette);
    this->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    QHostAddress IP("61.108.2.70");

    socket->connectToHost(IP,PORT);

    Type=SOCK::TYPE_USER_LOGIN;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));

    socket->waitForReadyRead();
    socket->read((char*)&Chair, sizeof(int)); // 좌석 수 받기
    qDebug () << "좌석의 수: " << Chair;

    setWindowTitle("3rd Project");

    QRegExp rx_1st(ID_RX);
    QRegExp rx_2nd(PW_RX);
    QRegExp rx_3rd(TP_RX);

    vali[0] = new QRegExpValidator(rx_1st, this);
    vali[1] = new QRegExpValidator(rx_2nd, this);
    vali[2] = new QRegExpValidator(rx_3rd, this);

    QString lblName[3]={"아이디","비밀번호","비회원(카드번호)"};
    int ypos=600;
    for(int i=0; i<3; i++)
    {
        lbl[i] = new QLabel(lblName[i],this);
        lbl[i]->setGeometry(422,ypos-7,180,30);
        lbl[i]->setAlignment(Qt::AlignRight);
        lbl[i]->setStyleSheet("color: rgb(204, 204, 0)");
        lbl[i]->setFont(QFont("HY동녘M",14));

        edit[i] = new QLineEdit(this);
        edit[i]->setFont(QFont("휴먼매직체",10));
        edit[i]->setGeometry(610,ypos-5,200,20);
        edit[i]->setValidator(vali[i]);
        ypos+=20;
    }
    edit[1]->setMaxLength(MAX_PASSWORD);
    edit[1]->setEchoMode(QLineEdit::Password);

    btn[0] = new QPushButton("로그인",this);
    btn[0]->setFont(QFont("휴먼매직체",11));
    btn[0]->setStyleSheet("background-color: rgb(204, 153, 102)");
    btn[0]->setGeometry(814,594,92,62);
    btn[1] = new QPushButton("회원가입",this);
    btn[1]->setFont(QFont("휴먼매직체",11));
    btn[1]->setStyleSheet("background-color: rgb(153, 204, 000)");
    btn[1]->setGeometry(905,594,120,31);
    btn[2] = new QPushButton("아이디/비번찾기",this);
    btn[2]->setFont(QFont("휴먼매직체",11));
    btn[2]->setStyleSheet("background-color: rgb(204, 255, 153)");
    btn[2]->setGeometry(905,625,120,31);
    // 시계
    label_date_time = new QLabel(this);
    label_date_time->setGeometry(230, 585, 250, 30);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myfunction()));
    timer->start(ONE_SECOND);
    // 좌석
    combo = new QComboBox(this);
    combo->setGeometry(350,420,150,25);
    combo->setFont(QFont("함초롬돋움",11));
    for(int i=0; i<Chair; i++)
    {
        combo->addItem(QString("%1%2%3").arg(i+1,2,10,QLatin1Char('0')).arg("번").arg(" 좌석"));
    }
    connect(edit[1],SIGNAL(returnPressed()),
            this, SLOT(Log_In()));
    connect(edit[2],SIGNAL(returnPressed()),
            this, SLOT(Log_In()));
    //
    connect(btn[0],SIGNAL(clicked()),this,SLOT(Log_In()));
    connect(btn[1],SIGNAL(clicked()),this,SLOT(CreateAccount()));
    connect(btn[2],SIGNAL(clicked()),this,SLOT(FindAccount()));
    // 종료 단축키...
    QAction *cls = new QAction(this);
    cls->setShortcut(Qt::Key_Q | Qt::CTRL);
    //
    connect(cls, SIGNAL(triggered()), this, SLOT(close()));
    this->addAction(cls);

    connect(socket, SIGNAL(disconnected()),
            this, SLOT(on_discconnected()));
}

void Widget::Log_In()
{
    // 회원
    if(edit[2]->text().length()==NOTHING && edit[1]->text().length()!=NOTHING && edit[0]->text().length()!=NOTHING)
    {
        qDebug() << "회원";
        QMessageBox box;
        box.setStandardButtons(QMessageBox::Ok);
        //box.setButtonText(QMessageBox::Ok,"확인");

        if(edit[0]->text().length()<MIN_ID)
        {
            box.setWindowTitle("메시지");
            box.setText("아이디 형식이 올바르지 않습니다.");
            box.exec();
            return;
        }
        else if(edit[1]->text().length()<MIN_PASSWORD)
        {
            box.setWindowTitle("메시지");
            box.setText("비밀번호 형식이 올바르지 않습니다.");
            box.exec();
            return;
        }
        // 로그인
        Type=DefineWidget::IN;
        socket->waitForBytesWritten();
        socket->write((char*)&Type, sizeof(int));
        // 회원
        Type=DefineWidget::PERMANENT;
        socket->waitForBytesWritten();
        socket->write((char*)&Type, sizeof(int));
        //
        char tempID[DefineMember::BUFFSIZE];
        QString str=edit[0]->text();
        qDebug() << str;
        strcpy(tempID, str.toStdString().c_str());
        // 아이디 전송
        socket->waitForBytesWritten();
        socket->write((char*)tempID, sizeof(char)*(DefineMember::BUFFSIZE));

        char tempPW[DefineMember::BUFFSIZE];
        str=edit[1]->text();
        qDebug() << str;
        strcpy(tempPW, str.toStdString().c_str());
        // 비밀번호 전송
        socket->waitForBytesWritten();
        socket->write((char*)tempPW, sizeof(char)*(DefineMember::BUFFSIZE));
        // 좌석 전송
        int idx=combo->currentIndex();
        qDebug () << "int idx=combo->currentIndex(): "<<idx;
        socket->waitForBytesWritten();
        socket->write((char*)&idx, sizeof(int));

        Doppelganger=NOTHING;
        socket->waitForReadyRead();
        socket->read((char*)&Doppelganger, sizeof(int));
        qDebug() << "Doppelganger: "<<Doppelganger;
        //
        if(Doppelganger==LOG_IN::WRONG)
        {
            QMessageBox::question(this,"메시지","가입하지 않은 아이디이거나, 잘못된 비밀번호입니다.",QMessageBox::Ok);
        }
        else if(Doppelganger==-1) // 로그인
        {
            Login_Process();
        }
        else if(Doppelganger==LOG_IN::NOT_ENOUGH)
        {
            QMessageBox::critical(this,"메시지","요금이 부족합니다.",QMessageBox::Ok);
            for(int i=0; i<3; i++) // 입력 초기화
            {
                edit[i]->clear();
            }
        }
        else if(Doppelganger==LOG_IN::OVER_LAP)
            QMessageBox::critical(this,"메시지","이미 선택된 좌석입니다.",QMessageBox::Ok);
    }
    else if(edit[2]->text().length()!=NOTHING && edit[1]->text().length()==NOTHING && edit[0]->text().length()==NOTHING)
    {
        qDebug() << "비회원";
        QMessageBox box;
        box.setStandardButtons(QMessageBox::Ok);

        if(edit[2]->text().length()<MIN_CARD)
        {
            box.setWindowTitle("메시지");
            box.setText("카드번호 형식이 올바르지 않습니다.");
            box.exec();
            return;
        }
        // 로그인
        Type=DefineWidget::IN;
        socket->waitForBytesWritten();
        socket->write((char*)&Type, sizeof(int));
        // 비회원
        Type=DefineWidget::TEMPORARY;
        socket->waitForBytesWritten();
        socket->write((char*)&Type, sizeof(int));
        // 카드번호 전송
        char cardID[DefineMember::BUFFSIZE];
        QString str=edit[2]->text();
        qDebug() << str;
        strcpy(cardID, str.toStdString().c_str());
        socket->waitForBytesWritten();
        socket->write((char*)cardID, sizeof(char)*(DefineMember::BUFFSIZE));
        // 좌석 전송
        int idx=combo->currentIndex();
        qDebug () << "int idx=combo->currentIndex(): "<<idx;
        socket->waitForBytesWritten();
        socket->write((char*)&idx, sizeof(int));

        Doppelganger=NOTHING;
        socket->waitForReadyRead();
        socket->read((char*)&Doppelganger, sizeof(int));

        if(Doppelganger==LOG_IN::WRONG)
            QMessageBox::question(this,"메시지","잘못된 카드번호입니다.",QMessageBox::Ok);
        else if(Doppelganger==-1) // 로그인
        {
            Login_Process();
        }
        else if(Doppelganger==LOG_IN::NOT_ENOUGH)
        {
            QMessageBox::critical(this,"메시지","요금이 부족합니다.",QMessageBox::Ok);
            for(int i=0; i<3; i++) // 입력 초기화
            {
                edit[i]->clear();
            }
        }
        else if(Doppelganger==LOG_IN::OVER_LAP)
            QMessageBox::critical(this,"메시지","이미 선택된 좌석입니다.",QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning( this,
                       "메시지",
                       "입력 형식이 올바르지 않습니다.",QMessageBox::Ok );
        return;
    }
}

void Widget::Login_Process()
{
    socket->waitForReadyRead();
    socket->read((char*)&people, sizeof(Member));
    people.DebugShowMember();

    QString time_format="HH:mm";
    QTime time=QTime::currentTime();
    QString time_text=time.toString(time_format);
    char Begin[DefineMember::BUFFSIZE];
    sprintf(Begin,"%s",(const char *)time_text.toStdString().c_str());
    //
    people.SetMemberSeat(combo->currentIndex()+1);
    people.SetMemberBegin(Begin);

    SendMemberClass();
    //
    maindialog = new MainDialog;
    maindialog->setFixedSize(455,400);
    //maindialog->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    maindialog->setWindowFlags( (Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint) & ~Qt::WindowCloseButtonHint );
    maindialog->move(1465,0);
    connect(this,SIGNAL(SendObj(Member)),maindialog,SLOT(RecvObj(Member)));
    emit SendObj(people);
    qDebug () << "객체";
    people.DebugShowMember();
    qDebug () << "SendObj(Member)";
    for(int i=0; i<3; i++) // 입력 초기화
    {
        edit[i]->clear();
    }
    this->hide();
    maindialog->exec();
    this->show();
    // 종료 -> 초기화
    people.InitializeMember();

    qDebug() << "종료 후 좌석의 상태 변화";
    people.DebugShowMember();

}

void Widget::SendMemberClass()
{
    qDebug () << "SendMemberClass()";
    Type=DefineWidget::PASS;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));

    socket->waitForBytesWritten();
    socket->write((char*)&people, sizeof(Member));
    qDebug () << "void Widget::SendMemberClass()";
}

void Widget::CreateAccount()
{
    createaccountdialog = new CreateAccountDialog;
    createaccountdialog->setFixedSize(500,500);
    createaccountdialog->setTcpsock(socket);
    createaccountdialog->exec();
}

void Widget::FindAccount()
{
    findaccountdialog = new FindAccountDialog;
    findaccountdialog->setFixedSize(500,300);
    findaccountdialog->setTcpsock(socket);
    findaccountdialog->exec();
}

void Widget::myfunction()
{
    QString time_format = "yyyy-MM-dd AP HH:mm";
    QDateTime time = QDateTime::currentDateTime();
    QString time_text = time.toString(time_format);

    label_date_time->setText(time_text);
    label_date_time->setStyleSheet("color: rgb(204, 204, 0)");
    label_date_time->setFont(QFont("Blippo Blk BT",14));
}

void Widget::on_discconnected()
{
    qDebug() << "disconnected => socket";
    disconnect(socket, SIGNAL(disconnected()));
    socket->deleteLater();
}

Widget::~Widget()
{
    delete ui;
}
