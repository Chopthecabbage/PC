#include "arrangement.h"

Arrangement::Arrangement() : payment(0)
{

}

Arrangement::Arrangement(int payment, Baggage baggage) : payment(payment)
{
    this->baggage = baggage;
}

void Arrangement::SetArrangement(int payment, Baggage baggage)
{
    this->payment = payment;
    this->baggage = baggage;
}

Baggage& Arrangement::GetBaggage()
{
    return baggage;
}

Arrangement& Arrangement::GetArrangement()
{
    return *this;
}

int Arrangement::GetArrangementNum() const
{
    return ArrangementNum;
}

int Arrangement::GetPayment() const
{
    return payment;
}

void Arrangement::SetArrangementNum(int ArrangementNum)
{
    this->ArrangementNum=ArrangementNum;
}

void Arrangement::SetPayment(int payment)
{
    this->payment=payment;
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
