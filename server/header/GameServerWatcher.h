#pragma once
/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include <thread>
#include <sstream>
#include <map>
#include <set>
#include <stdint.h>
#include <iostream>
#include "EpollSocket.h"
#include "CommonClass.h"
#include "Monster.h"


// 包处理函数指针
struct FuncProcess
{
	void(*funcProcessPacket)(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	FuncProcess()
	{
		funcProcessPacket = nullptr;
	}
};

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

	cCharactersInfo	CharactersInfo;	// 保存连接客户端的信息
	std::map<int, SOCKET> SessionSocket;	// 按会话保存套接字
	float			HitPoint;		// 击中伤害

	FuncProcess				fnProcess[100];	// 包处理结构
	HANDLE*			MonsterHandle;	// 怪物线程处理程序
	MonsterSet		MonstersInfo;	// 怪物集信息


	// 客户端数据处理函数
private:
	// 注册用户
	static void SignUp(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 登录DB
	static void Login(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 初始字符注册
	static void EnrollCharacter(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 字符位置同步
	static void SyncCharacters(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 处理字符注销
	static void LogoutCharacter(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 攻击角色
	static void HitCharacter(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 广播聊天
	static void BroadcastChat(std::stringstream& RecvStream, stSocketContext* pSocket);
	// 攻击怪物
	static void HitMonster(std::stringstream& RecvStream, stSocketContext* pSocket);

	// 广播
	static void Broadcast(std::stringstream& SendStream);
	// 广播新玩家数据
	static void BroadcastNewPlayer(cCharacter& player);
	// 将角色信息写入socket
	static void WriteCharactersInfoToSocket(stSocketContext* pSocket);

};
