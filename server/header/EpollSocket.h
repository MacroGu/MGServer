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


struct stSocketContext 			// some connected clients info
{
public:
	int fd;
	std::string client_ip;
	uint16_t uiMessageID;	// store client message ID
	std::string stToClient;	// response to client ж╩сп string json

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

class BaseSocketWatcher 
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

enum eEpollStatus
{
	EPOLL_RUNNING = 0,
	EPOLL_STOPPED
};

class EpollSocket 
{
private:
        int SetNonBlocking(int fd);

        int AcceptConnectSocket(int sockfd, std::string &client_ip);

        bool BindOnAddress(const stAddressInfo& addressInfo);

        void HandleAcceptEvent(int &epollfd, epoll_event &event, BaseSocketWatcher &socket_watcher);

        void HandleWriteableEvent(int &epollfd, epoll_event &event, BaseSocketWatcher &socket_watcher);

        void CloseAndReleaseOneEvent(epoll_event &event);

        bool InitThreadPool();

        bool StartThreadPool();

        bool CreateEpoll();

        bool AddListenSocketToEpoll();

        void HandleEpollEvent(epoll_event &e);

        void StartEpollEventLoop();

		void ReadTaskInThreads(void* data);

		stAddressInfo AddressInfo;

		BaseSocketWatcher* Watcher;

#ifndef _WIN32
		int _epollfd;
#else
		HANDLE _epollfd;
#endif
		int ListenedSocket;

        WorkerThread *WorkerThreadPtr;

		eEpollStatus EpollStatus;
public:

        EpollSocket();
       
         ~EpollSocket();
        
        void HandleEpollReadableEvent(epoll_event &event);

        bool StartEpoll();
        
        void StopEpoll();

        void SetThreadPoll(WorkerThread *tp);

		void SetAddressInfo(const stAddressInfo& addressInfo);
		
		void SetSocketWatcher(BaseSocketWatcher* watcher);
};

struct TaskData 
{
    epoll_event event;
    EpollSocket *es;
};

