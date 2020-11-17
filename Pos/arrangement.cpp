#include "arrangement.h"

Arrangement::Arrangement() : payment(0)
{

}

Arrangement::Arrangement(int payment, Baggage baggage) : payment(payment)
{
    this->baggage = baggage;
}

Arrangement& Arrangement::GetArrangement()
{
    return *this;
}

void Arrangement::SetPayment(int payment)
{
    this->payment=payment;
}

int Arrangement::GetPayment() const
{
    return this->payment;
}

void Arrangement::AddItem(Item item, int ItemIndividualCount)
{
    baggage.AddItem(item,ItemIndividualCount);
}

int Arrangement::GetBaggageTotalCount() const
{
    return baggage.GetBaggageTotalCount();
}

int Arrangement::GetBaggageTotalPrice() const
{
    return baggage.GetBaggageTotalPrice();
}
int Arrangement::GetItemIndividualCount(int Point) const
{
    return baggage.GetItemIndividualCount(Point);
}

int Arrangement::GetArrangementPayment() const
{
    return payment;
}

int Arrangement::GetArrangementNum() const
{
    return arrangement_num;
}

Baggage& Arrangement::GetBaggage()
{
    return baggage.GetBaggage();
}

QString Arrangement::QStringBaggageIndividualName(int Point)
{
    return baggage.QStringBaggageIndividualName(Point);
}

QString Arrangement::QStringBaggageIndividualPrice(int Point)
{
    return baggage.QStringBaggageIndividualPrice(Point);
}

QString Arrangement::QStringItemIndividualCount(int Point)
{
    return baggage.QStringItemIndividualCount(Point);
}

char* Arrangement::GetBaggageIndividualName(int Point)
{
    return baggage.GetBaggageIndividualName(Point);
}

int Arrangement::GetBaggageIndividualPrice(int Point) const
{
    return baggage.GetBaggageIndividualPrice(Point);
}
// 디버그
void Arrangement::DebugShowItem(int Point) const
{
    baggage.DebugShowItem(Point);
}
