#include "MysqlHandle.h"


MysqlHandle::MysqlHandle()
{
}


MysqlHandle::~MysqlHandle()
{
}

bool MysqlHandle::Init()
{
	MysqlInfo = ServerConf::GetInstance().GetMySqlInfo();
	if (!MysqlInfo)
	{
		LOG_ERROR("get Mysql info failed!");
		return false;
	}

	if (!Connect())
	{
		return false;
	}

	return true;
}

bool MysqlHandle::Connect()
{
	Conn = mysql_init(NULL);
	if (!mysql_real_connect(
		Conn, MysqlInfo->MySqlAddress.c_str(), MysqlInfo->userName.c_str(), MysqlInfo->password.c_str(), 
		MysqlInfo->database.c_str(), MysqlInfo->MySqlPort, NULL, 0) )
	{
		LOG_ERROR("[DB] DB 连接失败");
		return false;
	}

	return true;
}

void MysqlHandle::Close()
{
	mysql_close(Conn);
}

bool MysqlHandle::SearchAccount(const std::string& Id, const std::string& Password)
{
	bool bResult = false;
	std::string sql = "SELECT * FROM test.playeraccount WHERE id = '";
	sql += Id + "' and pw = '" + Password + "'";

	if (mysql_query(Conn, sql.c_str()))
	{
		LOG_ERROR("[DB] 查找失败");
		return false;
	}

	Res = mysql_use_result(Conn);
	Row = mysql_fetch_row(Res);
	if (Row != NULL)
	{
		bResult = true;
	}
	else
	{
		LOG_ERROR("[ERROR] 没有这样的ID");
		bResult = false;
	}
	mysql_free_result(Res);

	return bResult;
}

bool MysqlHandle::SignUpAccount(const std::string& Id, const std::string& Password)
{
	bool bResult = false;

	std::string sql = "INSERT INTO test.playeraccount (id, pw) VALUES";
	sql += " ('" + Id + "', '" + Password + "')";

	if (mysql_query(Conn, sql.c_str()))
	{
		LOG_ERROR("[DB] 重复的帐户");
		return false;
	}

	bResult = true;

	return bResult;
}