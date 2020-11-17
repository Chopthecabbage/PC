#include "cookdialog.h"
#include "ui_cookdialog.h"
#include <QUrl>
#include <QDesktopServices>

CookDialog::CookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CookDialog)
{
    ui->setupUi(this);

}

bool CookDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == fill && event->type()==QMouseEvent::MouseButtonPress)
    {
        fill->clear();
        line-=1;
        excessive=NOTHING;
    }
    return QWidget::eventFilter(watched, event);
}

void CookDialog::RecvClass(Member people)
{
    this->people=people;
    qDebug() << "void CookDialog::RecvClass(Member people)";
    this->people.DebugShowMember();
}

void CookDialog::setTcpsock(QTcpSocket *guerrilla_sock)
{
    this->guerrilla_sock=guerrilla_sock;
    //
    Type=DefineCook::MENU;
    guerrilla_sock->waitForBytesWritten(DELAY::TIME_1);
    guerrilla_sock->write((char*)&Type, sizeof(int));
    qDebug() << "setTcpsock(QTcpSocket *guerrilla_sock)";
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&TotalCount, sizeof(int));
    qDebug() << "TotalCount: " <<TotalCount;
    //
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&PREQUEL_COUNT, sizeof(int));
    qDebug() << "PREQUEL_COUNT: " <<PREQUEL_COUNT;
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&SEQUEL_COUNT, sizeof(int));
    qDebug() << "SEQUEL_COUNT: " <<SEQUEL_COUNT;
    //
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&NoodleCategoryCount, sizeof(int));
    qDebug() << "NoodleCategoryCount: " <<NoodleCategoryCount;
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&BeverageCategoryCount, sizeof(int));
    qDebug() << "BeverageCategoryCount: " <<BeverageCategoryCount;
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&SnackCategoryCount, sizeof(int));
    qDebug() << "SnackCategoryCount: " <<SnackCategoryCount;
    guerrilla_sock->waitForReadyRead(DELAY::TIME_2);
    guerrilla_sock->read((char*)&RiceCategoryCount, sizeof(int));
    qDebug() << "RiceCategoryCount: " <<RiceCategoryCount;
    //
    item_1=new Item[PREQUEL_COUNT]; item_2=new Item[SEQUEL_COUNT];
    guerrilla_sock->waitForReadyRead(DELAY::TIME_3);
    guerrilla_sock->read((char*)item_1, sizeof(Item)*PREQUEL_COUNT);
    guerrilla_sock->waitForReadyRead(DELAY::TIME_3);
    guerrilla_sock->read((char*)item_2, sizeof(Item)*SEQUEL_COUNT);
    for(int i=0; i<PREQUEL_COUNT; i++)
        item_1[i].DebugShowItem();
    for(int i=0; i<SEQUEL_COUNT; i++)
        item_2[i].DebugShowItem();
    item = new Item[PREQUEL_COUNT+SEQUEL_COUNT];
    for(int i=0; i<PREQUEL_COUNT; i++)
    {
        item[i]=item_1[i];
    }
    for(int i=0; i<SEQUEL_COUNT; i++)
    {
        item[i+PREQUEL_COUNT]=item_2[i];
    }
    for(int i=0; i<PREQUEL_COUNT+SEQUEL_COUNT; i++)
        item[i].DebugShowItem();
    tab = new QTabWidget(this);
    NoodleTab = new QWidget;
    BeverageTab = new QWidget;
    SnackTab = new QWidget;
    RiceTab = new QWidget;
    tab->setTabShape(QTabWidget::Triangular);
    tab->addTab(NoodleTab,"라면류");
    tab->addTab(BeverageTab,"음료류");
    tab->addTab(SnackTab,"과자류");
    tab->addTab(RiceTab,"식사류");
    tab->setGeometry(30,30,600,500);
    int Upstairs=25;
    int Downstairs=30;
    // 0번 탭...
    QString NoodleStr[NoodleCategoryCount];
    for(int i=0; i<NoodleCategoryCount; i++)
    {
        NoodleStr[i]=item[i].GetItemName();
    }
    QString NoodlePath[NoodleCategoryCount];
    for(int i=0; i<NoodleCategoryCount; i++)
    {
        NoodlePath[i]=item[i].GetItemPhoto();
    }
    //
    for(int i=0; i<NoodleCategoryCount; i++)
    {
        if(i==LINE_CONTROL::SHIFT)
        {
            Upstairs=LINE_CONTROL::UPSTAIRS;
            Downstairs=LINE_CONTROL::DOWNSTAIRS;
        }
        if(i<LINE_CONTROL::SHIFT)
        {
            NoodleLabel[i]=new QLabel(NoodleTab);
            NoodlePix[i]=QPixmap(NoodlePath[i]);
            NoodleLabel[i]->setPixmap(NoodlePix[i]);
            NoodleLabel[i]->setGeometry(Upstairs,50,150,150);
            NoodleLabel[i]->setScaledContents( true );
            NoodleLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            NoodleRadio[i]=new QRadioButton(NoodleStr[i],NoodleTab);
            NoodleRadio[i]->setGeometry(Downstairs,200,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
        else
        {
            NoodleLabel[i]=new QLabel(NoodleTab);
            NoodlePix[i]=QPixmap(NoodlePath[i]);
            NoodleLabel[i]->setPixmap(NoodlePix[i]);
            NoodleLabel[i]->setGeometry(Upstairs,250,150,150);
            NoodleLabel[i]->setScaledContents( true );
            NoodleLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            NoodleRadio[i]=new QRadioButton(NoodleStr[i],NoodleTab);
            NoodleRadio[i]->setGeometry(Downstairs,400,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
    }
    // 1번 탭...
    QString BeverageStr[BeverageCategoryCount];
    for(int i=0; i<BeverageCategoryCount; i++)
    {
        BeverageStr[i]=item[i+NoodleCategoryCount].GetItemName();
    }
    QString BeveragePath[BeverageCategoryCount];
    for(int i=0; i<BeverageCategoryCount; i++)
    {
        BeveragePath[i]=item[i+NoodleCategoryCount].GetItemPhoto();
    }
    //
    Upstairs=LINE_CONTROL::UPSTAIRS;
    Downstairs=LINE_CONTROL::DOWNSTAIRS;
    //
    for(int i=0; i<BeverageCategoryCount; i++)
    {
        if(i==LINE_CONTROL::SHIFT)
        {
            Upstairs=LINE_CONTROL::UPSTAIRS;
            Downstairs=LINE_CONTROL::DOWNSTAIRS;
        }
        if(i<LINE_CONTROL::SHIFT)
        {
            BeverageLabel[i]=new QLabel(BeverageTab);
            BeveragePix[i]=QPixmap(BeveragePath[i]);
            BeverageLabel[i]->setPixmap(BeveragePix[i]);
            BeverageLabel[i]->setGeometry(Upstairs,50,150,150);
            BeverageLabel[i]->setScaledContents( true );
            BeverageLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            BeverageRadio[i]=new QRadioButton(BeverageStr[i],BeverageTab);
            BeverageRadio[i]->setGeometry(Downstairs,200,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
        else
        {
            BeverageLabel[i]=new QLabel(BeverageTab);
            BeveragePix[i]=QPixmap(BeveragePath[i]);
            BeverageLabel[i]->setPixmap(BeveragePix[i]);
            BeverageLabel[i]->setGeometry(Upstairs,250,150,150);
            BeverageLabel[i]->setScaledContents( true );
            BeverageLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            BeverageRadio[i]=new QRadioButton(BeverageStr[i],BeverageTab);
            BeverageRadio[i]->setGeometry(Downstairs,400,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
    }
    // 2번 탭...
    QString SnackStr[SnackCategoryCount];
    for(int i=0; i<SnackCategoryCount; i++)
    {
        SnackStr[i]=item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemName();
    }
    QString SnackPath[SnackCategoryCount];
    for(int i=0; i<SnackCategoryCount; i++)
    {
        SnackPath[i]=item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemPhoto();
    }
    //
    Upstairs=LINE_CONTROL::UPSTAIRS;
    Downstairs=LINE_CONTROL::DOWNSTAIRS;
    //
    for(int i=0; i<SnackCategoryCount; i++)
    {
        if(i==LINE_CONTROL::SHIFT)
        {
            Upstairs=LINE_CONTROL::UPSTAIRS;
            Downstairs=LINE_CONTROL::DOWNSTAIRS;
        }
        if(i<3)
        {
            SnackLabel[i]=new QLabel(SnackTab);
            SnackPix[i]=QPixmap(SnackPath[i]);
            SnackLabel[i]->setPixmap(SnackPix[i]);
            SnackLabel[i]->setGeometry(Upstairs,50,150,150);
            SnackLabel[i]->setScaledContents( true );
            SnackLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            SnackRadio[i]=new QRadioButton(SnackStr[i],SnackTab);
            SnackRadio[i]->setGeometry(Downstairs,200,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
        else
        {
            SnackLabel[i]=new QLabel(SnackTab);
            SnackPix[i]=QPixmap(SnackPath[i]);
            SnackLabel[i]->setPixmap(SnackPix[i]);
            SnackLabel[i]->setGeometry(Upstairs,250,150,150);
            SnackLabel[i]->setScaledContents( true );
            SnackLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            SnackRadio[i]=new QRadioButton(SnackStr[i],SnackTab);
            SnackRadio[i]->setGeometry(Downstairs,400,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
    }
    // 3번 탭...
    QString RiceStr[RiceCategoryCount];
    for(int i=0; i<RiceCategoryCount; i++)
    {
        RiceStr[i]=item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemName();
    }
    QString RicePath[RiceCategoryCount];
    for(int i=0; i<RiceCategoryCount; i++)
    {
        RicePath[i]=item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemPhoto();
    }
    //
    Upstairs=LINE_CONTROL::UPSTAIRS;
    Downstairs=LINE_CONTROL::DOWNSTAIRS;
    //
    for(int i=0; i<RiceCategoryCount; i++)
    {
        if(i==LINE_CONTROL::SHIFT)
        {
            Upstairs=LINE_CONTROL::UPSTAIRS;
            Downstairs=LINE_CONTROL::DOWNSTAIRS;
        }
        if(i<LINE_CONTROL::SHIFT)
        {
            RiceLabel[i]=new QLabel(RiceTab);
            RicePix[i]=QPixmap(RicePath[i]);
            RiceLabel[i]->setPixmap(RicePix[i]);
            RiceLabel[i]->setGeometry(Upstairs,50,150,150);
            RiceLabel[i]->setScaledContents( true );
            RiceLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            RiceRadio[i]=new QRadioButton(RiceStr[i],RiceTab);
            RiceRadio[i]->setGeometry(Downstairs,200,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
        else
        {
            RiceLabel[i]=new QLabel(RiceTab);
            RicePix[i]=QPixmap(RicePath[i]);
            RiceLabel[i]->setPixmap(RicePix[i]);
            RiceLabel[i]->setGeometry(Upstairs,250,150,150);
            RiceLabel[i]->setScaledContents( true );
            RiceLabel[i]->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            Upstairs += LINE_CONTROL::PLUS_X;
            RiceRadio[i]=new QRadioButton(RiceStr[i],RiceTab);
            RiceRadio[i]->setGeometry(Downstairs,400,150,30);
            Downstairs += LINE_CONTROL::PLUS_X;
        }
    }
    // 그룹 박스...
    CountCheckBox = new QGroupBox(this);
    CountCheckBox->setGeometry(650,50,100,50);
    CountCheckBox->setTitle("수량");
    // 스핀 박스...
    Spin = new QSpinBox(CountCheckBox);
    Spin->setMinimum(0);
    Spin->setMaximum(9);
    Spin->setGeometry(650,60,60,30);
    Spin->setSuffix(" 개");
    Grid = new QGridLayout(CountCheckBox);
    Grid->addWidget(Spin);
    // 선택 버튼...
    AlphaBnt = new QPushButton("선택",this);
    AlphaBnt->setGeometry(650,100,100,50);
    // 사이드바...
    BasketBox = new QGroupBox(this);
    BasketBox->setGeometry(650,50,200,150);
    BasketBox->setTitle("사이드바");
    // 수직 레이아웃...
    layout = new QVBoxLayout(BasketBox);
    layout->addWidget(CountCheckBox);
    layout->addWidget(AlphaBnt);
    // 장바구니 담기 버튼
    OmegaBnt = new QPushButton("장바구니 담기",this);
    OmegaBnt->setGeometry(650,300,120,30);
    // 전체 취소...
    push[0] = new QPushButton("전체취소",this);
    push[0]->setGeometry(650,340,120,30);
    // 처음으로...
    push[1] = new QPushButton("처음으로",this);
    push[1]->setGeometry(650,380,120,30);
    // 주문수량...
    lbl[0] = new QLabel(this);
    lbl[0]->setGeometry(770,540,100,50);
    lbl[0]->setText("전체수량");
    lbl[0]->setStyleSheet("font-weight: bold; color: red");
    lbl[0]->setFont(QFont("맑은 고딕",15));
    lbl[1] = new QLabel(this);
    lbl[1]->setGeometry(770,500,100,50);
    lbl[1]->setText("");
    // 합계금액
    lbl[2] = new QLabel(this);
    lbl[2]->setGeometry(770,640,100,50);
    lbl[2]->setText("합계금액");
    lbl[2]->setStyleSheet("font-weight: bold; color: red");
    lbl[2]->setFont(QFont("맑은 고딕",15));
    lbl[3] = new QLabel(this);
    lbl[3]->setGeometry(770,600,100,50);
    lbl[3]->setText("");
    // 전송
    push[2] = new QPushButton("결제진행",this);
    push[2]->setGeometry(650,420,120,30);
    connect(AlphaBnt,SIGNAL(clicked()),this,SLOT(selection()));
    connect(OmegaBnt,SIGNAL(clicked()),this,SLOT(shopping()));
    connect(push[0],SIGNAL(clicked()),this,SLOT(revoke()));
    connect(push[1],SIGNAL(clicked()),this,SLOT(throwback()));
    connect(push[2],SIGNAL(clicked()),this,SLOT(forwarding()));
}

void CookDialog::selection()
{
    bool choose=true;
    int point;
    int stack;
    // 장바구니 꽉 차 있을 경우
    if(arr.GetBaggageTotalCount()==DefineCook::MAX_CART)
        return;
    if(tab->currentIndex()==CATEGORY::NOODLE) // 라면류
    {
        stack=NOTHING;
        if(Spin->value()==NOTHING)
        {
            QMessageBox::warning(this,"메시지","A slap on the wrist.",QMessageBox::Ok);
            return;
        }
        for(int i=0; i<NoodleCategoryCount; i++)
        {
            if(NoodleRadio[i]->isChecked())
            {
                if(excessive != ALREADY_HAVE)
                {
                    instant.SetItem(item[i].GetItemCode(),item[i].GetItemName(),item[i].GetItemPrice(),item[i].GetItemCategory(),item[i].GetItemPhoto());
                    amount=Spin->value();
                    choose=false;
                    point=i;
                    stack=ALREADY_HAVE;
                }
                else
                {
                    QMessageBox::information(this,"메시지","Wait in the wings.",QMessageBox::Ok);
                    qDebug() << "Wait in the wings";
                    return;
                }
            }
        }
        if(stack==NOTHING)
            return;
    }
    if(tab->currentIndex()==CATEGORY::BEVERAGE) // 음료
    {
        stack=NOTHING;
        if(Spin->value()==NOTHING)
        {
            QMessageBox::warning(this,"메시지","A slap on the wrist.",QMessageBox::Ok);
            return;
        }
        for(int i=0; i<BeverageCategoryCount; i++)
        {
            if(BeverageRadio[i]->isChecked())
            {
                if(excessive != ALREADY_HAVE)
                {
                    instant.SetItem(item[i+NoodleCategoryCount].GetItemCode(),item[i+NoodleCategoryCount].GetItemName(),item[i+NoodleCategoryCount].GetItemPrice(),item[i+NoodleCategoryCount].GetItemCategory(),item[i+NoodleCategoryCount].GetItemPhoto());
                    amount=Spin->value();
                    choose=false;
                    point=i+NoodleCategoryCount;
                    stack=ALREADY_HAVE;
                }
                else
                {
                    QMessageBox::information(this,"메시지","Wait in the wings.",QMessageBox::Ok);
                    qDebug() << "Wait in the wings";
                    return;
                }
            }
        }
        if(stack==NOTHING)
            return;
    }
    if(tab->currentIndex()==CATEGORY::SNACK) // 과자
    {
        stack=NOTHING;
        if(Spin->value()==NOTHING)
        {
            QMessageBox::warning(this,"메시지","A slap on the wrist.",QMessageBox::Ok);
            return;
        }
        for(int i=0; i<SnackCategoryCount; i++)
        {
            if(SnackRadio[i]->isChecked())
            {
                if(excessive != ALREADY_HAVE)
                {
                    instant.SetItem(item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemCode(),item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemName(),item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemPrice(),item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemCategory(),item[i+NoodleCategoryCount+BeverageCategoryCount].GetItemPhoto());
                    amount=Spin->value();
                    choose=false;
                    point=i+NoodleCategoryCount+BeverageCategoryCount;
                    stack=ALREADY_HAVE;
                }
                else
                {
                    QMessageBox::information(this,"메시지","Wait in the wings.",QMessageBox::Ok);
                    qDebug() << "Wait in the wings";
                    return;
                }
            }
        }
        if(stack==NOTHING)
            return;
    }
    if(tab->currentIndex()==CATEGORY::RICE) // 식사
    {
        stack=NOTHING;
        if(Spin->value()==NOTHING)
        {
            QMessageBox::warning(this,"메시지","A slap on the wrist.",QMessageBox::Ok);
            return;
        }
        for(int i=0; i<RiceCategoryCount; i++)
        {
            if(RiceRadio[i]->isChecked())
            {
                if(excessive != ALREADY_HAVE)
                {
                    instant.SetItem(item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemCode(),item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemName(),item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemPrice(),item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemCategory(),item[i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount].GetItemPhoto());
                    amount=Spin->value();
                    choose=false;
                    point=i+NoodleCategoryCount+BeverageCategoryCount+SnackCategoryCount;
                    stack=ALREADY_HAVE;
                }
                else
                {
                    QMessageBox::information(this,"메시지","Wait in the wings.",QMessageBox::Ok);
                    qDebug() << "Wait in the wings";
                    return;
                }
            }
        }
        if(stack==NOTHING)
            return;
    }
    // 같은 품목 처리...
    if(arr.GetBaggageTotalCount()>NOTHING)
    {
        for(int i=0; i<arr.GetBaggageTotalCount(); i++)
        {
            if(!strcmp(item[point].GetItemName(),arr.GetBaggageIndividualName(i)))
            {
                qDebug() << "중복";
                QMessageBox::warning(this,"메시지","More haste, less speed.",QMessageBox::Ok);
                return;
            }
        }
    }
    //
    if(Spin->value()!=NOTHING)
        QMessageBox::about(this,"메시지","Don't whistle until you are out of the wood.");
    // 선택
    if(choose==false)
    {
        line+=1;
        if(excessive!=ALREADY_HAVE)
        {
            fill = new QLabel(this);
            fill->installEventFilter(this);
            fill->setGeometry(650, 200, 150, 100);
            fill->show();
            fill->setText(QString("%1\n%2\n%3%4%5%6%7").arg("선택 목록").arg(item[point].GetItemName()).arg(item[point].GetItemPrice()).arg("원 ").arg("x ").arg(Spin->value()).arg("개"));
            qDebug()<< item[point].GetItemPrice();
            qDebug()<< Spin->value();
            fill->setWordWrap(true);
            excessive=ALREADY_HAVE;
        }
        else
        {
            QMessageBox::warning(this,"메시지","Wait in the wings",QMessageBox::Ok);
            return;
        }
    }
}
// 장바구니 함수
void CookDialog::shopping()
{
    qDebug()<<"머지";
    if(arr.GetBaggageTotalCount()==DefineCook::MAX_CART)
        return;
    if(arr.GetBaggageTotalCount()+1==line)
    {
        qDebug()<<"추가";
        fill->clear();
        arr.AddItem(instant.GetItem(),amount);
        //
        full[begin] = new QLabel(this);
        full[begin]->setGeometry(position, 525, 150, 100);
        full[begin]->show();
        full[begin]->setText(QString("%1%2\n%3\n%4%5%6%7%8").arg(begin+1).arg("번 목록").arg(arr.QStringBaggageIndividualName(begin)).arg(arr.QStringBaggageIndividualPrice(begin)).arg("원 ").arg("x ").arg(arr.QStringItemIndividualCount(begin)).arg("개"));
        full[begin]->setWordWrap(true);
        //
        int match;
        for(int i=0; i<TotalCount; i++)
        {
            if(!strcmp(arr.GetBaggageIndividualName(begin),item[i].GetItemName()))
            {
                match=i;
            }
        }
        for(int i=0; i<arr.GetBaggageTotalCount(); i++)
        {
            arr.DebugShowItem(i);
        }
        pix=QPixmap(item[match].GetItemPhoto());
        field[begin] = new QLabel(this);
        field[begin]->show();
        field[begin]->setPixmap(pix.scaled(100,150, Qt::KeepAspectRatio));
        field[begin]->setGeometry(position, 600, 150, 100);
        //
        position += 150;
        excessive=NOTHING;
        begin++;
        // 전체수량
        lbl[1]->setText(QString("%1%2").arg(arr.GetBaggageTotalCount()).arg("개"));
        lbl[1]->setStyleSheet("font-weight: bold; color: black");
        lbl[1]->setFont(QFont("맑은 고딕",25));
        // 합계금액
        lbl[3]->setText(QString("%1%2").arg(arr.GetBaggageTotalPrice()).arg("원"));
        lbl[3]->setStyleSheet("font-weight: bold; color: black");
        lbl[3]->setFont(QFont("맑은 고딕",15));
        Spin->setValue(0);
    }
}
// 전체취소
void CookDialog::revoke()
{
    if(arr.GetBaggageTotalCount()>NOTHING)
    {
        qDebug()<< "전체취소";
        for(int i=0; i<arr.GetBaggageTotalCount(); i++)
        {
            full[i]->clear();
            field[i]->clear();
            qDebug()<< i <<"번 삭제";
        }
        // 초기화...
        arr=resurrection;
        line=0;
        excessive=NOTHING;
        begin=0;
        position=30;
        lbl[1]->setText("");
        lbl[3]->setText("");
        Spin->setValue(0);
        fill->clear();
        qDebug()<< arr.GetBaggageTotalCount();
        for(int i=0; i<arr.GetBaggageTotalCount(); i++)
        {
            arr.DebugShowItem(i);
        }
    }
}
// 처음으로
void CookDialog::throwback()
{
    /*
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState
        || socket->waitForDisconnected(1000)) {
            qDebug("Disconnected!");
    }
    */
    this->close();
}
// 결제
void CookDialog::forwarding()
{
    if(arr.GetBaggageTotalCount()>NOTHING)
    {
        paydialog = new PayDialog(this);
        paydialog->setFixedSize(370,150);
        //paydialog->setWindowFlags( Qt::CustomizeWindowHint );
        // set black background
        /*
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::black);
        p_dialog->setAutoFillBackground(true);
        p_dialog->setPalette(pal);
        */
        qDebug() << "초기 결제수단: " << arr.GetPayment();
        connect(this,SIGNAL(SendClass(Arrangement)),paydialog,SLOT(RecvClass(Arrangement)));
        emit SendClass(arr);
        paydialog->exec();
        qDebug() <<"반환된 결제수단: " << arr.GetPayment();
        if(decision==NOTHING)
        {
            Type=DefineCook::PAYMENT;
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&Type, sizeof(int));
            guerrilla_sock->waitForBytesWritten();
            guerrilla_sock->write((char*)&arr, sizeof(Arrangement));
            qDebug() <<"guerrilla_sock->write((char*)&arr, sizeof(Arrangement));";
            this->close();
        }
        else
            decision=NOTHING;
    }
}

void CookDialog::RecvInt(int decision)
{
    this->decision=decision;
}

void CookDialog::RecvRecurringClass(Arrangement arr)
{
    this->arr=arr;
}

void CookDialog::closeEvent (QCloseEvent *event)
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
CookDialog::~CookDialog()
{
    delete[] item_2;
    delete[] item_1;
    delete[] item;

    delete ui;
}
