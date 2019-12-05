/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include <iostream>
#include <cstring>
#include <stdint.h>
#include <nlohmann/json.hpp>
#include "GameSocketWatcher.h"
#include "protocol.h"
#include "ServerConf.h"
#include "RedisHandle.h"
#include "MysqlHandle.h"
#include "Interface.h"


GameSocketWatcher::GameSocketWatcher()
{
	toCurClientData = nullptr;
	curClient = nullptr;
	allClients.clear();
}

GameSocketWatcher::~GameSocketWatcher()
{
	toCurClientData = nullptr;
	curClient = nullptr;
	allClients.clear();
}

int GameSocketWatcher::OnEpollAcceptEvent(stSocketContext& socket_context)
{
	int conn_sock = socket_context.fd;
	std::string clientIP = socket_context.client_ip;
	LOG_INFO("accept client IP: {}, connected fd: {}", clientIP, conn_sock);

	curClient = &socket_context;
	toCurClientData = nullptr;
	allClients.insert(std::make_pair(socket_context.fd, socket_context));
	Interface::GetInstance().NewClientAcceptCallBack(conn_sock);

	return 0;
}

int GameSocketWatcher::OnEpollReadableEvent(int& epollfd, epoll_event& event)
{
	if (event.data.ptr == nullptr)
	{
		LOG_ERROR("event.data.ptr is nullptr , epollfd: {}", epollfd);
		return false;
	}

	stSocketContext* socket_context = (stSocketContext*)event.data.ptr;
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

int GameSocketWatcher::OnEpollWriteableEvent(stSocketContext& socket_context)
{
	int client_fd = socket_context.fd;
	if (toCurClientData != nullptr)
	{
		int ret = send(socket_context.fd, toCurClientData->data, toCurClientData->dataLen, 0);
		if (ret < 0)
		{
			LOG_ERROR("send data to client failed ! client fd:  client IP: {}",
				client_fd, socket_context.client_ip);
			return WRITE_CONN_CLOSE;
		}
	}

	curClient = nullptr;
	return WRITE_CONN_ALIVE;
}

int GameSocketWatcher::OnEpollCloseEvent(stSocketContext& socket_context)
{
	LOG_DEBUG("close client client ip: {} client fd: {}", socket_context.client_ip, socket_context.fd);
	Interface::GetInstance().DissconnectClientCallBack(socket_context.fd);

	auto clientIter = allClients.find(socket_context.fd);
	if (clientIter != allClients.end())
	{
		allClients.erase(clientIter);
	}
	else
	{
		LOG_ERROR("client close but not in server fd: {} , ip: {}", socket_context.fd, socket_context.client_ip);
	}

	curClient = nullptr;
	return 0;
}

int GameSocketWatcher::SentDataToOneClient(int ClientFD, std::shared_ptr<stMsgToClient> sendData)
{
	auto clientIter = allClients.find(ClientFD);
	if (clientIter == allClients.end())
	{
		LOG_ERROR("send data to client failed ! can not found this client FD: {}", ClientFD);
		return WRITE_CONN_CLOSE;
	}

	int ret = send(ClientFD, sendData->data, sendData->dataLen, 0);
	if (ret < 0)
	{
		allClients.erase(clientIter);
		LOG_ERROR("send data to client failed ! client fd: {}  client IP: {}", ClientFD, clientIter->second.client_ip);
		return WRITE_CONN_CLOSE;
	}

	return 0;
}

void GameSocketWatcher::SendDataToCurClient(std::shared_ptr<stMsgToClient> toCurClient)
{
	toCurClientData = toCurClient;
}

bool GameSocketWatcher::HandleClientNormalSocketData(stSocketContext* socket_context, char clientData[], int dataLength)
{
	// LOG_INFO("recv client data: {}", clientData);

	// socket_context->stToClient = std::to_string();
	std::shared_ptr<stMsgToClient> recvData(new stMsgToClient());
	memcpy(recvData->data, clientData, dataLength);
	recvData->dataLen = dataLength;
	Interface::GetInstance().RecvClientDataCallBack(socket_context->fd, recvData);

	return true;
}
