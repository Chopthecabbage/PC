#ifndef COMMON_H
#define COMMON_H

#include <QTcpSocket>
#include <QHostAddress>
#include <cstring>
#include <QString>
#include <QStringList>
#include <QtDebug>
#include <QTime>
#include <QMessageBox>

#define MEM_BUF 50
#define SERVER_PORT 9083
#define SERVER_IP "61.108.2.70"
enum { TYPE_NON = 0, TYPE_MEM, TYPE_TEMP }; // 회원, 비회원
enum { CHECK_NON = 0, CHECK_OK }; // 있는회원, 없는회원
enum { SIG_CHECK_ID = 1, SIG_GET_REMAIN, SIG_CHARGE_TIME };

#endif // COMMON_H
