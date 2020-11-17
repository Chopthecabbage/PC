#include "findaccountdialog.h"
#include "ui_findaccountdialog.h"

FindAccountDialog::FindAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindAccountDialog)
{
    ui->setupUi(this);
    setWindowTitle("Easy come, easy go.");
    //
    qApp->installEventFilter(this);
    //
    QRegExp rx_1st(NM_RX);
    QRegExp rx_2nd(ID_RX);
    QRegExp rx_3rd(HP_RX);

    vali[0] = new QRegExpValidator(rx_1st, this);
    vali[1] = new QRegExpValidator(rx_2nd, this);
    vali[2] = new QRegExpValidator(rx_3rd, this);

    ui->NameEdit->setValidator(vali[0]);
    ui->IdEdit->setValidator(vali[1]);
    ui->HpEdit->setValidator(vali[2]);

    connect(ui->NameEdit,SIGNAL(returnPressed()),
            this, SLOT(on_IdBtn_clicked()));
    connect(ui->HpEdit,SIGNAL(returnPressed()),
            this, SLOT(on_PwBtn_clicked()));
}

void FindAccountDialog::setTcpsock(QTcpSocket *socket)
{
    this->socket=socket;
}

void FindAccountDialog::_handleWhatsThisEntry(QWidget * /*sender*/) {
    qDebug() << "QEvent::EnterWhatsThisMode";
    QMessageBox::information( this,
                   "도움말",
                   "Talk of an angel and he will appear.",QMessageBox::Ok );
}

bool FindAccountDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}

void FindAccountDialog::closeEvent (QCloseEvent *event)
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

FindAccountDialog::~FindAccountDialog()
{
    delete ui;
}

void FindAccountDialog::on_IdBtn_clicked()
{
    QMessageBox box;
    box.setStandardButtons(QMessageBox::Ok);

    if(ui->NameEdit->text().length()<MIN_NAME)
    {
        box.setWindowTitle("메시지");
        box.setText("이름 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }
    // 시그널...
    Type=DefineFind::FIND;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    // 아이디 찾기
    Type=DefineFind::NAME;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    //
    char tempNAME[DefineMember::BUFFSIZE];
    QString str=ui->NameEdit->text();
    qDebug() << str;
    strcpy(tempNAME, str.toStdString().c_str());

    socket->waitForBytesWritten();
    socket->write((char*)tempNAME, sizeof(char)*(DefineMember::BUFFSIZE));
    qDebug() <<"tempNAME: "<<tempNAME;
    qDebug() <<"socket->write((char*)tempNAME, sizeof(char)*(DefineMember::BUFFSIZE));";

    Doppelganger=NOTHING;
    socket->waitForReadyRead();
    socket->read((char*)&Doppelganger, sizeof(int));

    char tempID[DefineMember::BUFFSIZE];
    socket->waitForReadyRead();
    socket->read((char*)&tempID, sizeof(char)*(DefineMember::BUFFSIZE));

    qDebug() << "socket->read((char*)&tempID, sizeof(char)*(DefineMember::BUFFSIZE));";
    qDebug() << tempID;
    QString info="아이디 : ";
    QString temp(tempID);
    info+=temp;
    qDebug() << info;

    if(Doppelganger==LOG_IN::WRONG)
    {
        QMessageBox::information(this,"아이디 찾기","입력하신 정보와 일치하는 아이디가 없습니다.",QMessageBox::Ok);
    }
    else if(Doppelganger == -1)
    {
        QMessageBox::information(this,"아이디 찾기",info,QMessageBox::Ok);
    }
}

void FindAccountDialog::on_PwBtn_clicked()
{
    QMessageBox box;
    box.setStandardButtons(QMessageBox::Ok);

    if(ui->IdEdit->text().length()<MIN_ID)
    {
        box.setWindowTitle("메시지");
        box.setText("아이디 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }
    else if(ui->HpEdit->text().length()<MIN_PHONE)
    {
        box.setWindowTitle("메시지");
        box.setText("핸드폰번호 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }
    Type=DefineFind::FIND;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    // 비밀번호 찾기
    Type=DefineFind::PW;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    //
    char tempID[DefineMember::BUFFSIZE];
    QString str=ui->IdEdit->text();
    qDebug() << str;
    strcpy(tempID, str.toStdString().c_str());
    // 아이디 전송
    socket->waitForBytesWritten();
    socket->write((char*)tempID, sizeof(char)*(DefineMember::BUFFSIZE));

    char tempPHONE[DefineMember::BUFFSIZE];
    str=ui->HpEdit->text();
    qDebug() << str;
    strcpy(tempPHONE, str.toStdString().c_str());
    // 핸드폰번호 전송
    socket->waitForBytesWritten();
    socket->write((char*)tempPHONE, sizeof(char)*(DefineMember::BUFFSIZE));
    //
    Doppelganger=0;
    socket->waitForReadyRead();
    socket->read((char*)&Doppelganger, sizeof(int));
    //
    if(Doppelganger==LOG_IN::WRONG)
    {
        QMessageBox::information(this,"비밀번호 찾기","입력하신 정보와 일치하는 비밀번호가 없습니다.",QMessageBox::Ok);
    }
    else if(Doppelganger == -1) // 비밀번호 변경
    {
        updatepwdialog = new UpdatePwDialog;
        updatepwdialog->setFixedSize(400,200);
        //
        connect(this,SIGNAL(SendIdAndPhone(char *, char *)),updatepwdialog,SLOT(RecvIdAndPhone(char *, char *)));
        emit SendIdAndPhone(tempID,tempPHONE);
        //
        updatepwdialog->setTcpsock(socket);
        this->hide();
        updatepwdialog->exec();
        //
        this->close();
    }
}

void FindAccountDialog::on_pushButton_clicked()
{
    this->close();
}
