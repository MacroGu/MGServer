#ifdef _WIN32

#include <stdio.h>
#include <string.h>
#include <stdio.h>  
#include <winsock2.h>  
#include <stdint.h>
#include <iosfwd>
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <exception>
#include <iostream>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <set>
#include <list>
#include <nlohmann/json.hpp>
#include "protocol.h"
#include <windows.h>
#include "../server/header/defines.h"

#pragma comment(lib,"ws2_32.lib") 
#pragma pack(1)

// for convenience
using json = nlohmann::json;


#define  BUFFERSIZE 1024
#define SERVER_IP "106.12.80.224"
#define SERVER_PORT 2019


#define SUM_TOTAL 1000
#define THREAD_NUM 100


char  buffer[BUFFERSIZE];


int totalClients;
std::mutex g_mutex;
int lostClients;

void ThreadCallBack(int threadID)
{
	SOCKET clientsocket;
	SOCKADDR_IN serveraddr;

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

	if ((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)   //create a tcp socket  
	{
		printf("Create socket fail!\n");
		return ;
	}

	//connect to server  
	while (connect(clientsocket, (SOCKADDR*)& serveraddr, sizeof(serveraddr)) != 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
	} 
	g_mutex.lock();
	totalClients++;
	g_mutex.unlock();

	std::string sendData = "test client data 1";
	for (;;)
	{
		auto beforeTime = std::chrono::system_clock::now();
		if (send(clientsocket, sendData.c_str(), sendData.length(), 0) <= 0)
		{
			closesocket(clientsocket);
			--totalClients;
			g_mutex.lock();
			lostClients++;
			g_mutex.unlock();
			std::cout << "send data Error!, not last one package   : " << lostClients << std::endl;
			return;
		}

		char recvData[1024] = { 0 };
		if (recv(clientsocket, (char*)recvData, sendData.length(), 0) < 0)
		{
			closesocket(clientsocket);
			std::cout << "recv error !" << std::endl;
			--totalClients;
			g_mutex.lock();
			lostClients++;
			g_mutex.unlock();
			std::cout << "recv error !  : " << lostClients << std::endl;
			return;
		}
		auto millis = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - beforeTime).count();

		std::string receivedData = std::string(recvData);
		if (receivedData != sendData)
		{
			std::cout << "data send failed!" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

int  main(int argc, char ** argv) {

	WORD version = MAKEWORD(2, 2);
	WSADATA data;

	if (WSAStartup(version, &data) != 0) {
		std::cerr << "WSAStartup() failure" << std::endl;
		return -1;
	}

	totalClients = 0;
	lostClients = 0;
	std::thread AllThread[THREAD_NUM];
	while (true)
	{
		while (totalClients < THREAD_NUM)
		{
			AllThread[totalClients] = std::thread(&ThreadCallBack, totalClients);
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}

		while (true)
		{

			std::cout << "lost clients : " << lostClients << "   currrent total: " << totalClients << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}
	}

	WSACleanup();
	return 0;
}

#endif // !_WIN32
