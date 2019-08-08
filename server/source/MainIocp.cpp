#include <sstream>
#include <algorithm>
#include <process.h>
#include <string>
#include "MainIocp.h"
#include "MysqlHandle.h"

// static ������ʼ��
float				MainIocp::HitPoint = 0.1f;
std::map<int, SOCKET>	MainIocp::SessionSocket;
cCharactersInfo		MainIocp::CharactersInfo;
MonsterSet			MainIocp::MonstersInfo;
std::mutex g_mutex;

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	MainIocp* pOverlappedEvent = (MainIocp*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

unsigned int WINAPI CallMonsterThread(LPVOID p)
{
	MainIocp* pOverlappedEvent = (MainIocp*)p;
	pOverlappedEvent->MonsterManagementThread();
	return 0;
}

MainIocp::MainIocp()
{

	if (!MysqlHandle::GetInstance().Init())
	{
		LOG_ERROR("init Mysql failed!");
	}

	// Ϊ���鹦��ָ����书��
	fnProcess[EPacketType::SIGNUP].funcProcessPacket = SignUp;
	fnProcess[EPacketType::LOGIN].funcProcessPacket = Login;
	fnProcess[EPacketType::ENROLL_PLAYER].funcProcessPacket = EnrollCharacter;
	fnProcess[EPacketType::SEND_PLAYER].funcProcessPacket = SyncCharacters;
	fnProcess[EPacketType::HIT_PLAYER].funcProcessPacket = HitCharacter;
	fnProcess[EPacketType::CHAT].funcProcessPacket = BroadcastChat;
	fnProcess[EPacketType::LOGOUT_PLAYER].funcProcessPacket = LogoutCharacter;
	fnProcess[EPacketType::HIT_MONSTER].funcProcessPacket = HitMonster;
}


MainIocp::~MainIocp()
{
	// winsock ����ʹ��
	WSACleanup();
	// ɾ���ù��Ķ���
	if (SocketInfo)
	{
		delete[] SocketInfo;
		SocketInfo = NULL;
	}

	if (hWorkerHandle)
	{
		delete[] hWorkerHandle;
		hWorkerHandle = NULL;
	}
}

void MainIocp::StartServer()
{
	CreateMonsterManagementThread();
	IocpBase::StartServer();
}

bool MainIocp::CreateWorkerThread()
{
	unsigned int threadId;
	// ��ȡϵͳ��Ϣ
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	LOG_INFO("[INFO] CPU nums : {}\n", sysInfo.dwNumberOfProcessors);
	// �ʵ������Ĺ����߳� (CPU * 2) + 1
	nThreadCnt = sysInfo.dwNumberOfProcessors * 2;

	// thread handler ����
	hWorkerHandle = new HANDLE[nThreadCnt];
	for (int i = 0; i < nThreadCnt; i++)
	{
		hWorkerHandle[i] = (HANDLE*)_beginthreadex(
			NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadId
		);
		if (hWorkerHandle[i] == NULL)
		{
			LOG_ERROR("[ERROR] Worker Thread ����ʧ��\n");
			return false;
		}
		ResumeThread(hWorkerHandle[i]);
	}
	LOG_INFO("[INFO] Worker Thread �����ɹ�...\n");
	return true;
}

void MainIocp::Send(stSOCKETINFO* pSocket)
{
	int nResult;
	DWORD	sendBytes;
	DWORD	dwFlags = 0;

	nResult = WSASend(
		pSocket->socket,
		&(pSocket->dataBuf),
		1,
		&sendBytes,
		dwFlags,
		NULL,
		NULL
	);

	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		LOG_ERROR("[ERROR] WSASend ʧ�� : {}", WSAGetLastError());
	}


}

void MainIocp::CreateMonsterManagementThread()
{
	unsigned int threadId;

	MonsterHandle = (HANDLE*)_beginthreadex(
		NULL, 0, &CallMonsterThread, this, CREATE_SUSPENDED, &threadId
	);
	if (MonsterHandle == NULL)
	{
		LOG_ERROR("[ERROR] Monster Thread ����ʧ�� \n");
		return;
	}
	ResumeThread(MonsterHandle);

	LOG_INFO("[INFO] Monster Thread �ɹ�...\n");
}

void MainIocp::MonsterManagementThread()
{
	// ��ʼ������
	InitializeMonsterSet();
	int count = 0;
	// ��Ϸ�߼���ʼ
	while (true)
	{
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

		Sleep(33);
	}
}

void MainIocp::InitializeMonsterSet()
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

