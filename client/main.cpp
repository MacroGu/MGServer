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

#pragma comment(lib,"ws2_32.lib") 
#pragma pack(1)

// for convenience
using json = nlohmann::json;


#define  BUFFERSIZE 1024
#define SERVER_IP "192.168.137.174"
#define SERVER_PORT 2019


#define SUM_TOTAL 1000
#define THREAD_NUM 100


char  buffer[BUFFERSIZE];




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
	if (connect(clientsocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
	{
		printf("Connect fail!\n");
		return;
	}

	std::string sendDataStr = "";

	for (int i = 0; i < 10; i++)
	{
		sendDataStr += "test data send;";
	}

	std::string tempSendData = sendDataStr;

	std::list < uint64_t> Order100Set;


	while (true)
	{
		auto beforeTime = std::chrono::system_clock::now();

		if (send(clientsocket, sendDataStr.c_str(), sendDataStr.length(), 0) <= 0)
		{
			printf("send data Error!, not last one package \n");
			return ;
		}

		char recvData[1024] = { 0 };
		if (recv(clientsocket, (char*)recvData, sendDataStr.length(), 0) < 0)
		{
			std::cout << "recv error !" << std::endl;
		}

		if (std::strcmp(recvData, tempSendData.c_str()) != 0)
		{
			std::cout << "send data error " << std::endl;
		}

		auto millis = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - beforeTime).count();
		if (Order100Set.size() < SUM_TOTAL)
		{
			Order100Set.push_back(millis);
		}
		else
		{
			uint64_t maxTimeWait = 0;
			uint64_t minTimeWait = 99999999999;
			uint64_t avgTimeWait = 0;
			uint64_t sumTimeWait = 0;

			for (auto oneEle : Order100Set)
			{
				sumTimeWait += oneEle;
				if (maxTimeWait < oneEle)
				{
					maxTimeWait = oneEle;
				}

				if (minTimeWait > oneEle)
				{
					minTimeWait = oneEle;
				}
			}

			avgTimeWait = sumTimeWait / SUM_TOTAL;
			std::cout <<"thread ID:   " << threadID  << "   max:   " << maxTimeWait << "   min:   " << minTimeWait << "   avg:   " << avgTimeWait << std::endl;
			Order100Set.clear();
		}
	}
}

int  main(int argc, char ** argv) {



	WORD version = MAKEWORD(2, 2);
	WSADATA data;

	if (WSAStartup(version, &data) != 0) {
		std::cerr << "WSAStartup() failure" << std::endl;
		return -1;
	}

	std::thread AllThread[THREAD_NUM];
	for (int i = 1; i < THREAD_NUM; i++)
	{
		AllThread[i] = std::thread(&ThreadCallBack, i);
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
	
	ThreadCallBack(0);

	WSACleanup();
	return 0;
}

#endif // !_WIN32
