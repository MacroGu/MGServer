#pragma once

#include "singleton.h"
#include "../base/Interface.h"


class MsgDistribute : public ISingleton<MsgDistribute>
{
public:
	~MsgDistribute();


public:
	// 分发消息
	void DistributeMsg(int clientFD, std::shared_ptr<stMsgToClient> recvData);


private:
	friend ISingleton<MsgDistribute>;
	friend class std::unique_ptr<MsgDistribute>;

	MsgDistribute();

};

