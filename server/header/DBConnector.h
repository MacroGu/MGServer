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

	// DB ����
	bool Connect(
		const string& Server,
		const string& User,
		const string& Password,
		const string& Database,
		const int& Port
	);
	// DB ������ֹ
	void Close();

	// �����û��ʻ�
	bool SearchAccount(const string& Id, const string& Password);
	// ע���û��ʻ�
	bool SignUpAccount(const string& Id, const string& Password);

private:
	MYSQL* Conn;		// ������
	MYSQL_RES* Res;	// ���ֵ
	MYSQL_ROW Row;		// ���row
};