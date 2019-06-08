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
	std::string stToClient;	// response to client ֻ�� string json

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

		// ����������ӳɹ�����true, �һ�ȡ�� epoll_event�� ���򷵻� false
        bool HandleAcceptEvent(int &epollfd, epoll_event &event, BaseSocketWatcher &socket_watcher, epoll_event& accepted_event);

        void HandleWriteableEvent(int &epollfd, epoll_event &event, BaseSocketWatcher &socket_watcher);

        void CloseAndReleaseOneEvent(epoll_event &event);

        bool InitWorkerThread();

        bool StartWorkerThread();

        bool CreateEpoll();

        bool AddListenSocketToEpoll();

        void HandleEpollEvent(epoll_event &e);

        void StartEpollEventLoop();

		// ��Ϣ�����̻߳ص�
		void DataDealThreadCallBack(void* data);
		// ���������̻߳ص�
		void AcceptThreadCallBack(void* data);

		stAddressInfo AddressInfo;

		BaseSocketWatcher* Watcher;

#ifndef _WIN32
		int _epollfd;
#else
		HANDLE _epollfd;
#endif
		int ListenedSocket;

		BaseThread* MsgDealThreadPtr;			// ��Ϣ�����߳�
		BaseThread* AcceptThreadPtr;			// ���Ӵ����߳�
		std::mutex gMutex;						// �����̹߳���� mutx�� ���ײ�Ҫ��
		std::set<int> WaitAcceptFd;				// �״ν������ӵ�ʱ�� �Ὣ����룬2s �ڲ��������ݣ���Ὣ��Ͽ�
		eEpollStatus EpollStatus;

private:
	// ������Ӱ��Ч�ʣ� �������ķ����� ԭ�� ÿ���̶߳��Լ��������ж�дȨ�ޣ������߳�ֻ�ж�Ȩ�ޣ� 
	/*
		MsgDealThreadPtr  �� AllDealClients  ��R/WȨ��
		AcceptThreadPtr   �� AllAcceptedClients �� R/W Ȩ�ޣ� �� AllDealClients �� ����Ȩ��
	*/
		std::map<int, epoll_event> AllConnectedClients;			// ���н��������ҷ�����Ϣ�ĵĿͻ��ˣ���Ҫ�����߼�����Ŀͻ���

public:

        EpollSocket();
       
         ~EpollSocket();
        
		// ������������¼�
        void HandleEpollReadableEvent(epoll_event &event);

        bool StartEpoll();
        
        void StopEpoll();

		void SetAddressInfo(const stAddressInfo& addressInfo);
		
		void SetSocketWatcher(BaseSocketWatcher* watcher);
};
