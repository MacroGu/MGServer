/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/
#include "ServerLog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "ServerConf.h"



ServerLog::ServerLog()
{

}


ServerLog::~ServerLog()
{

}

const std::shared_ptr<spdlog::logger>& ServerLog::GetRotatingLogger()
{
	if (!RotatingLogger)
	{
		InitLog();
	}

	return RotatingLogger;
}

void ServerLog::InitLog()
{
	auto LoggerInfo = ServerConf::GetInstance().GetInstance().GetLoggerInfo();
	if (!RotatingLogger)
	{
		RotatingLogger = spdlog::rotating_logger_mt(LoggerInfo->LoggerName, LoggerInfo->LogFilePath, 
			LoggerInfo->MaxSingleFileSize, LoggerInfo->MaxLogFileNums);
	}

}
