#pragma once

// 使用多字节集 define
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

// IOCP 套接字结构
struct stSOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER];
	int				recvBytes;
	int				sendBytes;
};

// 包处理函数指针
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

	// 注册套接字并设置服务器信息
	bool Initialize();
	// 启动服务器
	virtual void StartServer();
	// 创建工作线程
	virtual bool CreateWorkerThread();
	// 任务线程
	virtual void WorkerThread();
	// 发送给客户
	virtual void Send(stSOCKETINFO* pSocket);
	// 听客户
	virtual void Recv(stSOCKETINFO* pSocket);

protected:
	stSOCKETINFO* SocketInfo;		// 套接字信息
	SOCKET			ListenSocket;	// 服务器监听套接字
	HANDLE			hIOCP;			// IOCP 对象句柄
	bool			bAccept;		// 请求操作标志
	bool			bWorkerThread;	// 任务线程行为标志
	HANDLE* hWorkerHandle;	// 工作线程句柄	
	int				nThreadCnt;
};