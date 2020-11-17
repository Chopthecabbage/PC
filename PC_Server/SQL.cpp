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

MYSQL_RES* SQL::CreateAccountOverlap()
{
    int query_stat;
	string query = "SELECT mem_id FROM members";    
	query_stat = mysql_query(conn, query.c_str());
    
	if (query_stat != 0)
	{
		cout << "아이디 정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
	
    return sql_result;
}

bool SQL::CreateAccountComplete(string id,string pw,string name,string birth,string phone,string status,string remain)
{
    int query_stat;
	string query;
	//query = "insert into members values('";
    query = "insert into members(mem_id, pw, mem_name,birth, phone, mem_status, mem_remain) values('";
    query += id;
    query += "','";
    query += pw;
	query += "','";
    query += name;
    query += "','";
    query += birth;
	query += "','";
    query += phone;
    query += "',";
    query += status;
	query += ",'";
    query += remain;
	query += "');";

    cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "데이터 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;
	}
	return true;
}
//
MYSQL_RES* SQL::FindAccountId()
{
    int query_stat;
	string query = "SELECT mem_id, mem_name FROM members";

	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "아이디, 이름 정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::FindAccountPw()
{
    int query_stat;
	string query = "SELECT mem_id, phone FROM members";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "아이디, 이름 정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

bool SQL::FindAccountUpdate(string name,string phone,string pw)
{
    int query_stat;
	string query;
	query = "UPDATE members SET pw = '";
    query += pw;
    query += "' WHERE mem_id = '";
    query += name;
    query += "' and phone = '";
    query += phone;
    query += "';";
    
    cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "데이터 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;
	}
	return true;
}

bool SQL::Log_In_Permanent_Update(string id,string pw)
{
    int query_stat;
	string query;
	query = "UPDATE members SET mem_status = '";
    query += "1";
    query += "' WHERE mem_id = '";
    query += id;
    query += "' and pw = '";
    query += pw;
    query += "';";
    
    cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "데이터 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;
	}
	return true;
}

MYSQL_RES* SQL::Log_In_Permanent()
{
    int query_stat;
    //where time_format('mem_remain','%H:%i')
	string query = "SELECT mem_id, pw, mem_name, birth, phone, mem_status, time_format(mem_remain,'%H:%i') FROM members";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "회원 정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

bool SQL::Log_In_Temporary_Update(string id)
{
    int query_stat;
	string query;
	query = "UPDATE temp_members SET temp_status = '";
    query += "1";
    query += "' WHERE temp_id = '";
    query += id;
    query += "';";
    
    cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "데이터 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;
	}
	return true;
}

