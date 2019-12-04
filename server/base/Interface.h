/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
	des: ��������Ϸ�߼�ģ�齻��
*/

#pragma once

#include <set>
#include "GameSocketWatcher.h"

class EpollSocket;


class Interface
{
public:
	Interface();
	~Interface();


public:
	// �µĿͻ�������
	int NewClientAcceptCallBack(int ClientFD);
	// ���յ��ͻ��˵�����
	int RecvClientDataCallBack(int ClientFD, std::shared_ptr<stMsgToClient> recvData);
	// �ͻ��˶Ͽ�����
	int DissconnectClientCallBack(int ClientFD);
	// ������Ϣ��ָ���ͻ��� ���ǵ�ǰ��Ϣ���Ŀͻ��ˣ�
	int SendDataToClient(int ClientFD, std::shared_ptr<stMsgToClient> sendData);
	// ������Ϣ����ǰ�ͻ��� ����ǰ��Ϣ���Ŀͻ��ˣ�
	void SendDataToCurClient(std::shared_ptr<stMsgToClient> sendData);

public:
	void SetSocketPool(EpollSocket* SocketPool);
	void SetGameSocketWatcher(GameSocketWatcher* Watcher);

private:
	EpollSocket* SocketPool;
	GameSocketWatcher* Watcher;


private:
	std::set<int> allClients;
};

extern Interface gInterface;