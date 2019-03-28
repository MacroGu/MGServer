/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/
#pragma once


// 目前 Kafka 的操作，采用异步方式， Kafka 类内部有线程和 kafka 进行通信
// 不使用消费后回调的方式 

#include <string>
#include <thread>
#include "ServerConf.h"
#include "rdkafkacpp.h"



class KafkaHandle
{

public:
	KafkaHandle();
	~KafkaHandle();


public:
	void Start();

	bool Init();


protected:
	bool InitKafkaInfo();
	bool InitConsumerHandle();
	void SelfMsgConsume(RdKafka::Message* message, void* opaque);


protected:
	void SelfTopicConsumeCallback(void* param);

private:
	std::shared_ptr<stKafkaInfo> KafkaInfo;
	RdKafka::Conf *ProducerConf;
	RdKafka::Conf *ProducerTconf;
	RdKafka::Conf *ConsumerConf;
	RdKafka::Conf *ConsumerTconf;

	std::string ConsumerErrstr;
	RdKafka::Consumer* ConsumerHandle;
	RdKafka::Topic* SelfTopic;
	RdKafka::Topic* GlobalTopic;
	std::thread SelfTopicConsume;

	bool bKafkaRunning;

};

