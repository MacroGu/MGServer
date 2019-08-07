#include "IocpBase.h"
#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>

IocpBase::IocpBase()
{
	// ��Ա������ʼ��
	bWorkerThread = true;
	bAccept = true;
}


IocpBase::~IocpBase()
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

bool IocpBase::Initialize()
{
	WSADATA wsaData;
	int nResult;
	// winsock 2.2 ����Ϊ�汾
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (nResult != 0)
	{
		LOG_ERROR("winsock ��ʼ��ʧ��");
		return false;
	}

	//�����׽���
	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (ListenSocket == INVALID_SOCKET)
	{
		LOG_ERROR("�׽��ִ���ʧ��");
		return false;
	}

	// ��������Ϣ����
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//�׽�������
	//ʹ��:: bind������boost��
	nResult = ::bind(ListenSocket, (struct sockaddr*) & serverAddr, sizeof(SOCKADDR_IN));

	if (nResult == SOCKET_ERROR)
	{
		LOG_ERROR("bind failed! ");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// �������ն���
	nResult = listen(ListenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		LOG_ERROR("listen failed! ");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void IocpBase::StartServer()
{
	int nResult;
	// �ͻ���Ϣ
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	SOCKET clientSocket;
	DWORD recvBytes;
	DWORD flags;

	// Completion Port ��������
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// Worker Thread
	if (!CreateWorkerThread()) return;

	LOG_INFO("���������� ");

	// �յ��ͻ�������
	while (bAccept)
	{
		clientSocket = WSAAccept(
			ListenSocket, (struct sockaddr*) & clientAddr, &addrLen, NULL, NULL
		);

		if (clientSocket == INVALID_SOCKET)
		{
			LOG_ERROR("Accept Failed! ");
			return;
		}

		SocketInfo = new stSOCKETINFO();
		SocketInfo->socket = clientSocket;
		SocketInfo->recvBytes = 0;
		SocketInfo->sendBytes = 0;
		SocketInfo->dataBuf.len = MAX_BUFFER;
		SocketInfo->dataBuf.buf = SocketInfo->messageBuffer;
		flags = 0;

		hIOCP = CreateIoCompletionPort(
			(HANDLE)clientSocket, hIOCP, (DWORD)SocketInfo, 0
		);

		// ָ��Ƕ���׽��ֲ�����Ҫ�����ʱִ�еĺ���
		nResult = WSARecv(
			SocketInfo->socket,
			&SocketInfo->dataBuf,
			1,
			&recvBytes,
			&flags,
			&(SocketInfo->overlapped),
			NULL
		);

		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			LOG_ERROR("IO Pending failed : {}", WSAGetLastError());
			return;
		}
	}

}

bool IocpBase::CreateWorkerThread()
{
	return false;
}

void IocpBase::Send(stSOCKETINFO* pSocket)
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
		LOG_ERROR(" WSA Send ʧ�� : {} ", WSAGetLastError());
	}


}

void IocpBase::Recv(stSOCKETINFO* pSocket)
{
	int nResult;
	// DWORD	sendBytes;
	DWORD	dwFlags = 0;

	// stSOCKETINFO ���ݳ�ʼ��
	ZeroMemory(&(pSocket->overlapped), sizeof(OVERLAPPED));
	ZeroMemory(pSocket->messageBuffer, MAX_BUFFER);
	pSocket->dataBuf.len = MAX_BUFFER;
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->recvBytes = 0;
	pSocket->sendBytes = 0;

	dwFlags = 0;

	// ����WSARecv�Դӿͻ��˻�ȡ��Ӧ
	nResult = WSARecv(
		pSocket->socket,
		&(pSocket->dataBuf),
		1,
		(LPDWORD)& pSocket,
		&dwFlags,
		(LPWSAOVERLAPPED) & (pSocket->overlapped),
		NULL
	);

	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		LOG_ERROR("WSARecv failed : {}", WSAGetLastError());
	}
}

void IocpBase::WorkerThread()
{

}