#include "daysale.h"
#include "ui_daysale.h"

Daysale::Daysale(QTcpSocket *sock,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Daysale),tot_price(0)
{
    ui->setupUi(this);
    this->writing_m_sock=sock;
    this->setWindowTitle("당일 매출 내역");
    ui->saletableWidget->setColumnWidth(0,100);
    ui->saletableWidget->setColumnWidth(6,150);
    onDaySearch();
}

Daysale::~Daysale()
{
    delete ui;
    qDebug()<<"Daysale 소멸자";
}


void Daysale::onDaySearch() //매출 조회창
{
    arrcnt=onData(arrcnt);
    if(arrcnt!=0)
    {
        salesinfo=new SalesInfo[arrcnt];
        item=new Item[arrcnt];
        QThread::usleep(300000);
        writing_m_sock->waitForReadyRead(500);
        writing_m_sock->read((char*)salesinfo,sizeof(SalesInfo)*arrcnt);
        QThread::usleep(300000);
        writing_m_sock->waitForReadyRead(500);
        writing_m_sock->read((char*)item,sizeof(Item)*arrcnt);
        ui->saletableWidget->setRowCount(0);
        ui->saletableWidget->scrollToTop();
        for(int i=0;i<arrcnt;i++)
        {
            qDebug()<<"Code: "<<salesinfo[i].GetCode();
            qDebug()<<"Arr_id: "<<salesinfo[i].GetArrId();
            qDebug()<<"amount: "<<salesinfo[i].GetAmount();
            qDebug()<<"Arr_date: "<<salesinfo[i].GetArrDate();
            qDebug()<<"payment: "<<salesinfo[i].GetPayment();
            qDebug()<<"Arr_Status: "<<salesinfo[i].GetArrStatus();
            qDebug()<<"item_name: "<<item[i].GetItemName();
            qDebug()<<"item_price: "<<item[i].GetItemPrice();
            PrintDaySales(i);
        }
        ui->totalsalesLineEdit->setText(QString::number(tot_price)); //총 매출 표시
        ui->totalsalesLineEdit->setAlignment(Qt::AlignRight); //오른정렬
        delete [] salesinfo;
        delete [] item;
    }

    else
    {
        QMessageBox::information(this,"info","해당 날짜 매출 내역이 없습니다.");
    }
}

int Daysale::onData(int arrcnt)
{
    int sig=ManagementSig::DAY_SERACH;

    QString date=QDate::currentDate().toString("yyyy-MM-dd");
    ui->dateLineEdit->setText(date);

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)&sig,sizeof(sig));

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)date.toUtf8().data(),sizeof(char)*DefineMember::BUFFSIZE); //당일 날짜 송신

    writing_m_sock->waitForReadyRead(500);
    writing_m_sock->read((char*)&arrcnt,sizeof(arrcnt)); //당일 날짜에 대한 DB 카운트 수신
    qDebug()<<"daysale arrcnt: "<<arrcnt;

    return arrcnt;
}

void Daysale::PrintDaySales(int num)
{
    ui->saletableWidget->insertRow(ui->saletableWidget->rowCount()); // Row를 추가합니다.
    int index = ui->saletableWidget->rowCount() - 1;
    qDebug() << "index: "<<index;

    QTableWidgetItem *id=new QTableWidgetItem(QString::number(salesinfo[num].GetArrId()));
    id->setTextAlignment(Qt::AlignCenter); //중앙 정렬
    ui->saletableWidget->setItem(index,0,id); //주문번호

    QTableWidgetItem *name=new QTableWidgetItem(item[num].GetItemName());
    name->setTextAlignment(Qt::AlignCenter);
    ui->saletableWidget->setItem(index,1,name); //상품명

    QTableWidgetItem *amount=new QTableWidgetItem(QString::number(salesinfo[num].GetAmount()));
    amount->setTextAlignment(Qt::AlignCenter);
    ui->saletableWidget->setItem(index,2,amount); //판매수량

    QTableWidgetItem *price=new QTableWidgetItem(QString::number(item[num].GetItemPrice()));
    price->setTextAlignment(Qt::AlignCenter);
    ui->saletableWidget->setItem(index,3,price); //상품금액

    QTableWidgetItem *pay=new QTableWidgetItem(QString::number(salesinfo[num].GetAmount()*item[num].GetItemPrice()));
    pay->setTextAlignment(Qt::AlignCenter);
    ui->saletableWidget->setItem(index,4,pay); //결제 금액

    if(salesinfo[num].GetPayment()==1)
    {
        QTableWidgetItem *payment=new QTableWidgetItem("현금");
        payment->setTextAlignment(Qt::AlignCenter);
        ui->saletableWidget->setItem(index,5,payment); //결제수단
    }
    else if(salesinfo[num].GetPayment()==2)
    {
        QTableWidgetItem *payment=new QTableWidgetItem("카드");
        payment->setTextAlignment(Qt::AlignCenter);
        ui->saletableWidget->setItem(index,5,payment); //결제수단
    }

    QTableWidgetItem *date=new QTableWidgetItem(salesinfo[num].GetArrDate());
    date->setTextAlignment(Qt::AlignCenter);
    ui->saletableWidget->setItem(index,6,date); //날짜시간
    tot_price+=(item[num].GetItemPrice()*salesinfo[num].GetAmount()); //총 매출액
}

void Daysale::on_backBtn_clicked()
{
    this->close();
}
