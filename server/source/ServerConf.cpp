/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include "ServerConf.h"
#include <iostream>

ServerConf::ServerConf()
{
	bAllConfLoadedRight = true;
	LoggerInfo = std::make_shared<stLoggerInfo>();
	RedisInfo = std::make_shared<stRedisInfo>();
	UniqueInfo = std::make_shared<stUniqueInfo>();
}

ServerConf::~ServerConf()
{
}


bool ServerConf::LoadServerConf()
{
	if (!LoadServerInfo(SERVER_CONF_PATH, WEBSOCKET_ADDRESS_INFO))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load local server configuration failed! WEBSOCKET_ADDRESS_INFO" << std::endl;
		return false;
	}

	if (!LoadServerInfo(SERVER_CONF_PATH, NORMALSOCKET_ADDRESS_INFO))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load local server configuration failed! NORMALSOCKET_ADDRESS_INFO" << std::endl;
		return false;
	}

	if (!LoadLogInfo(SERVER_CONF_PATH))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load log info configuration failed! " << std::endl;
		return false;
	}

	if (!LoadRedisInfo(SERVER_CONF_PATH))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load redis info configuration failed! " << std::endl;
		return false;
	}

	if (!LoadUniqueInfo(SERVER_CONF_PATH))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load server unique info configuration failed! " << std::endl;
		return false;
	}

	return true;
}

bool ServerConf::hasLoadConfRight()
{
	return bAllConfLoadedRight;
}

const stAddressInfo& ServerConf::GetWSAddressInfoConfiguration()
{
	return webSocketInfo;
}

const stAddressInfo& ServerConf::GetNSAddressInfoConfiguration()
{
	return normalSocketInfo;
}

std::shared_ptr<stLoggerInfo> ServerConf::GetLoggerInfo()
{
	return LoggerInfo;
}

std::shared_ptr<stRedisInfo> ServerConf::GetRedisInfo()
{
	return RedisInfo;
}

bool ServerConf::LoadServerInfo(const std::string& fileDir, const std::string& addressInfo)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(fileDir.c_str()))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load log info configuration failed! " << fileDir.c_str()<< " error:" << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlElement* server = doc.FirstChildElement();
	if (server == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find root in xml!" << std::endl;
		return false;
	}

	TiXmlElement* m_serverInfo = server->FirstChildElement(addressInfo.c_str());
	if (m_serverInfo == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find local server info xml attribute!" << std::endl;
		return false;
	}

	stAddressInfo* serverInfo = nullptr;
	if (WEBSOCKET_ADDRESS_INFO == addressInfo)
	{
		serverInfo = &webSocketInfo;
	}
	else if (NORMALSOCKET_ADDRESS_INFO == addressInfo)
	{
		serverInfo = &normalSocketInfo;
	}
	else
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  address info is wrong!" << std::endl;
		return false;
	}
	

	for (TiXmlElement* m_serverInfoValue = m_serverInfo->FirstChildElement("value");
		m_serverInfoValue != nullptr; m_serverInfoValue = m_serverInfoValue->NextSiblingElement())
	{
		const char* type = m_serverInfoValue->Attribute("type");
		const char* value = m_serverInfoValue->Attribute("value");

		if (strcmp(type, "serverIp") == 0)
		{
			serverInfo->serverIp = value;
		}
		else if (strcmp(type, "port") == 0)
		{
			serverInfo->port = atoi(value);
		}
		else if (strcmp(type, "maxevents") == 0)
		{
			serverInfo->maxEvents = atoi(value);
		}
		else if (strcmp(type, "backlog") == 0)
		{
			serverInfo->backlog = atoi(value);
		}
		else
		{
			std::cout << __FUNCTION__ << " : " << __LINE__ << "  server info conf type: " << type<< " value: " << value << std::endl;
			doc.Clear();
			return false;
		}
	}

	doc.Clear();
	return true;
}

