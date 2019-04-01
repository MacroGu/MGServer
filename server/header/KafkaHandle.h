/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/
#pragma once

#ifndef _WIN32


// Ŀǰ Kafka �Ĳ����������첽��ʽ�� Kafka ���ڲ����̺߳� kafka ����ͨ��
// ��ʹ�����Ѻ�ص��ķ�ʽ 

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
	void SelfMsgConsumeHandle(RdKafka::Message* message, void* opaque);


protected:
	void SelfTopicConsumeCallback(void* self);

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
	std::thread* SelfTopicConsume;

	bool bKafkaRunning;

};

#endif // !_WIN32
