#ifndef ARRANGEMENT_H
#define ARRANGEMENT_H

#include "baggage.h"

class Arrangement
{
protected:
    int ArrangementNum; // 주문번호
    int payment; // 1: 현금 2: 카드
    Baggage baggage;
public:
    Arrangement();
    Arrangement(int payment, Baggage baggage);
    void SetArrangement(int payment, Baggage baggage);

    Baggage& GetBaggage();
    Arrangement& GetArrangement();

    int GetArrangementNum() const;
    int GetPayment() const;
    void SetArrangementNum(int ArrangementNum);
    void SetPayment(int payment);

    void AddItem(Item item, int ItemIndividualCount);

    int GetBaggageTotalCount() const;
    int GetBaggageTotalPrice() const;
    int GetItemIndividualCount(int Point) const;

    QString QStringBaggageIndividualName(int Point);
    QString QStringBaggageIndividualPrice(int Point);
    QString QStringItemIndividualCount(int Point);

    char* GetBaggageIndividualName(int Point);
    int GetBaggageIndividualPrice(int Point) const;
    char* GetBaggageIndividualPhoto(int Point);
    // 디버그
    void DebugShowItem(int Point) const;
};

#endif // ARRANGEMENT_H
