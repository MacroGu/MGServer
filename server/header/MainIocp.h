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
	// ���������߳�
	virtual bool CreateWorkerThread() override;
	// �����߳�
	virtual void WorkerThread() override;
	// ���͸��ͻ�
	static void Send(stSOCKETINFO* pSocket);

	// �����߳�
	void CreateMonsterManagementThread();
	void MonsterManagementThread();

private:
	static cCharactersInfo	CharactersInfo;	// �������ӿͻ��˵���Ϣ
	static std::map<int, SOCKET> SessionSocket;	// ���Ự�����׽���
	static float			HitPoint;		// �����˺�

	FuncProcess				fnProcess[100];	// ������ṹ
	HANDLE* MonsterHandle;	// �����̴߳������
	static MonsterSet		MonstersInfo;	// ���Ｏ��Ϣ

	// ע���û�
	static void SignUp(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ��¼DB
	static void Login(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ��ʼ�ַ�ע��
	static void EnrollCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// �ַ�λ��ͬ��
	static void SyncCharacters(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// �����ַ�ע��
	static void LogoutCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ������ɫ
	static void HitCharacter(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// �㲥����
	static void BroadcastChat(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ��������
	static void HitMonster(std::stringstream& RecvStream, stSOCKETINFO* pSocket);

	// �㲥
	static void Broadcast(std::stringstream& SendStream);
	// �㲥���������
	static void BroadcastNewPlayer(cCharacter& player);
	// ����ɫ��Ϣд��socket
	static void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);

	// ��ʼ��������Ϣ
	void InitializeMonsterSet();
};