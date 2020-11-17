#include "SQL.h"

/*
    [ mysql 함수 설명 ] 
    http://www.mysqlkorea.com/sub.html?mcode=manual&scode=01&m_no=21855&cat1=22&cat2=596&cat3=606&lang=k
*/

SQL::SQL()
{
	conn = mysql_init(NULL); 
}

bool SQL::Connect()
{
    mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char *)NULL, 0);
    
    if (conn == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(conn));
        return false;
    }
    //한글 사용을위해 추가 
    mysql_query(conn, "set session character_set_connection=utf8;");
    mysql_query(conn, "set session character_set_results=utf8;");
    mysql_query(conn, "set session character_set_client=utf8;");
    return true;
}

void SQL::Disconnect()
{
    mysql_close(conn);
}

void SQL::Insert_Time_Payment(T_Pay_Info t_info, int temp_mem_id)
{
    int query_stat;
    string query = "INSERT INTO time_payment(mem_id, t_pay_date, t_pay_price, t_payment) VALUES('";
    query += to_string(temp_mem_id);
    query += "', NOW(), ";
    query += to_string(t_info.Get_Price());
    query += ", ";
    query += to_string(t_info.Get_Payment());
    query += ")";

    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "시간충전 결제정보 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
}

void SQL::Insert_Time_Payment(T_Pay_Info t_info)
{
    int query_stat;
    string mem_id(t_info.Get_Mem_Id());
    string query = "INSERT INTO time_payment(mem_id, t_pay_date, t_pay_price, t_payment) VALUES('";
    query += mem_id;
    query += "', NOW(), ";
    query += to_string(t_info.Get_Price());
    query += ", ";
    query += to_string(t_info.Get_Payment());
    query += ")";

    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "시간충전 결제정보 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
}

void SQL::Insert_Item_Payment(I_Pay_Info i_info)
{
    int query_stat;
    string query = "INSERT INTO item_payment VALUES(";
    query += to_string(i_info.Get_Arr_Id());
    query += ", NOW(), ";
    query += to_string(i_info.Get_Price());
    query += ", ";
    query += to_string(i_info.Get_Payment());
    query += ")";

     query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "음식주문 결제정보 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
}
