/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include <iostream>
#include "KafkaHandle.h"
#include "rdkafkacpp.h"



KafkaHandle::KafkaHandle()
{
	ProducerConf = nullptr;
	ProducerTconf = nullptr;
	ConsumerConf = nullptr;
	ConsumerTconf = nullptr;

	ConsumerHandle = nullptr;
	SelfTopic = nullptr;
	GlobalTopic = nullptr;

	bKafkaRunning = false;
}

KafkaHandle::~KafkaHandle()
{
	bKafkaRunning = false;
}

void KafkaHandle::Start()
{
	bKafkaRunning = true;
	int somedata = 1;
	SelfTopicConsume = std::thread(&KafkaHandle::SelfTopicConsumeCallback,this, &somedata);

}

bool KafkaHandle::Init()
{
	if (!InitKafkaInfo()) return false;

	if (!InitConsumerHandle()) return false;



	return true;

}

bool KafkaHandle::InitKafkaInfo()
{
	KafkaInfo = ServerConf::GetInstance().GetKafkaInfo();
	if (!KafkaInfo)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  get Kafka info failed!" << std::endl;
		return false;
	}

	ProducerConf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	if (!ProducerConf )
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  init ProducerConf failed!" << std::endl;
		return false;
	}

	ProducerTconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	if (!ProducerTconf)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  init ProducerTconf failed!" << std::endl;
		return false;
	}

	ConsumerConf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	if (!ConsumerConf)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  init ConsumerConf failed!" << std::endl;
		return false;
	}

	ConsumerTconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	if (!ConsumerTconf)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "  init ConsumerTconf failed!" << std::endl;
		return false;
	}

	return true;
}

bool KafkaHandle::InitConsumerHandle()
{
	ConsumerHandle = RdKafka::Consumer::create(ConsumerConf, ConsumerErrstr);
	if (!ConsumerHandle) {
		std::cout << __FUNCTION__ << " : " << __LINE__ << "Failed to create consumer: " << ConsumerErrstr << std::endl;
		return false;
	}

	SelfTopic = RdKafka::Topic::create(ConsumerHandle, KafkaInfo->selfTopic,
		ConsumerTconf, ConsumerErrstr);
	if (!SelfTopic)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "Failed to create SelfTopic: " << KafkaInfo->selfTopic
			<< " error: " << ConsumerErrstr << std::endl;
		return false;
	}

	GlobalTopic = RdKafka::Topic::create(ConsumerHandle, KafkaInfo->globalTopic,
		ConsumerTconf, ConsumerErrstr);
	if (!GlobalTopic)
	{
		std::cout << __FUNCTION__ << " : " << __LINE__ << "Failed to create GlobalTopic: " << KafkaInfo->globalTopic
			<< " error: " << ConsumerErrstr << std::endl;
		return false;
	}

	RdKafka::ErrorCode resp = ConsumerHandle->start(SelfTopic, KafkaInfo->selfPartition, 
		RdKafka::Topic::OFFSET_BEGINNING);	// ‘› ±–¥À¿ RdKafka::Topic::OFFSET_BEGINNING
	if (resp != RdKafka::ERR_NO_ERROR) {
		std::cout << __FUNCTION__ << " : " << __LINE__ << "Failed to start selfPartition consumer: " <<
			RdKafka::err2str(resp) << std::endl;
		return false;
	}

	resp = ConsumerHandle->start(GlobalTopic, KafkaInfo->globalPartition,
		RdKafka::Topic::OFFSET_BEGINNING);	// ‘› ±–¥À¿ RdKafka::Topic::OFFSET_BEGINNING
	if (resp != RdKafka::ERR_NO_ERROR) {
		std::cout << __FUNCTION__ << " : " << __LINE__ << "Failed to start GlobalTopic consumer: " <<
			RdKafka::err2str(resp) << std::endl;
		return false;
	}

	return true;
}

void KafkaHandle::SelfMsgConsume(RdKafka::Message* message, void* opaque)
{
	switch (message->err()) {
	case RdKafka::ERR__TIMED_OUT:
		break;

	case RdKafka::ERR_NO_ERROR:
		/* Real message */
		std::cout << "self consume : " << message->key() << " value: " <<
			message->payload() << std::endl;
		break;

	case RdKafka::ERR__PARTITION_EOF:
		/* Last message */
		break;

	case RdKafka::ERR__UNKNOWN_TOPIC:
	case RdKafka::ERR__UNKNOWN_PARTITION:
		std::cerr << "Consume failed: " << message->errstr() << std::endl;
		break;

	default:
		/* Errors */
		std::cerr << "Consume failed: " << message->errstr() << std::endl;
	}
}

void KafkaHandle::SelfTopicConsumeCallback(void* param)
{
	auto partition = KafkaInfo->selfPartition;
	while (bKafkaRunning) {
		RdKafka::Message *msg = ConsumerHandle->consume(SelfTopic, partition, 1000);
		SelfMsgConsume(msg, param);
		delete msg;
		ConsumerHandle->poll(0);
	}

}

