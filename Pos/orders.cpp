#include "orders.h"
#include "ui_orders.h"

Orders::Orders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Orders),
    writing_sock(nullptr),
    order_num(0),
    current_order_page(1),
    last_order_page(1)
{
    ui->setupUi(this);

    QStringList tableHeader;
    tableHeader << "메뉴명" << "가격" << "수량" << "주문금액";
    // 주문 테이블뷰 칼럼개수를 정하다.
    ui->order_info_table->setColumnCount(5);
    // 칼럼명을 적는다.
    ui->order_info_table->setHorizontalHeaderLabels(tableHeader);
    // 칼럼간격을 설정하다.
    ui->order_info_table->setColumnWidth(0,50);
    ui->order_info_table->setColumnWidth(1,125);
    ui->order_info_table->setColumnWidth(2,90);
    ui->order_info_table->setColumnWidth(3,80);
    ui->order_info_table->setColumnWidth(4,100);
}

Orders::~Orders()
{
    delete ui;
}

void Orders::set_current_order_info()
{
    Baggage bag;

    // 페이지 설정
    ui->current_order_page->setText(QString::number(current_order_page));
    last_order_page = this->arrangements->size();
    ui->last_order_page->setText(QString::number(last_order_page));

    // 현재 주문페이지의 주문번호 쓰기
    int order_num = (this->arrangements->begin()+(current_order_page-1))->GetArrangementNum();
    ui->order_num_label->setText(QString::number(order_num));

    // 테이블위젯에 주문한 메뉴목록을 쓰기
    int order_menu_cnt = (this->arrangements->begin()+(current_order_page-1))->GetBaggageTotalCount();
    bag = (this->arrangements->begin()+(current_order_page-1))->GetBaggage();

    ui->order_info_table->setRowCount(0);

    for(int i=0; i<order_menu_cnt; i++)
    {
        int number = i+1;
        QString menu_name = bag.GetBaggageIndividualName(i);
        int menu_price = bag.GetBaggageIndividualPrice(i);
        int menu_cnt = bag.GetItemIndividualCount(i);
        int tot_one_menu_price = menu_price*menu_cnt;
        qDebug() << "number :" << number;
        qDebug() << "menu_name :" << menu_name;
        qDebug() << "menu_price :" << menu_price;
        qDebug() << "menu_cnt :" << menu_cnt;
        qDebug() << "tot_menu_price :" << tot_one_menu_price;
        // 번호적기
        ui->order_info_table->insertRow(ui->order_info_table->rowCount());
        int index = ui->order_info_table->rowCount() - 1;
        // 메뉴명
        ui->order_info_table->setItem(index,0,new QTableWidgetItem(menu_name));
        // 가격
        ui->order_info_table->setItem(index,1,new QTableWidgetItem(QString::number(menu_price)));
        // 수량
        ui->order_info_table->setItem(index,2,new QTableWidgetItem(QString::number(menu_cnt)));
        // 주문금액
        ui->order_info_table->setItem(index,3,new QTableWidgetItem(QString::number(tot_one_menu_price)));
    }
    // 결제수단을 적는다.
    int payment = (this->arrangements->begin()+(current_order_page-1))->GetPayment();
    if(payment == 1)
    {
        ui->order_payment_method->setText("현금");
    }
    else if(payment == 2)
    {
        ui->order_payment_method->setText("카드");
    }
    // 총 주문금액을 적는다.
    int tot_order_price = (this->arrangements->begin()+(current_order_page-1))->GetBaggage().GetBaggageTotalPrice();
    ui->tot_order_price->setText(QString::number(tot_order_price)+" 원");
}

void Orders::recv_orders(QTcpSocket* socket,QVector<Arrangement>* arrangements)
{
    writing_sock = socket;
    this->arrangements = arrangements;
    set_current_order_info();
}

