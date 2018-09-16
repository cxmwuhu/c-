#include <WinSock2.h>

#pragma once
class CMySocket
{
public:
	CMySocket(void);
	~CMySocket(void);

public:
	SOCKET m_Socket;
	// 连接socket
	SOCKET m_Socket1;     //推送消息socket
	int ConnectEx(char* lpszIP, unsigned int port, int nSecond);
	// 设置异步模式
	int SetNoBlock(bool flag);
	int SendPack(char* buf, int nBytes);
	int RecvPack(char *buf, int nBytes);
	// 关闭socket
	void Close(SOCKET s);
};

