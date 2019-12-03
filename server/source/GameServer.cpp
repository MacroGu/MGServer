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


CGameServer::CGameServer()
{
}

CGameServer::~CGameServer()
{
	// WebSocketPool.StopEpoll();
	SocketPool.StopEpoll();
}

bool CGameServer::InitServer()
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

void CGameServer::StartServer()
{
	if (!InitServer()) return;

	SocketPool.SetAddressInfo(ADDRESS_INFO_CONFIGURE);
	SocketPool.SetSocketWatcher(new GameSocketWatcher());

	if (!SocketPool.StartEpoll()) return;

}
