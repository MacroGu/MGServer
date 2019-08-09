/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#pragma once
#include <thread>
#include <sstream>
#include <map>
#include <set>
#include <stdint.h>
#include <iostream>
#include "EpollSocket.h"
#include "CommonClass.h"
#include "GameServerWatcher.h"


class CGameServer
{
public:
	CGameServer();
	~CGameServer();

	// 初始化服务器
	bool InitServer();
	// 启动服务器
	void StartServer();

private:

	// 用于处理  普通socket 的客户端的数据
	EpollSocket SocketPool;
};