#ifndef ORDERS_H
#define ORDERS_H

#include "arrangement.h"
#include "baggage.h"
#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class Orders;
}

class Orders : public QDialog
{
    Q_OBJECT

public:
    explicit Orders(QWidget *parent = nullptr);
    ~Orders();
    void set_current_order_info();
public slots:
    void recv_orders(QTcpSocket*,QVector<Arrangement>*);
private slots:
    void on_order_proc_btn_clicked();
    void on_order_cancel_btn_clicked();
    void on_pre_order_btn_clicked();
    void on_next_order_btn_clicked();
    void on_exit_btn_clicked();
private:
    Ui::Orders *ui;
    QVector<Arrangement> *arrangements;
    QTcpSocket *writing_sock;
    int order_num;
    int current_order_page;
    int last_order_page;
};

#endif // ORDERS_H
