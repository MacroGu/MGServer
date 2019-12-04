/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once


#include "ServerLog.h"
#include "ServerConf.h"


// for epoll
#define SS_WRITE_BUFFER_SIZE 4096
#define SS_READ_BUFFER_SIZE 4096

#define WRITE_CONN_ALIVE 0
#define WRITE_CONN_CLOSE 1
#define WRITE_CONN_CONTINUE 2

#define READ_OVER 0
#define READ_CONTINUE 1
#define READ_CLOSE -1



// log

#ifdef _WIN32
#define LOG_INFO	SPDLOG_INFO
#define LOG_ERROR	SPDLOG_ERROR
#define LOG_DEBUG	SPDLOG_DEBUG
#define LOG_WARN	SPDLOG_WARN
#else
#define LOG_INFO	ServerLog::GetInstance().GetRotatingLogger()->info
#define LOG_ERROR	ServerLog::GetInstance().GetRotatingLogger()->error
#define LOG_DEBUG	ServerLog::GetInstance().GetRotatingLogger()->debug
#define LOG_WARN	ServerLog::GetInstance().GetRotatingLogger()->warn
#endif // _WIN32

// conf
#define SERVER_CONF_PATH "conf/serverinfo.xml"
#define ADDRESS_INFO "addressInfo"
#define ADDRESS_INFO_CONFIGURE ServerConf::GetInstance().GetAddressInfoConfiguration()


// ��Ϣ���Ͷ���
enum EPacketType
{
	LOGIN = 100,
	ENROLL_PLAYER,
	SEND_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	HIT_PLAYER,
	DAMAGED_PLAYER,
	CHAT,
	ENTER_NEW_PLAYER,
	SIGNUP,
	HIT_MONSTER,
	SYNC_MONSTER,
	SPAWN_MONSTER,
	DESTROY_MONSTER
};