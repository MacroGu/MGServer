#pragma once
#include <string>
#include <mysql.h>
#include "defines.h"
#include "singleton.h"
#include "ServerConf.h"


class MysqlHandle : public ISingleton<MysqlHandle>
{
public:
	~MysqlHandle();


	bool Init();


	// DB ����
	bool Connect();
	// DB ������ֹ
	void Close();

	// �����û��ʻ�
	bool SearchAccount(const std::string& Id, const std::string& Password);
	// ע���û��ʻ�
	bool SignUpAccount(const std::string& Id, const std::string& Password);

private:
	friend ISingleton<MysqlHandle>;
	friend class std::unique_ptr<MysqlHandle>;

	MysqlHandle();


private:
	MYSQL* Conn;		// ������
	MYSQL_RES* Res;	// ���ֵ
	MYSQL_ROW Row;		// ���row
	std::shared_ptr<stMySQLInfo> MysqlInfo;
};