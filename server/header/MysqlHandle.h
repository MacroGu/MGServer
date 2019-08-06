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


	// DB 连接
	bool Connect();
	// DB 连接终止
	void Close();

	// 查找用户帐户
	bool SearchAccount(const std::string& Id, const std::string& Password);
	// 注册用户帐户
	bool SignUpAccount(const std::string& Id, const std::string& Password);

private:
	friend ISingleton<MysqlHandle>;
	friend class std::unique_ptr<MysqlHandle>;

	MysqlHandle();


private:
	MYSQL* Conn;		// 连接器
	MYSQL_RES* Res;	// 结果值
	MYSQL_ROW Row;		// 结果row
	std::shared_ptr<stMySQLInfo> MysqlInfo;
};