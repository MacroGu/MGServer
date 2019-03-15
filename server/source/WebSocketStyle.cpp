/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#include "WebSocketStyle.h"


CWebSocketStyle::CWebSocketStyle()
{

}

CWebSocketStyle::~CWebSocketStyle()
{

}

bool CWebSocketStyle::isWSHandShake(std::string &request)
{
	size_t i = request.find("GET");
	if (i == std::string::npos) {
		return false;
	}
	return true;
}

bool CWebSocketStyle::wsHandshake(std::string &request, std::string &response)
{
	//得到客户端请求信息的key
	std::string tempKey = request;
	size_t i = tempKey.find("Sec-WebSocket-Key");
	if (i == std::string::npos) {
		return false;
	}
	tempKey = tempKey.substr(i + 19, 24);

	//拼接协议返回给客户端
	response = "HTTP/1.1 101 Switching Protocols\r\n";
	response += "Upgrade: websocket\r\nConnection: upgrade\r\n";
	response += "Sec-WebSocket-Accept: ";

	std::string realKey = tempKey;
	realKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	SHA1 sha;
	unsigned int message_digest[5];
	sha.Reset();
	sha << realKey.c_str();
	sha.Result(message_digest);
	for (int i = 0; i < 5; i++) {
		message_digest[i] = htonl(message_digest[i]);
	}
	realKey = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
	realKey += "\r\n";
	response += realKey.c_str();
	response += "\r\n";
	
	return true;
}

bool CWebSocketStyle::wsReadHeader(const unsigned char* cData, WebSocketStreamHeader* header)
{
	if (cData == NULL)return false;

	const unsigned char *buf = cData;

	header->fin = buf[0] & 0x80;
	header->masked = buf[1] & 0x80;
	unsigned char stream_size = buf[1] & 0x7F;

	header->opcode = buf[0] & 0x0F;
	if (header->opcode == WebSocketOpCode::ContinuationFrame) {
		//连续帧
		return false;
	}
	else if (header->opcode == WebSocketOpCode::TextFrame) {
		//文本帧
	}
	else if (header->opcode == WebSocketOpCode::BinaryFrame) {
		//二进制帧

	}
	else if (header->opcode == WebSocketOpCode::ConnectionClose) {
		//连接关闭消息
		return false;
	}
	else if (header->opcode == WebSocketOpCode::Ping) {
		//  ping
		return false;
	}
	else if (header->opcode == WebSocketOpCode::Pong) {
		// pong
		return false;
	}
	else {
		//非法帧
		return false;
	}

	if (stream_size <= 125) {
		//    small stream
		header->header_size = 6;
		header->payload_size = stream_size;
		header->mask_offset = 2;
	}
	else if (stream_size == 126) {
		//    medium stream 
		header->header_size = 8;
		unsigned short s = 0;
		memcpy(&s, (const char*)&buf[2], 2);
		header->payload_size = ntohs(s);
		header->mask_offset = 4;
	}
	else if (stream_size == 127) {

		unsigned long long l = 0;
		memcpy(&l, (const char*)&buf[2], 8);

		header->payload_size = l;
		header->mask_offset = 10;
	}
	else {
		//Couldnt decode stream size 非法大小数据包
		return false;
	}

	if (header->payload_size > MAX_WEBSOCKET_BUFFER) {
		return false;
	}

	return true;
}

bool CWebSocketStyle::wsDecodeFrame(const WebSocketStreamHeader& header, unsigned char cbSrcData[], unsigned short wSrcLen, unsigned char cbTagData[])
{
	const unsigned char *final_buf = cbSrcData;
	if (wSrcLen < header.header_size + 1) {
		return false;
	}

	char masks[4];
	memcpy(masks, final_buf + header.mask_offset, 4);
	memcpy(cbTagData, final_buf + header.mask_offset + 4, header.payload_size);

	for (int i = 0; i < header.payload_size; ++i) {
		cbTagData[i] = (cbTagData[i] ^ masks[i % 4]);
	}
	//如果是文本包，在数据最后加一个结束字符“\0”
	if (header.opcode == WebSocketOpCode::TextFrame)
		cbTagData[header.payload_size] = '\0';

	return true;
}

