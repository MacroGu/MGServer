#include <WinSock2.h>
#include <iostream>
#include <sstream>
#include <map>

#pragma comment(lib,"ws2_32.lib") 
#pragma pack(1)

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
// 	WSADATA wsaData;
// 	// ��Winsock�汾����Ϊ2.2
// 	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
// 	if (nRet != 0) {
// 		std::cout << "Error : " << WSAGetLastError() << std::endl;
// 		return false;
// 	}
// 
// 	// TCP �����׽���
// 	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 	if (clientSocket == INVALID_SOCKET) {
// 		std::cout << "Error : " << WSAGetLastError() << std::endl;
// 		return false;
// 	}
// 
// 	std::cout << "socket initialize success." << std::endl;
// 
// 	// �洢Ҫ���ӵķ�������Ϣ�Ľṹ
// 	SOCKADDR_IN stServerAddr;
// 
// 	char	szOutMsg[MAX_BUFFER];
// 	char	sz_socketbuf_[MAX_BUFFER];
// 	stServerAddr.sin_family = AF_INET;
// 	// Ҫ��ϵ�ķ������˿� IP
// 	stServerAddr.sin_port = htons(SERVER_PORT);
// 	stServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
// 
// 	nRet = connect(clientSocket, (sockaddr*)& stServerAddr, sizeof(sockaddr));
// 	if (nRet == SOCKET_ERROR) {
// 		std::cout << "Error : " << WSAGetLastError() << std::endl;
// 		return false;
// 	}
// 
// 	std::cout << "Connection success..." << std::endl;
// 
// 	std::stringstream SendStream;
// 	SendStream << EPacketType::SIGNUP << std::endl;
// 	SendStream << "tes2t" << std::endl;
// 	SendStream << "test" << std::endl;
// 
// 	int nSend = send(clientSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0);
// 
// 	int nRecv = recv(clientSocket, sz_socketbuf_, MAX_BUFFER, 0);
// 
// 	std::stringstream RecvStream;
// 	bool result;
// 	RecvStream << sz_socketbuf_;
// 	RecvStream >> result;
// 	printf_s("%d\n", result);
// 
// 	closesocket(clientSocket);
// 	WSACleanup();
// 	std::cout << "Client has been terminated..." << std::endl;

	return 0;
}