MYSQL_RES* SQL::Log_In_Temporary()
{
    int query_stat;
	string query = "SELECT temp_id, temp_status, time_format(temp_remain,'%H:%i') FROM temp_members";
	
	cout << query << endl;
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "비회원 정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::ItemTotalCount()
{
	int query_stat;
	string query = "SELECT COUNT(*) FROM item";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "전체 행 갯수 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::ItemList_1()
{
	int query_stat;
	string query = "SELECT * FROM item Where category = 0 or category = 1";

	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::ItemList_2()
{
	int query_stat;
	string query = "SELECT * FROM item WHERE category = 2 OR category = 3";

	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::PREQUEL()
{
	int query_stat;
	string query = "SELECT COUNT(*) FROM item WHERE category = 0 OR category = 1";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::SEQUEL()
{
	int query_stat;
	string query = "SELECT COUNT(*) FROM item WHERE category = 2 OR category = 3";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::calPREQUEL()
{
	int query_stat;
	string query = "SELECT COUNT(*) FROM item WHERE category = 0";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

MYSQL_RES* SQL::calSEQUEL()
{
	int query_stat;
	string query = "SELECT COUNT(*) FROM item WHERE category = 2";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

bool SQL::ProcessIsCompleted(Arrangement arr)
{
	int query_stat;
    string query = "INSERT INTO arrangement(arr_date, payment, arr_status) VALUES(NOW(), ";
    query += to_string(arr.GetPayment());
    query += ", 1 )";
    query_stat = mysql_query(conn, query.c_str());
    if(query_stat != 0)
    {
        cout << "주문 저장 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;       
    }
    cout << "주문 저장 성공" << endl;
    query = "SELECT LAST_INSERT_ID()"; // 추가 후 PK 값 가져오기
    query_stat = mysql_query(conn, query.c_str());
    if(query_stat != 0)
    {
        cout << "PK 가져오기 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;       
    }
    sql_result = mysql_store_result(conn);
    sql_row = mysql_fetch_row(sql_result);    
    int arr_pk = atoi(sql_row[0]);
	cout << "PK 가져오기 성공 : " << arr_pk << endl;
	mysql_free_result(sql_result);

	int BaggageTotalCount=arr.GetBaggage().GetBaggageTotalCount();
	cout<<"BaggageTotalCount=arr.GetBaggage().GetBaggageTotalCount(): "<< arr.GetBaggage().GetBaggageTotalCount();
    
    for(int i = 0; i < BaggageTotalCount; i++)
    {
        int code = arr.GetBaggage().GetItemIndividual(i).GetItemCode();
		int amount = arr.GetBaggage().GetItemIndividualCount(i); 
        query = "INSERT INTO arrange_detail VALUES(";   
        query += to_string(arr_pk);
        query += ", ";            
        query += to_string(code);
        query += ", ";
        query += to_string(amount);
        query += ")";
        query_stat = mysql_query(conn, query.c_str());
        if(query_stat != 0)
        {
            cout << "상세 주문 저장 실패" << endl;
            fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
			return false;        
        }
    }
    cout << "상세 주문 저장 성공" << endl;
	return true;    
}

MYSQL_RES* SQL::need_arr_id()
{
	int query_stat;
	string query = "SELECT LAST_INSERT_ID()";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "정보 가져오기 실패" << endl;
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
	}
	sql_result = mysql_store_result(conn);
    return sql_result;
}

bool SQL::CloseEventChangeData(Member member,int judge)
{
	int query_stat;
	string str=member.GetMemberId();
    if(judge==1)
	{
		string query = "UPDATE members SET mem_status = 0 WHERE mem_id = '";
		query += str;
		query += "';";
		
		cout << query << endl;
		query_stat = mysql_query(conn, query.c_str());

		if(query_stat != 0)
		{
			cout << "상태 변경 실패" << endl;
			fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
			return false;         
		}
		cout << "상태 변경 성공" << endl;

		query = "UPDATE members SET mem_remain = time_format('00:00','%H:%i') WHERE mem_id = '";
		query += str;
		query += "';";

		cout << query << endl;
		query_stat = mysql_query(conn, query.c_str());

		if(query_stat != 0)
		{
			cout << "시간 변경 실패" << endl;
			fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
			return false;        
		}
		cout << "시간 변경 성공" << endl;
		
		str=to_string(member.GetMemberSeat());
		cout <<"str=GetMemberSeat(): "<<str<<endl;
		query = "DELETE FROM chat WHERE seat_num ='";
		query += str;
		query += "';";

		cout << query << endl;
		query_stat = mysql_query(conn, query.c_str());

		if(query_stat != 0)
		{
			cout << "회원 채팅 기록 삭제 실패" << endl;
			fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
			return false;        
		}
		cout << "회원 채팅 기록 삭제 성공" << endl;
	}
	else if(judge==2)
	{
		string query = "UPDATE temp_members SET temp_status = 0 WHERE temp_id = '";
		query += str;
		query += "';";
		
		cout << query << endl;
		query_stat = mysql_query(conn, query.c_str());

		if(query_stat != 0)
		{
			cout << "상태 변경 실패" << endl;
			fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
			return false;         
		}
		cout << "상태 변경 성공" << endl;

		query = "UPDATE temp_members SET temp_remain = time_format('00:00','%H:%i') WHERE temp_id = '";
		query += str;
		query += "';";

		cout << query << endl;
		query_stat = mysql_query(conn, query.c_str());

		if(query_stat != 0)
		{
			cout << "시간 변경 실패" << endl;
			fprintf(stderr, "Mysql query error : %s", mysql_error(conn)); 
			return false;       
		}
		cout << "시간 변경 성공" << endl;
		
		str=to_string(member.GetMemberSeat());
		cout <<"str=GetMemberSeat(): "<<str<<endl;
		query = "DELETE FROM chat WHERE seat_num ='";
		query += str;
		query += "';";

		cout << query << endl;
		query_stat = mysql_query(conn, query.c_str());

		if(query_stat != 0)
		{
			cout << "비회원 채팅 기록 삭제 실패" << endl;
			fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
			return false;        
		}
		cout << "비회원 채팅 기록 삭제 성공" << endl;
	}
	return true;
}

bool SQL::ShiftEventChangeData_Mem(Member member)
{
	int query_stat;
	string str=member.GetMemberId();
    string query = "UPDATE members SET mem_status = 0 WHERE mem_id = '";
	query += str;
	query += "';";
	
	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());

	if(query_stat != 0)
    {
        cout << "상태 변경 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));  
        return false;      
    }
    cout << "상태 변경 성공" << endl;

	string remain=member.GetMemberRemain();
	cout <<"remain: "<<remain<<endl;
	query = "UPDATE members SET mem_remain = time_format('";
	query += remain;
	query += "','%H:%i') WHERE mem_id = '";
	query += str;
	query += "';";

	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());

	if(query_stat != 0)
    {
        cout << "시간 변경 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;        
    }
    cout << "시간 변경 성공" << endl;
	
	str=to_string(member.GetMemberSeat());
	cout <<"str=GetMemberSeat(): "<<str<<endl;
	query = "DELETE FROM chat WHERE seat_num ='";
	query += str;
	query += "';";

	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());

	if(query_stat != 0)
    {
        cout << "회원 채팅 기록 삭제 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;        
    }
    cout << "회원 채팅 기록 삭제 성공" << endl;
	return true;
}

bool SQL::ShiftEventChangeData_Tem(Member member)
{
	int query_stat;
	string str=member.GetMemberId();
	cout<<"비회원: "<<str<<endl; 
    string query = "UPDATE temp_members SET temp_status = 0 WHERE temp_id = ";
	query += str;
	query += ";";
	
	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());

	if(query_stat != 0)
    {
        cout << "비회원 상태 변경 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn)); 
        return false;         
    }
    cout << "비회원 상태 변경 성공" << endl;

	string remain=member.GetMemberRemain();
	cout <<"비회원 remain: "<<remain<<endl;
	query = "UPDATE temp_members SET temp_remain = time_format('";
	query += remain;
	query += "','%H:%i') WHERE temp_id = ";
	query += str;
	query += ";";

	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());

	if(query_stat != 0)
    {
        cout << "비회원 시간 변경 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn)); 
		return false;       
    }
	cout << "비회원 시간 변경 성공" << endl;
	
	str=to_string(member.GetMemberSeat());
	cout <<"str=GetMemberSeat(): "<<str<<endl;
	query = "DELETE FROM chat WHERE seat_num ='";
	query += str;
	query += "';";

	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());

	if(query_stat != 0)
    {
        cout << "비회원 채팅 기록 삭제 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return false;        
    }
    cout << "비회원 채팅 기록 삭제 성공" << endl;
	return true;
}

bool SQL::UpdatChatting(int Sitting,int Writer,string Message)
{
	int query_stat;
	string query;
    query = "insert into chat values(";
	query += to_string(Sitting+1);
	query += ",";
	query += to_string(1);
	query += ",'";
	query += Message;
	query += "',";
	query += "NOW()";
	query += ");";
	
	cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());
	if(query_stat != 0)
    {
        cout << "데이터 추가 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn)); 
		return false;       
    }
	cout << "데이터 추가 성공" << endl;
	return true;
}

