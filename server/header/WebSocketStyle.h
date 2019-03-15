/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once

#include <string>
#include "base64.h"
#include "sha1.h"
#include "defines.h"
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

//Websocket数据包数据头信息
struct WebSocketStreamHeader {
	unsigned int header_size;                //数据包头大小
	int mask_offset;                    //掩码偏移
	unsigned int payload_size;                //数据大小
	bool fin;                                               //帧标记
	bool masked;                            //掩码
	unsigned char opcode;                    //操作码
	unsigned char res[3];
};

// 数据包操作类型
enum WebSocketOpCode {
	ContinuationFrame = 0x0,                //连续帧
	TextFrame = 0x1,						//文本帧
	BinaryFrame = 0x2,						//二进制帧
	ConnectionClose = 0x8,                  //连接关闭
	Ping = 0x9,
	Pong = 0xA,
	EmptyFrame = 0xF0,
	ErrorFrame = 0xF1,
	OperingFrame = 0xF3,
	ClosingFrame = 0x08
};

class CWebSocketStyle
{
public:
	CWebSocketStyle();
	~CWebSocketStyle();


	// 一些用于判断 ws 数据格式的函数
public:
	// 握手
	bool isWSHandShake(std::string &request);
	
	// 如果是，解析握手协议重新组装准备send回复给client
	bool wsHandshake(std::string &request, std::string &response);

	// 首先解析包头信息
	bool wsReadHeader(const unsigned char* cData, WebSocketStreamHeader* header);

	// 然后根据包头解析出真是数据
	bool wsDecodeFrame(const WebSocketStreamHeader& header, unsigned char cbSrcData[], unsigned short wSrcLen, unsigned char cbTagData[]);

	// 组装server发给client协议
	bool wsEncodeFrame(std::string inMessage, std::string &outFrame, enum WebSocketOpCode frameType);

	// 解析客户端 数据 string
	bool wsDecodeFrame(std::string inFrame, std::string& outMessage);

};
