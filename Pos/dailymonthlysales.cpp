#include "dailymonthlysales.h"
#include "ui_dailymonthlysales.h"

DailyMonthlySales::DailyMonthlySales(QTcpSocket *sock,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DailyMonthlySales),arrcnt(0)
{
    ui->setupUi(this);
    this->writing_m_sock=sock;
    this->setWindowTitle("일월별 매출 내역");
    ui->salestableWidget->setColumnWidth(0,100);
    ui->salestableWidget->setColumnWidth(6,150);
}

DailyMonthlySales::~DailyMonthlySales()
{
    delete ui;
    delete date;
    qDebug()<<"DailyMonthSales 소멸자실행";
}

void DailyMonthlySales::InitDate(const QDate date)
{
    this->date=new QDate();
    this->date->setDate(date.year(),date.month(),date.day());
    qDebug()<<"connect date: "<<this->date->toString("yyyy-MM-dd");
}

void DailyMonthlySales::onDaily()
{
    onData(ManagementSig::DAY_CHOICE);
    if(arrcnt!=0)
    {
        onMainData();
    }
    else
    {
        QMessageBox::information(this,"info","해당 일자 매출이 없습니다.");
    }

}

void DailyMonthlySales::onMonth()
{
    onData(ManagementSig::MONTH_CHOICE);
    if(arrcnt!=0)
    {
        onMainData();
    }
    else
    {
        QMessageBox::information(this,"info","해당 월 매출이 없습니다.");
    }
}

void DailyMonthlySales::onMainData()
{
    salesinfo=new SalesInfo[arrcnt];
    item=new Item[arrcnt];


    QThread::usleep(300000);
    writing_m_sock->waitForReadyRead(2000);
    writing_m_sock->read((char*)salesinfo,sizeof(SalesInfo)*arrcnt);

    QThread::usleep(300000);
    writing_m_sock->waitForReadyRead(2000);
    writing_m_sock->read((char*)item,sizeof(Item)*arrcnt);

    ui->salestableWidget->setRowCount(0);
    ui->salestableWidget->scrollToTop();
    for(int i=0;i<arrcnt;i++)
    {
        qDebug()<<"상품 코드: "<<salesinfo[i].GetCode();
        qDebug()<<"주문 번호: "<<salesinfo[i].GetArrId();
        qDebug()<<"주문 수량: "<<salesinfo[i].GetAmount();
        qDebug()<<"주문 날짜: "<<salesinfo[i].GetArrDate();
        qDebug()<<"결제 수단: "<<salesinfo[i].GetPayment();
        qDebug()<<"주문 처리 상태: "<<salesinfo[i].GetArrStatus();
        qDebug()<<"상품 이름: "<<item[i].GetItemName();
        qDebug()<<"상품 가격: "<<item[i].GetItemPrice();
        PrintSales(i);
    }
    ui->totalLineEdit->setText(QString::number(tot_price)); //총 매출 표시
    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    qDebug()<<"arr read: "<<sizeof(SalesInfo)*arrcnt;
    qDebug()<<"item arr: "<<sizeof(Item)*arrcnt;
    tot_price=0;
    delete [] salesinfo;
    delete [] item;
}

void DailyMonthlySales::onData(int sig)
{
    QString date;

    if(sig==ManagementSig::MONTH_CHOICE)
        date=this->date->toString("yyyy-MM");
    else
        date=this->date->toString("yyyy-MM-dd");
    qDebug()<<"전송 sig: "<<sig;
    qDebug()<<"전송 date: "<<date;
    writing_m_sock->waitForBytesWritten(300);
    writing_m_sock->write((const char*)&sig,sizeof(sig)); //sig 송신
    writing_m_sock->waitForBytesWritten(300);
    writing_m_sock->write((const char*)date.toUtf8().data(),sizeof(char)*DefineMember::BUFFSIZE); //날짜 송신
    writing_m_sock->waitForReadyRead(500);
    writing_m_sock->read((char*)&arrcnt,sizeof(arrcnt)); //당일 날짜에 대한 DB 카운트 수신
    qDebug()<<"dailymonth arrcnt: "<<arrcnt;
}

void DailyMonthlySales::PrintSales(int num)
{
    ui->salestableWidget->insertRow(ui->salestableWidget->rowCount()); // Row를 추가합니다.
    int index = ui->salestableWidget->rowCount() - 1;
    qDebug() << "index: "<<index;

    QTableWidgetItem *id=new QTableWidgetItem(QString::number(salesinfo[num].GetArrId()));
    id->setTextAlignment(Qt::AlignCenter); //중앙 정렬
    ui->salestableWidget->setItem(index,0,id); //주문번호

    QTableWidgetItem *name=new QTableWidgetItem(item[num].GetItemName());
    name->setTextAlignment(Qt::AlignCenter);
    ui->salestableWidget->setItem(index,1,name); //상품명

    QTableWidgetItem *amount=new QTableWidgetItem(QString::number(salesinfo[num].GetAmount()));
    amount->setTextAlignment(Qt::AlignCenter);
    ui->salestableWidget->setItem(index,2,amount); //판매수량

    QTableWidgetItem *price=new QTableWidgetItem(QString::number(item[num].GetItemPrice()));
    price->setTextAlignment(Qt::AlignCenter);
    ui->salestableWidget->setItem(index,3,price); //상품금액

    QTableWidgetItem *pay=new QTableWidgetItem(QString::number(salesinfo[num].GetAmount()*item[num].GetItemPrice()));
    pay->setTextAlignment(Qt::AlignCenter);
    ui->salestableWidget->setItem(index,4,pay); //결제 금액

    if(salesinfo[num].GetPayment()==1)
    {
        QTableWidgetItem *payment=new QTableWidgetItem("현금");
        payment->setTextAlignment(Qt::AlignCenter);
        ui->salestableWidget->setItem(index,5,payment); //결제수단
    }
    else if(salesinfo[num].GetPayment()==2)
    {
        QTableWidgetItem *payment=new QTableWidgetItem("카드");
        payment->setTextAlignment(Qt::AlignCenter);
        ui->salestableWidget->setItem(index,5,payment); //결제수단
    }

    QTableWidgetItem *date=new QTableWidgetItem(salesinfo[num].GetArrDate());
    date->setTextAlignment(Qt::AlignCenter);
    ui->salestableWidget->setItem(index,6,date); //날짜시간
    tot_price+=(item[num].GetItemPrice()*salesinfo[num].GetAmount()); //총 매출액
}

void DailyMonthlySales::on_backBtn_clicked()
{
    this->close();
}

void DailyMonthlySales::on_monBtn_2_clicked()
{
    onMonth();
}

void DailyMonthlySales::on_dayBtn_2_clicked()
{
    onDaily();
}

void DailyMonthlySales::on_calendarWidget_2_currentPageChanged(int year, int month)
{
    this->date->setDate(year,month,1); //달력 페이지가 바뀌면 그달의 1일로 날짜 설정
    qDebug()<<"달력 페이지 변경: "<<this->date->toString("yyyy-MM-dd");
}

void DailyMonthlySales::on_calendarWidget_2_clicked(const QDate &date)
{
    this->date->setDate(date.year(),date.month(),date.day()); //클릭한 날짜로 설정
    qDebug()<<"날짜 클릭 변경: "<<this->date->toString("yyyy-MM-dd");
}