bool SQL::UpdateRemainEveryMinute(string Nick,string UpdateRemain,int Membership)
{
    if(Membership==1)
    {
        int query_stat;
        string query = "UPDATE members SET mem_remain = '"; 
        query += UpdateRemain;
        query += "' WHERE mem_id = '";
        query += Nick;
        query += "';";
        
        cout << query << endl;
        query_stat = mysql_query(conn, query.c_str());

        if(query_stat != 0)
        {
            cout << "남은 시간 변경 실패" << endl;
            fprintf(stderr, "Mysql query error : %s", mysql_error(conn));  
            return false;      
        }
        cout << "남은 시간 변경 성공" << endl;  
    }
    else if(Membership==2)
    {
        int query_stat;
        string query = "UPDATE temp_members SET temp_remain = '"; 
        query += UpdateRemain;
        query += "' WHERE temp_id = ";
        query += Nick;
        query += ";";
        
        cout << query << endl;
        query_stat = mysql_query(conn, query.c_str());

        if(query_stat != 0)
        {
            cout << "남은 시간 변경 실패" << endl;
            fprintf(stderr, "Mysql query error : %s", mysql_error(conn));  
            return false;      
        }
        cout << "남은 시간 변경 성공" << endl;  
    }
    return true;
}


void SQL::Disconnect()
{
    //mysql_free_result(sql_result);
    mysql_close(conn);
}

