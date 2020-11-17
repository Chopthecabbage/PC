#include "item.h"

Item::Item()
{
    this->ItemCode=0;
    memset(ItemName,0,sizeof(ItemName));
    this->ItemPrice=0;
    this->ItemCategory=0;
    memset(ItemPhoto,0,sizeof(ItemPhoto));
}

Item::Item(int ItemCode, char* ItemName, int ItemPrice,int ItemCategory, char* ItemPhoto)
{
    this->ItemCode=ItemCode;
    strcpy(this->ItemName,ItemName);
    this->ItemPrice=ItemPrice;
    this->ItemCategory=ItemCategory;
    strcpy(this->ItemPhoto,ItemPhoto);
}

Item& Item::GetItem()
{
    return *this;
}

int Item::GetItemCode() const
{
    return ItemCode;
}

char* Item::GetItemName()
{
    return ItemName;
}

int Item::GetItemPrice() const
{
    return ItemPrice;
}

int Item::GetItemCategory() const
{
    return ItemCategory;
}

char* Item::GetItemPhoto()
{
    return ItemPhoto;
}

void Item::SetItemCode(int ItemCode)
{
    this->ItemCode=ItemCode;
}

void Item::SetItemName(char* ItemName)
{
    strcpy(this->ItemName,ItemName);
}

void Item::SetItemPrice(int ItemPrice)
{
    this->ItemPrice=ItemPrice;
}

void Item::SetItemCategory(int ItemCategory)
{
    this->ItemCategory=ItemCategory;
}

void Item::SetItemPhoto(char* ItemPhoto)
{
    strcpy(this->ItemPhoto,ItemPhoto);
}

void Item::InitializeItem()
{
    this->ItemCode=0;
    memset(ItemName,0,sizeof(ItemName));
    this->ItemPrice=0;
    this->ItemCategory=0;
    memset(ItemPhoto,0,sizeof(ItemPhoto));
}
/*
void Item::operator=(const Item &item)
{
    this->ItemCode=item.ItemCode;
    strcpy(this->ItemName,item.ItemName);
    this->ItemPrice=item.ItemPrice;
    this->ItemCategory=item.ItemCategory;
    strcpy(this->ItemPhoto,item.ItemPhoto);
}
*/
// 디버그
void Item::DebugShowItem() const
{
    qDebug() <<"코드: "<<ItemCode<<" 이름: "<<ItemName<<" 가격: "<<ItemPrice<<" 카테고리: "<<ItemCategory<<" 경로: "<<ItemPhoto;
}
