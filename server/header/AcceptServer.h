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

// web socket ������
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

// ��ͨsocket ������
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


class CGameServer
{
public:
	CGameServer();
	~CGameServer();

	bool StartServer();

private:

	// ���ڴ��� websocket �Ŀͻ��˵�����
	EpollSocket WebSocketPool;

	// ���ڴ���  ��ͨsocket �Ŀͻ��˵�����
	EpollSocket NormalSocketPool;
};