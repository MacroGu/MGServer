/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#pragma once
#include <thread>
#include <sstream>
#include <map>
#include <set>
#include <stdint.h>
#include "GameSocketWatcher.h"


class GameServer
{
public:
	GameServer();
	~GameServer();

	// ��ʼ��������
	bool InitServer();
	// ����������
	void StartServer();

private:

	// ���ڴ���  ��ͨsocket �Ŀͻ��˵�����
	EpollSocket SocketPool;
};