///////////////////[ 민근 ]//////////////////////////////////
int SQL::Select_Exists_Member(string check_id)
{
    int query_stat;
    string query = "SELECT EXISTS (SELECT * FROM members WHERE mem_id = '"+ check_id + "');";
   
    query_stat = mysql_query(conn, query.c_str());
    if (query_stat != 0)
	{
		cout << "멤버 있나없나 체크 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
    sql_result = mysql_store_result(conn);
    sql_row = mysql_fetch_row(sql_result);
    
    //cout << "exists : " << sql_row[0] << endl;
    return atoi(sql_row[0]);    
}

MYSQL_RES* SQL::Select_Member_By_ID(string search_id)
{
    int query_stat;
	string query = "SELECT * FROM members WHERE mem_id = '"+ search_id + "';";
	query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "ID로 멤버찾기 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
    sql_result = mysql_store_result(conn);
    return sql_result;
}

int SQL::Insert_Temp_Member(string remain)
{
    int query_stat;
    string query = "INSERT INTO temp_members(temp_remain, temp_status) VALUES('";
    query += remain;
    query += "', 0)";
    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "비회원 추가 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}

    query = "SELECT LAST_INSERT_ID()";
    query_stat = mysql_query(conn, query.c_str());
    if(query_stat != 0)
    {
        cout << "비회원ID 가져오기 실패" << endl;
        fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));        
    }
    sql_result = mysql_store_result(conn);
    sql_row = mysql_fetch_row(sql_result);    
    int temp_mem_id = atoi(sql_row[0]);    
    
    return temp_mem_id;
}

void SQL::Update_Member_Remain(string mem_id, string remain)
{
    int query_stat;
    string query = "UPDATE members SET mem_remain = '";
    query += remain;
    query += "' WHERE mem_id = '";
    query += mem_id;
    query += "' ";

    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "회원 시간충전 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
}

MYSQL_RES* SQL::Select_Order_Info(int order_num)
{
    int query_stat;
    string query = "SELECT arr_id, payment,";
    query += "(select sum(i.item_price * ad.amount) from arrange_detail as ad, item as i ";
    query += "where ad.code = i.code and arr_id = ";
    query += to_string(order_num);
    query += ") as tot_price  from arrangement where arr_id = ";
    query += to_string(order_num);

    query_stat = mysql_query(conn, query.c_str());
	if (query_stat != 0)
	{
		cout << "주문정보 가져오기 실패 " << endl;
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(conn));		
	}
    sql_result = mysql_store_result(conn);

    return sql_result;
}

/////////////////////////////////////////////////////////////
///////////////////[ 태환 ]//////////////////////////////////

// 구태환
bool SQL::isManagerExist(string input_id)
{
    bool isExist = false;
    string query;
    int query_stat;

    query += "SELECT COUNT(*) FROM manager WHERE login_id = '";
    query += input_id;
    query += "';";

   query_stat = mysql_query(conn,query.c_str());
   
   sql_result = mysql_store_result(conn);

    sql_row = mysql_fetch_row(sql_result);

    if(atoi(sql_row[0]) == 1)
    {
        cout << "존재" << endl;
        isExist = true;
    }
    return isExist;
}

int SQL::GetSeatCnt(string input_id)
{
    string query;
    int query_stat;

    query += "SELECT seat_cnt FROM manager WHERE login_id='";
    query += input_id;
    query += "';";

    query_stat = mysql_query(conn,query.c_str());
   
   sql_result = mysql_store_result(conn);

    sql_row = mysql_fetch_row(sql_result);

    cout << "seat cnt :" << sql_row[0] << endl;

    return atoi(sql_row[0]);    
}

int SQL::SetProcOrder(string order_num)
{
    string query;
    int query_stat;

    query += "UPDATE arrangement SET arr_status = 2 WHERE arr_id=";
    query += order_num;
    query += ";";

	cout << "query :" << query << endl;

   query_stat = mysql_query(conn,query.c_str());
   cout << "query_stat :" << query_stat << endl;
   return query_stat;
}

int SQL::SetCancelOrder(string order_num)
{
    string query;
    int query_stat;

    query += "UPDATE arrangement SET arr_status = 3 WHERE arr_id = ";
    query += order_num;
    query += ";";

	cout << "query :" << query << endl;

   query_stat = mysql_query(conn,query.c_str());
   cout << "query_stat :" << query_stat << endl;
   return query_stat;
}

