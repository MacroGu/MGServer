/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once


#include "ServerLog.h"
#include "ServerConf.h"

// log
#define LOG_INFO	ServerLog::GetInstance().GetInstance().GetRotatingLogger()->info
#define LOG_ERROR	ServerLog::GetInstance().GetInstance().GetRotatingLogger()->error
#define LOG_DEBUG	ServerLog::GetInstance().GetInstance().GetRotatingLogger()->debug
#define LOG_WARN	ServerLog::GetInstance().GetInstance().GetRotatingLogger()->warn

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


