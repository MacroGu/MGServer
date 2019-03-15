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

// web socket 处理类
class WebSocketWatcher : public SocketWatcher
{
public:
	WebSocketWatcher();

	virtual ~WebSocketWatcher();

	virtual int OnEpollAcceptEvent(stSocketContext &socket_context);

	virtual int OnEpollReadableEvent(int &epollfd, epoll_event &event);

	virtual int OnEpollWriteableEvent(stSocketContext &socket_context);

	virtual int OnEpollCloseEvent(stSocketContext &socket_context);

private:
	// handle client message
	bool HandleClientWebSocketData(stSocketContext *socket_context, char clientData[], int dataLength);


	CWebSocketStyle webSocketStyle;

};

// 普通socket 处理类
class NormalSocketWatcher : public SocketWatcher
{
public:
	NormalSocketWatcher();

	virtual ~NormalSocketWatcher();

	virtual int OnEpollAcceptEvent(stSocketContext &socket_context);

	virtual int OnEpollReadableEvent(int &epollfd, epoll_event &event);

	virtual int OnEpollWriteableEvent(stSocketContext &socket_context);

	virtual int OnEpollCloseEvent(stSocketContext &socket_context);

private:
	// handle client message
	bool HandleClientNormalSocketData(stSocketContext *socket_context, char clientData[], int dataLength);

};


class CAcceptServer
{
public:
	CAcceptServer();
	~CAcceptServer();

	bool StartServer();

private:

	// 用于处理 websocket 的客户端的数据
	EpollSocket WebSocketPool;

	// 用于处理  普通socket 的客户端的数据
	EpollSocket NormalSocketPool;
};