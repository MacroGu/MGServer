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

	// 为分组功能指针分配功能
	fnProcess[EPacketType::SIGNUP].funcProcessPacket = SignUp;
	fnProcess[EPacketType::LOGIN].funcProcessPacket = Login;
	fnProcess[EPacketType::ENROLL_PLAYER].funcProcessPacket = EnrollCharacter;
	fnProcess[EPacketType::SEND_PLAYER].funcProcessPacket = SyncCharacters;
	fnProcess[EPacketType::HIT_PLAYER].funcProcessPacket = HitCharacter;
	fnProcess[EPacketType::CHAT].funcProcessPacket = BroadcastChat;
	fnProcess[EPacketType::LOGOUT_PLAYER].funcProcessPacket = LogoutCharacter;
	fnProcess[EPacketType::HIT_MONSTER].funcProcessPacket = HitMonster;
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


void GameSocketWatcher::SignUp(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	std::string Id;
	std::string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	LOG_INFO("[INFO] 注册客户端 {}/{}\n", Id, Pw);

	std::stringstream SendStream;
	SendStream << EPacketType::SIGNUP << std::endl;
	SendStream << MysqlHandle::GetInstance().SignUpAccount(Id, Pw) << std::endl;

	pSocket->stToClient = SendStream.str().c_str();

}

void GameSocketWatcher::Login(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	std::string Id;
	std::string Pw;

	RecvStream >> Id;
	RecvStream >> Pw;

	LOG_INFO("[INFO] 登录尝试 {}/{}\n", Id, Pw);

	std::stringstream SendStream;
	SendStream << EPacketType::LOGIN << std::endl;
	SendStream << MysqlHandle::GetInstance().SearchAccount(Id, Pw) << std::endl;

	pSocket->stToClient = SendStream.str().c_str();
}

void GameSocketWatcher::EnrollCharacter(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	cCharacter info;
	RecvStream >> info;

	LOG_INFO("[INFO][{}] 角色注册 - X : [{}], Y : [{}], Z : [{}], Yaw : [{}], Alive : [{}], Health : [{}]\n",
		info.SessionId, info.X, info.Y, info.Z, info.Yaw, info.IsAlive, info.HealthValue);

	cCharacter* pinfo = &CharactersInfo.players[info.SessionId];
	
	// 保存角色的位置			
	pinfo->SessionId = info.SessionId;
	pinfo->X = info.X;
	pinfo->Y = info.Y;
	pinfo->Z = info.Z;

	// 保存角色的旋转
	pinfo->Yaw = info.Yaw;
	pinfo->Pitch = info.Pitch;
	pinfo->Roll = info.Roll;

	// 保存角色的速度
	pinfo->VX = info.VX;
	pinfo->VY = info.VY;
	pinfo->VZ = info.VZ;

	// 角色属性
	pinfo->IsAlive = info.IsAlive;
	pinfo->HealthValue = info.HealthValue;
	pinfo->IsAttacking = info.IsAttacking;

	SessionSocket[info.SessionId] = pSocket->socket;

	LOG_INFO("[INFO] 客户端数量 : {}\n", SessionSocket.size());

	//Send(pSocket);
	BroadcastNewPlayer(info);
}

void GameSocketWatcher::SyncCharacters(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	cCharacter info;
	RecvStream >> info;

	// 	 	LOG_INFO("[INFO][{}] 接收信息 - {}\n",
	// 	 		info.SessionId, info.IsAttacking);	
	g_mutex.lock();
	cCharacter* pinfo = &CharactersInfo.players[info.SessionId];

	// 保存角色的位置					
	pinfo->SessionId = info.SessionId;
	pinfo->X = info.X;
	pinfo->Y = info.Y;
	pinfo->Z = info.Z;

	// 保存角色的旋转
	pinfo->Yaw = info.Yaw;
	pinfo->Pitch = info.Pitch;
	pinfo->Roll = info.Roll;

	// 保存角色的速度
	pinfo->VX = info.VX;
	pinfo->VY = info.VY;
	pinfo->VZ = info.VZ;

	pinfo->IsAttacking = info.IsAttacking;

	g_mutex.unlock();
	WriteCharactersInfoToSocket(pSocket);
	Send(pSocket);
}

