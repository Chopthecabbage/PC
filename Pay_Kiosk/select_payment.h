#ifndef SELECT_PAYMENT_H
#define SELECT_PAYMENT_H

#include <QDialog>

namespace Ui {
class Select_Payment;
}

class Select_Payment : public QDialog
{
    Q_OBJECT

private:
    Ui::Select_Payment *ui;
    QWidget *temp;

public:
    explicit Select_Payment(QWidget *parent = nullptr);
    ~Select_Payment();

signals:
    void Send_Payment(int);
private slots:
    void on_cash_Btn_clicked();
    void on_credit_Btn_clicked();
    void on_exit_Btn_clicked();


};

#endif // SELECT_PAYMENT_H
