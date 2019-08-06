#include "DBConnector.h"


DBConnector::DBConnector()
{
}


DBConnector::~DBConnector()
{
}

bool DBConnector::Connect(const string& Server, const string& User, const string& Password, const string& Database, const int& Port)
{
	Conn = mysql_init(NULL);
	if (!mysql_real_connect(
		Conn, Server.c_str(), User.c_str(), Password.c_str(), Database.c_str(), Port, NULL, 0)
		)
	{
		LOG_ERROR("[DB] DB 连接失败");
		return false;
	}

	return true;
}

void DBConnector::Close()
{
	mysql_close(Conn);
}

bool DBConnector::SearchAccount(const string& Id, const string& Password)
{
	bool bResult = false;
	string sql = "SELECT * FROM sungminworld.playeraccount WHERE id = '";
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

bool DBConnector::SignUpAccount(const string& Id, const string& Password)
{
	bool bResult = false;

	string sql = "INSERT INTO sungminworld.playeraccount (id, pw) VALUES";
	sql += " ('" + Id + "', '" + Password + "')";

	if (mysql_query(Conn, sql.c_str()))
	{
		LOG_ERROR("[DB] 重复的帐户");
		return false;
	}

	bResult = true;

	return bResult;
}