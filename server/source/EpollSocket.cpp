/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#ifndef _WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#else
#endif // !_WIN32
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <cerrno>
#include <errno.h>
#include <string.h>
#include "EpollSocket.h"
#include <iostream>

EpollSocket::EpollSocket() 
{
    ThreadPoolPtr = NULL;
    EpollSocketStatus = S_RUN;
    ClientTotalNums = 0;
	ListenedSocket = 0;
	Watcher = nullptr;
#ifdef _WIN32
	_epollfd = INVALID_HANDLE_VALUE;
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
#else
	_epollfd = -1;
#endif // _WIN32
}

EpollSocket::~EpollSocket() 
{
    if (ThreadPoolPtr != nullptr) 
	{
        delete ThreadPoolPtr;
        ThreadPoolPtr = NULL;
    }
#ifdef _WIN32
	WSACleanup();
#endif // _WIN32

}

int EpollSocket::SetNonBlocking(int fd) {
#ifndef _WIN32
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	unsigned long flags = 1; /* 这里根据需要设置成0或1 */
	return ioctlsocket(fd, FIONBIO, &flags);
#endif
};

bool EpollSocket::BindOnAddress(const stAddressInfo& addressInfo)
{
    /* listen on sock_fd, new connection on new_fd */
    ListenedSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenedSocket == -1)
	{
        LOG_ERROR("socket error: %s" , strerror(errno));
        return false;
    }
    int opt = 1;
#ifndef _WIN32
    setsockopt(ListenedSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#else
	setsockopt(ListenedSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#endif
    struct sockaddr_in my_addr; /* my address information */
    memset (&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(addressInfo.port); /* short, network byte order */
	if ("" == addressInfo.serverIp || "localhost" == addressInfo.serverIp)
	{
		my_addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		my_addr.sin_addr.s_addr = inet_addr(addressInfo.serverIp.c_str());
	}

    if (bind(ListenedSocket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
	{
        LOG_ERROR("bind error: %s" , strerror(errno));
        return false;
    }
    if (listen(ListenedSocket, addressInfo.backlog) == -1)
	{
        LOG_ERROR("listen error: %s" , strerror(errno));
        return false;
    }

    return true;
}

int EpollSocket::AcceptConnectSocket(int sockfd, std::string &client_ip) 
{
    int new_fd;
    struct sockaddr_in their_addr; /* connector's address information */
    socklen_t sin_size = sizeof(struct sockaddr_in);

    if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1) 
	{
        LOG_ERROR("accept error: %s" , strerror(errno));
        return -1;
    }

    client_ip = inet_ntoa(their_addr.sin_addr);
    LOG_DEBUG("server: got connection from %s" ,  client_ip.c_str());
    return new_fd;
}

void EpollSocket::HandleAcceptEvent(int &epollfd, epoll_event &event, SocketWatcher &socket_handler) 
{
    int sockfd = event.data.fd;

    std::string client_ip;
    int conn_sock = AcceptConnectSocket(sockfd, client_ip);
    if (-1 == conn_sock) 
	{
		LOG_ERROR("AcceptConnectSocket failed!");
        return;
    }
	if (0 != SetNonBlocking(conn_sock))
	{
		LOG_ERROR("set conn sock nonblocking failed!");
		return;
	}

	AllClientsSharedMutex.lock();
    ClientTotalNums++;
	AllClientsSharedMutex.unlock();
    LOG_DEBUG("get accept socket which listen fd: %d, conn_sock_fd: %d" , sockfd , conn_sock);

    stSocketContext *socket_context = new stSocketContext();
    socket_context->fd = conn_sock;
    socket_context->client_ip = client_ip;

    socket_handler.OnEpollAcceptEvent(*socket_context);

    struct epoll_event conn_sock_ev;
    conn_sock_ev.events = EPOLLIN | EPOLLONESHOT;
    conn_sock_ev.data.ptr = socket_context;

    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, conn_sock, &conn_sock_ev) == -1) 
	{
        LOG_ERROR("epoll_ctl: conn_sock: %s" , strerror(errno));
        CloseAndReleaseOneEvent(event);
		return;
	}
}

void EpollSocket::ReadTaskInThreads(void* data)
{
    TaskData *td = (TaskData *) data;
    td->es->HandleEpollReadableEvent(td->event);
    delete td;
}

void EpollSocket::HandleEpollReadableEvent(epoll_event &event) 
{
    stSocketContext *socket_context = (stSocketContext *) event.data.ptr;
    int fd = socket_context->fd;

    int ret = Watcher->OnEpollReadableEvent((int&)_epollfd, event);
    if (ret == READ_CLOSE) 
	{
        CloseAndReleaseOneEvent(event);
		LOG_ERROR("Handle Epoll Readable event failed! fd: {}, IP: {}", 
			socket_context->fd, socket_context->client_ip);
		return;
    }
    if (ret == READ_CONTINUE) 
	{
        event.events = EPOLLIN | EPOLLONESHOT;
        epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event);
    } else if (ret == READ_OVER) 
	{ // READ_OVER
        event.events = EPOLLOUT | EPOLLONESHOT;
        epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event);
    } else 
	{
        LOG_ERROR("Unknown read ret: %d" ,  ret);
		return;
    }
}

