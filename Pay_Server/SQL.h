#pragma once
#include "Common.h"
#include "T_Pay_Info.h"
#include "I_Pay_Info.h"

class SQL
{
private:
    MYSQL *conn;
    MYSQL_RES *sql_result;
    MYSQL_ROW sql_row;
    
public:
    SQL();  
    bool Connect();
    void Disconnect(); 
    void Insert_Time_Payment(T_Pay_Info t_info, int temp_mem_id);     
    void Insert_Time_Payment(T_Pay_Info t_info);    
    void Insert_Item_Payment(I_Pay_Info i_info); 
};