void MainIocp::WorkerThread()
{
	// ���ܵ��óɹ�
	BOOL	bResult;
	int		nResult;
	// Overlapped I/O �����������ݴ�С
	DWORD	recvBytes;
	DWORD	sendBytes;
	// Completion Key Ҫ���յ�ָ�����
	stSOCKETINFO* pCompletionKey;
	// I/O ������socket ָ��  
	stSOCKETINFO* pSocketInfo;
	DWORD	dwFlags = 0;


	while (bWorkerThread)
	{
		/**
		 * �˺��������߳���WaitingThread�����ϵȴ���
		 * ������ɵ��ص�I / O����ʱ����IOCP�����м�������ɵĲ���
		 * �����ڴ���
		 */
		bResult = GetQueuedCompletionStatus(hIOCP,
			&recvBytes,				// �ֽ�ʵ����ת����
			(PULONG_PTR)& pCompletionKey,	// completion key
			(LPOVERLAPPED*)& pSocketInfo,			// overlapped I/O ����
			INFINITE				// �ȴ�ʱ��
		);

		if (!bResult && recvBytes == 0)
		{
			LOG_INFO("[INFO] socket {} �Ͽ��� \n", pSocketInfo->socket);
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		pSocketInfo->dataBuf.len = recvBytes;

		if (recvBytes == 0)
		{
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		try
		{
			// ������
			int PacketType;
			// �����л��ͻ�����Ϣ
			std::stringstream RecvStream;

			RecvStream << pSocketInfo->dataBuf.buf;
			RecvStream >> PacketType;

			// ������
			if (fnProcess[PacketType].funcProcessPacket != nullptr)
			{
				fnProcess[PacketType].funcProcessPacket(RecvStream, pSocketInfo);
			}
			else
			{
				LOG_ERROR("[ERROR] δ��������ݰ� : {}\n", PacketType);
			}
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("[ERROR] δ֪������ : {}\n", e.what());
		}

		// �ͻ��ȴ�
		Recv(pSocketInfo);
	}
}

void MainIocp::SignUp(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	std::string Id;
	std::string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	LOG_INFO("[INFO] ע��ͻ��� {}/{}\n", Id, Pw);

	std::stringstream SendStream;
	SendStream << EPacketType::SIGNUP << std::endl;
	SendStream << MysqlHandle::GetInstance().SignUpAccount(Id, Pw) << std::endl;

	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

	Send(pSocket);
}

void MainIocp::Login(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	std::string Id;
	std::string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	LOG_INFO("[INFO] ��¼���� {}/{}\n", Id, Pw);

	std::stringstream SendStream;
	SendStream << EPacketType::LOGIN << std::endl;
	SendStream << MysqlHandle::GetInstance().SearchAccount(Id, Pw) << std::endl;

	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

	Send(pSocket);
}

void MainIocp::EnrollCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	cCharacter info;
	RecvStream >> info;

	LOG_INFO("[INFO][{}] ��ɫע�� - X : [{}], Y : [{}], Z : [{}], Yaw : [{}], Alive : [{}], Health : [{}]\n",
		info.SessionId, info.X, info.Y, info.Z, info.Yaw, info.IsAlive, info.HealthValue);

	g_mutex.lock();

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

	g_mutex.unlock();
	SessionSocket[info.SessionId] = pSocket->socket;

	LOG_INFO("[INFO] �ͻ������� : {}\n", SessionSocket.size());

	//Send(pSocket);
	BroadcastNewPlayer(info);
}

void MainIocp::SyncCharacters(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	cCharacter info;
	RecvStream >> info;

	// 	 	LOG_INFO("[INFO][{}] ������Ϣ - {}\n",
	// 	 		info.SessionId, info.IsAttacking);	
	g_mutex.lock();
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

	pinfo->IsAttacking = info.IsAttacking;

	g_mutex.unlock();
	WriteCharactersInfoToSocket(pSocket);
	Send(pSocket);
}

void MainIocp::LogoutCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	int SessionId;
	RecvStream >> SessionId;
	LOG_INFO("[INFO] ({})����ע������\n", SessionId);
	g_mutex.lock();
	CharactersInfo.players[SessionId].IsAlive = false;
	g_mutex.unlock();
	SessionSocket.erase(SessionId);
	LOG_INFO("[INFO] �ͻ����� : %d\n", SessionSocket.size());
	WriteCharactersInfoToSocket(pSocket);
}

void MainIocp::HitCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	// ����ỰID
	int DamagedSessionId;
	RecvStream >> DamagedSessionId;
	LOG_INFO("[INFO] %d �յ����˺� \n", DamagedSessionId);
	g_mutex.lock();
	CharactersInfo.players[DamagedSessionId].HealthValue -= HitPoint;
	if (CharactersInfo.players[DamagedSessionId].HealthValue < 0)
	{
		// �Ը���������
		CharactersInfo.players[DamagedSessionId].IsAlive = false;
	}
	g_mutex.unlock();
	WriteCharactersInfoToSocket(pSocket);
	Send(pSocket);
}

void MainIocp::BroadcastChat(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	stSOCKETINFO* client = new stSOCKETINFO;

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

void MainIocp::HitMonster(std::stringstream& RecvStream, stSOCKETINFO* pSocket)
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

	// ��������ҹ㲥
	/*std::stringstream SendStream;
	SendStream << EPacketType::HIT_MONSTER << std::endl;
	SendStream << MonstersInfo << std::endl;
	Broadcast(SendStream);*/
}

void MainIocp::BroadcastNewPlayer(cCharacter& player)
{
	std::stringstream SendStream;
	SendStream << EPacketType::ENTER_NEW_PLAYER << std::endl;
	SendStream << player << std::endl;

	Broadcast(SendStream);
}

void MainIocp::Broadcast(std::stringstream& SendStream)
{
	stSOCKETINFO* client = new stSOCKETINFO;
	for (const auto& kvp : SessionSocket)
	{
		client->socket = kvp.second;
		CopyMemory(client->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		client->dataBuf.buf = client->messageBuffer;
		client->dataBuf.len = SendStream.str().length();

		Send(client);
	}
}

void MainIocp::WriteCharactersInfoToSocket(stSOCKETINFO* pSocket)
{
	std::stringstream SendStream;

	// ���л�
	SendStream << EPacketType::RECV_PLAYER << std::endl;
	SendStream << CharactersInfo << std::endl;

	// �� ��Ҫ!!! ������ֱ��д��data.buf���Է�������
	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();
}