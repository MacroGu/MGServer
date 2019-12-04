/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
	des: 用于与游戏逻辑模块交互
*/

#pragma once

#include <set>
#include "GameSocketWatcher.h"

class EpollSocket;


class Interface
{
public:
	Interface();
	~Interface();


public:
	// 新的客户端连接
	int NewClientAcceptCallBack(int ClientFD);
	// 接收到客户端的数据
	int RecvClientDataCallBack(int ClientFD, std::shared_ptr<stMsgToClient> recvData);
	// 客户端断开连接
	int DissconnectClientCallBack(int ClientFD);
	// 发生消息给指定客户端 （非当前消息来的客户端）
	int SendDataToClient(int ClientFD, std::shared_ptr<stMsgToClient> sendData);
	// 发生消息给当前客户端 （当前消息来的客户端）
	void SendDataToCurClient(std::shared_ptr<stMsgToClient> sendData);

public:
	void SetSocketPool(EpollSocket* SocketPool);
	void SetGameSocketWatcher(GameSocketWatcher* Watcher);

private:
	EpollSocket* SocketPool;
	GameSocketWatcher* Watcher;


private:
	std::set<int> allClients;
};

extern Interface gInterface;