#ifndef DAILYMONTHLYSALES_H
#define DAILYMONTHLYSALES_H

#include <QDialog>
#include <QCalendarWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDate>
#include <QThread>
#include "Common.h"
#include "salesinfo.h"
#include "item.h"

namespace Ui {
class DailyMonthlySales;
}

class DailyMonthlySales : public QDialog
{
    Q_OBJECT

public:
    explicit DailyMonthlySales(QTcpSocket *sock,QWidget *parent = nullptr);
    ~DailyMonthlySales();
    void PrintSales(int num);
    void onMonth();
    void onDaily();
    void onData(int sig);
    void onMainData();

private slots:


    void on_backBtn_clicked();


    void InitDate(const QDate date);



    void on_monBtn_2_clicked();

    void on_dayBtn_2_clicked();

    void on_calendarWidget_2_currentPageChanged(int year, int month);

    void on_calendarWidget_2_clicked(const QDate &date);

private:
    Ui::DailyMonthlySales *ui;
    QTcpSocket *writing_m_sock; //중계서버 연결 소켓
    QDate *date;
    SalesInfo *salesinfo;
    Item *item;
    int tot_price;
    int arrcnt;
};

#endif // DAILYMONTHLYSALES_H
