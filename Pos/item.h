#ifndef ITEM_H
#define ITEM_H

#include "Common.h"

class Item
{
private:
    int ItemCode;           // 아이템 코드
    char ItemName[DefineItem::BUFFSIZE]; // 아이템 이름
    int ItemPrice;     // 아이템 가격
    int ItemCategory;       // 아이템 분류
    char ItemPhoto[DefineItem::BUFFSIZE];     // 사진 경로
public:
    Item();
    Item(int ItemCode, char* ItemName, int ItemPrice,int ItemCategory, char* Itemphoto);

    Item& GetItem();

    int GetItemCode() const;
    char* GetItemName();
    int GetItemPrice() const;
    int GetItemCategory() const;
    char* GetItemPhoto();

    void SetItemCode(int ItemCode);
    void SetItemName(char* ItemName);
    void SetItemPrice(int ItemPrice);
    void SetItemCategory(int ItemCategory);
    void SetItemPhoto(char* ItemPhoto);

    void InitializeItem();

    //void operator=(const Item &item);

    // 디버그
    void DebugShowItem() const;
};

#endif // ITEM_H
