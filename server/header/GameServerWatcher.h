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


// ��������ָ��
struct FuncProcess
{
	void(*funcProcessPacket)(std::stringstream& RecvStream, stSOCKETINFO* pSocket);
	FuncProcess()
	{
		funcProcessPacket = nullptr;
	}
};

// socket ������
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

	cCharactersInfo	CharactersInfo;	// �������ӿͻ��˵���Ϣ
	std::map<int, SOCKET> SessionSocket;	// ���Ự�����׽���
	float			HitPoint;		// �����˺�

	FuncProcess				fnProcess[100];	// ������ṹ
	HANDLE*			MonsterHandle;	// �����̴߳������
	MonsterSet		MonstersInfo;	// ���Ｏ��Ϣ


	// �ͻ������ݴ�����
private:
	// ע���û�
	static void SignUp(std::stringstream& RecvStream, stSocketContext* pSocket);
	// ��¼DB
	static void Login(std::stringstream& RecvStream, stSocketContext* pSocket);
	// ��ʼ�ַ�ע��
	static void EnrollCharacter(std::stringstream& RecvStream, stSocketContext* pSocket);
	// �ַ�λ��ͬ��
	static void SyncCharacters(std::stringstream& RecvStream, stSocketContext* pSocket);
	// �����ַ�ע��
	static void LogoutCharacter(std::stringstream& RecvStream, stSocketContext* pSocket);
	// ������ɫ
	static void HitCharacter(std::stringstream& RecvStream, stSocketContext* pSocket);
	// �㲥����
	static void BroadcastChat(std::stringstream& RecvStream, stSocketContext* pSocket);
	// ��������
	static void HitMonster(std::stringstream& RecvStream, stSocketContext* pSocket);

	// �㲥
	static void Broadcast(std::stringstream& SendStream);
	// �㲥���������
	static void BroadcastNewPlayer(cCharacter& player);
	// ����ɫ��Ϣд��socket
	static void WriteCharactersInfoToSocket(stSocketContext* pSocket);

};
