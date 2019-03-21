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
	uint32_t threadNums;

	stAddressInfo()
	{
		serverIp = "";
		port = 0;
		maxEvents = 0;
		backlog = 0;
		threadNums = 0;
	}
};

// log info
struct stLoggerInfo
{
	std::string LoggerName;
	std::string LogFilePath;
	uint32_t MaxSingleFileSize;
	uint8_t MaxLogFileNums;

	stLoggerInfo()
	{
		LoggerName = "";
		LogFilePath = "";
		MaxSingleFileSize = 0;
		MaxLogFileNums = 0;
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

class ServerConf : public ISingleton<ServerConf>
{

public:
	// load all configure by conf file 
	bool LoadServerConf();
	// if load conf file right return true
	bool hasLoadConfRight();
	// get websocket address info configuration
	const stAddressInfo& GetWSAddressInfoConfiguration();
	// get normal socket addresss info configuration
	const stAddressInfo& GetNSAddressInfoConfiguration();
	// get log configuration
	std::shared_ptr<stLoggerInfo> GetLoggerInfo();
	// get redis configuration
	std::shared_ptr<stRedisInfo> GetRedisInfo();

protected:
	// load local server configure
	bool LoadServerInfo(const std::string& fileDir, const std::string& addressInfo);
	// load log configure
	bool LoadLogInfo(const std::string& FilePath);
	// load redis configure
	bool LoadRedisInfo(const std::string& FilePath);

private:
	// WebSocket address info
	stAddressInfo webSocketInfo;
	// normal socket address info
	stAddressInfo normalSocketInfo;
	// log configuration
	std::shared_ptr<stLoggerInfo> LoggerInfo;
	// redis configuration
	std::shared_ptr<stRedisInfo> RedisInfo;


	// if all configuration has be loaded right
	bool bAllConfLoadedRight;

private:

	friend ISingleton<ServerConf>;
	friend class std::unique_ptr<ServerConf>;

	ServerConf();

public:
	~ServerConf();
};
