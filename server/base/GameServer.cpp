/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include <iostream>
#include <stdint.h>
#include <nlohmann/json.hpp>
#include "GameServer.h"
#include "protocol.h"
#include "ServerConf.h"
#include "RedisHandle.h"
#include "MysqlHandle.h"
#include "Interface.h"

extern Interface gInterface;

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
	SocketPool.StopEpoll();
}

bool GameServer::InitServer()
{
	if (!ServerConf::GetInstance().LoadServerConf())
	{
		LOG_ERROR("load server info configure file read failed!");
	}
	else
	{
		LOG_INFO("load server configuration successfully! ");
	}


	// 	if (!RedisHandle::GetInstance().Init())
	// 	{
	// 		LOG_ERROR("init redis failed!");
	// 		return false;
	// 	}

	if (!MysqlHandle::GetInstance().Init())
	{
		LOG_ERROR("init Mysql failed!");
		return false;
	}

	return true;
}

void GameServer::StartServer()
{
	if (!InitServer()) return;

	GameSocketWatcher watcher;
	SocketPool.SetAddressInfo(ADDRESS_INFO_CONFIGURE);
	SocketPool.SetSocketWatcher(&watcher);


	gInterface.SetSocketPool(&SocketPool);
	gInterface.SetGameSocketWatcher(&watcher);

	if (!SocketPool.StartEpoll()) return;

}