void Orders::on_order_proc_btn_clicked()
{
    QMessageBox msgbox;


    msgbox.setText("처리하시겠습니까?");
    msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int ret = msgbox.exec();
    int order_proc_result;
    switch (ret)
    {
        case QMessageBox::Ok:
        {
            // 주문처리 시그널을 보낸다.
            writing_sock->waitForBytesWritten(500);
            int signal = TYPE_ORDER_PROC;
            writing_sock->write((char*)&signal,sizeof(int));
            // 처리할 주문번호를 보낸다. -> 주문번호에 맞는 주문건의 status를 1로변경한다.
            writing_sock->waitForBytesWritten(500);
            int order_num = (this->arrangements->begin()+(current_order_page-1))->GetArrangementNum();
            writing_sock->write((char*)&order_num,sizeof(int));

            // 처리결과를 받는다.
            writing_sock->waitForReadyRead(500);
            writing_sock->read((char*)&order_proc_result,sizeof(int));
            if(order_proc_result==0)
            {
                QMessageBox::information(this,"알림","주문처리 성공!");

                // 대기중인 주문을 삭제한다.
                this->arrangements->erase((this->arrangements->begin()+(current_order_page-1)));
                if(this->arrangements->size()==0)
                {
                    QMessageBox::information(this,"알림","더이상 주문이 없습니다.");
                    close();
                }

                // 마지막 주문수 조정
                ui->last_order_page->setText(QString::number(this->arrangements->size()));
                // 현재 주문 조정
                if(current_order_page-1 == 0)
                {
                    current_order_page = 1;
                }
                else
                {
                    current_order_page--;
                }
                ui->current_order_page->setText(QString::number(current_order_page));
                // 조정된 주문정보 출력
                set_current_order_info();

            }
            else if(order_proc_result!=0) //실패
            {
                QMessageBox::information(this,"알림","주문처리 실패!");
            }
        }
        break;
    }
}

void Orders::on_order_cancel_btn_clicked()
{
    QMessageBox msgbox;

    msgbox.setText("취소하시겠습니까?");
    msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int ret = msgbox.exec();
    int order_cancel_result;
    switch (ret)
    {
        case QMessageBox::Ok:
        {
            // 주문취소 시그널을 보낸다.
            writing_sock->waitForBytesWritten(500);
            int signal = TYPE_ORDER_CANCEL;
            writing_sock->write((char*)&signal,sizeof(int));
            // 취소할 주문번호를 보낸다 -> 주문번호에 맞는 주문건의 status를 2로변경한다.
            writing_sock->waitForBytesWritten(500);
            int order_num = (this->arrangements->begin()+(current_order_page-1))->GetArrangementNum();
            writing_sock->write((char*)&order_num,sizeof(int));

            // 처리결과를 받는다.
            writing_sock->waitForReadyRead(500);
            writing_sock->read((char*)&order_cancel_result,sizeof(int));

            if(order_cancel_result==0)
            {
                QMessageBox::information(this,"알림","주문취소 성공!");
                // 대기중인 주문을 삭제한다.
                this->arrangements->erase((this->arrangements->begin()+(current_order_page-1)));

                if(this->arrangements->size()==0)
                {
                    QMessageBox::information(this,"알림","더이상 주문이 없습니다.");
                    QString waiting_order_info;
                    // 버튼의
                    close();
                }
                // 마지막 주문수 조정
                ui->last_order_page->setText(QString::number(this->arrangements->size()));
                // 현재 주문 조정
                if(current_order_page-1 == 0)
                {
                    current_order_page = 1;
                }
                else
                {
                    current_order_page--;
                }
                ui->current_order_page->setText(QString::number(current_order_page));
                // 조정된 주문정보 출력
                set_current_order_info();
            }
            else if(order_cancel_result !=0)
            {
                QMessageBox::information(this,"알림","주문취소 실패!");
            }
        }
        break;
    }
}

void Orders::on_pre_order_btn_clicked()
{
    // 현재페이지를 감소시킨다.
    if(current_order_page-1 != 0)
    {
        current_order_page--;
        set_current_order_info();
    }
    else
    {
        QMessageBox::information(this,"알림","첫 주문페이지입니다.");
    }
}

void Orders::on_next_order_btn_clicked()
{
    // 현재페이지를 증가시킨다.
    if(current_order_page+1 <= last_order_page)
    {
       current_order_page++;
       set_current_order_info();
    }
    else
    {
        QMessageBox::information(this,"알림","마지막 주문페이지입니다.");
    }
}

void Orders::on_exit_btn_clicked()
{
    this->close();
}
