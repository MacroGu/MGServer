#pragma once

#include "singleton.h"
#include "../base/Interface.h"
#include "Monster.h"


class MsgDistribute : public ISingleton<MsgDistribute>
{
public:
	~MsgDistribute();


public:
	// �ַ���Ϣ
	void DistributeMsg(int clientFD, std::shared_ptr<stMsgToClient> recvData);



	// -------  for testing 
	static cCharactersInfo	CharactersInfo;	// �������ӿͻ��˵���Ϣ
	static std::set<int> SessionSocket;	// ���Ự�����׽���
	static float			HitPoint;		// Ÿ�� ������
	static MonsterSet		MonstersInfo;	// ���Ｏ��Ϣ

	// ע���û�
	static void SignUp(std::stringstream& RecvStream, int clientFD);
	// ��¼DB
	static void Login(std::stringstream& RecvStream, int clientFD);
	// ��ʼ�ַ�ע��
	static void EnrollCharacter(std::stringstream& RecvStream, int clientFD);
	// �ַ�λ��ͬ��
	static void SyncCharacters(std::stringstream& RecvStream, int clientFD);
	// �����ַ�ע��
	static void LogoutCharacter(std::stringstream& RecvStream, int clientFD);
	// ������ɫ
	static void HitCharacter(std::stringstream& RecvStream, int clientFD);
	// �㲥����
	static void BroadcastChat(std::stringstream& RecvStream, int clientFD);
	// ��������
	static void HitMonster(std::stringstream& RecvStream, int clientFD);

	// �㲥
	static void Broadcast(std::stringstream& SendStream);
	// �㲥���������
	static void BroadcastNewPlayer(cCharacter& player);
	// ����ɫ��Ϣд��socket
	static void WriteCharactersInfoToSocket(std::shared_ptr<stMsgToClient> characterInfo);

	// ��ʼ��������Ϣ
	void InitializeMonsterSet();

	void RunGameLogic();

	// protobuf 
	bool message_1001(int getaSock, s_GateHeaders& gateHead, char* data, int len);	//��ɫ�����ͼ


	// -------   for testing end



private:
	friend ISingleton<MsgDistribute>;
	friend class std::unique_ptr<MsgDistribute>;

	MsgDistribute();

};

