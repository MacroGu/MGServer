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

struct stMsgToClient
{
	std::string temp;
};

// socket 处理类
class GameSocketWatcher : public BaseSocketWatcher
{
public:
	GameSocketWatcher();

	virtual ~GameSocketWatcher();

	virtual int OnEpollAcceptEvent(stSocketContext& socket_context);

	virtual int OnEpollReadableEvent(int& epollfd, epoll_event& event);

	virtual int OnEpollWriteableEvent(stSocketContext& socket_context);

	virtual int OnEpollCloseEvent(stSocketContext& socket_context);

private:
	// handle client message
	bool HandleClientNormalSocketData(stSocketContext* socket_context, char clientData[], int dataLength);


private:
	std::map<int, stMsgToClient> allClients;		// 所有的客户端
	stMsgToClient curClient;		// 当前的客户端
};