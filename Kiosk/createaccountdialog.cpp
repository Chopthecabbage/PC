#include "createaccountdialog.h"
#include "ui_createaccountdialog.h"

CreateAccountDialog::CreateAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateAccountDialog)
{
    ui->setupUi(this);
    setWindowTitle("The end crowns the work.");
    // Set up the event filter
    qApp->installEventFilter(this);
    ui->sign->installEventFilter(this);
    //
    QRegExp rx_1st(ID_RX);
    QRegExp rx_2nd(NM_RX);
    QRegExp rx_3rd(PW_RX);
    QRegExp rx_4th(PW_RX);
    QRegExp rx_5th(FRONT_RX);
    QRegExp rx_6th(BACK_RX);

    vali[0] = new QRegExpValidator(rx_1st, this);
    vali[1] = new QRegExpValidator(rx_2nd, this);
    vali[2] = new QRegExpValidator(rx_3rd, this);
    vali[3] = new QRegExpValidator(rx_4th, this);
    vali[4] = new QRegExpValidator(rx_5th, this);
    vali[5] = new QRegExpValidator(rx_6th, this);
    //
    QLabel *title = new QLabel("회원가입",this);
    title->setGeometry(70,5,200,50);
    title->setStyleSheet("font-weight: bold; color: black");
    title->setFont(QFont("중간안상수체",16));

    QString lblName[4]={"*아이디","*이름","*비밀번호","*비밀번호 확인"};
    int width=200;
    int ypos=60;
    for(int i=0; i<4; i++)
    {
        lbl[i] = new QLabel(lblName[i],this);
        lbl[i]->setGeometry(25,ypos,150,20);
        edit[i] = new QLineEdit(this);
        edit[i]->setGeometry(150,ypos,width,20);
        edit[i]->setValidator(vali[i]);
        if(i==0)
            width+=100;
        else if(i==2 || i==3)
        {
            edit[i]->setMaxLength(MAX_PASSWORD);
            edit[i]->setEchoMode(QLineEdit::Password);
        }
        ypos+=50;
    }

    QString ruleName[2]={"규칙 : 첫 문자 영문 + 영문/숫자 + 4~10자", "규칙 : 영문, 숫자, 특수문자 + 6~12자"};
    ypos=80;
    for(int i=0; i<2; i++)
    {
        rule[i] = new QLabel(ruleName[i],this);
        rule[i]->setGeometry(5,ypos,300,20);
        ypos+=100;
    }
    QLabel *Leftover[3];
    QString LeftoverName[3]={"*생년원일","성별","*휴대폰번호"};
    QLabel *Rightover[3];
    QString RightoverName[3]={"년도","월","일"};
    ypos=250;
    int xpos=150;
    for(int i=0; i<3; i++)
    {
        Leftover[i] = new QLabel(LeftoverName[i],this);
        Leftover[i]->setGeometry(25,ypos,150,20);
        Rightover[i] = new QLabel(RightoverName[i],this);
        Rightover[i]->setGeometry(xpos,230,100,20);
        xpos+=110;
        ypos+=50;
    }
    xpos=150;
    for(int i=0; i<3; i++)
    {
        combo[i] = new QComboBox(this);
        combo[i]->setGeometry(xpos,250,100,20);
        xpos+=110;
    }
    for(int i=2000; i>1949; i--)
    {
        combo[0]->addItem(QString("%1%2").arg(i).arg("년"));
    }
    for(int i=0; i<12; i++)
    {
        combo[1]->addItem(QString("%1%2").arg(i+1,2,10,QLatin1Char('0')).arg("월"));
    }
    for(int i=0; i<31; i++)
    {
        combo[2]->addItem(QString("%1%2").arg(i+1,2,10,QLatin1Char('0')).arg("일"));
    }
    //
    QString rbtnName[2]={"남성","여성"};
    xpos=150;
    for(int i=0; i<2; i++)
    {
        rbtn[i] = new QRadioButton(rbtnName[i],this);
        rbtn[i]->setGeometry(xpos,300,100,20);
        xpos+=70;
    }
    //
    phone = new QComboBox(this);
    xpos=150;
    phone->setGeometry(xpos,350,80,20);

    QString phoneName[5]={"010","011","016","017","019"};
    for(int i=0; i<5; i++)
    {
        phone->addItem(phoneName[i]);
    }
    //
    xpos=260;
    for(int i=0; i<2; i++)
    {
        hp[i] = new QLineEdit(this);
        hp[i]->setGeometry(xpos,350,80,20);
        xpos+=110;
    }
    hp[0]->setValidator(vali[4]);
    hp[1]->setValidator(vali[5]);

    QLabel *sign[2];
    xpos=240;
    for(int i=0; i<2; i++)
    {
        sign[i] = new QLabel("-",this);
        sign[i]->setGeometry(xpos,350,20,20);
        xpos+=110;
    }
    QString btnName[3]={"중복확인","가입완료","닫기"};
    xpos=360;
    ypos=58;
    width=90;
    int height=24;
    for(int i=0; i<3; i++)
    {
        btn[i] = new QPushButton(btnName[i],this);
        btn[i]->setFocusPolicy(Qt::NoFocus);
        btn[i]->setGeometry(xpos,ypos,width,height);
        if(i==0) {
            xpos=120; ypos=420; width=120; height=40;
        }else {
            xpos+=150;
        }
    }
    //
    connect(edit[0],SIGNAL(returnPressed()),
            this, SLOT(overlap()));
    connect(btn[0], SIGNAL(clicked()),
            this, SLOT(overlap()));
    connect(btn[1], SIGNAL(clicked()),
            this, SLOT(complete()));
    connect(btn[2], SIGNAL(clicked()),
            this, SLOT(regression()));
}

