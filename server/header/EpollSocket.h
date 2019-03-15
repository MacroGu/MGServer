/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#pragma once

#ifndef _WIN32
#include <sys/epoll.h>
#include <sys/socket.h>
#else
#pragma comment(lib,"ws2_32.lib")
#include "wepoll.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif // !_WIN32
#include <vector>
#include <set>
#include <string>
#include <stdint.h>
#include "ThreadPool.h"
#include "defines.h"

#define SS_WRITE_BUFFER_SIZE 4096
#define SS_READ_BUFFER_SIZE 4096

#define WRITE_CONN_ALIVE 0
#define WRITE_CONN_CLOSE 1
#define WRITE_CONN_CONTINUE 2

#define READ_OVER 0
#define READ_CONTINUE 1
#define READ_CLOSE -1


struct stSocketContext 			// some connected clients info
{
public:
	int fd;
	std::string client_ip;
	uint16_t uiMessageID;	// store client message ID
	std::string stToClient;	// response to client 只有 string json

	stSocketContext()
	{
		fd = 0;
		client_ip = "";
		uiMessageID = 0;
		stToClient = "";
	}

	~stSocketContext()
	{
		fd = 0;
		client_ip = "";
		uiMessageID = 0;
	}
};

class SocketWatcher 
{
    public:
        virtual int OnEpollAcceptEvent(stSocketContext &socket_context) = 0;

        virtual int OnEpollReadableEvent(int &epollfd, epoll_event &event) = 0;

        /**
         * return :
         * if return value == 1, we will close the connection
         * if return value == 2, we will continue to write
         */
        virtual int OnEpollWriteableEvent(stSocketContext &socket_context) = 0;

        virtual int OnEpollCloseEvent(stSocketContext &socket_context) = 0;

};

enum eEpollSocketStatus
{
    S_RUN = 0,
    S_REJECT_CONN = 1,
    S_STOP = 2
};

class EpollSocket 
{
private:
        int SetNonBlocking(int fd);

        int AcceptConnectSocket(int sockfd, std::string &client_ip);

        bool BindOnAddress(const stAddressInfo& addressInfo);

        void HandleAcceptEvent(int &epollfd, epoll_event &event, SocketWatcher &socket_watcher);

        void HandleWriteableEvent(int &epollfd, epoll_event &event, SocketWatcher &socket_watcher);

        void CloseAndReleaseOneEvent(epoll_event &event);

        bool InitThreadPool();

        bool StartThreadPool();

        bool CreateEpoll();

        bool AddListenSocketToEpoll();

        void HandleEpollEvent(epoll_event &e);

        void StartEpollEventLoop();

		stAddressInfo AddressInfo;

		SocketWatcher* Watcher;

#ifndef _WIN32
		int _epollfd;
#else
		HANDLE _epollfd;
#endif
		int ListenedSocket;

		// 所有的客户端共享的互斥锁
		std::mutex AllClientsSharedMutex; 

		// 链接本epoll 的客户端的总数
        uint32_t ClientTotalNums;
        
        ThreadPool *ThreadPoolPtr;
        
		eEpollSocketStatus EpollSocketStatus;

		void ReadTaskInThreads(void* data);
public:

        EpollSocket();
       
         ~EpollSocket();
        
        void HandleEpollReadableEvent(epoll_event &event);

        bool StartEpoll();
        
        void StopEpoll();

        void SetThreadPoll(ThreadPool *tp);

		void SetAddressInfo(const stAddressInfo& addressInfo);
		
		void SetSocketWatcher(SocketWatcher* watcher);
};

struct TaskData 
{
    epoll_event event;
    EpollSocket *es;
};

