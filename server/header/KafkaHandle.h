/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/
#pragma once


// Ŀǰ Kafka �Ĳ����������첽��ʽ�� Kafka ���ڲ����̺߳� kafka ����ͨ��
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

