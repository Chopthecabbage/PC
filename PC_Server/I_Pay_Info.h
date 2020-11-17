#pragma once

class I_Pay_Info
{
private:
    int arr_id; // 주문번호
    int price; // 총 가격
    int payment; // 결제수단

public:
    I_Pay_Info() : arr_id(0), price(0), payment(0)
    {  }
    I_Pay_Info(int arr_id, int price, int payment)
    {
        this->arr_id = arr_id;
        this->price = price;
        this->payment = payment;
    }    
    void Set_Arr_Id(int arr_id) { this->arr_id = arr_id; }
    void Set_Price(int price) { this->price = price; }
    void Set_Payment(int payment) { this->payment = payment; }

    int Get_Arr_Id() const { return arr_id; }
    int Get_Price() const { return price; }
    int Get_Payment() const { return payment; }    
};

