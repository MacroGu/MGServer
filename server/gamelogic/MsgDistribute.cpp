

#include "MsgDistribute.h"
#include "../base/MysqlHandle.h"



std::set<int>	MsgDistribute::SessionSocket;
cCharactersInfo		MsgDistribute::CharactersInfo;
MonsterSet			MsgDistribute::MonstersInfo;
float				MsgDistribute::HitPoint = 0.1f;

MsgDistribute::MsgDistribute()
{
	InitializeMonsterSet();
}

MsgDistribute::~MsgDistribute()
{
}

void MsgDistribute::DistributeMsg(int clientFD, std::shared_ptr<stMsgToClient> recvData)
{
	if (recvData == nullptr)
	{
		return;
	}

	// 包类型
	int PacketType;
	// 反序列化客户端信息
	std::stringstream RecvStream;

	RecvStream << recvData->data;
	RecvStream >> PacketType;

	switch (PacketType)
	{
		case LOGIN:
			{
				MsgDistribute::Login(RecvStream, clientFD);
			}
			break;
		case ENROLL_PLAYER:

			break;
		case SEND_PLAYER:

			break;
		case RECV_PLAYER:

			break;
		case LOGOUT_PLAYER:

			break;
		case HIT_PLAYER:

			break;
		case DAMAGED_PLAYER:

			break;
		case CHAT:

			break;
		case ENTER_NEW_PLAYER:

			break;
		case SIGNUP:
			{
				MsgDistribute::SignUp(RecvStream, clientFD);
			}
			break;
		case HIT_MONSTER:

			break;
		case SYNC_MONSTER:

			break;
		case SPAWN_MONSTER:

			break;
		case DESTROY_MONSTER:

			break;
		default:
			break;
	}

}

