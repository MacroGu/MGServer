/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once


// 建立连接后  客户端请求存储自己的数据
#define MSG_ID_CLIENT_INFO_REQ		10000
// 服务端返回存储的结果
// 0: 存储成功  1： 已经有客户端的信息，更新  2：没有target的信息 3: 失败 没有curClientName
#define MSG_ID_CLIENT_INFO_RET		10001

// 请求转发数据到目标client
#define MSG_ID_FORWARD_TO_TARGET_REQ	10002
// 如果转发成功是不返回的
#define MSG_ID_FORWARD_TO_TARGET_RET	10003
// 获取到 其他客户端转发来的数据
#define MSG_ID_RECV_FORWARD_DATA	10004

