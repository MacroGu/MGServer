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
#include "WebSocketStyle.h"
#include "EpollSocket.h"

// socket 处理类
class GameSocketWatcher : public BaseSocketWatcher
{
public:
	GameSocketWatcher();

	virtual ~GameSocketWatcher();

	virtual int OnEpollAcceptEvent(stSocketContext &socket_context);

	virtual int OnEpollReadableEvent(int &epollfd, epoll_event &event);

	virtual int OnEpollWriteableEvent(stSocketContext &socket_context);

	virtual int OnEpollCloseEvent(stSocketContext &socket_context);

private:
	// handle client message
	bool HandleClientNormalSocketData(stSocketContext *socket_context, char clientData[], int dataLength);


	// 如果需要解析websocket 协议， 用此实现
	CWebSocketStyle webSocketStyle;

};


class CGameServer
{
public:
	CGameServer();
	~CGameServer();

	bool StartServer();

private:

	// 用于处理  普通socket 的客户端的数据
	EpollSocket SocketPool;
};