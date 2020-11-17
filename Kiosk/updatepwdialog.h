#ifndef UPDATEPWDIALOG_H
#define UPDATEPWDIALOG_H

#include <QDialog>
#include "common.h"
#include "member.h"

namespace Ui {
class UpdatePwDialog;
}

class UpdatePwDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatePwDialog(QWidget *parent = nullptr);
    ~UpdatePwDialog();
    void setTcpsock(QTcpSocket *);
    virtual bool eventFilter(QObject *object, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::UpdatePwDialog *ui;
    QTcpSocket *socket;
    //
    QRegExpValidator *vali;
    //
    int Type;
    int Doppelganger=0;
    //
    char tempID[DefineMember::BUFFSIZE];
    char tempPHONE[DefineMember::BUFFSIZE];
    char tempPW[DefineMember::BUFFSIZE];
    //
    void _handleWhatsThisEntry(QWidget * /*sender*/);

private slots:
    void on_UpdateBtn_clicked();
    void on_RecurrenceBtn_clicked();
    void RecvIdAndPhone(char *, char *);

signals:

};

#endif // UPDATEPWDIALOG_H