void MsgDistribute::SignUp(std::stringstream& RecvStream, int clientFD)
{
	std::string Id;
	std::string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	LOG_INFO("[INFO] 注册客户端 {}/{}\n", Id, Pw);

	std::stringstream SendStream;
	SendStream << EPacketType::SIGNUP << std::endl;
	SendStream << MysqlHandle::GetInstance().SignUpAccount(Id, Pw) << std::endl;

	std::shared_ptr<stMsgToClient> sendData(new stMsgToClient);
	memcpy(sendData->data, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	sendData->dataLen = SendStream.str().length();

	Interface::GetInstance().SendDataToCurClient(sendData);
}

void MsgDistribute::Login(std::stringstream& RecvStream, int clientFD)
{
	std::string Id;
	std::string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	LOG_INFO(" 登录尝试 {}/{}\n", Id, Pw);

	std::stringstream SendStream;
	SendStream << EPacketType::LOGIN << std::endl;
	SendStream << MysqlHandle::GetInstance().SearchAccount(Id, Pw) << std::endl;

	std::shared_ptr<stMsgToClient> sendData(new stMsgToClient);
	memcpy(sendData->data, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	sendData->dataLen = SendStream.str().length();

	Interface::GetInstance().SendDataToCurClient(sendData);
}

void MsgDistribute::EnrollCharacter(std::stringstream& RecvStream, int clientFD)
{
	cCharacter info;
	RecvStream >> info;

	LOG_INFO("[{}] 角色注册 - X : [{}], Y : [{}], Z : [{}], Yaw : [{}], Alive : [{}], Health : [{}]\n",
		info.SessionId, info.X, info.Y, info.Z, info.Yaw, info.IsAlive, info.HealthValue);

	cCharacter* pinfo = &CharactersInfo.players[info.SessionId];

	// 保存角色的位置			
	pinfo->SessionId = info.SessionId;
	pinfo->X = info.X;
	pinfo->Y = info.Y;
	pinfo->Z = info.Z;

	// 保存角色的旋转
	pinfo->Yaw = info.Yaw;
	pinfo->Pitch = info.Pitch;
	pinfo->Roll = info.Roll;

	// 保存角色的速度
	pinfo->VX = info.VX;
	pinfo->VY = info.VY;
	pinfo->VZ = info.VZ;

	// 角色属性
	pinfo->IsAlive = info.IsAlive;
	pinfo->HealthValue = info.HealthValue;
	pinfo->IsAttacking = info.IsAttacking;

	SessionSocket.insert(clientFD);

	LOG_INFO(" 客户端数量 : {}\n", SessionSocket.size());

	//Send(pSocket);
	BroadcastNewPlayer(info);
}

void MsgDistribute::SyncCharacters(std::stringstream& RecvStream, int clientFD)
{
	cCharacter info;
	RecvStream >> info;


	cCharacter* pinfo = &CharactersInfo.players[info.SessionId];
	pinfo->SessionId = info.SessionId;
	pinfo->X = info.X;
	pinfo->Y = info.Y;
	pinfo->Z = info.Z;

	// 保存角色的旋转
	pinfo->Yaw = info.Yaw;
	pinfo->Pitch = info.Pitch;
	pinfo->Roll = info.Roll;

	// 保存角色的速度
	pinfo->VX = info.VX;
	pinfo->VY = info.VY;
	pinfo->VZ = info.VZ;

	pinfo->IsAttacking = info.IsAttacking;


// 	WriteCharactersInfoToSocket(pSocket);
// 	Interface::GetInstance().SendDataToCurClient(sendData);
}

void MsgDistribute::LogoutCharacter(std::stringstream& RecvStream, int clientFD)
{

}

void MsgDistribute::HitCharacter(std::stringstream& RecvStream, int clientFD)
{

}

void MsgDistribute::BroadcastChat(std::stringstream& RecvStream, int clientFD)
{

}

void MsgDistribute::HitMonster(std::stringstream& RecvStream, int clientFD)
{

}

void MsgDistribute::Broadcast(std::stringstream& SendStream)
{
	for (auto oneClient : SessionSocket)
	{
		std::shared_ptr<stMsgToClient> sendData(new stMsgToClient);
		memcpy(sendData->data, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		sendData->dataLen = SendStream.str().length();

		Interface::GetInstance().SendDataToCurClient(sendData);
	}
}

void MsgDistribute::BroadcastNewPlayer(cCharacter& player)
{

}

void MsgDistribute::WriteCharactersInfoToSocket(int clientFD)
{

}

void MsgDistribute::InitializeMonsterSet()
{
	// 怪物数据初始化
	Monster mFields;

	mFields.X = -5746;
	mFields.Y = 3736;
	mFields.Z = 7362;
	mFields.Health = 100.0f;
	mFields.Id = 1;
	mFields.MovePoint = 10.f;
	MonstersInfo.monsters[mFields.Id] = mFields;

	mFields.X = -5136;
	mFields.Y = 1026;
	mFields.Z = 7712;
	mFields.Id = 2;
	MonstersInfo.monsters[mFields.Id] = mFields;

	mFields.X = -3266;
	mFields.Y = 286;
	mFields.Z = 8232;
	mFields.Id = 3;
	MonstersInfo.monsters[mFields.Id] = mFields;

	mFields.X = -156;
	mFields.Y = 326;
	mFields.Z = 8352;
	mFields.Id = 4;
	MonstersInfo.monsters[mFields.Id] = mFields;
}

void MsgDistribute::RunGameLogic()
{
	int count = 0;

	LOG_INFO("CALL MsgDistribute::RunGameLogic");
	for (auto& kvp : MonstersInfo.monsters)
	{
		auto& monster = kvp.second;
		for (auto& player : CharactersInfo.players)
		{
			// 当玩家或怪物死亡时忽略
			if (!player.second.IsAlive || !monster.IsAlive())
				continue;

			if (monster.IsPlayerInHitRange(player.second) && !monster.bIsAttacking)
			{
				monster.HitPlayer(player.second);
				continue;
			}

			if (monster.IsPlayerInTraceRange(player.second) && !monster.bIsAttacking)
			{
				monster.MoveTo(player.second);
				continue;
			}
		}
	}

	count++;
	// 0.5 每秒向客户发送怪物信息
	if (count > 15)
	{
		std::stringstream SendStream;
		SendStream << EPacketType::SYNC_MONSTER << std::endl;
		SendStream << MonstersInfo << std::endl;

		count = 0;
		Broadcast(SendStream);
	}

}
