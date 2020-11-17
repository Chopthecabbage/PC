#include "paydialog.h"
#include "ui_paydialog.h"

PayDialog::PayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayDialog)
{
    ui->setupUi(this);
    setWindowTitle("Truth is stronger than fiction.");
    qApp->installEventFilter(this);
}

void PayDialog::RecvClass(Arrangement arr)
{
    this->arr=arr;
    qDebug() << "복사된 값";
    for(int i=0; i<this->arr.GetBaggageTotalCount(); i++)
    {
        arr.DebugShowItem(i);
    }
}

PayDialog::~PayDialog()
{
    delete ui;
}

void PayDialog::on_GoBtn_clicked()
{
    if(ui->CashBtn->isChecked()) // 현금
    {
        arr.SetPayment(DefinePay::CASH);
        connect(this,SIGNAL(SendRecurringClass(Arrangement)),this->parent(),SLOT(RecvRecurringClass(Arrangement)));
        emit SendRecurringClass(arr);
        qDebug() <<"현금 전송";
        this->close();
    }
    else if(ui->CardBtn->isChecked())
    {
        arr.SetPayment(DefinePay::CARD);
        connect(this,SIGNAL(SendRecurringClass(Arrangement)),this->parent(),SLOT(RecvRecurringClass(Arrangement)));
        emit SendRecurringClass(arr);
        qDebug() <<"카드 전송";
        this->close();
    }
    else
    {
        qDebug() << "둘다 아닌 경우";
        return;
    }
}

void PayDialog::on_BackBtn_clicked()
{
    decision=1;
    connect(this,SIGNAL(SendInt(int)),this->parent(),SLOT(RecvInt(int)));
    emit SendInt(decision);
    this->close();
}

void PayDialog::closeEvent (QCloseEvent *event)
{
    event->accept();
    /*
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Walk on eggshells.",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
    */
}

void PayDialog::_handleWhatsThisEntry(QWidget * /*sender*/) {
    qDebug() << "QEvent::EnterWhatsThisMode";
    QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=MTFLI6bVnCM"));
}

bool PayDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}
