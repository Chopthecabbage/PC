#include "Baggage.h"

Baggage::Baggage() : BaggageTotalCount(0), BaggageTotalPrice(0)
{
    memset(ItemIndividualCount,0,sizeof(int[DefineBaggage::MAXSIZE]));
}

Item& Baggage::GetItemIndividual(int Point)
{
    return item[Point];
}

Baggage& Baggage::GetBaggage()
{
    return *this;
}

void Baggage::AddItem(Item item, int ItemIndividualCount)
{
    this->item[BaggageTotalCount]=item;
    this->ItemIndividualCount[BaggageTotalCount]=ItemIndividualCount;
    BaggageTotalPrice+=this->item[BaggageTotalCount].GetItemPrice()*this->ItemIndividualCount[BaggageTotalCount];
    BaggageTotalCount++;
}

int Baggage::GetBaggageTotalCount() const
{
    return BaggageTotalCount;
}

int Baggage::GetBaggageTotalPrice() const
{
    return BaggageTotalPrice;
}

int Baggage::GetItemIndividualCount(int Point) const
{
    return ItemIndividualCount[Point];
}

void Baggage::SetItemIndividualCount(int ItemIndividualCount)
{
    this->ItemIndividualCount[BaggageTotalCount]=ItemIndividualCount;
}
/*
QString Baggage::QStringBaggageIndividualName(int Point)
{
    return QString(item[Point].GetItemName());
}

QString Baggage::QStringBaggageIndividualPrice(int Point)
{
    return QString(item[Point].GetItemPrice());
}
QString Baggage::QStringItemIndividualCount(int Point)
{
    return QString(GetItemIndividualCount(Point));
}
*/
char* Baggage::GetBaggageIndividualName(int Point)
{
    return item[Point].GetItemName();
}

int Baggage::GetBaggageIndividualPrice(int Point) const
{
    return item[Point].GetItemPrice();
}

char* Baggage::GetBaggageIndividualPhoto(int Point)
{
    return item[Point].GetItemPhoto();
}
// 디버그
void Baggage::DebugShowItem(int Point) const
{
    item[Point].DebugShowItem();
    cout<<"수량: "<<ItemIndividualCount[Point]<<endl;
    //qDebug() << "수량: " <<ItemIndividualCount[Point];
}