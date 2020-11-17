#include "updatepwdialog.h"
#include "ui_updatepwdialog.h"

UpdatePwDialog::UpdatePwDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdatePwDialog)
{
    ui->setupUi(this);
    setWindowTitle("Strike while the iron is hot.");
    qApp->installEventFilter(this);
    //
    QRegExp rx_1st(PW_RX);
    vali = new QRegExpValidator(rx_1st, this);
    ui->pwEdit->setValidator(vali);
    ui->passwordEdit->setValidator(vali);

    ui->pwEdit->setMaxLength(MAX_PASSWORD);
    ui->pwEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setMaxLength(MAX_PASSWORD);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);

    connect(ui->passwordEdit,SIGNAL(returnPressed()),
            this, SLOT(on_UpdateBtn_clicked()));
}

void UpdatePwDialog::RecvIdAndPhone(char *tempID,char *tempPHONE)
{
    strcpy(this->tempID,tempID);
    strcpy(this->tempPHONE,tempPHONE);
}

void UpdatePwDialog::setTcpsock(QTcpSocket *socket)
{
    this->socket=socket;
}

UpdatePwDialog::~UpdatePwDialog()
{
    delete ui;
}

void UpdatePwDialog::on_UpdateBtn_clicked()
{
    QMessageBox box;
    box.setStandardButtons(QMessageBox::Ok);

    if(ui->pwEdit->text().length()<MIN_PASSWORD)
    {
        box.setWindowTitle("메시지");
        box.setText("비밀번호 형식이 올바르지 않습니다.");
        box.exec();
        return;
    }
    else if(ui->pwEdit->text() != ui->passwordEdit->text())
    {
        box.setWindowTitle("메시지");
        box.setText("비밀번호가 일치하지 않습니다.");
        box.exec();
        return;
    }

    Type=DefineUpdate::UPDATE;
    socket->waitForBytesWritten();
    socket->write((char*)&Type, sizeof(int));
    // 아이디 전송
    socket->waitForBytesWritten();
    socket->write((char*)tempID, sizeof(char)*(DefineMember::BUFFSIZE));
    // 핸드폰번호 전송
    socket->waitForBytesWritten();
    socket->write((char*)tempPHONE, sizeof(char)*(DefineMember::BUFFSIZE));
    // 비밀번호 전송
    QString str=ui->pwEdit->text();
    qDebug() << str;
    strcpy(tempPW, str.toStdString().c_str());
    socket->waitForBytesWritten();
    socket->write((char*)tempPW, sizeof(char)*(DefineMember::BUFFSIZE));

    this->close();
}

void UpdatePwDialog::on_RecurrenceBtn_clicked()
{
    this->close();
}

void UpdatePwDialog::closeEvent (QCloseEvent *event)
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

void UpdatePwDialog::_handleWhatsThisEntry(QWidget * /*sender*/) {
    qDebug() << "QEvent::EnterWhatsThisMode";
    QMessageBox::information( this,
                   "도움말",
                   "Talk of an angel and he will appear." );
}

bool UpdatePwDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    return QObject::eventFilter(object, event);
}
