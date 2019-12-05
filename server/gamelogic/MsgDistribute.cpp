

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

	// ������
	int PacketType;
	// �����л��ͻ�����Ϣ
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
			{
				MsgDistribute::EnrollCharacter(RecvStream, clientFD);
			}
			break;
		case SEND_PLAYER:
			{
				MsgDistribute::SyncCharacters(RecvStream, clientFD);
			}
			break;
		case LOGOUT_PLAYER:
			{
				MsgDistribute::LogoutCharacter(RecvStream, clientFD);
			}
			break;
		case HIT_PLAYER:
			{
				MsgDistribute::HitCharacter(RecvStream, clientFD);
			}
			break;
		case CHAT:
			{
				MsgDistribute::BroadcastChat(RecvStream, clientFD);
			}
			break;
		case SIGNUP:
			{
				MsgDistribute::SignUp(RecvStream, clientFD);
			}
			break;
		case HIT_MONSTER:
			{
				MsgDistribute::HitMonster(RecvStream, clientFD);
			}
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

	LOG_INFO("[INFO] ע��ͻ��� {}/{}\n", Id, Pw);

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

	LOG_INFO(" ��¼���� {}/{}\n", Id, Pw);

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

	LOG_INFO("[{}] ��ɫע�� - X : [{}], Y : [{}], Z : [{}], Yaw : [{}], Alive : [{}], Health : [{}]\n",
		info.SessionId, info.X, info.Y, info.Z, info.Yaw, info.IsAlive, info.HealthValue);

	cCharacter* pinfo = &CharactersInfo.players[info.SessionId];

	// �����ɫ��λ��			
	pinfo->SessionId = info.SessionId;
	pinfo->X = info.X;
	pinfo->Y = info.Y;
	pinfo->Z = info.Z;

	// �����ɫ����ת
	pinfo->Yaw = info.Yaw;
	pinfo->Pitch = info.Pitch;
	pinfo->Roll = info.Roll;

	// �����ɫ���ٶ�
	pinfo->VX = info.VX;
	pinfo->VY = info.VY;
	pinfo->VZ = info.VZ;

	// ��ɫ����
	pinfo->IsAlive = info.IsAlive;
	pinfo->HealthValue = info.HealthValue;
	pinfo->IsAttacking = info.IsAttacking;

	SessionSocket.insert(clientFD);

	LOG_INFO(" �ͻ������� : {}\n", SessionSocket.size());

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

	// �����ɫ����ת
	pinfo->Yaw = info.Yaw;
	pinfo->Pitch = info.Pitch;
	pinfo->Roll = info.Roll;

	// �����ɫ���ٶ�
	pinfo->VX = info.VX;
	pinfo->VY = info.VY;
	pinfo->VZ = info.VZ;

	pinfo->IsAttacking = info.IsAttacking;
	std::shared_ptr<stMsgToClient> sendData(new stMsgToClient);

	WriteCharactersInfoToSocket(sendData);
	Interface::GetInstance().SendDataToCurClient(sendData);
}

void MsgDistribute::LogoutCharacter(std::stringstream& RecvStream, int clientFD)
{
	int SessionId;
	RecvStream >> SessionId;
	 LOG_INFO("({})����ע������\n", SessionId);
	CharactersInfo.players[SessionId].IsAlive = false;
	SessionSocket.erase(clientFD);
	LOG_INFO("�ͻ����� : %d\n", SessionSocket.size());
	std::shared_ptr<stMsgToClient> sendData(new stMsgToClient);

	WriteCharactersInfoToSocket(sendData);
	// Interface::GetInstance().SendDataToCurClient(sendData);

}

void MsgDistribute::HitCharacter(std::stringstream& RecvStream, int clientFD)
{
	// ����ỰID
	int DamagedSessionId;
	RecvStream >> DamagedSessionId;
	LOG_INFO(" %d �յ����˺� \n", DamagedSessionId);
	CharactersInfo.players[DamagedSessionId].HealthValue -= HitPoint;
	if (CharactersInfo.players[DamagedSessionId].HealthValue < 0)
	{
		// �Ը���������
		CharactersInfo.players[DamagedSessionId].IsAlive = false;
	}

	std::shared_ptr<stMsgToClient> sendData(new stMsgToClient);

	WriteCharactersInfoToSocket(sendData);
	Interface::GetInstance().SendDataToCurClient(sendData);
}

void MsgDistribute::BroadcastChat(std::stringstream& RecvStream, int clientFD)
{
	int SessionId;
	std::string Temp;
	std::string Chat;

	RecvStream >> SessionId;
	getline(RecvStream, Temp);
	Chat += std::to_string(SessionId) + "_:_";
	while (RecvStream >> Temp)
	{
		Chat += Temp + "_";
	}
	Chat += '\0';

	LOG_INFO("[CHAT] {}\n", Chat);

	std::stringstream SendStream;
	SendStream << EPacketType::CHAT << std::endl;
	SendStream << Chat;

	Broadcast(SendStream);
}

void MsgDistribute::HitMonster(std::stringstream& RecvStream, int clientFD)
{
	// �������д���
	int MonsterId;
	RecvStream >> MonsterId;
	MonstersInfo.monsters[MonsterId].Damaged(30.f);

	if (!MonstersInfo.monsters[MonsterId].IsAlive())
	{
		std::stringstream SendStream;
		SendStream << EPacketType::DESTROY_MONSTER << std::endl;
		SendStream << MonstersInfo.monsters[MonsterId] << std::endl;

		Broadcast(SendStream);

		MonstersInfo.monsters.erase(MonsterId);
	}
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
	std::stringstream SendStream;
	SendStream << EPacketType::ENTER_NEW_PLAYER << std::endl;
	SendStream << player << std::endl;

	Broadcast(SendStream);
}

void MsgDistribute::WriteCharactersInfoToSocket(std::shared_ptr<stMsgToClient> characterInfo)
{
	std::stringstream SendStream;

	// ���л�
	SendStream << EPacketType::RECV_PLAYER << std::endl;
	SendStream << CharactersInfo << std::endl;

	// �� ��Ҫ!!! ������ֱ��д��data.buf���Է�������
	memcpy(characterInfo->data, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	characterInfo->dataLen = SendStream.str().length();
}

void MsgDistribute::InitializeMonsterSet()
{
	// �������ݳ�ʼ��
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
			// ����һ��������ʱ����
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
	// 0.5 ÿ����ͻ����͹�����Ϣ
	if (count > 15)
	{
		std::stringstream SendStream;
		SendStream << EPacketType::SYNC_MONSTER << std::endl;
		SendStream << MonstersInfo << std::endl;

		count = 0;
		Broadcast(SendStream);
	}

}
