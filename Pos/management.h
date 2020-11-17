#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QTableWidget>
#include <QMessageBox>
#include "member.h"
#include "Common.h"

namespace Ui {
class Management;
}

class Management : public QDialog
{
    Q_OBJECT

public:
    explicit Management(QTcpSocket *sock,QWidget *parent = nullptr);
    ~Management();
    void onSearch(); //조회 회원 해당 데이터 서버에서 송수신
    void PrintInfo(); //회원 정보 출력 함수
    void DeletInfo(); //회원 정보 삭제 함수
    void ModifyInfo(); //회원 정보 수정 함수

private slots:
    void on_searchBtn_clicked();

    void on_nameLineEdit_returnPressed();

    void on_infoTableWidget_itemChanged(QTableWidgetItem *item);

    void on_deleteBtn_clicked();

    void on_modifyBtn_clicked();

    void on_pushButton_clicked();

    void on_infoTableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_infoTableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::Management *ui;
    QTcpSocket *writing_m_sock; //중계서버 연결 소켓
    Member member;
    QTableWidgetItem *item;
    QString del;
};

#endif // MANAGEMENT_H
