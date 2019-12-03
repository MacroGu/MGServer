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
#include <iostream>
#include "EpollSocket.h"
#include "CommonClass.h"
#include "GameServerWatcher.h"


class CGameServer
{
public:
	CGameServer();
	~CGameServer();

	// ��ʼ��������
	bool InitServer();
	// ����������
	void StartServer();

private:

	// ���ڴ���  ��ͨsocket �Ŀͻ��˵�����
	EpollSocket SocketPool;
};