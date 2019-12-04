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
	char data[SS_WRITE_BUFFER_SIZE];
	int dataLen;

	stMsgToClient()
	{
		memset(data, 0, SS_WRITE_BUFFER_SIZE);
		dataLen = 0;
	}

	~stMsgToClient()
	{
		memset(data, 0, SS_WRITE_BUFFER_SIZE);
		dataLen = 0;
	}
};

// socket ������
class GameSocketWatcher : public BaseSocketWatcher
{

public:
	GameSocketWatcher();

	virtual ~GameSocketWatcher();

	virtual int OnEpollAcceptEvent(stSocketContext& socket_context) override;

	virtual int OnEpollReadableEvent(int& epollfd, epoll_event& event) override;

	virtual int OnEpollWriteableEvent(stSocketContext& socket_context) override;

	virtual int OnEpollCloseEvent(stSocketContext& socket_context) override;


public:
	int SentDataToOneClient(int ClientFD, std::shared_ptr<stMsgToClient> sendData);
	void SendDataToCurClient(std::shared_ptr<stMsgToClient> toCurClient);


private:
	// handle client message
	bool HandleClientNormalSocketData(stSocketContext* socket_context, char clientData[], int dataLength);


private:
	std::shared_ptr<stMsgToClient> toCurClientData;  // ��ǰ�Ŀͻ��� �����ݣ�Ч�ʸ�
	stSocketContext* curClient;						 // ��ǰ������Ϣ�� ��Ӧ�Ŀͻ���
	std::map<int, stSocketContext> allClients;		 // ���еĿͻ���
};