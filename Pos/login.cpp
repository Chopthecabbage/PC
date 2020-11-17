#include "login.h"
#include "ui_login.h"


#include <QHostAddress>
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    _socket = new QTcpSocket();
    // 중계서버에 연결시도
    _socket->connectToHost(QHostAddress("61.108.2.70"),MIDDLE_PORT);
    // 관리자 타입번호 전달
    int type = 2;
    _socket->waitForBytesWritten(1500);
    _socket->write((char*)&type,sizeof(int));
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_btn_clicked()
{
    QString login_id;
    char c_login_id[20] = "";
    int login_result = 0;

    // 입력한 아이디를 서버로 전달한다.

    login_id = ui->manager_id_line->text();

    strcpy(c_login_id,login_id.toStdString().c_str());

    if(login_id!="")
    {
        _socket->waitForBytesWritten(1500);
        _socket->write(c_login_id,sizeof(char[20]));
        ui->manager_id_line->setText("");

        // 아이디 일치여부 결과를 전달받는다.
        _socket->waitForReadyRead(1500);
        _socket->read((char*)(&login_result),sizeof(int));
        if(login_result > 0)
        {
            qDebug() << "로그인 성공!" << login_result;
            manager_dialog = new Manager(this);
            connect(this,SIGNAL(send_values(QTcpSocket*,int)),manager_dialog,SLOT(recv_values(QTcpSocket*,int)));
            emit send_values(_socket,login_result);
            manager_dialog->exec();
        }
        else if(login_result <= 0 )
        {
            QMessageBox::information(this,"Warning","아이디 불일치!");
        }
    }

    else
    {
        QMessageBox::information(this,"Warning","빈칸입니다.");
    }
}
