/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once

#include <memory>

#include <singleton.h>
#include "spdlog/logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> //support for stdout logging
#include <spdlog/sinks/basic_file_sink.h> // support for basic file logging
#include <spdlog/sinks/rotating_file_sink.h> // support for rotating file logging


class ServerLog : public ISingleton<ServerLog>
{
public:
	// Get Rotating logger
	const std::shared_ptr<spdlog::logger>& GetRotatingLogger();

private:
	std::shared_ptr<spdlog::logger> RotatingLogger;

private:

	friend ISingleton<ServerLog>;
	friend class std::unique_ptr<ServerLog>;

	ServerLog();

public:
	~ServerLog();
};