int SQL::GetMsgCnt(int seat_num)
{
    int query_stat;
    string query;

    query += "SELECT COUNT(*) FROM chat WHERE seat_num=";
    query += to_string(seat_num);
    query += ";";
    query_stat = mysql_query(conn, query.c_str());
    
    if(query_stat != 0)
    {
        cout << "GetMsgCnt 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn)); 
        return false;       
    }
    cout << "GetMsgCnt 성공" << endl;

    sql_result = mysql_store_result(conn);
    sql_row = mysql_fetch_row(sql_result);
 
    return atoi(sql_row[0]);    
}

MYSQL_RES* SQL::GetMsgAll(int seat_num,int chat_cnt)
{
    int query_stat;

    string query;

    query += "SELECT CASE WHEN writer=2 THEN 'Admin' ELSE CONCAT('Seat ',CONVERT(seat_num,CHAR)) END AS 'Writer',";
    query += "message AS 'Msg' FROM chat WHERE seat_num=";
    query += to_string(seat_num);
    query += ";";
    cout << chat_cnt << "개 채팅출력" << endl;
    
    cout << "query :" << query << endl;

    query_stat = mysql_query(conn,query.c_str());
    if(query_stat != 0)
    {
        cout << "GetMsgAll 쿼리 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn));      
    }

    cout << "GetMsgAll 성공" << endl;
    sql_result = mysql_store_result(conn);
    return sql_result;
}

bool SQL::InsertChatInfo(int chat_form,int selected_seat_num,char *chat_msg)
{
    int query_stat;
    string query;
    string msg(chat_msg);
    // 채팅 형식에(0: 선택채팅 1 : 전체채팅) 맞는 메시지를 저장한다. 
    // 첫번쨰인자 : 어떤 좌석과의 대화인지 (0:관리자일방대화,1~:대화할 좌석번호)
    // 두번째인자 : 누가 채팅한 사람인지 (1:이용자 ,2:관리자)
    
    
    if(chat_form == 0)
    {
        query += "INSERT INTO chat VALUES (";
        query += to_string(selected_seat_num);
        query += ",";
        query += to_string(2);
        query += ",";
        query += "'";
        query += msg;
        query += "',";
        query += "NOW());";
    }
    else if(chat_form == 1) // 전체
    {
        // 첫번쨰인자 : 어떤 좌석과의 대화인지 (0:관리자일방대화,1~:대화할 좌석번호)
        // 두번째인자 : 누가 채팅한 사람인지 (1:이용자 ,2:관리자)
        query += "INSERT INTO chat VALUES (0,2,";
        query += "'";
        query += msg;
        query += "',";
        query += "NOW());";
    }

    cout << query << endl;
    query_stat = mysql_query(conn, query.c_str());
    if(query_stat != 0)
    {
        cout << "데이터 추가 실패" << endl;
        fprintf(stderr, "Mysql query error : %s", mysql_error(conn)); 
        return false;       
    }
    cout << "데이터 추가 성공" << endl;
    return true;
}

///////////준혁
Member SQL::GetMemberInfo(string mem_id)
{
    string query;
    Member temp;
    query="SELECT * FROM members WHERE mem_id='" + mem_id + "'";
    if (mysql_query(conn,query.c_str()))
	{
		error_check();
	}
	sql_result = mysql_store_result(conn);
	if (sql_result == NULL)
	{
		error_check();
	}
    sql_row = mysql_fetch_row(sql_result);

    if(sql_row!=NULL)
    {
        cout<<"row0: "<<sql_row[0]<<endl;
        cout<<"row1: "<<sql_row[1]<<endl;
        cout<<"row2: "<<sql_row[2]<<endl;
        cout<<"row3: "<<sql_row[3]<<endl;
        cout<<"row4: "<<sql_row[4]<<endl;
        cout<<"row5: "<<sql_row[5]<<endl;
        cout<<"row6: "<<sql_row[6]<<endl;
        temp.SettingNotSeatAndBegin(sql_row[0],sql_row[1],sql_row[2],sql_row[3],sql_row[4],atoi(sql_row[5]),sql_row[6]);
    }

    return temp;
}

void SQL::ModifiedInfo(Member member)
{
    string pw(member.GetMemberPassword());
    string query = "UPDATE members SET pw='"+ pw +"',";

    string name(member.GetMemberName());
    query += "mem_name='"+ name +"',";

    string birth(member.GetMemberBirth());
    query += "birth='"+ birth +"',";

    string phone(member.GetMemberPhone());
    query += "phone ='"+ phone +"',";

    string status=to_string(member.GetMemberStatus());
    query += "mem_status="+ status +",";

    string remain(member.GetMemberRemain());
    string id(member.GetMemberId());
    query += "mem_remain='"+ remain +"' WHERE mem_id='"+ id + "'";

	if (mysql_query(conn, query.c_str()))
	{
		error_check();
	}
    cout << "수정 완료" <<endl;
}

