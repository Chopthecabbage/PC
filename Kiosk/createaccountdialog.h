#ifndef CREATEACCOUNTDIALOG_H
#define CREATEACCOUNTDIALOG_H

#include <QDialog>

#include "common.h"
#include "member.h"

namespace Ui {
class CreateAccountDialog;
}

class CreateAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAccountDialog(QWidget *parent = nullptr);
    ~CreateAccountDialog();
    void setTcpsock(QTcpSocket *);
    virtual bool eventFilter(QObject *object, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::CreateAccountDialog *ui;
    QTcpSocket *socket;
    //
    QLabel *lbl[4];
    QLineEdit *edit[4];
    //
    QLabel *rule[2];
    //
    QComboBox *combo[3];
    //
    QComboBox *phone;
    //
    QLineEdit *hp[2];
    //
    QPushButton *btn[3];
    //
    QRegExpValidator *vali[6];
    //
    QRadioButton *rbtn[2];
    //
    Member people;
    //
    int Type;
    int Doppelganger=0;

    void _handleWhatsThisEntry(QWidget * /*sender*/);

private slots:
    void overlap();
    void complete();
    void regression();

signals:

};

#endif // CREATEACCOUNTDIALOG_H
