/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#pragma once

#include "ServerConf.h"

ServerConf::ServerConf()
{
	bAllConfLoadedRight = true;
}

ServerConf::~ServerConf()
{

}


bool ServerConf::loadConf(const std::string& serverKind)
{
	if (!LoadServerInfo(SERVER_CONF_PATH, serverKind))
	{
		LOG_ERROR("load local server configuration failed! ");
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

bool ServerConf::LoadServerInfo(const std::string& fileDir, const std::string& addressInfo)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(fileDir.c_str()))
	{
		LOG_ERROR("load file:{} error:{}", fileDir.c_str(), doc.ErrorDesc());
		return false;
	}

	TiXmlElement* server = doc.FirstChildElement();
	if (server == nullptr)
	{
		LOG_ERROR("can not find root in xml!");
		return false;
	}

	TiXmlElement* m_serverInfo = server->FirstChildElement(addressInfo.c_str());
	if (m_serverInfo == nullptr)
	{
		LOG_ERROR("can not find local server info xml attribute!");
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
		LOG_ERROR("address info is wrong!");
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
		else if (strcmp(type, "threadNums") == 0)
		{
			serverInfo->threadNums = atoi(value);
		}
		else
		{
			LOG_ERROR("server info conf type:{} value:{}" ,type, value);
			doc.Clear();
			return false;
		}
	}

	doc.Clear();
	return true;
}
