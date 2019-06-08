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
#include <map>
#include <string>
#include <chrono>
#include <stdint.h>
#include "WorkerThread.h"
#include "defines.h"


class Task;
class BaseThread;
class DealThread;
class AcceptThread;
struct stWorkerTaskData;
struct stAcceptTaskData;

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

		// 如果建立连接成功返回true, 且获取七 epoll_event， 否则返回 false
        bool HandleAcceptEvent(int &epollfd, epoll_event &event, BaseSocketWatcher &socket_watcher, epoll_event& accepted_event);

        void HandleWriteableEvent(int &epollfd, epoll_event &event, BaseSocketWatcher &socket_watcher);

        void CloseAndReleaseOneEvent(epoll_event &event);

        bool InitWorkerThread();

        bool StartWorkerThread();

        bool CreateEpoll();

        bool AddListenSocketToEpoll();

        void HandleEpollEvent(epoll_event &e);

        void StartEpollEventLoop();

		// 消息处理线程回调
		void DataDealThreadCallBack(void* data);
		// 建立连接线程回调
		void AcceptThreadCallBack(void* data);

		stAddressInfo AddressInfo;

		BaseSocketWatcher* Watcher;

#ifndef _WIN32
		int _epollfd;
#else
		HANDLE _epollfd;
#endif
		int ListenedSocket;

		BaseThread* MsgDealThreadPtr;			// 消息处理线程
		BaseThread* AcceptThreadPtr;			// 连接处理线程
		std::mutex gMutex;						// 三个线程共享的 mutx， 轻易不要用
		std::set<int> WaitAcceptFd;				// 首次建立连接的时候， 会将其加入，2s 内不发送数据，则会将其断开
		eEpollStatus EpollStatus;

private:
	// 加锁会影响效率， 不加锁的方法， 原理， 每个线程对自己的容器有读写权限，其他线程只有读权限， 
	/*
		MsgDealThreadPtr  对 AllDealClients  有R/W权限
		AcceptThreadPtr   对 AllAcceptedClients 有 R/W 权限， 对 AllDealClients 仅 读的权限
	*/
		std::map<int, epoll_event> AllConnectedClients;			// 所有建立连接且发了消息的的客户端，需要进行逻辑处理的客户端

public:

        EpollSocket();
       
         ~EpollSocket();
        
		// 处理接收数据事件
        void HandleEpollReadableEvent(epoll_event &event);

        bool StartEpoll();
        
        void StopEpoll();

		void SetAddressInfo(const stAddressInfo& addressInfo);
		
		void SetSocketWatcher(BaseSocketWatcher* watcher);
};
