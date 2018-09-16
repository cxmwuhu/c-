#include "StdAfx.h"
#include "MySocket.h"
#include <stdio.h>


CMySocket::CMySocket(void)
{
	m_Socket = INVALID_SOCKET;
	m_Socket1 = INVALID_SOCKET;
	//��ʼ���׽��ְ汾
	WORD wRequestVersion;
	WSADATA wData;
	wRequestVersion = MAKEWORD(2,2);
	int err = WSAStartup(wRequestVersion,&wData);
}


CMySocket::~CMySocket(void)
{
	if(m_Socket!=INVALID_SOCKET)
	{
		closesocket(m_Socket);
	}
}


// ����socket
int CMySocket::ConnectEx(char* lpszIP, unsigned int port, int nSecond)
{
	//����socket
	m_Socket = ::socket(AF_INET,SOCK_STREAM,0);
	//SetNoBlock(1);
	SOCKADDR_IN sockServer;
	sockServer.sin_family = AF_INET;
	sockServer.sin_addr.S_un.S_addr = inet_addr(lpszIP);
	sockServer.sin_port = htons(port);

	//socket����
	int ret = connect(m_Socket,(sockaddr*)&sockServer,sizeof(sockaddr));
	if(ret==SOCKET_ERROR)
	{
		//socket���Ӵ��󣻹ص�socket��
		closesocket(m_Socket);
		return -1;
	}

	//���׽��ּӵ�select������
	FD_SET fdset;
	FD_ZERO(&fdset);
	FD_SET(m_Socket,&fdset);
	timeval tv;
	tv.tv_sec = nSecond;
	tv.tv_usec = 0;
	int retselect = select(0,0,&fdset,0,&tv);
	if(retselect == SOCKET_ERROR)
	{
		closesocket(m_Socket);
		return -1;
	}
	return 0;
}


// �����첽ģʽ
int CMySocket::SetNoBlock(bool flag)
{
	unsigned long int af = flag?1:0;
	return ioctlsocket(m_Socket,FIONBIO,&af);
}


int CMySocket::SendPack(char* buf, int nBytes)
{
	int mbyte=0,sum=0;
	while(nBytes>0)
	{
		mbyte = send(m_Socket,buf+sum,nBytes,0);
		if(mbyte<=0)
		{
			//�������ݰ�ʧ�ܣ��ر�socket
			closesocket(m_Socket);
			return -1;
		}
		nBytes -= mbyte;
		sum += mbyte;
	}
	return sum;
}


int CMySocket::RecvPack(char *buf, int nBytes)
{
	int mRecvbyte = 0, nSum = 0;
	while(nBytes>0)
	{
		mRecvbyte = recv(m_Socket,buf+nSum,nBytes,0);
		if(mRecvbyte==0 || mRecvbyte==SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			printf("�������Ϣ:%d",error);
			//�հ�ʧ����
			closesocket(m_Socket);
			return nSum;
		}
		nBytes -= mRecvbyte;
		nSum += mRecvbyte;
	}
	return nSum;
}


// �ر�socket
void CMySocket::Close(SOCKET s)
{
	if(m_Socket!=INVALID_SOCKET)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		s=INVALID_SOCKET;
	}
	return;
}