void EpollSocket::HandleWriteableEvent(int &epollfd, epoll_event &event, SocketWatcher &socket_handler) {
    stSocketContext *socket_context = (stSocketContext *) event.data.ptr;
    int fd = socket_context->fd;
    //LOG_DEBUG("start write data");

    int ret = socket_handler.OnEpollWriteableEvent(*socket_context);
    if(ret == WRITE_CONN_CLOSE) 
	{
        CloseAndReleaseOneEvent(event);
		return;
    }

    if (ret == WRITE_CONN_CONTINUE) 
	{
        event.events = EPOLLOUT | EPOLLONESHOT;
    } 
	else if (ret == WRITE_CONN_ALIVE) 
	{
        if (EpollSocketStatus == S_REJECT_CONN) 
		{
            return CloseAndReleaseOneEvent(event);
        }
        // wait for next request
        event.events = EPOLLIN | EPOLLONESHOT;
    } 
	else 
	{
        LOG_ERROR("Unknown write ret: %d" , ret);
    }
#ifndef _WIN32
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
#else
	epoll_ctl((HANDLE)epollfd, EPOLL_CTL_MOD, fd, &event);
#endif // !_WIN32
}

void EpollSocket::SetThreadPoll(ThreadPool *tp) 
{
    this->ThreadPoolPtr = tp;
}

void EpollSocket::SetAddressInfo(const stAddressInfo& addressInfo)
{
	this->AddressInfo.serverIp = addressInfo.serverIp;
	this->AddressInfo.backlog = addressInfo.backlog;
	this->AddressInfo.port = addressInfo.port;
	this->AddressInfo.maxEvents = addressInfo.maxEvents;
	this->AddressInfo.threadNums = addressInfo.threadNums;
}

void EpollSocket::SetSocketWatcher(SocketWatcher* watcher)
{
	if (watcher == nullptr)
	{
		LOG_ERROR("thread Nums configure error !");
		return;
	}

	this->Watcher = watcher;
}

bool EpollSocket::InitThreadPool() 
{
   // LOG_INFO("thread pool not set, we will build for core size: %d" , core_size);
	if (AddressInfo.threadNums <= 0)
	{
		LOG_ERROR("thread Nums configure error !");
		return false;
	}
    ThreadPoolPtr = new ThreadPool();
    ThreadPoolPtr->SetWorkThreadNums(AddressInfo.threadNums);

    return true;
}

bool EpollSocket::AddListenSocketToEpoll() 
{
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = ListenedSocket;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, ListenedSocket, &ev) == -1)
	{
		LOG_ERROR("epoll_ctl: listen_sock: %s", strerror(errno));
		return false;
	}

    return true;
}

