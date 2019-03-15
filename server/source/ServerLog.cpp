/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include "ServerLog.h"
#include "spdlog/sinks/rotating_file_sink.h"



ServerLog::ServerLog()
{
	RotatingLogger = spdlog::rotating_logger_mt("basic_logger", "logs/basic.log", 1024 * 5, 5);
}


ServerLog::~ServerLog()
{

}

const std::shared_ptr<spdlog::logger>& ServerLog::GetRotatingLogger()
{
	if (!RotatingLogger)
	{
		RotatingLogger = spdlog::rotating_logger_mt("basic_logger", "logs/basic.log", 1024 * 5, 5);
	}

	return RotatingLogger;
}
