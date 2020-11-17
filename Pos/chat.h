#ifndef CHAT_H
#define CHAT_H

#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>

class Chat
{
private:
    char writer[21];
    char message[200];
public :
    Chat();
    char* GetWriter();
    char* GetMsg();
    void SetWriter(char *writer);
    void SetMsg(char *msg);
};

#endif // CHAT_H
