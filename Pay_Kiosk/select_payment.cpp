#include "select_payment.h"
#include "ui_select_payment.h"

Select_Payment::Select_Payment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_Payment)
{
    ui->setupUi(this);    
    connect(this, SIGNAL(Send_Payment(int)), parent, SLOT(Recv_Payment(int)));
}

Select_Payment::~Select_Payment()
{
    delete ui;
}

void Select_Payment::on_cash_Btn_clicked()
{
    int payment = 1;
    emit Send_Payment(payment);
    this->close();
}

void Select_Payment::on_credit_Btn_clicked()
{
    int payment = 2;
    emit Send_Payment(payment);
    this->close();
}

void Select_Payment::on_exit_Btn_clicked()
{
    this->close();
}

