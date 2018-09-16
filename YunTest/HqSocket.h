#pragma once
class CHqSocket
{
public:
	CHqSocket(void);
	~CHqSocket(void);

public:
	void Close(SOCKET s);
	int RecvPack(char *precv,int nbytes,int nSecond);
	int SendPack(const char *psend,int nbytes,int nSecond);
	int SetNoBlock(bool opflag);
	int ConnectSvr(char * lpszIpAddr, UINT nPort, int nSecond);
	int ConnectServer(char * lpszIpAddr, UINT nPort);
	int SendMsg(const char *psend, int nbytes);
	int RecvMsg(char *precv, int nbytes);
	SOCKET m_Socket;

};

