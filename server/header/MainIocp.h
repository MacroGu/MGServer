#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <map>
#include <vector>
#include <iostream>
#include "CommonClass.h"
#include "IocpBase.h"
#include "Monster.h"

using namespace std;

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
	static map<int, SOCKET> SessionSocket;	// ���Ự�����׽���
	static float			HitPoint;		// �����˺�
	static CRITICAL_SECTION	csPlayers;		// CharactersInfo �ؼ�����

	FuncProcess				fnProcess[100];	// ������ṹ
	HANDLE* MonsterHandle;	// �����̴߳������
	static MonsterSet		MonstersInfo;	// ���Ｏ��Ϣ

	// ע���û�
	static void SignUp(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ��¼DB
	static void Login(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ��ʼ�ַ�ע��
	static void EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// �ַ�λ��ͬ��
	static void SyncCharacters(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// �����ַ�ע��
	static void LogoutCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ������ɫ
	static void HitCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// �㲥����
	static void BroadcastChat(stringstream& RecvStream, stSOCKETINFO* pSocket);
	// ��������
	static void HitMonster(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// �㲥
	static void Broadcast(stringstream& SendStream);
	// �㲥���������
	static void BroadcastNewPlayer(cCharacter& player);
	// ����ɫ��Ϣд��socket
	static void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);

	// ��ʼ��������Ϣ
	void InitializeMonsterSet();
};