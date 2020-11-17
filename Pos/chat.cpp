#include "chat.h"

Chat::Chat()
{
    strcpy(writer,"");
    strcpy(message,"");
}


void Chat::SetWriter(char *writer)
{
    strcpy(this->writer,writer);
}

void Chat::SetMsg(char *msg)
{
    strcpy(message,msg);
}

char* Chat::GetWriter()
{
    return writer;
}
char* Chat::GetMsg()
{
    return message;
}
