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
	char temp[128];
};

// socket 处理类
class GameSocketWatcher : public BaseSocketWatcher
{
public:
	// 一些暴露给游戏逻辑的接口
	// 广播 1 ， 由客户端收到消息后，经过处理，再进行广播，

	// 广播 2 ，服务器产生的事件，比如定时事件，而主动广播


	// 发送给指定客户端

public:
	GameSocketWatcher();

	virtual ~GameSocketWatcher();

	virtual int OnEpollAcceptEvent(stSocketContext& socket_context) override;

	virtual int OnEpollReadableEvent(int& epollfd, epoll_event& event) override;

	virtual int OnEpollWriteableEvent(stSocketContext& socket_context) override;

	virtual int OnEpollCloseEvent(stSocketContext& socket_context) override;

private:
	// handle client message
	bool HandleClientNormalSocketData(stSocketContext* socket_context, char clientData[], int dataLength);


private:
	std::map<int, stMsgToClient> allClients;		// 所有的客户端
	stMsgToClient curClient;		// 当前的客户端
};