void SQL::DeletedInfo(Member member)
{
    string id(member.GetMemberId());
    string query = "DELETE FROM members WHERE mem_id='" + id + "'";

	if (mysql_query(conn, query.c_str()))
	{
		error_check();
	}
    cout << "삭제 완료" << endl;
}

int SQL::GetDateCnt(string date,int sig)
{
    string query;
    if(sig==ManagementSig::MONTH_CHOICE) //월 매출
    {
        query="SELECT COUNT(arr_date) FROM arrangement ";
        query+="RIGHT JOIN arrange_detail ON arrangement.arr_id=arrange_detail.arr_id WHERE arr_date like '"+ date +"%' and arr_status=2";
    }
    else //일 매출
    {
        query="SELECT COUNT(arr_date) FROM arrangement ";
        query+="RIGHT JOIN arrange_detail ON arrangement.arr_id=arrange_detail.arr_id WHERE DATE(arr_date)='"+ date + "' and arr_status=2";
    }
    
    if (mysql_query(conn,query.c_str()))
	{
		error_check();
	}
	sql_result = mysql_store_result(conn);
	if (sql_result == NULL)
	{
		error_check();
	}
    sql_row = mysql_fetch_row(sql_result);

    if(sql_row!=NULL)
    {
        cout<<"row0: "<<sql_row[0]<<endl;
        return atoi(sql_row[0]);
    }
    return 0;
}

SalesInfo* SQL::GetArrInfo(SalesInfo *salesinfo,string date,int sig)
{    
    string query;

    if(sig==ManagementSig::MONTH_CHOICE) //월 매출
    {
        query="SELECT arrangement.arr_id,arr_date,payment,arr_status,code,amount FROM arrangement ";
        query+="LEFT JOIN arrange_detail ON arrangement.arr_id=arrange_detail.arr_id WHERE arr_date like '"+ date +"%' and arr_status=2";
    }
    else //일 매출
    {
        query="SELECT arrangement.arr_id,arr_date,payment,arr_status,code,amount FROM arrangement ";
        query+="LEFT JOIN arrange_detail ON arrangement.arr_id=arrange_detail.arr_id WHERE DATE(arr_date)='"+ date + "' and arr_status=2";
    }
    if (mysql_query(conn,query.c_str()))
	{
		error_check();
	}
	sql_result = mysql_store_result(conn);
	if (sql_result == NULL)
	{
		error_check();
	}
    
    int num=0;
    int cnt=0;
    int num_fields = mysql_num_fields(sql_result);
 
    while ((sql_row = mysql_fetch_row(sql_result)))
    {
        cnt=0;
        for (int j = 0; j < num_fields; j++)
        {
            if(cnt==0)
            {
                salesinfo[num].SetArrId(atoi(sql_row[j]));
            }

            else if(cnt==1)
            {
                salesinfo[num].SetArrDate(sql_row[j]);
            }

            else if(cnt==2)
            {
                salesinfo[num].SetPayment(atoi(sql_row[j]));
            }

            else if(cnt==3)
            {
                salesinfo[num].SetArrStatus(atoi(sql_row[j]));
            }

            else if(cnt==4)
            {
                salesinfo[num].SetCode(atoi(sql_row[j]));
            }

            else if(cnt==5)
            {
                salesinfo[num].SetAmount(atoi(sql_row[j]));
            }
            cnt++;
        }        
        num++;
	}

    return salesinfo;
}

Item SQL::GetArrItemInfo(SalesInfo *salesinfo)
{
    Item item;
    string code=to_string(salesinfo->GetCode());
    string query="SELECT item_name,item_price FROM item WHERE code="+ code;
    if (mysql_query(conn,query.c_str()))
	{
		error_check();
	}
	sql_result = mysql_store_result(conn);
	if (sql_result == NULL)
	{
		error_check();
	}
    sql_row = mysql_fetch_row(sql_result);

    if(sql_row!=NULL)
    {
	    item.SetItemName(sql_row[0]);
        item.SetItemPrice(atoi(sql_row[1]));
	}

    return item;
}

void SQL::error_check()
{
	string error= mysql_error(conn);
    cout<<error<<endl;
}