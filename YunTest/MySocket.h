#include <WinSock2.h>

#pragma once
class CMySocket
{
public:
	CMySocket(void);
	~CMySocket(void);

public:
	SOCKET m_Socket;
	// ����socket
	SOCKET m_Socket1;     //������Ϣsocket
	int ConnectEx(char* lpszIP, unsigned int port, int nSecond);
	// �����첽ģʽ
	int SetNoBlock(bool flag);
	int SendPack(char* buf, int nBytes);
	int RecvPack(char *buf, int nBytes);
	// �ر�socket
	void Close(SOCKET s);
};

