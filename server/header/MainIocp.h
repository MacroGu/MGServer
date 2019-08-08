#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <map>
#include <vector>
#include <iostream>
#include <mutex>
#include "CommonClass.h"
#include "IocpBase.h"
#include "Monster.h"

#define DB_ADDRESS		"localhost"
#define	DB_PORT			3306
#define DB_ID			"root"
#define DB_PW			"anfrhrl"
#define DB_SCHEMA		"sungminworld"

class MainIocp : public IocpBase
{
public:
	MainIocp();
	virtual ~MainIocp();

	virtual void StartServer() override;
	// 创建工作线程
	virtual bool CreateWorkerThread() override;
	// 任务线程
	virtual void WorkerThread() override;
	// 发送给客户
	static void Send(stSOCKETINFO* pSocket);

	// 怪物线程
	void CreateMonsterManagementThread();
	void MonsterManagementThread();

private:
	static cCharactersInfo	CharactersInfo;	// 保存连接客户端的信息
	static std::map<int, SOCKET> SessionSocket;	// 按会话保存套接字
	static float			HitPoint;		// 击中伤害

	FuncProcess				fnProcess[100];	// 包处理结构
	HANDLE* MonsterHandle;	// 怪物线程处理程序
	static MonsterSet		MonstersInfo;	// 怪物集信息

	// 注册用户
	static void SignUp(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 登录DB
	static void Login(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 初始字符注册
	static void EnrollCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 字符位置同步
	static void SyncCharacters(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 处理字符注销
	static void LogoutCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 攻击角色
	static void HitCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 广播聊天
	static void BroadcastChat(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// 攻击怪物
	static void HitMonster(std::stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 广播
	static void Broadcast(std::stringstream& SendStream);
	// 广播新玩家数据
	static void BroadcastNewPlayer(cCharacter& player);
	// 将角色信息写入socket
	static void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);

	// 初始化怪物信息
	void InitializeMonsterSet();
};