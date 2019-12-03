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

// socket ������
class GameSocketWatcher : public BaseSocketWatcher
{
public:
	// һЩ��¶����Ϸ�߼��Ľӿ�
	// �㲥 1 �� �ɿͻ����յ���Ϣ�󣬾��������ٽ��й㲥��

	// �㲥 2 ���������������¼������綨ʱ�¼����������㲥


	// ���͸�ָ���ͻ���

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
	std::map<int, stMsgToClient> allClients;		// ���еĿͻ���
	stMsgToClient curClient;		// ��ǰ�Ŀͻ���
};