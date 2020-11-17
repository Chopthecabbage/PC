#ifndef FINDACCOUNTDIALOG_H
#define FINDACCOUNTDIALOG_H

#include <QDialog>

#include "common.h"
#include "member.h"
#include "updatepwdialog.h"

namespace Ui {
class FindAccountDialog;
}

class FindAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindAccountDialog(QWidget *parent = nullptr);
    ~FindAccountDialog();
    void setTcpsock(QTcpSocket *);
    virtual bool eventFilter(QObject *object, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::FindAccountDialog *ui;
    QTcpSocket *socket;
    UpdatePwDialog *updatepwdialog;
    //
    QRegExpValidator *vali[3];
    int Type;
    int Doppelganger=0;
    int SeatInfo;
    //
    void _handleWhatsThisEntry(QWidget * /*sender*/);

private slots:
    void on_IdBtn_clicked();
    void on_PwBtn_clicked();

    void on_pushButton_clicked();

signals:
    void SendIdAndPhone(char *, char *);

};

#endif // FINDACCOUNTDIALOG_H