void GameSocketWatcher::LogoutCharacter(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	int SessionId;
	RecvStream >> SessionId;
	LOG_INFO("[INFO] ({})接收注销请求\n", SessionId);
	g_mutex.lock();
	CharactersInfo.players[SessionId].IsAlive = false;
	g_mutex.unlock();
	SessionSocket.erase(SessionId);
	LOG_INFO("[INFO] 客户数量 : %d\n", SessionSocket.size());
	WriteCharactersInfoToSocket(pSocket);
}

void GameSocketWatcher::HitCharacter(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	// 点击会话ID
	int DamagedSessionId;
	RecvStream >> DamagedSessionId;
	LOG_INFO("[INFO] %d 收到的伤害 \n", DamagedSessionId);
	g_mutex.lock();
	CharactersInfo.players[DamagedSessionId].HealthValue -= HitPoint;
	if (CharactersInfo.players[DamagedSessionId].HealthValue < 0)
	{
		// 性格死亡治疗
		CharactersInfo.players[DamagedSessionId].IsAlive = false;
	}
	g_mutex.unlock();
	WriteCharactersInfoToSocket(pSocket);
	Send(pSocket);
}

void GameSocketWatcher::BroadcastChat(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	stSocketContext* client = new stSocketContext;

	int SessionId;
	std::string Temp;
	std::string Chat;

	RecvStream >> SessionId;
	getline(RecvStream, Temp);
	Chat += std::to_string(SessionId) + "_:_";
	while (RecvStream >> Temp)
	{
		Chat += Temp + "_";
	}
	Chat += '\0';

	LOG_INFO("[CHAT] {}\n", Chat);

	std::stringstream SendStream;
	SendStream << EPacketType::CHAT << std::endl;
	SendStream << Chat;

	Broadcast(SendStream);
}

void GameSocketWatcher::HitMonster(std::stringstream& RecvStream, stSocketContext* pSocket)
{
	// 怪物命中处理
	int MonsterId;
	RecvStream >> MonsterId;
	MonstersInfo.monsters[MonsterId].Damaged(30.f);

	if (!MonstersInfo.monsters[MonsterId].IsAlive())
	{
		std::stringstream SendStream;
		SendStream << EPacketType::DESTROY_MONSTER << std::endl;
		SendStream << MonstersInfo.monsters[MonsterId] << std::endl;

		Broadcast(SendStream);

		MonstersInfo.monsters.erase(MonsterId);
	}

	// 向其他玩家广播
	/*std::stringstream SendStream;
	SendStream << EPacketType::HIT_MONSTER << std::endl;
	SendStream << MonstersInfo << std::endl;
	Broadcast(SendStream);*/
}

void GameSocketWatcher::BroadcastNewPlayer(cCharacter& player)
{
	std::stringstream SendStream;
	SendStream << EPacketType::ENTER_NEW_PLAYER << std::endl;
	SendStream << player << std::endl;

	Broadcast(SendStream);
}

void GameSocketWatcher::Broadcast(std::stringstream& SendStream)
{
	stSocketContext* client = new stSocketContext;
	for (const auto& kvp : SessionSocket)
	{
		client->socket = kvp.second;
		CopyMemory(client->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		client->dataBuf.buf = client->messageBuffer;
		client->dataBuf.len = SendStream.str().length();

		Send(client);
	}
}

void GameSocketWatcher::WriteCharactersInfoToSocket(stSocketContext* pSocket)
{
	std::stringstream SendStream;

	// 序列化
	SendStream << EPacketType::RECV_PLAYER << std::endl;
	SendStream << CharactersInfo << std::endl;

	// ！ 重要!!! 将数据直接写入data.buf可以发送垃圾
	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();
}
