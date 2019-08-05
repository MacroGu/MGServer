#pragma comment(lib, "ws2_32.lib")
// 使用多字节集时 define
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <sstream>
#include <map>
#include "CommonClass.h"
#include "../server/header/defines.h"

using namespace std;

#define	MAX_BUFFER		4096
#define SERVER_PORT		8000
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

int main()
{
	WSADATA wsaData;
	// 将Winsock版本初始化为2.2
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		LOG_ERROR(" WSAStartup Error : {}", WSAGetLastError());
		return false;
	}

	// TCP socket 创建
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		LOG_ERROR(" TCP socket 创建 : {}", WSAGetLastError());
		return false;
	}

	LOG_INFO("socket initialize success.");

	// 存储要连接的服务器信息的结构
	SOCKADDR_IN stServerAddr;

	char	szOutMsg[MAX_BUFFER];
	char	sz_socketbuf_[MAX_BUFFER];
	stServerAddr.sin_family = AF_INET;
	// 要连接的服务器端口和IP
	stServerAddr.sin_port = htons(SERVER_PORT);
	stServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	nRet = connect(clientSocket, (sockaddr*)& stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		LOG_ERROR(" TCP connect  : {}", WSAGetLastError());
		return false;
	}

	LOG_INFO("Connection success...");

	stringstream SendStream;
	SendStream << EPacketType::SIGNUP << endl;
	SendStream << "tes2t" << endl;
	SendStream << "test" << endl;

	int nSend = send(clientSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0);

	int nRecv = recv(clientSocket, sz_socketbuf_, MAX_BUFFER, 0);

	stringstream RecvStream;
	bool result;
	RecvStream << sz_socketbuf_;
	RecvStream >> result;
	LOG_INFO("recv: {}", result);

	closesocket(clientSocket);
	WSACleanup();
	LOG_INFO("Client has been terminated...");

	return 0;
}