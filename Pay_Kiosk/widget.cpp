#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    sock.connectToHost(QHostAddress("61.108.2.70"), SERVER_PORT);
    mem_check = CHECK_NON;
    type = TYPE_NON;
    price = 0;
    payment = 0;
    time = "00:00";

    ui->id_label->clear();
    ui->id_result_label->clear();
    ui->remain_label->clear();
    ui->time_label->clear();
    ui->price_label->clear();
    ui->paymentBtn->hide();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Recv_Payment(int _payment)
{
    payment = _payment;
}

// 아이디 검색 버튼
void Widget::on_id_searchBtn_clicked()
{
    char search_id[MEM_BUF];
    int signal;
    int result = -1;

    strcpy(search_id, ui->id_line_edit->text().toUtf8());
    signal = SIG_CHECK_ID;
    sock.write((char*)&signal, sizeof(int));
    sock.write(search_id, sizeof(search_id)); // 검색할 아이디 전송
    sock.waitForReadyRead();
    sock.read((char*)&result, sizeof(int)); // 결과 리턴받음
    qDebug() << "result : " << result;

    if(result == 0) // 일치하는 아이디 없음
    {
        mem_check =CHECK_NON;
        ui->id_result_label->setText("일치하는 아이디가 없습니다");
    }
    else if(result == 1) // 일치하는 아이디 있음
    {
        ui->id_line_edit->clear();
        ui->id_result_label->clear();
        mem_check = CHECK_OK;
        signal = SIG_GET_REMAIN;        
        sock.write((char*)&signal, sizeof(int));
        sock.write(search_id, sizeof(search_id)); // 검색할 아이디 전송

        sock.waitForReadyRead(500);
        sock.read((char*)&member, sizeof(Member));
        qDebug() <<"id " << member.Get_Mem_Id();
        qDebug() <<"remain "<< member.Get_Remain();

        QString temp(member.Get_Remain());
        ui->remain_label->setText(temp.chopped(3)); // 남은시간 출력
        ui->id_label->setText(member.Get_Mem_Id()); // 회원 아이디 출력

    }
    Change_Label();
}
// 버튼 클릭시 UI에 Label 변경
void Widget::Change_Label()
{
    QString price_to_str = QString("%L2").arg(price);
    price_to_str += "원";

    ui->time_label->setText(time); // 충전 시간
    ui->price_label->setText(price_to_str); // 충전 요금

    if(type == TYPE_TEMP || mem_check == CHECK_NON) // 검색한 회원이 없거나, 비회원인 경우
    {       
        ui->id_label->clear();
        ui->remain_label->clear();
    }
    if(type == TYPE_NON)
    {
        ui->time_label->clear(); // 충전 시간
        ui->price_label->clear(); // 충전 요금
    }

    // 결제버튼 나타내기, 숨기기
    if((type==TYPE_MEM && mem_check==CHECK_OK) || (type==TYPE_TEMP))
        ui->paymentBtn->show();
    else
        ui->paymentBtn->hide();
}

void Widget::on_tabWidget_currentChanged(int index)
{
    ui->id_label->clear();
    ui->remain_label->clear();
    ui->id_result_label->clear();
    ui->time_label->clear();
    ui->price_label->clear();
    type = TYPE_NON;
}

