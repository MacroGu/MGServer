// iocp-client.cpp: �������̨Ӧ�ó������ڵ㡣
//

#pragma comment(lib, "ws2_32.lib")
// �����ʱʹ�ö��ֽڼ� define
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <sstream>
#include <map>
#include "./genproto/map.pb.h"
#include "./genproto/login.pb.h"


#define	MAX_BUFFER		4096
#define SERVER_PORT		2019
#define SERVER_IP		"127.0.0.1"
#define MAX_CLIENTS		100

struct stSOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER];
	int				recvBytes;
	int				sendBytes;
};

enum EPacketType
{
	LOGIN,
	ENROLL_PLAYER,
	SEND_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	HIT_PLAYER,
	DAMAGED_PLAYER,
	CHAT,
	ENTER_NEW_PLAYER,
	SIGNUP,
	HIT_MONSTER,
	SYNC_MONSTER,
	SPAWN_MONSTER,
	DESTROY_MONSTER,
	TEST_PROTO
};


int main()
{
	WSADATA wsaData;
	// ��Winsock�汾��ʼ��Ϊ2.2
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// TCP �����׽���
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "socket initialize success." << std::endl;

	// �洢��������Ϣ�Խ������ӵĽṹ
	SOCKADDR_IN stServerAddr;

	char	szOutMsg[MAX_BUFFER];
	char	sz_socketbuf_[MAX_BUFFER];
	stServerAddr.sin_family = AF_INET;
	// Ҫ��ϵ�ķ������˿� IP
	stServerAddr.sin_port = htons(SERVER_PORT);
	stServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	getchar();
	nRet = connect(clientSocket, (sockaddr*)& stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Connection success...  next send" << std::endl;
	
	getchar();

	std::stringstream SendStream;
	SendStream << EPacketType::TEST_PROTO << std::endl;
	map::Map_EnterMap_Request_1001		_protoRequest;
	_protoRequest.set_mapid(2019);
	_protoRequest.set_roleuid(1205);
	int _len = _protoRequest.ByteSize();
	char* _backBuf = new char[_len];
	_protoRequest.SerializeToArray(_backBuf, _len);
	SendStream << _backBuf;

	int nSend = send(clientSocket, _backBuf, _len , 0);
	std::cout << " recv: " << std::endl;
	getchar();

	int nRecv = recv(clientSocket, sz_socketbuf_, MAX_BUFFER, 0);

	std::stringstream RecvStream;
	int PacketType;
	bool result;
	RecvStream << sz_socketbuf_;
	RecvStream >> PacketType;
	RecvStream >> result;
	printf_s("%d\n", result);

	closesocket(clientSocket);
	WSACleanup();
	std::cout << "Client has been terminated..." << std::endl;

	return 0;
}