void CreateAccountDialog::overlap()
{
    if(edit[0]->text().length()<MIN_ID)
    {
        QMessageBox::warning(this,"ID 중복체크","ID 형식이 올바르지 않습니다.",QMessageBox::Ok);
        return;
    }

    char tempID[DefineMember::BUFFSIZE];
    QString str=edit[0]->text();
    strcpy(tempID, str.toStdString().c_str());

    Type=DefineCreate::OVERLAP;
    qDebug() <<"CreateAccountDialog::overlap()";
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    qDebug() <<"socket->write((char*)&Type, sizeof(int));";
    socket->waitForBytesWritten();
    socket->write((char*)tempID, sizeof(char)*(DefineMember::BUFFSIZE));
    qDebug() <<"socket->write((char*)tempID, sizeof(char)*(DefineMember::BUFFSIZE));";

    Doppelganger=NOTHING;
    socket->waitForReadyRead();
    socket->read((char*)&Doppelganger, sizeof(int));

    if(Doppelganger==LOG_IN::WRONG)
    {
        QMessageBox::information(this,"ID 중복체크","사용 가능한 ID 입니다.",QMessageBox::Ok);
        qDebug() <<"중복x";
    }
    else if(Doppelganger == -1)
    {
        QMessageBox::critical(this,"ID 중복체크","이미 사용중인 ID 입니다.",QMessageBox::Ok);
        qDebug() <<"중복";
        return;
    }
}

void CreateAccountDialog::complete()
{
    QMessageBox box;
    box.setStandardButtons(QMessageBox::Ok);

    if(Doppelganger==NOTHING)
    {
        QMessageBox::warning(this,"메시지","회원 ID 중복확인을 해주십시오.",QMessageBox::Ok);
        return;
    }
    else if(edit[1]->text().length()<MIN_NAME)
    {
        box.setWindowTitle("메시지");
        box.setText("이름 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }
    else if(edit[2]->text().length()<MIN_PASSWORD)
    {
        box.setWindowTitle("메시지");
        box.setText("비밀번호가 올바르지 않습니다.");
        box.exec();
        return;
    }
    else if(edit[2]->text() != edit[3]->text())
    {
        box.setWindowTitle("메시지");
        box.setText("비밀번호가 일치하지 않습니다.");
        box.exec();
        return;
    }
    else if(hp[0]->text().length()<PHONE_NUMBER_FORMAT::MIDDLE)
    {
        box.setWindowTitle("메시지");
        box.setText("휴대폰번호 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }
    else if(hp[1]->text().length()<PHONE_NUMBER_FORMAT::BOTTOM)
    {
        box.setWindowTitle("메시지");
        box.setText("휴대폰번호 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }

    char tempID[DefineMember::BUFFSIZE];
    char tempNAME[DefineMember::BUFFSIZE];
    char tempPASSWORD[DefineMember::BUFFSIZE];
    QString str=edit[0]->text();
    //const char * tempID = MemberId.toStdString().c_str();

    strcpy(tempID, str.toStdString().c_str());
    people.SetMemberId(tempID);
    str=edit[1]->text();
    strcpy(tempNAME, str.toStdString().c_str());
    people.SetMemberName(tempNAME);
    str=edit[2]->text();
    sprintf(tempPASSWORD,"%s",(const char *)((QByteArray)(str.toStdString()).data()));
    people.SetMemberPassword(tempPASSWORD);

    str=combo[0]->currentText();
    QString vvvv=str.left(4);
    str=combo[1]->currentText();
    QString month=str.left(2);
    str=combo[2]->currentText();
    QString day=str.left(2);

    str=vvvv+"-"+month+"-"+day;
    char tempBIRTH[DefineMember::BUFFSIZE];
    sprintf(tempBIRTH,"%s",(const char *)str.toStdString().c_str());
    people.SetMemberBirth(tempBIRTH);
    //
    str=phone->currentText()+hp[0]->text()+hp[1]->text();
    char tempPHONE[DefineMember::BUFFSIZE];
    strcpy(tempPHONE, str.toStdString().c_str());
    people.SetMemberPhone(tempPHONE);
    // 추가 시간 넣기 위한 코드
    str="00:00";
    char tempREMAIN[DefineMember::BUFFSIZE];
    sprintf(tempREMAIN,"%s",(const char *)str.toStdString().c_str());
    people.SetMemberRemain(tempREMAIN);
    //
    people.DebugShowMember();
    //
    Type=DefineCreate::COMPLETE;
    qDebug() <<"CreateAccountDialog::complete()";
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    qDebug() <<"socket->write((char*)&Type, sizeof(int));";
    socket->waitForBytesWritten();
    socket->write((char*)&people, sizeof(Member));
    qDebug() <<"socket->write((char*)&people, sizeof(Member));";

    this->close();
}

void CreateAccountDialog::regression()
{
    this->close();
}

void CreateAccountDialog::setTcpsock(QTcpSocket *socket)
{
    this->socket=socket;
}

void CreateAccountDialog::_handleWhatsThisEntry(QWidget * /*sender*/) {
    qDebug() << "QEvent::EnterWhatsThisMode";
    QMessageBox::information( this,
                   "도움말",
                   "Talk of an angel and he will appear.",QMessageBox::Ok );
}

bool CreateAccountDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    else if ((object == ui->sign) && (event->type() == QEvent::MouseButtonPress))
    {
        QDesktopServices::openUrl(QUrl("https://www.picaplay.com/agree.do"));
        return true;
    }
    return QObject::eventFilter(object, event);
}

void CreateAccountDialog::closeEvent (QCloseEvent *event)
{
    event->accept();
    /*
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Walk on eggshells.",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
    */
}

CreateAccountDialog::~CreateAccountDialog()
{
    delete ui;
}
