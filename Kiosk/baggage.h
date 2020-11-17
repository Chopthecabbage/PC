#ifndef BAGGAGE_H
#define BAGGAGE_H

#include "common.h"
#include "item.h"

class Baggage
{
private:
    Item item[5];
    int ItemIndividualCount[5];
    int BaggageTotalCount;
    int BaggageTotalPrice;
public:
    Baggage();

    Item& GetItemIndividual(int Point); // 장바구니 개별 아이템의 정보
    Baggage& GetBaggage();

    void AddItem(Item item, int ItemIndividualCount); // 장바구니 추가
    int GetBaggageTotalCount() const; // 장바구니 총 개수
    int GetBaggageTotalPrice() const; // 장바구니 총 가격
    int GetItemIndividualCount(int Point) const; // 장바구니 개별 아이템 개수
    void SetItemIndividualCount(int ItemIndividualCount);

    QString QStringBaggageIndividualName(int Point);
    QString QStringBaggageIndividualPrice(int Point);
    QString QStringItemIndividualCount(int Point);

    char* GetBaggageIndividualName(int Point);
    int GetBaggageIndividualPrice(int Point) const;
    char* GetBaggageIndividualPhoto(int Point);
    // 디버그
    void DebugShowItem(int Point) const;
};

#endif // BAGGAGE_H