void EpollSocket::HandleEpollEvent(epoll_event &e) 
{
    if (e.data.fd == ListenedSocket)		// 仅仅建立连接的时候进行判断，因为只有此时fd才和server 的监听fd 相等
	{
        // accept connection
        if (EpollSocketStatus == S_RUN) 
		{
            this->HandleAcceptEvent((int&)_epollfd, e, *Watcher);
        } 
		else 
		{
            LOG_INFO("current status: %s , not accept new connect" , EpollSocketStatus);
			AllClientsSharedMutex.lock();
            if (ClientTotalNums == 0 && EpollSocketStatus == S_REJECT_CONN) 
			{
                EpollSocketStatus = S_STOP;
                LOG_INFO("client is empty and ready for stop server!");
            }
			AllClientsSharedMutex.unlock();
        }
    } 
	else if (e.events & EPOLLIN) 
	{
        // handle readable async
        //LOG_DEBUG("start handle readable event");
        TaskData *tdata = new TaskData();
        tdata->event = e;
        tdata->es = this;

        Task *task = new Task(std::bind(&EpollSocket::ReadTaskInThreads, this, tdata), tdata);
        if (!ThreadPoolPtr->AddTask(task))
		{
            LOG_WARN("add read task fail: ,we will close connect.");
            CloseAndReleaseOneEvent(e);
            delete tdata;
            delete task;
        }
    } 
	else if (e.events & EPOLLOUT) 
	{
        // writeable
        this->HandleWriteableEvent((int&)_epollfd, e, *Watcher);
    } 
	else 
	{
		LOG_WARN("unknown events : %d" , e.events);
    }
}

bool EpollSocket::CreateEpoll() 
{
    // Since Linux 2.6.8, the size argument is ignored, but must be greater than zero;
    // and under windows select, it should be same as select size 
    _epollfd = epoll_create(1024);
#ifdef _WIN32
	if (_epollfd == NULL)
#else
    if (_epollfd == -1) 
#endif
	{
		LOG_ERROR("epoll_create: %s", strerror(errno));
		return false;
	}

    return true;
}

bool EpollSocket::StartThreadPool() 
{
    if (ThreadPoolPtr == NULL) 
	{
		if (!InitThreadPool())
		{
			LOG_ERROR("initial thread pool failed!");
			return false;
		}
    }
    return ThreadPoolPtr->Start();
}

void EpollSocket::StartEpollEventLoop() 
{
    epoll_event *events = new epoll_event[AddressInfo.maxEvents];
    while (EpollSocketStatus != S_STOP) 
	{
        int fds_num = epoll_wait(_epollfd, events, AddressInfo.maxEvents, -1);
        if (fds_num == -1) 
		{
            if (errno == EINTR) 
			{ /*The call was interrupted by a signal handler*/
                continue;
            }
            LOG_ERROR("epoll_wait error: %s" , strerror(errno));
            break;
        }

        for (int i = 0; i < fds_num; i++) 
		{
            this->HandleEpollEvent(events[i]);
        }
    }
    LOG_INFO("epoll wait loop stop ...");
    if (events != NULL) 
	{
        delete[] events;
        events = NULL;
		LOG_WARN("still have events not handle, and loop end");
    }
}

bool EpollSocket::StartEpoll() 
{
	bool ret = true;

	do
	{
		ret = StartThreadPool();
		if (!ret) break;

		ret = BindOnAddress(AddressInfo);
		if (!ret) break;

		ret = CreateEpoll();
		if (!ret) break;

		ret = AddListenSocketToEpoll();
		if (!ret) break;

	} while (0);

	if (!ret)
	{
		LOG_ERROR("error : %d", ret);
		return ret;
	}

	StartEpollEventLoop();
	return true;
}

void EpollSocket::StopEpoll() 
{
    EpollSocketStatus = S_REJECT_CONN;
    LOG_INFO("stop epoll , current clients: %d" , ClientTotalNums);
}

void EpollSocket::CloseAndReleaseOneEvent(epoll_event &epoll_event) 
{
    if (epoll_event.data.ptr == NULL) 
	{
        return;
    }
    LOG_DEBUG("connect close");

    stSocketContext *hc = (stSocketContext *) epoll_event.data.ptr;
    Watcher->OnEpollCloseEvent(*hc);

    int fd = hc->fd;
    epoll_event.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &epoll_event);

    delete (stSocketContext *) epoll_event.data.ptr;
    epoll_event.data.ptr = NULL;

#ifndef _WIN32
    int ret = close(fd);
#else
	int ret = closesocket(fd);
#endif

	AllClientsSharedMutex.lock();
    ClientTotalNums--;
    if (ClientTotalNums == 0 && EpollSocketStatus == S_REJECT_CONN) 
	{
        EpollSocketStatus = S_STOP;
        LOG_INFO("client is empty and ready for stop server!");
    }
	AllClientsSharedMutex.unlock();

	if (ret != 0)
	{
		LOG_ERROR("connect close complete which fd: {}, ret: {}", fd, ret);
		return;
	}

}
