#include "string.h"
#pragma once
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