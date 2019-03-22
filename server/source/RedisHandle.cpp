/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include <iostream>
#include "RedisHandle.h"



RedisHandle::RedisHandle()
{
	RedisInfo = ServerConf::GetInstance().GetRedisInfo();
	RedisContext = nullptr;
}

RedisHandle::~RedisHandle()
{
	redisFree(RedisContext);
	RedisContext = nullptr;
}

bool RedisHandle::Init()
{
	if (!RedisInfo)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  get Redis info failed!" << std::endl;
		return false;
	}
	
	if (!ConnectRedis())
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  connect to Redis failed!" << std::endl;
		return false;
	}


	return true;
}

bool RedisHandle::ConnectRedis()
{
	if (RedisContext != nullptr)
	{
		LOG_WARN("RedisContext is not null, and try connect Redis, it will be release first!");
		redisFree(RedisContext);
	}

	struct timeval timeout = { 1, 500000 };		// 1.5 seconds to wait for connecting
	RedisContext = redisConnectWithTimeout(RedisInfo->redisAddress.c_str(), RedisInfo->redisPort, timeout);
	if (RedisContext == nullptr || RedisContext->err != 0)
	{
		LOG_ERROR("connect to Redis failed!");
		return false;
	}

	return true;
}

bool RedisHandle::SetString(const std::string& StrKey, const std::string& StrValue)
{
	if (RedisContext == nullptr || RedisContext->err != 0)
	{
		LOG_ERROR(" SetString lose Redis connect ,will Reconnect later ");
		if (!ConnectRedis())
		{
			LOG_ERROR("GetString Reconnect Redis failed! ");
			return false;
		}
	}

	auto reply = (redisReply*)redisCommand(RedisContext, "SET %s %s",
		StrKey.c_str() , StrValue.c_str());//Ö´ÐÐÐ´ÈëÃüÁî
	if (reply == nullptr)
	{
		LOG_ERROR("set redis string value failed! key : {} , value : {} ", StrKey, StrValue);
		freeReplyObject(reply);
		return false;
	}

	if (reply->type == REDIS_REPLY_STATUS && replyStr.compare(reply->str) == 0)
	{
		
		LOG_DEBUG("set Redis string value success, key : {} , value : {} ", StrKey, StrValue);
		freeReplyObject(reply);
		return true;
	}


	LOG_ERROR("set string value failed!, key : {} , value : {} , reply: {} ",
		StrKey, StrValue, reply->str);
	freeReplyObject(reply);
	return false;

}

bool RedisHandle::GetString(const std::string& StrKey, std::string& StrValue)
{
	if (RedisContext == nullptr || RedisContext->err != 0)
	{
		LOG_ERROR(" GetString lose Redis connect ,will Reconnect later ");
		if (!ConnectRedis())
		{
			LOG_ERROR("GetString Reconnect Redis failed! ");
			return false;
		}
	}

	auto reply = (redisReply*)redisCommand(RedisContext, "GET %s",
		StrKey.c_str());//Ö´ÐÐ¶ÁÈ¡ÃüÁî

	if (reply == nullptr)
	{
		LOG_ERROR("get redis string value failed! key : {} ", StrKey);
		freeReplyObject(reply);
		return false;
	}

	if (reply->type == REDIS_REPLY_STRING)
	{
		LOG_DEBUG("get Redis string value success, key : {} , value : {} ", StrKey, reply->str);
		StrValue = reply->str;
		freeReplyObject(reply);
		return true;
	}

	LOG_ERROR("get string value failed!, key : {} , reply: {} ",
		StrKey, reply->str);
	freeReplyObject(reply);

	return true;
}