bool ServerConf::LoadLogInfo(const std::string& FilePath)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(FilePath.c_str()))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load file: " << FilePath << " error: " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlElement* server = doc.FirstChildElement();
	if (server == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find root in xml!" << std::endl;
		return false;
	}

	TiXmlElement* LogInfo = server->FirstChildElement("loggerinfo");
	if (LogInfo == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find loggerinfo info xml attribute " << std::endl;
		return false;
	}

	for (TiXmlElement* m_serverInfoValue = LogInfo->FirstChildElement("value");
		m_serverInfoValue != nullptr; m_serverInfoValue = m_serverInfoValue->NextSiblingElement())
	{
		const char* type = m_serverInfoValue->Attribute("type");
		const char* value = m_serverInfoValue->Attribute("value");

		if (strcmp(type, "LoggerName") == 0)
		{
			LoggerInfo->LoggerName = value;
		}
		else if (strcmp(type, "LogFilePath") == 0)
		{
			LoggerInfo->LogFilePath = value;
		}
		else if (strcmp(type, "MaxSingleFileSize") == 0)
		{
			LoggerInfo->MaxSingleFileSize = atoi(value);
		}
		else if (strcmp(type, "MaxLogFileNums") == 0)
		{
			LoggerInfo->MaxLogFileNums = atoi(value);
		}
		else
		{
			std::cout << __FUNCTION__ << " : " << __LINE__ << "  log info conf type: " << type << " value: " << value << std::endl;
			doc.Clear();
			return false;
		}
	}

	doc.Clear();
	return true;
}

bool ServerConf::LoadRedisInfo(const std::string& FilePath)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(FilePath.c_str()))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load file: " << FilePath << " error: " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlElement* server = doc.FirstChildElement();
	if (server == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find root in xml!" << std::endl;
		return false;
	}

	TiXmlElement* LogInfo = server->FirstChildElement("redisInfo");
	if (LogInfo == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find loggerinfo info xml attribute " << std::endl;
		return false;
	}

	for (TiXmlElement* m_serverInfoValue = LogInfo->FirstChildElement("value");
		m_serverInfoValue != nullptr; m_serverInfoValue = m_serverInfoValue->NextSiblingElement())
	{
		const char* type = m_serverInfoValue->Attribute("type");
		const char* value = m_serverInfoValue->Attribute("value");

		if (strcmp(type, "redisAddress") == 0)
		{
			RedisInfo->redisAddress = value;
		}
		else if (strcmp(type, "redisPort") == 0)
		{
			RedisInfo->redisPort = atoi(value);
		}
		else
		{
			std::cout << __FUNCTION__ << " : " << __LINE__ << "  redis info conf type: " << type << " value: " << value << std::endl;
			doc.Clear();
			return false;
		}
	}

	doc.Clear();
	return true;
}

bool ServerConf::LoadUniqueInfo(const std::string& FilePath)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(FilePath.c_str()))
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  load file: " << FilePath << " error: " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlElement* server = doc.FirstChildElement();
	if (server == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find root in xml!" << std::endl;
		return false;
	}

	TiXmlElement* LogInfo = server->FirstChildElement("uniqueInfo");
	if (LogInfo == nullptr)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  can not find server unique info xml attribute " << std::endl;
		return false;
	}

	for (TiXmlElement* m_serverInfoValue = LogInfo->FirstChildElement("value");
		m_serverInfoValue != nullptr; m_serverInfoValue = m_serverInfoValue->NextSiblingElement())
	{
		const char* type = m_serverInfoValue->Attribute("type");
		const char* value = m_serverInfoValue->Attribute("value");

		if (strcmp(type, "serverID") == 0)
		{
			UniqueInfo->serverID = value;
		}
		else
		{
			std::cout << __FUNCTION__ << " : " << __LINE__ << " server unique info conf type: " << type << " value: " << value << std::endl;
			doc.Clear();
			return false;
		}
	}

	doc.Clear();
	return true;
}
