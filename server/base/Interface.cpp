/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include "Interface.h"
#include "GameSocketWatcher.h"
#include "EpollSocket.h"
#include "defines.h"
#include "../gamelogic/MsgDistribute.h"



Interface::Interface()
{
	SocketPool = nullptr;
	Watcher = nullptr;
}

Interface::~Interface()
{

}

int Interface::NewClientAcceptCallBack(int ClientFD)
{
	allClients.insert(ClientFD);

	return 0;
}

int Interface::RecvClientDataCallBack(int ClientFD, std::shared_ptr<stMsgToClient> recvData)
{

	// LOG_INFO("recv data: {} ", recvData->data);

	// 分发消息
	MsgDistribute::GetInstance().DistributeMsg(ClientFD, recvData);

	return 0;
}

int Interface::DissconnectClientCallBack(int ClientFD)
{
	allClients.erase(ClientFD);

	return 0;
}

int Interface::SendDataToClient(int ClientFD, std::shared_ptr<stMsgToClient> sendData)
{
	if (!Watcher)
	{
		LOG_ERROR("watcher is nullptr");
		return 1;
	}

	return Watcher->SentDataToOneClient(ClientFD, sendData);
}

void Interface::SendDataToCurClient(std::shared_ptr<stMsgToClient> sendData)
{
	if (!Watcher)
	{
		LOG_ERROR("watcher is nullptr");
		return;
	}

	Watcher->SendDataToCurClient(sendData);
}

void Interface::GameLogicLoop()
{
	LOG_INFO("CALL Interface::GameLogicLoop");
	MsgDistribute::GetInstance().RunGameLogic();
}

void Interface::SetSocketPool(EpollSocket* mSocketPool)
{
	if (!mSocketPool)
	{
		LOG_ERROR("mSocketPool is nullptr!");
		return;
	}

	if (this->SocketPool)
	{
		LOG_ERROR("SocketPool is nullptr!");
		return;
	}

	this->SocketPool = mSocketPool;
}

void Interface::SetGameSocketWatcher(GameSocketWatcher* mWatcher)
{
	if (!mWatcher)
	{
		LOG_ERROR("mWatcher is nullptr!");
		return;
	}

	if (this->Watcher)
	{
		LOG_ERROR("Watcher is nullptr!");
		return;
	}

	this->Watcher = mWatcher;
}
