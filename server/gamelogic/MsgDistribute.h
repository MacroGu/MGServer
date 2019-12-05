#pragma once

#include "singleton.h"
#include "../base/Interface.h"
#include "Monster.h"


class MsgDistribute : public ISingleton<MsgDistribute>
{
public:
	~MsgDistribute();


public:
	// 分发消息
	void DistributeMsg(int clientFD, std::shared_ptr<stMsgToClient> recvData);



	// -------  for testing 
	static cCharactersInfo	CharactersInfo;	// 保存连接客户端的信息
	static std::set<int> SessionSocket;	// 按会话保存套接字
	static float			HitPoint;		// 鸥拜 单固瘤
	static MonsterSet		MonstersInfo;	// 怪物集信息

	// 注册用户
	static void SignUp(std::stringstream& RecvStream, int clientFD);
	// 登录DB
	static void Login(std::stringstream& RecvStream, int clientFD);
	// 初始字符注册
	static void EnrollCharacter(std::stringstream& RecvStream, int clientFD);
	// 字符位置同步
	static void SyncCharacters(std::stringstream& RecvStream, int clientFD);
	// 处理字符注销
	static void LogoutCharacter(std::stringstream& RecvStream, int clientFD);
	// 攻击角色
	static void HitCharacter(std::stringstream& RecvStream, int clientFD);
	// 广播聊天
	static void BroadcastChat(std::stringstream& RecvStream, int clientFD);
	// 攻击怪物
	static void HitMonster(std::stringstream& RecvStream, int clientFD);

	// 广播
	static void Broadcast(std::stringstream& SendStream);
	// 广播新玩家数据
	static void BroadcastNewPlayer(cCharacter& player);
	// 将角色信息写入socket
	static void WriteCharactersInfoToSocket(std::shared_ptr<stMsgToClient> characterInfo);

	// 初始化怪物信息
	void InitializeMonsterSet();

	void RunGameLogic();

	// protobuf 
	bool message_1001(int getaSock, s_GateHeaders& gateHead, char* data, int len);	//角色进入地图


	// -------   for testing end



private:
	friend ISingleton<MsgDistribute>;
	friend class std::unique_ptr<MsgDistribute>;

	MsgDistribute();

};

