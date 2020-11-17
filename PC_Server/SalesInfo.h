#pragma once
#include "Common.h"

class SalesInfo
{
private:
    int ArrangementNum; //주문 번호
    char arr_date[DefineItem::BUFFSIZE]; //주문 날짜,시간
    int payment; //결제 수단
    int arr_status; //주문처리 상태
    int code; //상품 코드
    int amount; //판매 수량

public:
    SalesInfo();
    SalesInfo(int id,char *date,int pay,int status,int c,int a);
    void InitSalesInfo();

    //setting
    void Setting(int id,char *date,int pay,int status,int c,int a);

    //Get
    int GetArrId() const;
    const char* GetArrDate() const;
    int GetPayment() const;
    int GetArrStatus() const;
    int GetCode() const;
    int GetAmount() const;

    //Set
    void SetArrId(const int id);
    void SetArrDate(const char* date);
    void SetPayment(const int pay);
    void SetArrStatus(const int status);
    void SetCode(const int c);
    void SetAmount(const int a);

};
