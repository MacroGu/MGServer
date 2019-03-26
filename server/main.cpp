/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/


#include "AcceptServer.h"
#include "rdkafkacpp.h"
#include <iostream>


static void metadata_print(const std::string &topic,
	const RdKafka::Metadata *metadata) {
	std::cout << "Metadata for " << (topic.empty() ? "" : "all topics")
		<< "(from broker " << metadata->orig_broker_id()
		<< ":" << metadata->orig_broker_name() << std::endl;

	/* Iterate brokers */
	std::cout << " " << metadata->brokers()->size() << " brokers:" << std::endl;
	RdKafka::Metadata::BrokerMetadataIterator ib;
	for (ib = metadata->brokers()->begin();
		ib != metadata->brokers()->end();
		++ib) {
		std::cout << "  broker " << (*ib)->id() << " at "
			<< (*ib)->host() << ":" << (*ib)->port() << std::endl;
	}
	/* Iterate topics */
	std::cout << metadata->topics()->size() << " topics:" << std::endl;
	RdKafka::Metadata::TopicMetadataIterator it;
	for (it = metadata->topics()->begin();
		it != metadata->topics()->end();
		++it) {
		std::cout << "  topic \"" << (*it)->topic() << "\" with "
			<< (*it)->partitions()->size() << " partitions:";

		if ((*it)->err() != RdKafka::ERR_NO_ERROR) {
			std::cout << " " << err2str((*it)->err());
			if ((*it)->err() == RdKafka::ERR_LEADER_NOT_AVAILABLE)
				std::cout << " (try again)";
		}
		std::cout << std::endl;

		/* Iterate topic's partitions */
		RdKafka::TopicMetadata::PartitionMetadataIterator ip;
		for (ip = (*it)->partitions()->begin();
			ip != (*it)->partitions()->end();
			++ip) {
			std::cout << "    partition " << (*ip)->id()
				<< ", leader " << (*ip)->leader()
				<< ", replicas: ";

			/* Iterate partition's replicas */
			RdKafka::PartitionMetadata::ReplicasIterator ir;
			for (ir = (*ip)->replicas()->begin();
				ir != (*ip)->replicas()->end();
				++ir) {
				std::cout << (ir == (*ip)->replicas()->begin() ? "" : ",") << *ir;
			}

			/* Iterate partition's ISRs */
			std::cout << ", isrs: ";
			RdKafka::PartitionMetadata::ISRSIterator iis;
			for (iis = (*ip)->isrs()->begin(); iis != (*ip)->isrs()->end(); ++iis)
				std::cout << (iis == (*ip)->isrs()->begin() ? "" : ",") << *iis;

			if ((*ip)->err() != RdKafka::ERR_NO_ERROR)
				std::cout << ", " << RdKafka::err2str((*ip)->err()) << std::endl;
			else
				std::cout << std::endl;
		}
	}
}

int  main(int argc, char ** argv) {

	CAcceptServer acceptServer;

	acceptServer.StartServer();

	return 0;
}