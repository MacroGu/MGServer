/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/
#pragma once


// 目前 Kafka 的操作，采用异步方式， Kafka 类内部有线程和 kafka 进行通信
#include <string>
#include "ServerConf.h"
#include "rdkafkacpp.h"

class KafkaHandle
{

public:
	KafkaHandle();
	~KafkaHandle();


public:
	static void Start(const std::string &topic,
		const RdKafka::Metadata *metadata);

protected:

};

