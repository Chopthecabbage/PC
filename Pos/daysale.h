#ifndef DAYSALE_H
#define DAYSALE_H

#include <QDialog>
#include <QTcpSocket>
#include <QTableWidget>
#include <QMessageBox>
#include <QDate>
#include <QThread>
#include "Common.h"
#include "SalesInfo.h"
#include "item.h"

namespace Ui {
class Daysale;
}

class Daysale : public QDialog
{
    Q_OBJECT

public:
    explicit Daysale(QTcpSocket *sock,QWidget *parent = nullptr);
    ~Daysale();
    void PrintDaySales(int num);
    int onData(int arrcnt);
    void onDaySearch();

private slots:
    void on_backBtn_clicked();

private:
    Ui::Daysale *ui;
    QTcpSocket *writing_m_sock; //중계서버 연결 소켓
    SalesInfo *salesinfo;
    Item *item;
    int tot_price;
    int arrcnt;
};

#endif // DAYSALE_H
