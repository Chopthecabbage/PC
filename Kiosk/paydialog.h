#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QDialog>
#include "arrangement.h"

namespace Ui {
class PayDialog;
}

class PayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PayDialog(QWidget *parent = nullptr);
    ~PayDialog();
    virtual bool eventFilter(QObject *object, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::PayDialog *ui;
    Arrangement arr;
    int decision;

private slots:
    void RecvClass(Arrangement);
    void on_GoBtn_clicked();
    void on_BackBtn_clicked();
    //
    void _handleWhatsThisEntry(QWidget * /*sender*/);

signals:
    void SendRecurringClass(Arrangement);
    void SendInt(int);

};

#endif // PAYDIALOG_H