void Widget::on_paymentBtn_clicked()
{
    Select_Payment *sel_pay;
    int temp_mem_id;
    sel_pay = new Select_Payment(this); // 현금, 카드 선택
    sel_pay->exec();

    if(payment != 0)
    {
        pay_info.Set_Price(price); // 결제 금액
        pay_info.Set_Charge_Time((char*)time.toStdString().c_str()); // 충전시간
        pay_info.Set_Payment(payment); // 결제 수단
        if(type == TYPE_MEM) // 회원
        {
            pay_info.Set_Mem_Id(member.Get_Mem_Id());
            pay_info.Set_Mem_Status(member.Get_Mem_Status());
            pay_info.Set_Mem_Type(TYPE_MEM);
            if(member.Get_Mem_Status() == 0) // 비로그인 회원 잔여시간 계산
            {
                QString temp_remain(member.Get_Remain());  // 잔여시간
                QString str = Time_Add(temp_remain, time); // 잔여시간 + 충전시간
                pay_info.Set_Mem_Remain((char*)str.toStdString().c_str()); // 잔여시간 셋팅
                qDebug() << "비로그인 회원 remain : " << pay_info.Get_Mem_Remain();
            }
        }
        if(type == TYPE_TEMP) // 비회원
        {
            char result[MEM_BUF];
            strcpy(result, time.toStdString().c_str());
            pay_info.Set_Mem_Remain(result);
            pay_info.Set_Mem_Type(TYPE_TEMP);
            qDebug() << "비회원 remain : " << pay_info.Get_Mem_Remain();
        }
        /*
        qDebug() << "pay_info id " << pay_info.Get_Mem_Id();
        qDebug() << "pay_info remain " << pay_info.Get_Mem_Remain();
        qDebug() << "pay_info price " << pay_info.Get_Price();
        qDebug() << "pay_info payment " << pay_info.Get_Payment();
        qDebug() << "pay_info status " << pay_info.Get_Mem_Status();
        */
        int signal = SIG_CHARGE_TIME;
        sock.write((char*)&signal, sizeof(int));
        sock.write((char*)&pay_info, sizeof(pay_info));
        sock.waitForBytesWritten();

        if(pay_info.Get_Mem_Type() == TYPE_TEMP)
        {
            QString msg = "   결제 완료\n비회원 ID : ";
            sock.waitForReadyRead();
            sock.read((char*)&temp_mem_id, sizeof(int));
            qDebug() << "temp_id " << temp_mem_id;
            msg += QString::number(temp_mem_id);
            msg += "    ";
            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.setStyleSheet("QLabel{font:bold 14px;} QPushButton {font: bold 12px;}");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.setButtonText(QMessageBox::Yes, "확인");
            msgBox.exec();
        }
        else
        {
             QString msg = "   결제 완료\n회원 ID : ";
             msg += member.Get_Mem_Id();
             msg += "    ";
             QMessageBox msgBox;
             msgBox.setText(msg);
             msgBox.setStyleSheet("QLabel{font:bold 14px;} QPushButton {font: bold 12px;}");
             msgBox.setStandardButtons(QMessageBox::Yes);
             msgBox.setButtonText(QMessageBox::Yes, "확인");
             msgBox.exec();
        }

        memset(&pay_info, 0, sizeof(pay_info));
        type = TYPE_NON;
        mem_check = CHECK_NON;
        Change_Label();
    }   
}

QString Widget::Time_Add(QString time1, QString time2)
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

// 회원 요금 충전 버튼
void Widget::on_memBtn_1_clicked()
{
    type = TYPE_MEM;
    price = 1000;
    time = "01:20";
    Change_Label();
}
void Widget::on_memBtn_2_clicked()
{
    type = TYPE_MEM;
    price = 2000;
    time = "02:40";
    Change_Label();
}
void Widget::on_memBtn_3_clicked()
{
    type = TYPE_MEM;
    price = 3000;
    time = "04:00";
    Change_Label();
}
void Widget::on_memBtn_4_clicked()
{
    type = TYPE_MEM;
    price = 5000;
    time = "07:00";
    Change_Label();
}
void Widget::on_memBtn_5_clicked()
{
    type = TYPE_MEM;
    price = 10000;
    time = "15:00";
    Change_Label();
}
void Widget::on_memBtn_6_clicked()
{
    type = TYPE_MEM;
    price = 30000;
    time = "50:00";
    Change_Label();
}
// 비회원 요금 충전 버튼
void Widget::on_tempBtn_1_clicked()
{
    type = TYPE_TEMP;
    price = 1000;
    time = "01:00";
    mem_check = CHECK_NON;
    Change_Label();
}
void Widget::on_tempBtn_2_clicked()
{
    type = TYPE_TEMP;
    price = 2000;
    time = "02:00";
    mem_check = CHECK_NON;
    Change_Label();
}
void Widget::on_tempBtn_3_clicked()
{
    type = TYPE_TEMP;
    price = 3000;
    time = "03:00";
    mem_check = CHECK_NON;
    Change_Label();
}
void Widget::on_tempBtn_4_clicked()
{
    type = TYPE_TEMP;
    price = 5000;
    time = "06:00";
    mem_check = CHECK_NON;
    Change_Label();
}
void Widget::on_tempBtn_5_clicked()
{
    type = TYPE_TEMP;
    price = 10000;
    time = "13:00";
    mem_check = CHECK_NON;
    Change_Label();
}
void Widget::on_tempBtn_6_clicked()
{
    type = TYPE_TEMP;
    price = 30000;
    time = "45:00";
    mem_check = CHECK_NON;
    Change_Label();
}

