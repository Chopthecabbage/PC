#include "salesinfo.h"

SalesInfo::SalesInfo()
    : ArrangementNum(0),payment(0),arr_status(0),code(0),amount(0)
{
    memset(arr_date,0,sizeof(arr_date));

}
SalesInfo::SalesInfo(int id,char *date,int pay,int status,int c,int a)
    : ArrangementNum(id),payment(pay),arr_status(status),code(c),amount(a)
{
    strcpy(arr_date,date);
}
void SalesInfo::InitSalesInfo()
{
    memset(arr_date,0,sizeof(arr_date));
    ArrangementNum=0;
    payment=0;
    arr_status=0;
    code=0;
    amount=0;

}

//setting
void SalesInfo::Setting(int id,char *date,int pay,int status,int c,int a)
{
    strcpy(arr_date,date);
    ArrangementNum=id;
    payment=pay;
    arr_status=status;
    code=c;
    amount=a;
}

//Get
int SalesInfo::GetArrId() const
{
    return ArrangementNum;
}
const char* SalesInfo::GetArrDate() const
{
    return arr_date;
}
int SalesInfo::GetPayment() const
{
    return payment;
}
int SalesInfo::GetArrStatus() const
{
    return arr_status;
}
int SalesInfo::GetCode() const
{
    return code;
}
int SalesInfo::GetAmount() const
{
    return amount;
}

//Set
void SalesInfo::SetArrId(const int id)
{
    ArrangementNum=id;
}
void SalesInfo::SetArrDate(const char* date)
{
    strcpy(arr_date,date);
}
void SalesInfo::SetPayment(const int pay)
{
    payment=pay;
}
void SalesInfo::SetArrStatus(const int status)
{
    arr_status=status;
}
void SalesInfo::SetCode(const int c)
{
    code=c;
}
void SalesInfo::SetAmount(const int a)
{
    amount=a;
}

