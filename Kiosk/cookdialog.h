#ifndef COOKDIALOG_H
#define COOKDIALOG_H

#include <QDialog>
#include "common.h"
#include "member.h"
#include "arrangement.h"
#include "paydialog.h"

namespace Ui {
class CookDialog;
}

class CookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CookDialog(QWidget *parent = nullptr);
    ~CookDialog();
    void setTcpsock(QTcpSocket*);
    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::CookDialog *ui;
    PayDialog *paydialog;
    QTcpSocket *guerrilla_sock;
    Member people;

    int Type;
    int TotalCount; // 총 메뉴 개수
    int NoodleCategoryCount; // 라면 메뉴 개수
    int BeverageCategoryCount; // 음료 메뉴 개수
    int SnackCategoryCount; // 과자 메뉴 개수
    int RiceCategoryCount; // 식사 메뉴 개수

    Item *item;
    Item *item_1;
    Item *item_2;
    int PREQUEL_COUNT;
    int SEQUEL_COUNT;

    QTabWidget *tab;
    QWidget *NoodleTab;
    QWidget *BeverageTab;
    QWidget *SnackTab;
    QWidget *RiceTab;

    QRadioButton *NoodleRadio[10];
    QLabel *NoodleLabel[10];
    QPixmap NoodlePix[10];

    QRadioButton *BeverageRadio[10];
    QLabel *BeverageLabel[10];
    QPixmap BeveragePix[10];

    QRadioButton *SnackRadio[10];
    QLabel *SnackLabel[10];
    QPixmap SnackPix[10];

    QRadioButton *RiceRadio[10];
    QLabel *RiceLabel[10];
    QPixmap RicePix[10];
    //
    QGroupBox *CountCheckBox;
    QSpinBox *Spin;
    QGridLayout *Grid;
    //
    QPushButton *AlphaBnt;
    QPushButton *OmegaBnt;
    QVBoxLayout *layout;
    //
    QGroupBox *BasketBox;
    //
    QPushButton *push[3];
    QLabel *lbl[4];
    //
    QLabel *fill;
    QLabel *full[5];
    QLabel *field[5];
    int excessive=0;
    int begin=0;
    int position=30;
    QPixmap pix;
    Item instant;
    int amount;
    int line=0;
    //
    Arrangement arr;
    Arrangement resurrection;
    //
    int decision=0;

private slots:
    void RecvClass(Member);
    void selection();
    void shopping();
    void revoke();
    void throwback();
    void forwarding();
    void RecvInt(int);
    void RecvRecurringClass(Arrangement);

signals:
    void SendClass(Arrangement);

};

#endif // COOKDIALOG_H