bool CWebSocketStyle::wsDecodeFrame(std::string inFrame, std::string& outMessage)
{
	int ret = OperingFrame;
	const char *frameData = inFrame.c_str();
	const int frameLength = inFrame.size();
	if (frameLength < 2)
	{
		ret = ErrorFrame;
	}

	// 检查扩展位并忽略  
	if ((frameData[0] & 0x70) != 0x0)
	{
		ret = ErrorFrame;
	}

	// fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文  
	ret = (frameData[0] & 0x80);
	if ((frameData[0] & 0x80) != 0x80)
	{
		ret = ErrorFrame;
	}

	// mask位, 为1表示数据被加密  
	if ((frameData[1] & 0x80) != 0x80)
	{
		ret = ErrorFrame;
	}

	// 操作码  
	uint16_t payloadLength = 0;
	uint8_t payloadFieldExtraBytes = 0;
	uint8_t opcode = static_cast<uint8_t>(frameData[0] & 0x0f);
	if (opcode == TextFrame)
	{
		// 处理utf-8编码的文本帧  
		payloadLength = static_cast<uint16_t>(frameData[1] & 0x7f);
		if (payloadLength == 0x7e)
		{
			uint16_t payloadLength16b = 0;
			payloadFieldExtraBytes = 2;
			memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);
			payloadLength = ntohs(payloadLength16b);
		}
		else if (payloadLength == 0x7f)
		{
			// 数据过长,暂不支持  
			ret = ErrorFrame;
		}
	}
	else if (opcode == BinaryFrame || opcode == Ping || opcode == Pong)
	{
		// 二进制/ping/pong帧暂不处理  
	}
	else if (opcode == ClosingFrame)
	{
		ret = ClosingFrame;
	}
	else
	{
		ret = ErrorFrame;
	}

	// 数据解码  
	if ((ret != ErrorFrame) && (payloadLength > 0))
	{
		// header: 2字节, masking key: 4字节  
		const char *maskingKey = &frameData[2 + payloadFieldExtraBytes];
		char *payloadData = new char[payloadLength + 1];
		memset(payloadData, 0, payloadLength + 1);
		memcpy(payloadData, &frameData[2 + payloadFieldExtraBytes + 4], payloadLength);
		for (int i = 0; i < payloadLength; i++)
		{
			payloadData[i] = payloadData[i] ^ maskingKey[i % 4];
		}

		outMessage = payloadData;
		delete[] payloadData;
	}

	return ret;
}

bool CWebSocketStyle::wsEncodeFrame(std::string inMessage, std::string &outFrame, enum WebSocketOpCode frameType)
{
	const uint32_t messageLength = inMessage.size();
	if (messageLength > 32767)
	{
		// 暂不支持这么长的数据  
		return false;
	}

	uint8_t payloadFieldExtraBytes = (messageLength <= 0x7d) ? 0 : 2;
	// header: 2字节, mask位设置为0(不加密), 则后面的masking key无须填写, 省略4字节  
	uint8_t frameHeaderSize = 2 + payloadFieldExtraBytes;
	uint8_t *frameHeader = new uint8_t[frameHeaderSize];
	memset(frameHeader, 0, frameHeaderSize);

	// fin位为1, 扩展位为0, 操作位为frameType  
	frameHeader[0] = static_cast<uint8_t>(0x80 | frameType);

	// 填充数据长度
	if (messageLength <= 0x7d)
	{
		frameHeader[1] = static_cast<uint8_t>(messageLength);
	}
	else
	{
		frameHeader[1] = 0x7e;
		uint16_t len = htons(messageLength);
		memcpy(&frameHeader[2], &len, payloadFieldExtraBytes);
	}

	// 填充数据  
	uint32_t frameSize = frameHeaderSize + messageLength;
	char *frame = new char[frameSize + 1];
	memcpy(frame, frameHeader, frameHeaderSize);
	memcpy(frame + frameHeaderSize, inMessage.c_str(), messageLength);
	outFrame = std::string(frame, frameSize);
	delete[] frame;
	delete[] frameHeader;
	return true;
}
