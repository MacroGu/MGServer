/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once


#include "ServerLog.h"
#include "ServerConf.h"

// log
#define LOG_INFO(fmt, ...) \
		ServerLog::GetInstance().GetInstance().GetRotatingLogger()->info("{}: {}  "##fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) \
		ServerLog::GetInstance().GetInstance().GetRotatingLogger()->error("{}: {}  "##fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) \
		ServerLog::GetInstance().GetInstance().GetRotatingLogger()->debug("{}: {}  "##fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) \
		ServerLog::GetInstance().GetInstance().GetRotatingLogger()->warn("{}: {}  "##fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) \
		ServerLog::GetInstance().GetInstance().GetRotatingLogger()->critical("{}: {}  "##fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)


// conf
#define SERVER_CONF_PATH "conf/serverinfo.xml"
#define WEBSOCKET_ADDRESS_INFO "wsaddressInfo"
#define NORMALSOCKET_ADDRESS_INFO "nsaddressInfo"
#define WS_ADDRESS_INFO_CONFIGURE ServerConf::GetInstance().GetWSAddressInfoConfiguration()
#define NS_ADDRESS_INFO_CONFIGURE ServerConf::GetInstance().GetNSAddressInfoConfiguration()

// buffer size
#define MAX_WEBSOCKET_BUFFER 4096




// max thread nums for thread pool
#define MAX_THREAD_NUMS_THREAD_POOL 128


