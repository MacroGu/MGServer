/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include <iostream>
#include <stdint.h>
#include <nlohmann/json.hpp>
#include "GameServer.h"
#include "protocol.h"
#include "ServerConf.h"
#include "RedisHandle.h"
#include "MysqlHandle.h"


GameSocketWatcher::GameSocketWatcher()
{

}

GameSocketWatcher::~GameSocketWatcher()
{

}

int GameSocketWatcher::OnEpollAcceptEvent(stSocketContext &socket_context)
{
	int conn_sock = socket_context.fd;
	std::string clientIP = socket_context.client_ip;
	LOG_INFO("accept client IP: {}, connected fd: {}", clientIP, conn_sock);

	return 0;
}

int GameSocketWatcher::OnEpollReadableEvent(int &epollfd, epoll_event &event)
{
	if (event.data.ptr == nullptr)
	{
		LOG_ERROR("event.data.ptr is nullptr , epollfd: {}", epollfd);
		return false;
	}

	stSocketContext *socket_context = (stSocketContext *)event.data.ptr;
	int fd = socket_context->fd;

	char read_buffer[SS_READ_BUFFER_SIZE];
	memset(read_buffer, 0, SS_READ_BUFFER_SIZE);

	int read_size = recv(fd, read_buffer, SS_READ_BUFFER_SIZE, 0);
	if (read_size == -1 && errno == EINTR) {
		return READ_CONTINUE;
	}
	if (read_size == -1 /* io err*/ || read_size == 0 /* close */)
	{
		return READ_CLOSE;
	}
	//DEBUG_MSG("read success which read size: " << read_size);


	// read_buffer can be parsed data here
	this->HandleClientNormalSocketData(socket_context, read_buffer, read_size);


	return READ_OVER;
}

int GameSocketWatcher::OnEpollWriteableEvent(stSocketContext &socket_context)
{
	int client_fd = socket_context.fd;
	int ret = send(socket_context.fd, socket_context.stToClient.c_str(), socket_context.stToClient.length(), 0);
	if (ret < 0)
	{
		LOG_ERROR("send data to client failed ! client fd:  client IP: {}", 
				client_fd, socket_context.client_ip);
		return WRITE_CONN_CLOSE;
	}

	return WRITE_CONN_ALIVE;
}

int GameSocketWatcher::OnEpollCloseEvent(stSocketContext &socket_context)
{
	LOG_DEBUG("close client client ip: {} client fd: {}", socket_context.client_ip, socket_context.fd);

	return 0;
}

bool GameSocketWatcher::HandleClientNormalSocketData(stSocketContext *socket_context, char clientData[], int dataLength)
{
	LOG_INFO("recv client data: {}", clientData);

	socket_context->stToClient = clientData;
	return true;
}

CGameServer::CGameServer()
{
}

CGameServer::~CGameServer()
{
	// WebSocketPool.StopEpoll();
	SocketPool.StopEpoll();
}

bool CGameServer::StartServer()
{
	if (!ServerConf::GetInstance().LoadServerConf())
	{
		LOG_ERROR("load server info configure file read failed!");
		return false;
	}
	else
	{
		LOG_INFO("load server configuration successfully! ");
	}


// 	if (!RedisHandle::GetInstance().Init())
// 	{
// 		LOG_ERROR("init redis failed!");
// 		return false;
// 	}

// 	if (!MysqlHandle::GetInstance().Init())
// 	{
// 		LOG_ERROR("init Mysql failed!");
// 		return false;
// 	}

	SocketPool.SetAddressInfo(ADDRESS_INFO_CONFIGURE);
	SocketPool.SetSocketWatcher(new GameSocketWatcher());

	if (!SocketPool.StartEpoll()) return false;

	return true;
}
