/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once

#include <memory>
#include <tinyxml.h>
#include <stdint.h>
#include "singleton.h"
#include "defines.h"


// local server info
struct stAddressInfo
{
	std::string serverIp;
	uint32_t port;
	uint32_t maxEvents;
	uint32_t backlog;
	uint32_t WorkerThreadTaskMax;
	uint32_t timeCheckAcceptClient;

	stAddressInfo()
	{
		serverIp = "";
		port = 0;
		maxEvents = 0;
		backlog = 0;
		WorkerThreadTaskMax = 0;
		timeCheckAcceptClient = 0;
	}
};

// log info
struct stLoggerInfo
{
	std::string LoggerName;
	std::string LogFilePath;
	uint8_t hour;
	uint8_t minutes;
	uint8_t flushCache;

	stLoggerInfo()
	{
		LoggerName = "";
		LogFilePath = "";
		hour = 0;
		minutes = 0;
		flushCache = 0;
	}

};

// redis info
struct stRedisInfo
{
	std::string redisAddress;
	uint32_t redisPort;

	stRedisInfo()
	{
		redisAddress = "";
		redisPort = 0;
	}

};

// MySQL info
struct stMySQLInfo
{
	std::string MySqlAddress;
	uint32_t MySqlPort;
	std::string userName;
	std::string password;
	std::string database;

	stMySQLInfo()
	{
		MySqlAddress = "";
		MySqlPort = 0;
		userName = "";
		password = "";
		database = "";
	}

};

// current server unique info
struct stUniqueInfo
{
	std::string serverID;

	stUniqueInfo()
	{
		serverID = "";
	}

};

class ServerConf : public ISingleton<ServerConf>
{

public:
	// load all configure by conf file 
	bool LoadServerConf();
	// if load conf file right return true
	bool hasLoadConfRight();
	// get normal socket addresss info configuration
	const stAddressInfo& GetAddressInfoConfiguration();
	// get log configuration
	std::shared_ptr<stLoggerInfo> GetLoggerInfo();
	// get redis configuration
	std::shared_ptr<stRedisInfo> GetRedisInfo();
	// get Mysql configuration
	std::shared_ptr<stMySQLInfo> GetMySqlInfo();

protected:
	// load local server configure
	bool LoadServerInfo(const std::string& fileDir, const std::string& addressInfo);
	// load log configure
	bool LoadLogInfo(const std::string& FilePath);
	// load redis configure
	bool LoadRedisInfo(const std::string& FilePath);
	// load redis configure
	bool LoadMySqlInfo(const std::string& FilePath);
	// load server unique info configure
	bool LoadUniqueInfo(const std::string& FilePath);


private:
	// normal socket address info
	stAddressInfo AddressInfo;
	// log configuration
	std::shared_ptr<stLoggerInfo> LoggerInfo;
	// redis configuration
	std::shared_ptr<stRedisInfo> RedisInfo;
	// Mysql configuration
	std::shared_ptr<stMySQLInfo> MySqlInfo;
	// current server unique info configuration
	std::shared_ptr<stUniqueInfo> UniqueInfo;


	// if all configuration has be loaded right
	bool bAllConfLoadedRight;

private:

	friend ISingleton<ServerConf>;
	friend class std::unique_ptr<ServerConf>;

	ServerConf();

public:
	~ServerConf();
};
