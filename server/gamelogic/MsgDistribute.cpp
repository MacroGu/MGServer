

#include "MsgDistribute.h"


MsgDistribute::MsgDistribute()
{
}

MsgDistribute::~MsgDistribute()
{
}

void MsgDistribute::DistributeMsg(int clientFD, std::shared_ptr<stMsgToClient> recvData)
{
	if (recvData == nullptr)
	{
		return;
	}

	// 包类型
	int PacketType;
	// 反序列化客户端信息
	std::stringstream RecvStream;

	RecvStream << recvData->data;
	RecvStream >> PacketType;

	switch (PacketType)
	{
	case LOGIN:

		break;

	case ENROLL_PLAYER:

		break;
	case SEND_PLAYER:

		break;
	case RECV_PLAYER:

		break;
	case LOGOUT_PLAYER:

		break;
	case HIT_PLAYER:

		break;
	case DAMAGED_PLAYER:

		break;
	case CHAT:

		break;
	case ENTER_NEW_PLAYER:

		break;
	case SIGNUP:

		break;
	case HIT_MONSTER:

		break;
	case SYNC_MONSTER:

		break;
	case SPAWN_MONSTER:

		break;
	case DESTROY_MONSTER:

		break;
	default:
		break;
	}


}
