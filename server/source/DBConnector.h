#pragma once
#include "defines.h"
#include <mysql.h>
#include <string>

using namespace std;

class DBConnector
{
public:
	DBConnector();
	~DBConnector();

	// DB 连接
	bool Connect(
		const string& Server,
		const string& User,
		const string& Password,
		const string& Database,
		const int& Port
	);
	// DB 连接终止
	void Close();

	// 查找用户帐户
	bool SearchAccount(const string& Id, const string& Password);
	// 注册用户帐户
	bool SignUpAccount(const string& Id, const string& Password);

private:
	MYSQL* Conn;		// 连接器
	MYSQL_RES* Res;	// 结果值
	MYSQL_ROW Row;		// 结果row
};