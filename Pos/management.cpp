#include "management.h"
#include "ui_management.h"

Management::Management(QTcpSocket *sock,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Management)
{
    ui->setupUi(this);
    writing_m_sock=sock;
    this->setWindowTitle("회원 관리");
    del.clear();
}

Management::~Management()
{
    delete ui;
    qDebug()<<"Management 소멸자";
}

void Management::on_searchBtn_clicked() // 회원 이름으로 조회
{
    if(ui->nameLineEdit->text()!="")
        onSearch();
    else
    {
        QMessageBox::information(this,"info","이름을 입력하세요.");
    }
}

void Management::on_nameLineEdit_returnPressed() //enter 키 입력 이벤트 함수
{
    if(ui->nameLineEdit->text()!="")
        onSearch();
    else
    {
        QMessageBox::information(this,"info","이름을 입력하세요.");
    }
}

void Management::onSearch() //조회 회원 해당 데이터 서버에서 송수신
{
    char * info= ui->nameLineEdit->text().toUtf8().data();
    int sig=ManagementSig::MEMBERS_MANAGEMENT;

    qDebug()<<info;
    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)&sig,sizeof(sig)); //signal 송신

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write(info,sizeof(char)*DefineMember::BUFFSIZE); //회원 이름 송신

    writing_m_sock->waitForReadyRead(500);
    writing_m_sock->read((char*)&member,sizeof(Member)); //해당 회원에 대한 정보 수신 및 member에 저장
    qDebug()<<member.GetMemberId();
    qDebug()<<member.GetMemberName();
    qDebug()<<member.GetMemberPassword();
    qDebug()<<member.GetMemberBirth();
    qDebug()<<member.GetMemberPhone();
    qDebug()<<member.GetMemberRemain();

    if(strcmp(info,member.GetMemberId())==0)
        PrintInfo();
    else
    {
        QMessageBox::information(this,"info","해당 ID가 존재하지 않습니다.");
    }
    ui->nameLineEdit->clear();
}

void Management::PrintInfo() //회원 정보 출력 함수
{
    qDebug() << "PrintInfo()";
    ui->infoTableWidget->insertRow(ui->infoTableWidget->rowCount()); // Row를 추가합니다.
    int index = ui->infoTableWidget->rowCount() - 1;
    qDebug() << index;
    ui->infoTableWidget->setItem(index,0,new QTableWidgetItem(member.GetMemberId())); //ID

    ui->infoTableWidget->setItem(index,1,new QTableWidgetItem(member.GetMemberPassword())); //PASSWORD

    ui->infoTableWidget->setItem(index,2,new QTableWidgetItem(member.GetMemberName())); //NAME

    ui->infoTableWidget->setItem(index,3,new QTableWidgetItem(member.GetMemberBirth())); //BIRTH

    ui->infoTableWidget->setItem(index,4,new QTableWidgetItem(member.GetMemberPhone())); //PHONE

    ui->infoTableWidget->setItem(index,5,new QTableWidgetItem(QString::number(member.GetMemberStatus()))); //STATUS

    ui->infoTableWidget->setItem(index,6,new QTableWidgetItem(member.GetMemberRemain())); //REMAIN
}

void Management::ModifyInfo() //회원 정보 수정 함수
{
    int sig=ManagementSig::MEMBERS_MODIFY;

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)&sig,sizeof(sig)); //signal 송신

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)&member,sizeof(Member)); //변경된 멤버 클래스 송신
}

void Management::DeletInfo() //회원 정보 삭제 함수
{
    int sig=ManagementSig::MEMBERS_DELETE;

    int index = ui->infoTableWidget->rowCount() - 1;
    qDebug() << index;

    ui->infoTableWidget->removeRow(index);

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)&sig,sizeof(sig)); //signal 송신

    writing_m_sock->waitForBytesWritten(500);
    writing_m_sock->write((const char*)del.toUtf8().data(),sizeof(char)*DefineMember::BUFFSIZE); //변경된 멤버 클래스 송신
    del.clear();
}

void Management::on_infoTableWidget_itemChanged(QTableWidgetItem *item) //column의 item 내용이 변경되면 실행
{
    QString temp=item->text();
    int c;

    c=item->column();

    if(c==1)
        member.SetMemberPassword(temp.toUtf8().data());
    else if(c==2)
        member.SetMemberName(temp.toUtf8().data());
    else if(c==3)
        member.SetMemberBirth(temp.toUtf8().data());
    else if(c==4)
        member.SetMemberPhone(temp.toUtf8().data());
    else if(c==5)
        member.SetMemberStatus(temp.toInt());
    else if(c==6)
        member.SetMemberRemain(temp.toUtf8().data());
}

void Management::on_deleteBtn_clicked() //삭제 버튼 클릭
{
    QMessageBox msg;

    if(del!="")
    {
        QMessageBox::StandardButton b= msg.warning(this,"삭제","삭제 하시겠습니까?",QMessageBox::Yes | QMessageBox::No);
        if(b==QMessageBox::Yes)
        {
            DeletInfo();
            msg.warning(this,"삭제","삭제 완료!");
        }
        else
            del.clear();
    }
    else
        QMessageBox::information(this,"info","삭제할 회원을 선택하십시오.");
}

void Management::on_modifyBtn_clicked() //수정 버튼 클릭
{
    QMessageBox msg;
    ModifyInfo();
    msg.information(this,"수정","수정 완료!");
}

void Management::on_pushButton_clicked()
{
    this->close();
}

void Management::on_infoTableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    QMessageBox msg;
    int c;

    c=item->column();
    if(c==0)
    {
        msg.critical(this,"경고","id는 바꿀수 없습니다.");
    }
}

void Management::on_infoTableWidget_itemClicked(QTableWidgetItem *item)
{
   del=item->text();
}
