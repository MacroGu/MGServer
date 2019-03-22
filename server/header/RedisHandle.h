/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


// Ŀǰ redis �Ĵ洢 ��֧��ͬ����ʽ�� �����ٸĳ��첽��ʽ
#include <string>
#include "singleton.h"
#include "ServerConf.h"
#include "hiredis.h"



class RedisHandle : public ISingleton<RedisHandle>
{


public:
	bool Init();

	bool SetString(const std::string& StrKey, const std::string& StrValue);

	bool GetString(const std::string& StrKey, std::string& StrValue);


protected:
	bool ConnectRedis();


private:

	friend ISingleton<RedisHandle>;
	friend class std::unique_ptr<RedisHandle>;

	RedisHandle();


private:
	std::shared_ptr<stRedisInfo> RedisInfo;
	redisContext* RedisContext;
	const std::string replyStr = "OK";
public:
	~RedisHandle();
};

