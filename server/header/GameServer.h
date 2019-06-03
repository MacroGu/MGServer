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

// socket ������
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


	// �����Ҫ����websocket Э�飬 �ô�ʵ��
	CWebSocketStyle webSocketStyle;

};


class CGameServer
{
public:
	CGameServer();
	~CGameServer();

	bool StartServer();

private:

	// ���ڴ���  ��ͨsocket �Ŀͻ��˵�����
	EpollSocket SocketPool;
};