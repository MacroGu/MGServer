#pragma once

// ʹ�ö��ֽڼ� define
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <map>
#include <vector>
#include <iostream>
#include "defines.h"
#include "CommonClass.h"

using namespace std;

#define	MAX_BUFFER		4096
#define SERVER_PORT		8000
#define MAX_CLIENTS		100

// IOCP �׽��ֽṹ
struct stSOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER];
	int				recvBytes;
	int				sendBytes;
};

// ��������ָ��
struct FuncProcess
{
	void(*funcProcessPacket)(stringstream& RecvStream, stSOCKETINFO* pSocket);
	FuncProcess()
	{
		funcProcessPacket = nullptr;
	}
};

class IocpBase
{
public:
	IocpBase();
	virtual ~IocpBase();

	// ע���׽��ֲ����÷�������Ϣ
	bool Initialize();
	// ����������
	virtual void StartServer();
	// ���������߳�
	virtual bool CreateWorkerThread();
	// �����߳�
	virtual void WorkerThread();
	// ���͸��ͻ�
	virtual void Send(stSOCKETINFO* pSocket);
	// ���ͻ�
	virtual void Recv(stSOCKETINFO* pSocket);

protected:
	stSOCKETINFO* SocketInfo;		// �׽�����Ϣ
	SOCKET			ListenSocket;	// �����������׽���
	HANDLE			hIOCP;			// IOCP ������
	bool			bAccept;		// ���������־
	bool			bWorkerThread;	// �����߳���Ϊ��־
	HANDLE* hWorkerHandle;	// �����߳̾��	
	int				nThreadCnt;
};