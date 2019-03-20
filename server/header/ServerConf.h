/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once

#include <memory>
#include <tinyxml.h>
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

class ServerConf : public ISingleton<ServerConf>
{

public:
	// load all configure by conf file 
	bool loadConf(const std::string& serverKind);
	// if load conf file right return true
	bool hasLoadConfRight();
	// get websocket address info configuration
	const stAddressInfo& GetWSAddressInfoConfiguration();
	// get normal socket addresss info configuration
	const stAddressInfo& GetNSAddressInfoConfiguration();

protected:
	// load local server configure
	bool LoadServerInfo(const std::string& fileDir, const std::string& addressInfo);

private:
	// WebSocket address info
	stAddressInfo webSocketInfo;
	// normal socket address info
	stAddressInfo normalSocketInfo;

	// if all configuration has be loaded right
	bool bAllConfLoadedRight;

private:

	friend ISingleton<ServerConf>;
	friend class std::unique_ptr<ServerConf>;

	ServerConf();

public:
	~ServerConf();
};
